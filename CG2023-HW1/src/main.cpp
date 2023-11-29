#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION
#include <glm/glm.hpp>

#include "camera.h"
#include "opengl_context.h"
#include "utils.h"

#define ANGLE_TO_RADIAN(x) (float)((x)*M_PI / 180.0f)
#define RADIAN_TO_ANGEL(x) (float)((x)*180.0f / M_PI)

#define CIRCLE_SEGMENT 64

#define ROTATE_SPEED 1.0f
#define FLYING_SPEED ROTATE_SPEED / 20.f

#define RED 0.905f, 0.298f, 0.235f
#define BLUE 0.203f, 0.596f, 0.858f
#define GREEN 0.18f, 0.8f, 0.443f

void resizeCallback(GLFWwindow* window, int width, int height) {
  OpenGLContext::framebufferResizeCallback(window, width, height);
  auto ptr = static_cast<Camera*>(glfwGetWindowUserPointer(window));
  if (ptr) {
    ptr->updateProjectionMatrix(OpenGLContext::getAspectRatio());
  }
}

float objectX = 0.0f;
float objectY = 0.0f;
float objectZ = 0.0f;
float objectAngle = 0.0f;
float wingAngle = 0.0f;

float PropellerAngle = 0.0f;
float objectSpeed = 0.05f;

float rotationSpeed = 1.0f;
float PropellerrotationSpeed = 20.0f;

bool spacebarPressed = false;
bool keyLeftPressed = false;
bool keyRightPressed = false;
bool keyUpPressed = false;
bool keyDownPressed = false;

bool wingdirection = true;

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
  // There are three actions: press, release, hold(repeat)
  if (action == GLFW_REPEAT) return;

  // Press ESC to close the window.
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    return;
  }
  /* TODO#4-1: Detect key-events, perform rotation or fly
   *       1. Use switch && case to find the key you want.
   *       2. Press "SPACE" for fly up, fly forward and wing rotate meanwhile.
   *       3. Press "GLFW_KEY_LEFT" for turn left.
   *       4. Press "GLFW_KEY_RIGHT " for turn right.
   * Hint:
   *       glfw3's key list (https://www.glfw.org/docs/3.3/group__keys.html)
   *       glfw3's action codes (https://www.glfw.org/docs/3.3/group__input.html#gada11d965c4da13090ad336e030e4d11f)
   * Note:
   *       You should finish rendering your airplane first.
   *       Otherwise you will spend a lot of time debugging this with a black screen.
   */
  if (key == GLFW_KEY_SPACE) {
    if (action == GLFW_PRESS) {
      spacebarPressed = true;
    } else if (action == GLFW_RELEASE) {
      spacebarPressed = false;
    }
  }

  if (key == GLFW_KEY_LEFT) {
    if (action == GLFW_PRESS) {
      keyLeftPressed = true;
    } else if (action == GLFW_RELEASE) {
      keyLeftPressed = false;
    }
  }

  if (key == GLFW_KEY_RIGHT) {
    if (action == GLFW_PRESS) {
      keyRightPressed = true;
    } else if (action == GLFW_RELEASE) {
      keyRightPressed = false;
    }
  }

  if (key == GLFW_KEY_UP) {
    if (action == GLFW_PRESS) {
      keyUpPressed = true;
    } else if (action == GLFW_RELEASE) {
      keyUpPressed = false;
    }
  }

  if (key == GLFW_KEY_DOWN) {
    if (action == GLFW_PRESS) {
      keyDownPressed = true;
    } else if (action == GLFW_RELEASE) {
      keyDownPressed = false;
    }
  }
}

void initOpenGL() {
  // Initialize OpenGL context, details are wrapped in class.
#ifdef __APPLE__
  // MacOS need explicit request legacy support
  OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
#else
  // OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
  OpenGLContext::createContext(43, GLFW_OPENGL_COMPAT_PROFILE);
#endif
  GLFWwindow* window = OpenGLContext::getWindow();
  /* TODO#0: Change window title to "HW1 - `your student id`"
   *        Ex. HW1 - 312550000
   */
  glfwSetWindowTitle(window, "HW1 - 312555017");
  glfwSetKeyCallback(window, keyCallback);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#ifndef NDEBUG
  OpenGLContext::printSystemInfo();
  // This is useful if you want to debug your OpenGL API calls.
  OpenGLContext::enableDebugCallback();
#endif
}

/* TODO#2: Render body, wing, tail
 * Hint:
 *       glBegin/glEnd (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBegin.xml)
 *       glColor3f (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glBegin.xml)
 *       glVertex3f (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glVertex.xml)
 *       glNormal (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glNormal.xml)
 *       glScalef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glScale.xml)
 * Note:
 *       You can refer to ppt "Draw Cylinder" page and `CIRCLE_SEGMENT`
 *       You can use color defined above
 */

void drawCylinder(float radius, float length) {
  int segments = CIRCLE_SEGMENT;
  float angleIncrement = 360.0f / segments;
  // Render the body as a cylinder
  glPushMatrix();
  glTranslatef(0.0f, 0.5f, 0.0f);  // Translate to the center of the cylinder
  glBegin(GL_QUAD_STRIP);          // Use QUAD_STRIP for the sides of the cylinder
  for (int i = 0; i <= segments; i++) {
    float angle = ANGLE_TO_RADIAN(i * angleIncrement);
    float x = radius * cos(angle);
    float y = radius * sin(angle);
    glNormal3f(x, y, 0.0f);
    // Define two points on the cylinder's side for each segment
    glVertex3f(x, y, length / 2.0f);   // Top point
    glVertex3f(x, y, -length / 2.0f);  // Bottom point
  }
  glEnd();
  // Draw the top and bottom disks to close the cylinder
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0.0f, 0.0f, -1.0f);
  glVertex3f(0.0f, 0.0f, -length / 2.0f);  // Center of the bottom disk
  for (int i = segments; i >= 0; i--) {
    float angle = ANGLE_TO_RADIAN(i * angleIncrement);
    float x = radius * cos(angle);
    float y = radius * sin(angle);
    glVertex3f(x, y, -length / 2.0f);  // Points on the bottom disk
  }
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0.0f, 0.0f, -1.0f);
  glVertex3f(0.0f, 0.0f, length / 2.0f);  // Center of the top disk
  for (int i = 0; i <= segments; i++) {
    float angle = ANGLE_TO_RADIAN(i * angleIncrement);
    float x = radius * cos(angle);
    float y = radius * sin(angle);
    glVertex3f(x, y, length / 2.0f);  // Points on the top disk
  }
  glEnd();
  glPopMatrix();  // Restore the transformation matrix
}

// Call this function to draw the airplane body in your main rendering loop
void drawAirplaneBody() {
  glPushMatrix();
  glColor3f(0.18f, 0.596f, 0.858f);  // Set the color of the body
  drawCylinder(0.5f, 4.0f);          // Draw the cylinder for the body
  glPopMatrix();
}

void drawCuboid(float length, float width, float height) {
  // Calculate half dimensions for convenience
  float halfLength = length / 2.0f;
  float halfWidth = width / 2.0f;
  float halfHeight = height / 2.0f;
  glBegin(GL_QUADS);  // Begin drawing quads (for each face)
  // Front face
  glNormal3f(0.0f, 1.0f, 0.0f);
  glVertex3f(-halfLength, halfHeight, -halfWidth);
  glVertex3f(-halfLength, halfHeight, halfWidth);
  glVertex3f(halfLength, halfHeight, halfWidth);
  glVertex3f(halfLength, halfHeight, -halfWidth);
  // Back face
  glNormal3f(0.0f, -1.0f, 0.0f);
  glVertex3f(-halfLength, -halfHeight, -halfWidth);
  glVertex3f(halfLength, -halfHeight, -halfWidth);
  glVertex3f(halfLength, -halfHeight, halfWidth);
  glVertex3f(-halfLength, -halfHeight, halfWidth);
  // Top face
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-halfLength, halfHeight, halfWidth);
  glVertex3f(-halfLength, -halfHeight, halfWidth);
  glVertex3f(halfLength, -halfHeight, halfWidth);
  glVertex3f(halfLength, halfHeight, halfWidth);
  // Bottom face
  glNormal3f(0.0f, 0.0f, -1.0f);
  glVertex3f(-halfLength, halfHeight, -halfWidth);
  glVertex3f(halfLength, halfHeight, -halfWidth);
  glVertex3f(halfLength, -halfHeight, -halfWidth);
  glVertex3f(-halfLength, -halfHeight, -halfWidth);
  // Right face
  glNormal3f(1.0f, 0.0f, 0.0f);
  glVertex3f(halfLength, halfHeight, -halfWidth);
  glVertex3f(halfLength, halfHeight, halfWidth);
  glVertex3f(halfLength, -halfHeight, halfWidth);
  glVertex3f(halfLength, -halfHeight, -halfWidth);
  // Left face
  glVertex3f(-halfLength, halfHeight, -halfWidth);
  glVertex3f(-halfLength, -halfHeight, -halfWidth);
  glVertex3f(-halfLength, -halfHeight, halfWidth);
  glVertex3f(-halfLength, halfHeight, halfWidth);
  glEnd();  // End drawing quads
}

void drawLeftWing() {
  glPushMatrix();
  glColor3f(0.905f, 0.298f, 0.235f);
  glTranslatef(2.0f, 0.5f, 0.0f);
  drawCuboid(4.0f, 1.0f, 0.5f);
  glPopMatrix();
}
void drawRightWing() {
  glPushMatrix();
  glColor3f(0.905f, 0.298f, 0.235f);
  glTranslatef(-2.0f, 0.5f, 0.0f);
  drawCuboid(4.0f, 1.0f, 0.5f);
  glPopMatrix();
}
void calculateNormal(const float vertex1[], const float vertex2[], const float vertex3[], float normal[]) {
  float edge1[3], edge2[3];
  // Calculate two edges of the triangle
  for (int i = 0; i < 3; i++) {
    edge1[i] = vertex2[i] - vertex1[i];
    edge2[i] = vertex3[i] - vertex1[i];
  }
  // Calculate the cross product of the edges to get the normal vector
  normal[0] = edge1[1] * edge2[2] - edge1[2] * edge2[1];
  normal[1] = edge1[2] * edge2[0] - edge1[0] * edge2[2];
  normal[2] = edge1[0] * edge2[1] - edge1[1] * edge2[0];
  // Normalize the normal vector
  float length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
  normal[0] /= length;
  normal[1] /= length;
  normal[2] /= length;
}

void drawTetrahedron() {
  glPushMatrix();
  glTranslatef(0.0f, 0.5f, 2.0f);
  glBegin(GL_TRIANGLES);
  glColor3f(0.18f, 0.8f, 0.443f);
  // Define the vertices of the tetrahedron
  float vertexA[] = {0.0f, 0.0f, 0.0f};
  float vertexB[] = {1.0f, 0.0f, 1.0f};
  float vertexC[] = {-1.0f, 0.0f, 1.0f};
  float vertexD[] = {0.0f, -0.5f, 1.0f};
  // Calculate the normal for each triangle
  float normal1[3], normal2[3], normal3[3], normal4[3];
  // Triangle 1 (base)
  calculateNormal(vertexA, vertexC, vertexB, normal1);
  // Triangle 2
  calculateNormal(vertexA, vertexD, vertexC, normal2);
  // Triangle 3
  calculateNormal(vertexD, vertexC, vertexA, normal3);
  // Triangle 4
  calculateNormal(vertexB, vertexD, vertexC, normal4);
  // Define the triangles (faces) of the tetrahedron
  // Triangle 1 (base)
  // glNormal3f(0.0f, -1.0f, 0.0f);  // Normal for lighting (optional)
  glNormal3fv(normal1);
  glVertex3fv(vertexA);
  glVertex3fv(vertexC);
  glVertex3fv(vertexB);
  // Triangle 2
  // glNormal3f(0.0f, 0.8944f, 0.4472f);  // Normal for lighting (optional)
  glNormal3fv(normal2);
  glVertex3fv(vertexA);
  glVertex3fv(vertexD);
  glVertex3fv(vertexC);
  // Triangle 3
  // glNormal3f(0.0f, 0.8944f, -0.4472f);  // Normal for lighting (optional)
  glNormal3f(-0.5f, -1.0f, 0.5f);
  glVertex3fv(vertexA);
  glVertex3fv(vertexB);
  glVertex3fv(vertexD);
  // Triangle 4
  // glNormal3f(0.0f, 0.8944f, 0.4472f);  // Normal for lighting (optional)
  // glNormal3fv(normal4);
  glVertex3fv(vertexB);
  glVertex3fv(vertexC);
  glVertex3fv(vertexD);
  glEnd();
  glPopMatrix();
}

void drawBall(float radius, int slices, int stacks) {
  const float PI = 3.14159265358979323846;
  glBegin(GL_QUAD_STRIP);

  for (int i = 0; i <= stacks; i++) {
    float theta1 = i * PI / stacks;
    float theta2 = (i + 1) * PI / stacks;
    for (int j = 0; j <= slices; j++) {
      float phi = j * 2 * PI / slices;
      float x = radius * sin(theta1) * cos(phi);
      float y = radius * sin(theta1) * sin(phi);
      float z = radius * cos(theta1);
      glVertex3f(x, y, z);

      x = radius * sin(theta2) * cos(phi);
      y = radius * sin(theta2) * sin(phi);
      z = radius * cos(theta2);
      glVertex3f(x, y, z);
    }
  }
  glEnd();
}

void DrawPropeller() {
  glPushMatrix();
  glNormal3f(0.0f, 1.0f, 0.0f);
  glColor3f(0.545f, 0.271f, 0.075f);  // Set the color of the body
  glTranslatef(0.0f, 0.5f, -2.0f);
  drawBall(0.5f, 20, 20);  // Draw the propeller body
  glPopMatrix();
}
void drawBlades() {
  glPushMatrix();
  glNormal3f(0.0f, 1.0f, 0.0f);
  glColor3f(0.545f, 0.271f, 0.075f);  // Set the color of the body
  glTranslatef(0.0f, 0.5f, -2.4f);
  glRotatef(0 + PropellerAngle, 0.0f, 0.0f, 1.0f);  // Rotate the blades
  drawCuboid(1.6f, 0.1f, 0.25f);                    // Draw the blades
  glNormal3f(0.0f, 1.0f, 0.0f);
  glPopMatrix();
  glPushMatrix();

  glColor3f(0.545f, 0.271f, 0.075f);
  glTranslatef(0.0f, 0.5f, -2.4f);
  glRotatef(90 + PropellerAngle, 0.0f, 0.0f, 1.0f);  // Rotate the blades
  drawCuboid(1.6f, 0.1f, 0.25f);                     // Draw the blades
  glNormal3f(0.0f, 1.0f, 0.0f);
  glPopMatrix();
}

void light() {
  GLfloat light_specular[] = {0.6, 0.6, 0.6, 1};
  GLfloat light_diffuse[] = {0.6, 0.6, 0.6, 1};
  GLfloat light_ambient[] = {0.4, 0.4, 0.4, 1};
  GLfloat light_position[] = {50.0, 75.0, 80.0, 1.0};
  // z buffer enable
  glEnable(GL_DEPTH_TEST);
  // enable lighting
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_NORMALIZE);
  // set light property
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}

int main() {
  initOpenGL();
  GLFWwindow* window = OpenGLContext::getWindow();

  // Init Camera helper
  Camera camera(glm::vec3(0, 5, 10));
  camera.initialize(OpenGLContext::getAspectRatio());
  // Store camera as glfw global variable for callbasks use
  glfwSetWindowUserPointer(window, &camera);

  // Main rendering loop
  while (!glfwWindowShouldClose(window)) {
    // Polling events.
    glfwPollEvents();
    // Update camera position and view
    camera.move(window);
    // GL_XXX_BIT can simply "OR" together to use.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /// TO DO Enable DepthTest
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(camera.getProjectionMatrix());
    // ModelView Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(camera.getViewMatrix());

#ifndef DISABLE_LIGHT
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearDepth(1.0f);
    light();
#endif

    /* TODO#4-2: Update
     *       You may update position and orientation of airplane here or not.
     *       Feel free to not follow TA's structure. However, don't violate the spec.
     *
     * Hint:
     * Note:
     *       You can use `ROTATE_SPEED` and `FLYING_SPEED` as the speed constant.
     *       If the rotate/flying speed is too slow or too fast, please change `ROTATE_SPEED` or `FLYING_SPEED` value.
     *       You should finish keyCallback first.
     */

    // if press the space the plane would move up ,forward, and also also rotate the propeller
    if (spacebarPressed) {
      objectY += objectSpeed * 0.3;

      objectZ -= objectSpeed * cos(ANGLE_TO_RADIAN(objectAngle));

      objectX -= objectSpeed * sin(ANGLE_TO_RADIAN(objectAngle));

      PropellerAngle += PropellerrotationSpeed;
      // set the range for wing angle
      if (wingAngle >= 20.0) {
        wingdirection = false;
      } else if (wingAngle <= -20.0) {
        wingdirection = true;
      }
      if (wingdirection) {
        wingAngle += rotationSpeed * 0.5;
      } else {
        wingAngle -= rotationSpeed * 0.5;
      }

      // set the maximum height the plane can reach
      if (objectY >= 10) {
        objectY = 10;
      }
    } else {  // when the space release the plane would fall down
      objectY -= std::abs(objectSpeed) * 0.3;
      if (objectY <= 0) {
        objectY = 0;
      }
    }
    // turn left
    if (keyLeftPressed) {
      objectAngle += rotationSpeed;
    }
    // turn right
    if (keyRightPressed) {
      objectAngle -= rotationSpeed;
    }
    // speed up the moving speed of plane when up is pressed
    if (keyUpPressed) {
      objectSpeed += 0.006;
      PropellerrotationSpeed += 2;
      rotationSpeed += 0.01;
      if (PropellerrotationSpeed >= 40) {
        PropellerrotationSpeed = 40;
      }
    }
    // speed down the moving speed of plane when the down is pressed
    if (keyDownPressed) {
      objectSpeed -= 0.006;
      PropellerrotationSpeed -= 0.005;
      rotationSpeed -= 0.01;
      if (objectSpeed <= 0) {
        objectSpeed = 0;
      }
      if (PropellerrotationSpeed <= 0) {
        PropellerrotationSpeed = 0;
      }
    }
    glPushMatrix();

    // draw the plane according to the position and rotation angle
    glTranslatef(objectX, objectY, objectZ);
    glRotatef(objectAngle, 0.0f, 1.0f, 0.0f);
    drawAirplaneBody();

    drawTetrahedron();
    DrawPropeller();
    glPopMatrix();
    // draw the propeller
    glPushMatrix();
    glTranslatef(objectX, objectY, objectZ);
    glRotatef(objectAngle, 0.0f, 1.0f, 0.0f);
    drawBlades();
    glPopMatrix();

    // draw the wing
    glPushMatrix();
    glTranslatef(objectX, objectY, objectZ);
    glRotatef(objectAngle, 0.0f, 1.0f, 0.0f);
    glRotatef(wingAngle, 0.0f, 0.0f, 1.0f);
    drawLeftWing();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(objectX, objectY, objectZ);
    glRotatef(objectAngle, 0.0f, 1.0f, 0.0f);
    glRotatef(wingAngle, 0.0f, 0.0f, -1.0f);
    drawRightWing();
    glPopMatrix();

    // Render a white board

    glPushMatrix();
    glScalef(3, 1, 3);
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glVertex3f(5.0f, 0.0f, -5.0f);
    glVertex3f(5.0f, 0.0f, 5.0f);
    glEnd();
    glPopMatrix();

    /* TODO#3: Render the airplane
     *       1. Render the body.
     *       2. Render the wings.(Don't forget to assure wings rotate at the center of body.)
     *       3. Render the tail.
     * Hint:
     *       glPushMatrix/glPopMatrix (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glPushMatrix.xml)
     *       glRotatef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glRotate.xml)
     *       glTranslatef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glTranslate.xml)
     *       glColor3f (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glColor.xml)
     *       glScalef (https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glScale.xml)
     * Note:
     *       You may implement functions for drawing components of airplane first
     *       You should try and think carefully about changing the order of rotate and translate
     */

#ifdef __APPLE__
    // Some platform need explicit glFlush
    glFlush();
#endif
    glfwSwapBuffers(window);
  }
  return 0;
}
