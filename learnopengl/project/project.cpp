// glad, include glad *before* glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core_types.h"
#include "vec4.h"
#include "vec3.h"
#include "mat4.h"
#include "quat.h"
#include "transform.h"
#include "shader_program.h"
#include "fps_camera.h"
#include "free_camera.h"
#include "perlin.h"
#include <cmath>
#include "constants.h"
#include "model.h"
#include "lights/directional.h"
#include "lights/spot.h"
#include "lights/point.h"
#include "cubemap.h"
#include "drone.h"
#include <string>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, glp::FreeCamera<float32> &camera, float32 deltaTime);
void processInput(GLFWwindow *window, glp::Drone<float32> &camera, float32 deltaTime);
uint32 loadTexture(const char *path);

const uint32 WIDTH = 1280;
const uint32 HEIGHT = 720;

glp::Drone<float32> droneCamera;
glp::FreeCamera<float32> camera;
float32 lastX = WIDTH / 2.0f;
float32 lastY = HEIGHT / 2.0f;
bool firstMouse = true;

int main()
{
    // Initialize glfw
    glfwInit();

    // Set some options
    // See https://www.glfw.org/docs/latest/window.html#window_hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window object that holds all the windowing data
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // We pass GLAD the function to load the address of the OpenGL function pointers
    // which is OS-specific. GLFW gives us glfwGetProcAddress that defines the correct
    // function based on which OS we're compiling for.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Tell opengl the size of the rendering window
    glViewport(0, 0, WIDTH, HEIGHT);

    // Set callback for resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);

    glp::ShaderProgram skyboxProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/skybox/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/skybox/fragment.glsl"
    );

    glp::ShaderProgram terrainProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/terrain/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/terrain/fragment.glsl");

    glp::ShaderProgram droneProgram(
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/drone/vertex.glsl",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/shaders/drone/fragment.glsl"
    );

    glp::Model terrain("/Users/lpraat/develop/computer_graphics/learnopengl/chapters/models/terrain/terrain.obj");
    terrain.getNodes()[1].meshes[0].addTexture("texture_diffuse", "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/models/terrain/diff2.jpg");
    terrain.getNodes()[1].meshes[0].addTexture("texture_normal", "/Users/lpraat/develop/computer_graphics/learnopengl/chapters/models/terrain/nrm.png");
    terrain.print();

    glp::Model drone("/Users/lpraat/develop/computer_graphics/learnopengl/chapters/models/drone/drone_obj.obj");
    drone.print();

    // glp::Model nano("/Users/lpraat/develop/computer_graphics/learnopengl/chapters/models/nanosuit/nanosuit.obj");
    // nano.print();

    // Skybox
    glp::CubeMap skybox(
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/skybox/right.jpg",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/skybox/left.jpg",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/skybox/top.jpg",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/skybox/bottom.jpg",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/skybox/front.jpg",
        "/Users/lpraat/develop/computer_graphics/learnopengl/project/skybox/back.jpg"

    );

    // Lights
    glp::DirectionalLight<float32> directionalLight {
        {0.05f, 0.05f, 0.05f},
        {0.4f, 0.4f, 0.4f},
        {0.5f, 0.5f, 0.5f},
        {0.5f, -1.0f, 0.8f}
    };

    glp::SpotLight<float32> spotLight {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        1.0f,
        0.09f,
        0.032f,
        std::cos(12.5f * glp::toRadians()),
        std::cos(17.5f * glp::toRadians())
    };

    terrainProgram.use();
    terrainProgram.setFloat("shininess", 32.0f);

    // Directional light
    terrainProgram.setVec3("dirLight.direction", directionalLight.dir);
    terrainProgram.setVec3("dirLight.ambient", directionalLight.ambient);
    terrainProgram.setVec3("dirLight.diffuse", directionalLight.diffuse);
    terrainProgram.setVec3("dirLight.specular", directionalLight.specular);

    // Spotlight
    terrainProgram.setVec3("spotLight.ambient", spotLight.ambient);
    terrainProgram.setVec3("spotLight.diffuse", spotLight.diffuse);
    terrainProgram.setVec3("spotLight.specular", spotLight.specular);
    terrainProgram.setFloat("spotLight.constant", spotLight.constant);
    terrainProgram.setFloat("spotLight.linear", spotLight.linear);
    terrainProgram.setFloat("spotLight.quadratic", spotLight.quadratic);
    terrainProgram.setFloat("spotLight.cutOff", spotLight.cutOff);
    terrainProgram.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);

    droneProgram.use();
    droneProgram.setFloat("shininess", 32.0f);

    // Directional light
    droneProgram.setVec3("dirLight.direction", directionalLight.dir);
    droneProgram.setVec3("dirLight.ambient", directionalLight.ambient);
    droneProgram.setVec3("dirLight.diffuse", directionalLight.diffuse);
    droneProgram.setVec3("dirLight.specular", directionalLight.specular);

    // Spotlight
    droneProgram.setVec3("spotLight.ambient", spotLight.ambient);
    droneProgram.setVec3("spotLight.diffuse", spotLight.diffuse);
    droneProgram.setVec3("spotLight.specular", spotLight.specular);
    droneProgram.setFloat("spotLight.constant", spotLight.constant);
    droneProgram.setFloat("spotLight.linear", spotLight.linear);
    droneProgram.setFloat("spotLight.quadratic", spotLight.quadratic);
    droneProgram.setFloat("spotLight.cutOff", spotLight.cutOff);
    droneProgram.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);

    // Set projection matrix
    glp::Mat4 projection{glp::perspectiveProjection(45.0f, 0.1f, 1000.0f, static_cast<float32>(WIDTH) / HEIGHT)};

    terrainProgram.use();
    terrainProgram.setMat4("projection", projection);

    droneProgram.use();
    droneProgram.setMat4("projection", projection);

    skyboxProgram.use();
    skyboxProgram.setMat4("projection", projection);


    float32 deltaTime = 0.0f;
    float32 lastFrame = 0.0f;
    float32 currFrame;

    // Use z-buffer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    bool lookAtMode = false;

    glp::Mat4<float32> lookAtView = glp::Mat4<float32>::identity();
    glp::Mat4<float32> cameraView = glp::Mat4<float32>::identity();

    while (!glfwWindowShouldClose(window))
    {
        currFrame = glfwGetTime();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        {
            lookAtMode = !lookAtMode;
        }

        // Input

        if (!lookAtMode) {
            processInput(window, camera, deltaTime);
            cameraView = camera.getView();
        } else {
            processInput(window, droneCamera, deltaTime);
            droneCamera.updatePosition();
            droneCamera.updateOrientation();
        }

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Drone
        droneProgram.use();
        droneProgram.setVec3("spotLight.position", spotLight.position);
        droneProgram.setVec3("spotLight.direction", spotLight.direction);
        droneProgram.setVec3("viewPos", droneCamera.getPosition());


        glp::Quat<float32> orientation = droneCamera.getOrientation();
        glp::Vec3<float32> position = droneCamera.getPosition();

        std::cout << "Drone position" << std::endl;

        glp::Mat4<float32> model = glp::translate(position).dot(orientation.toRotMatrix()).dot(glp::scale(10.0f));

        glp::Quat<float32> d {0, 0, 0.0f, 1.0f};
        glp::Quat<float32> dd = orientation * d * orientation.conjugate();
        glp::Vec3<float32> ddd = {dd.x, dd.y, dd.z};

        glp::Quat<float32> up {0.0f, 0.0f, 1.0f, 0.0f};
        glp::Quat<float32> uup = orientation * up * orientation.conjugate();
        glp::Vec3<float32> uuup = {uup.x, uup.y, uup.z};

        glp::Vec3<float32> lookAtPosition = position + 10*ddd + 4.0*uuup;

        glp::Vec3<float32> target = position;
        lookAtView = glp::lookAt(lookAtPosition, target,  uuup);

        droneProgram.setVec3("viewPos", droneCamera.getPosition());


        if (lookAtMode) {
            droneProgram.setMat4("view", lookAtView);
        } else {
            droneProgram.setMat4("view", cameraView);
        }


        drone.getNodes()[0].model = model;

        //glp::Quat<float32> q((float)glfwGetTime() * -50.0f, glp::Vec3<float32> {0, 1, 0});
      //  drone.getNodes()[1].model = q.toRotMatrix();
        drone.getNodes()[3].model = glp::translate(0.25f, 0.0f, 0.0f).dot(glp::rotateY((float)glfwGetTime() * -1000.0f)).dot(glp::translate(-0.25f, -0.0f, -0.0f));
        drone.getNodes()[5].model = glp::translate(-0.25f, 0.0f, 0.0f).dot(glp::rotateY((float)glfwGetTime() * -1000.0f)).dot(glp::translate(+0.25f, -0.0f, -0.0f));
      //  glp::translate(0.1f, 0.0f, 0.0f).dot(q.toRotMatrix()).dot(glp::translate());
        // drone.getNodes()[6].model = q.toRotMatrix().dot(glp::scale(0.2f));
        // drone.getNodes()[7].model = q.toRotMatrix().dot(glp::scale(0.2f));
        // drone.getNodes()[8].model = q.toRotMatrix().dot(glp::scale(0.2f));

        drone.updateModelMatrices();
        // Set model
       // droneProgram.setMat3("normalMatrix", droneModel.mat3().transposedInverse());
        drone.draw(droneProgram);

        drone.resetModelMatrices();

        // Terrain
        terrainProgram.use();
        terrainProgram.setVec3("spotLight.position", spotLight.position);
        terrainProgram.setVec3("spotLight.direction", spotLight.direction);
      //  terrainProgram.setVec3("viewPos", camera.getPosition());

        if (lookAtMode) {
            terrainProgram.setMat4("view", lookAtView);
        } else {
            terrainProgram.setMat4("view", cameraView);
        }

        // TODO CARE THEY ARE SET IN MESH NOW
        // glp::Mat4<float32> terrainModel = glp::translate(0.0f, -0.75f, 0.0f);
        // terrainProgram.setMat4("model", terrainModel);
        // terrainProgram.setMat3("normalMatrix", terrainModel.mat3().transposedInverse());

        terrain.draw(terrainProgram);
        // // Skybox
        skyboxProgram.use();

        if (lookAtMode) {
            // Remove translation from the view matrix
            lookAtView.w0 = 0; lookAtView.w1 = 0; lookAtView.w2 = 0; lookAtView.w3 = 1;
            lookAtView.x3 = 0; lookAtView.y3 = 0; lookAtView.z3 = 0;

            skyboxProgram.setMat4("view", lookAtView);
        } else {
            // Remove translation from the view matrix
            cameraView.w0 = 0; cameraView.w1 = 0; cameraView.w2 = 0; cameraView.w3 = 1;
            cameraView.x3 = 0; cameraView.y3 = 0; cameraView.z3 = 0;

            skyboxProgram.setMat4("view", cameraView);
        }

        skybox.draw(skyboxProgram);


        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clear resources
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, glp::FreeCamera<float32> &camera, float32 deltaTime)
{
    // check if the user has pressed the escape key(if it's not, glfwGetKey returns GLFW_RELEASE)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::PITCH_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::PITCH_M, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::YAW_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::YAW_M, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::ROLL_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::ROLL_M, deltaTime);
    }
}

void processInput(GLFWwindow *window, glp::Drone<float32> &camera, float32 deltaTime)
{
    // check if the user has pressed the escape key(if it's not, glfwGetKey returns GLFW_RELEASE)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.updateDeltaPosition(glp::FreeCameraMovement::DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::PITCH_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::PITCH_M, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::YAW_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::YAW_M, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::ROLL_P, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        camera.updateDeltaOrientation(glp::FreeCameraMovement::ROLL_M, deltaTime);
    }
}