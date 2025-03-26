import gym
import numpy as np
import random
import tensorflow as tf
from tensorflow.keras import Sequential
from tensorflow.keras.layers import Dense
from collections import deque

# Hyperparameters
ENV_NAME = "CartPole-v1"
EPISODES = 1
GAMMA = 0.99  # Discount factor
LEARNING_RATE = 0.001
MEMORY_SIZE = 2000
BATCH_SIZE = 64
EPSILON_START = 1.0
EPSILON_END = 0.01
EPSILON_DECAY = 0.995
TARGET_UPDATE_FREQ = 10

# Create the Q-network
def build_model(state_size, action_size):
    model = Sequential([
        Dense(24, input_dim=state_size, activation='relu'),
        Dense(24, activation='relu'),
        Dense(action_size, activation='linear')
    ])
    model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=LEARNING_RATE),
                  loss='mse')
    return model

# Experience Replay Buffer
class ReplayBuffer:
    def __init__(self, max_size):
        self.buffer = deque(maxlen=max_size)
    
    def add(self, experience):
        state, action, reward, next_state, done = experience
        # Ensure all elements are properly formatted
        state = np.asarray(state, dtype=np.float32)
        next_state = np.asarray(next_state, dtype=np.float32)
        self.buffer.append((state, action, reward, next_state, done)) 
    def sample(self, batch_size):
        return random.sample(self.buffer, batch_size)
    
    def size(self):
        return len(self.buffer)

def train_dqn():
    # Initialize environment and parameters
    env = gym.make(ENV_NAME, render_mode="rgb_array")
    # Use RecordVideo wrapper to record training episodes
    env = gym.wrappers.RecordVideo(env, './video', episode_trigger=lambda x: x % 50 == 0)  # Record every 50th episode

    state_size = env.observation_space.shape[0]
    action_size = env.action_space.n
    
    model = build_model(state_size, action_size)
    target_model = build_model(state_size, action_size)
    target_model.set_weights(model.get_weights())
    
    replay_buffer = ReplayBuffer(MEMORY_SIZE)
    
    epsilon = EPSILON_START
    rewards = []
    
    for episode in range(EPISODES):
        state = env.reset()
        print(state)
        state = np.asarray(state[0], dtype=np.float32)  # Ensure compatibility with NumPy's modern API
        state = np.reshape(state, [1, state_size])
        total_reward = 0
        
        while True:
            # Epsilon-Greedy Action Selection
            if np.random.rand() <= epsilon:
                action = random.randrange(action_size)
            else:
                q_values = model.predict(state, verbose=0)
                action = np.argmax(q_values[0])
            
            # Take action, observe reward and next state
            next_state, reward, done, truncated, _ = env.step(action)
            next_state = np.asarray(next_state, dtype=np.float32)  # Use np.asarray for modern NumPy compatibility
            next_state = np.reshape(next_state, [1, state_size])
            
            # Store experience in replay buffer
            replay_buffer.add((state, action, reward, next_state, done))
            state = next_state
            total_reward += reward
            
            # Train the model with a batch from the replay buffer
            if replay_buffer.size() >= BATCH_SIZE:
                batch = replay_buffer.sample(BATCH_SIZE)
                states = np.zeros((BATCH_SIZE, state_size))
                targets = np.zeros((BATCH_SIZE, action_size))
                
                for i, (s, a, r, s_next, d) in enumerate(batch):
                    # Ensure states are properly shaped
                    states[i] = s.reshape(state_size)
                    
                    # Get current Q-values
                    target = model.predict(s.reshape(1, state_size), verbose=0)
                    
                    # Calculate target Q-value
                    if d:  # If done
                        target[0][a] = r
                    else:
                        t = target_model.predict(s_next.reshape(1, state_size), verbose=0)
                        target[0][a] = r + GAMMA * np.amax(t[0])
                    
                    # Store target Q-values
                    targets[i] = target[0]
                
                # Train the model
                model.fit(states, targets, epochs=1, verbose=0)
            
            # Update target model weights periodically
            if episode % TARGET_UPDATE_FREQ == 0:
                target_model.set_weights(model.get_weights())
            
            if done:
                rewards.append(total_reward)
                print(f"Episode: {episode}, Total Reward: {total_reward}, Epsilon: {epsilon:.4f}")
                break
        
        # Decay epsilon
        if epsilon > EPSILON_END:
            epsilon *= EPSILON_DECAY
    
    env.close()
    return rewards

if __name__ == "__main__":
    rewards = train_dqn()
