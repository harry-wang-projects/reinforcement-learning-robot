Learning robot project

This robot navigates around a room using a rotating ToF sensor. It aims to cover a wide area without crashing into anything. 

Unlike regular robots, this robot will "learn" how to avoid obstacles using a neural network. It will be given ToF sensor readings and will be asked to output the motor controls. In addition, I will install a bumper to detect when the robot crashes (alternatively, I could count ToF sensor readings under 10 cm as a "crash" ). The robot will be given a score system, and will lose points if the robot does not move or if it crashes.

If successful enough, the robot can be used to map out rooms.

Inputs:
- ToF sensor reading array
- Whether or not the robot crashed
Outputs:
- Motor 1 speed/direction
- Motor 2 speed/direction

Rules:
- Not moving will cause the robot to lose points
- Moving straight will give more points than turning. The higher the speed, the more points.
- Crashing will cause the robot to lose many points
- Going backwards will deduct points
- Staying in the same area for very long will deduct points
- Bonus: The robot could be rewarded for facing certain directions/going to certain points.
- Bonus: An IR beacon can be placed such that if the robot receives an IR signal, it loses points.


Phases of this project:
1. Creating a robot with a functional ToF sensor and bumper that can be remotely driven
2. Training a neural network that can allow the robot to drive without crashing
3. Improving the neural network and the hardware (e.g. adding sensors to the back, adding an IR beacon that the robot should follow/avoid)
4. Using more advanced sensors (e.g. Lidar, camera)
