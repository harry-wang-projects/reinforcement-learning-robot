import time
import gym
env = gym.make("MountainCar-v0", render_mode="rgb_array")
observation, info = env.reset(seed=7649)


from pyvirtualdisplay import Display
display = Display(visible=False, size=(3000, 2000))
_ = display.start()

import os
from gym.wrappers.monitoring.video_recorder import VideoRecorder
before_training = "before_training.mp4"


import matplotlib.pyplot as plt

def render():
  state_image = env.render()
  plt.imshow(state_image)
  plt.show()

render()


video = VideoRecorder(env, before_training, enabled=True)

for i in range(1000):
    env.render()
    video.capture_frame()
    print("i = " + str(i))
    if observation[1] > 0:
       action = 2
    else:
       action = 0
    if observation[0] < -1:
       action = 1
    observation, reward, terminated, truncated, info = env.step(action)
    print("step", i, observation, reward, truncated, info)

    if terminated or truncated:
        observation, info = env.reset()


video.close()
env.close()

