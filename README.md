# Cloth Simulation

Mass-spring model to simulate a cloth with a GUI using QT with different scenario's.

Explicit Verlet is used to calculate speed.

The GUI can be used to load OBJ files and render them in OpenGL with a texture and lighting.

The cloth is simulated using a mass-spring model where mesh vertices are modelled as point masses and the mesh edges are modelled as springs.

The buttons in the GUI toggle the following scenario's.

SS1: a square piece of cloth floating horizontally in the air, which then free-falls onto a sphere on the ground.

SS1 Rotate: SS1 with the sphere rotating in-place round the Y axis (up-axis) and friction between the sphereand the cloth.

SS2: a square piece of cloth floating horizontally in the air first, then free-falling with two adjacent corners fixed in the space.

SS2 Wind: SS2 with wind blowing.

To get a video of the simulation, the "Save Video (Series of images)" button can be used to save screenshots of the simulation. ffmpeg can be later used to create a video using the screenshots

# Notes:
- GLM and Eigen are used for calculations
- This was run on a Macbook using QT 5.15.1 clang 64-bit. Some issues may be encountered when using newer versions.
- The texture path can be changed in cloth.cpp to change the texture .
- The path to the folder where the screenshots are saved can be changed too. 

## Issues:
1. Currently , when the simulation starts and the cloth is freefalling , it doesn't accelerate.
2. The cloth has sticky collision with the sphere , this is as forces that are calculated to simulate sliding weren't implemented.

# Images

### Cloth simulation with sphere
<img width="1147" alt="image" src="https://github.com/OHashish/cloth-sim/assets/95832061/7ca7a1dd-d12e-4a2a-bf9f-1e5c269ade6f">

### Cloth simulation with sphere with corners hanging
<img width="1145" alt="image" src="https://github.com/OHashish/cloth-sim/assets/95832061/2b37cb03-8fd9-4d8b-9bc1-cb1cb1adc1b5">
