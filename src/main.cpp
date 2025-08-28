/*
Copyright 2025 Corentin Vaillant
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Camera.hpp"
#include "GlBuffer.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"
#include "constants.hpp"
#include "gl_utils.hpp"
#include "macros.hpp"

// Rendering

FPSPerspectiveCam camera;

// Physics

float dt, lastFrame;

// Mouse

float cursorLastX, cursorLastY, cameraYaw, cameraPitch;

bool firstMouse;

// -- Functions --
void mouse_callback(GLFWwindow*, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow*, int width, int height);

// -- Initializers --

void initGlfw() {
  expect_true(glfwInit(), "Failed to initialize GLFW", -1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* initWindow() {
  auto window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  expect_ptr(window, "Failed to create GLFW window", -1);
  glfwMakeContextCurrent(window);
  expect_true(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
              "Failed to initialize GLAD", -1);

  glViewport(0, 0, 800, 600);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);

  return window;
}

// -- Callbacks --
void mouse_callback(GLFWwindow*, double xpos, double ypos) {
  float& lastX = cursorLastX;
  float& lastY = cursorLastY;
  float& camYaw = cameraYaw;
  float& camPitch = cameraPitch;

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.002f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  camYaw += xoffset;
  camPitch += yoffset;

  float limit = glm::half_pi<float>() - 0.1f;  // π/2 - 0.1
  if (camPitch > limit) camPitch = limit;
  if (camPitch < -limit) camPitch = -limit;

  glm::vec3 direction;
  direction.x = cos(camYaw) * cos(camPitch);
  direction.y = sin(camPitch);
  direction.z = sin(camYaw) * cos(camPitch);
  camera.setFront(glm::normalize(direction));
}

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
  glViewport(0, 0, width, height);

  camera.setAspect(static_cast<float>(width) / static_cast<float>(height));
}

// -- Utils --
glm::vec2 getResolution(GLFWwindow* window) {
  int width, heigth;
  glfwGetWindowSize(window, &width, &heigth);
  return glm::vec2(static_cast<float>(width), static_cast<float>(heigth));
}

// -- Updates --
void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = 10.0f * dt;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.translate(cameraSpeed * camera.getFront());
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.translate(-cameraSpeed * camera.getFront());
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.translate(
        -glm::normalize(glm::cross(camera.getFront(), camera.getUp())) *
        cameraSpeed);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.translate(
        glm::normalize(glm::cross(camera.getFront(), camera.getUp())) *
        cameraSpeed);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    camera.translate(camera.getUp() * cameraSpeed);
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    camera.translate(-camera.getUp() * cameraSpeed);

  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    std::cout << "[time : " << lastFrame << "] FPS : " << 1.0 / dt << std::endl;
}

int main() {
  // MARK: Init
  {
    initGlfw();
    auto window = initWindow();

    //- Init VAO and VBO (vertex array object, vertex buffer object)

    VertexArray cubeVAO = VertexArray();

    VertexBuffer<GLfloat> VBO = VertexBuffer<GLfloat>(vertices, VERTICES_SIZE);

    VertexLayout layout =
        VertexLayout().push<GLfloat>(3).push<GLfloat>(3).push<GLfloat>(2);
    cubeVAO.addBuffer(VBO, layout);

    // second, configure the light's VAO (VBO stays the same; the vertices are
    // the same for the light object which is also a 3D cube)
    VertexArray lightCubeVAO = VertexArray();

    lightCubeVAO.addBuffer(VBO, layout);

    // Textures
    Texture diffuse("../resources/container2.png", GL_RGBA, GL_RGBA);
    Texture specular("../resources/container2_specular.png", GL_RGBA, GL_RGBA);
    Texture emission("../resources/matrix.jpg", GL_RGBA, GL_RGB);

    // Programs
    Program cubeProgram(Program(cubeVertShaderSrc, cubeFragShaderSrc));
    Program lightProgram(Program(cubeVertShaderSrc, lightFragShaderSrc));

    // - Draw parameters
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    lastFrame = glfwGetTime();
    dt = 0;

    VBO.unbind();
    cubeVAO.unbind();
    lightCubeVAO.unbind();

    throwOnGlError("Error in init");

    // MARK: Update

    while (!glfwWindowShouldClose(window)) {
      float time = glfwGetTime();
      dt = time - lastFrame;
      lastFrame = time;

      // updates
      processInput(window);

      // renders
      const glm::vec3 CLEAR_COLOR = glm::vec3(0.1f);
      glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glm::mat4 projection = camera.getProjectionMat();
      glm::mat4 view = camera.getViewMat();

      // Cubes

      cubeVAO.bind();
      for (int i(0); i < CUBE_POSITION_NUMBER; i++) {
        auto cubePos = cubePositions[i];
        glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePos);
        model = glm::rotate(
            model, time,
            glm::vec3(cos(i), sin(i),
                      (static_cast<float>(i) / CUBE_POSITION_NUMBER)));

        cubeProgram.setUniform3f("objectColor", 1.0f, 0.5f, 0.31f);

        // Sun
        cubeProgram.setUniform3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
        cubeProgram.setUniform3f("dirLight.ambient", CLEAR_COLOR);
        cubeProgram.setUniform3f("dirLight.diffuse", CLEAR_COLOR);
        cubeProgram.setUniform3f("dirLight.specular", CLEAR_COLOR);

        // Points
        char* attribName = new char[32];
        for (uint j(0); j < POINT_LIGHT_POSITION_NUMBER; j++) {
          auto timePalette = palette(time / (j + 2));

          snprintf(attribName, 32, "pointLights[%d].%s", j, "position");
          cubeProgram.setUniform3f(attribName, pointLightPositions[j]);

          snprintf(attribName, 32, "pointLights[%d].%s", j, "constant");
          cubeProgram.setUniform1f(attribName, 1.0f);

          snprintf(attribName, 32, "pointLights[%d].%s", j, "linear");
          cubeProgram.setUniform1f(attribName, 0.09f);

          snprintf(attribName, 32, "pointLights[%d].%s", j, "quadratic");
          cubeProgram.setUniform1f(attribName, 0.032f);

          snprintf(attribName, 32, "pointLights[%d].%s", j, "ambient");
          cubeProgram.setUniform3f(attribName, timePalette);

          snprintf(attribName, 32, "pointLights[%d].%s", j, "diffuse");
          cubeProgram.setUniform3f(attribName, CLEAR_COLOR);

          snprintf(attribName, 32, "pointLights[%d].%s", j, "specular");
          cubeProgram.setUniform3f(attribName, timePalette);
        }
        delete[] attribName;

        // Spot
        const glm::vec3 CAMERA_SPOT_COLOR(1.0f);
        cubeProgram.setUniform3f("spotLight.position", camera.getPosition());
        cubeProgram.setUniform3f("spotLight.direction", camera.getFront());
        cubeProgram.setUniform1f("spotLight.cutOff",
                                 glm::cos(glm::radians(12.5f)));
        cubeProgram.setUniform1f("spotLight.outerCutOff",
                                 glm::cos(glm::radians(15.0f)));

        cubeProgram.setUniform3f("spotLight.ambient", CLEAR_COLOR);
        cubeProgram.setUniform3f("spotLight.diffuse", CAMERA_SPOT_COLOR);
        cubeProgram.setUniform3f("spotLight.specular", CAMERA_SPOT_COLOR);

        cubeProgram.setUniform1f("spotLight.constant", 1.0f);
        cubeProgram.setUniform1f("spotLight.linear", 0.09f);
        cubeProgram.setUniform1f("spotLight.quadratic", 0.032f);

        cubeProgram.setUniform3f("viewPos", camera.getPosition());

        cubeProgram.setUniformTexture2D("material.diffuse", diffuse);
        cubeProgram.setUniformTexture2D("material.specular", specular);
        cubeProgram.setUniformTexture2D("material.emission", emission);
        cubeProgram.setUniform1f("material.shininess", 32.0f);

        cubeProgram.setUniformMat4fv("view", glm::value_ptr(view));
        cubeProgram.setUniformMat4fv("projection", glm::value_ptr(projection));
        cubeProgram.setUniformMat4fv("model", glm::value_ptr(model));

        cubeProgram.useProgram();

        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      // Lamp

      for (uint i(0); i < POINT_LIGHT_POSITION_NUMBER; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.5f));

        auto timePalette = palette(time / (i + 2));
        lightProgram.setUniformMat4fv("model", glm::value_ptr(model));
        lightProgram.setUniformMat4fv("view", glm::value_ptr(view));
        lightProgram.setUniformMat4fv("projection", glm::value_ptr(projection));
        lightProgram.setUniform3f("color", timePalette);
        lightProgram.useProgram();

        lightCubeVAO.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      // check and call events and swap the buffers
      glfwSwapBuffers(window);
      glfwPollEvents();

      // check for errors
      throwOnGlError("error detected after update");
    }

    // MARK: Cleaning
    //[...]
  }
  glfwTerminate();
}
