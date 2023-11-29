# Flight Simulator
This flight simulator project utilizes OpenGL for rendering a simple airplane model in a 3D environment. Below is an overview of key components and functionalities in the provided code.
# Flight Simulator

This flight simulator project utilizes OpenGL for rendering a simple airplane model in a 3D environment. Below is an overview of key components and functionalities in the provided code.

## Camera

The `updateViewMatrix` function in the `Camera` class calculates the view matrix based on the camera's position, rotation, and orientation. It follows these steps:

1. Converts the rotation quaternion to a rotation matrix (`rotationMatrix`).
2. Applies the rotation matrix to the original front and up vectors to get the new front and up vectors.
3. Calculates the right vector using the cross product of the new front and up vectors.
4. Constructs the view matrix using the right, up, front, and position vectors.

## Projection Matrix

The `updateProjectionMatrix` function in the `Camera` class calculates the perspective projection matrix. It sets up the field of view (FOV), near and far clipping planes, and calculates the matrix components (`a`, `b`, `c`, `d`). The projection matrix is then filled using these components.

## Main.cpp

- **Window Title:** The window title is set to "HW1 - 312555017" in the provided code.
- **Rendering Components:** The TODO#2 section instructs the rendering of the airplane's body, wings, and tail.

## Rendering Functions

### Airplane Body (`drawAirplaneBody`):

- Uses `drawCylinder` to render the cylindrical body of the airplane.
- Sets the body color using `glColor3f`.

### Wings (`drawLeftWing`, `drawRightWing`):

- Uses `drawCuboid` to render the wings of the airplane.
- Sets the wing color using `glColor3f`.

### Tail (`drawTetrahedron`):

- Uses `calculateNormal` and `glNormal3fv` for lighting (optional).
- Sets the tail color using `glColor3f`.
- Draws a tetrahedron representing the tail of the airplane.

### Propeller (`DrawPropeller`, `drawBlades`):

- Draws the propeller body using `drawCylinder`.
- Draws two blades using `drawCuboid`, applying rotation based on `PropellerAngle`.
- Sets the propeller and blade color using `glColor3f`.

## Recommendations

- Ensure that the necessary OpenGL libraries are linked properly.
- Verify that the quaternion-to-matrix conversion is implemented correctly.
- Check the rotation and translation operations in rendering functions.
## Demo

![image](https://github.com/Yang-Shun-Yu/computer-graph/blob/master/Airplane/airplaneimage.png)
   
