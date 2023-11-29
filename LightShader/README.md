# write a program based on the provided template that implementes several shader effect on different texture with GLSL

## Lighting
- point light
- ![image](https://github.com/Yang-Shun-Yu/computer-graph/blob/master/LightShader/pointlighting.png)
- directional light
- ![image](https://github.com/Yang-Shun-Yu/computer-graph/blob/master/LightShader/directionallighting.png)
- spot light
- ![image](https://github.com/Yang-Shun-Yu/computer-graph/blob/master/LightShader/spotlighting.png)
## Features

- **Materials:** Three different materials are defined - Flat White, Shiny Red, and Clear Blue. These materials have specific ambient, diffuse, specular, and shininess properties.

- **Programs:** Three shader programs are used - ExampleProgram, BasicProgram, and LightProgram. These programs handle the rendering process with different shading techniques.

- **Models:** Various 3D models are loaded, including a cube, a mug, a bottle, and a custom plane. These models have associated textures, scaling, and positioning.

- **Objects:** Objects in the scene are instances of models with specific transformations (translations, rotations, and scalings). Each object can have its own material and texture.

- **Lighting:** The program supports directional, point, and spot lights. Lighting parameters such as color, position, and attenuation can be adjusted. The shaders calculate ambient, diffuse, and specular lighting for each light type.

- **Camera:** The scene is viewed through a camera that can be controlled using keyboard input. The camera supports movement and rotation.

## Setup and Execution

1. **Dependencies:**
   - GLFW
   - GLAD
   - GLM

2. **Build:**
   - Make sure to link the necessary libraries (GLFW, GLAD) in your build system.
   - Compile the program using a C++ compiler (e.g., g++).

      ```bash
      g++ main.cpp gl_helper.cpp camera.cpp context.cpp model.cpp opengl_context.cpp program.cpp utils.cpp -o main -lglfw -ldl
      ```

3. **Run:**
   - Execute the compiled program.

      ```bash
      ./main
      ```

4. **Controls:**
   - Use the arrow keys to move the camera.
   - Press F9 to toggle mouse cursor visibility (useful for camera control).
   - Press keys 1-6 to switch between different shader programs and enable/disable lights.
   - Press keys K, L, I, O to interactively control light positions.

## Additional Notes

- The program allows for easy extension by adding new materials, models, and objects.
- Shaders can be modified for experimentation with different lighting models.

## Demo
![image](https://github.com/Yang-Shun-Yu/computer-graph/blob/master/LightShader/texturesthreelight.png)
