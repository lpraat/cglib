// glad, include glad *before* glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {

    // Initialize glfw
    glfwInit();

    // Set some options
    // See https://www.glfw.org/docs/latest/window.html#window_hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window object that holds all the windowing data
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // We pass GLAD the function to load the address of the OpenGL function pointers
    // which is OS-specific. GLFW gives us glfwGetProcAddress that defines the correct
    // function based on which OS we're compiling for.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Tell opengl the size of the rendering window
    glViewport(0, 0, 800, 600);

    // Set callback for resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window)) {

        // Input
        processInput(window);

        // Rendering commands here

        // At the start of each render iteration we always want to clear the screen
        // otherwise we would still see the results from the previous iteration
        // We can clear the screen's color buffer using the glClear function
        // where we pass in buffer bits to specify which buffer we would like to clear
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // Whenever we call glCear and clear the color buffer, the entire colorbuffer will
        // be filled with the color as configured by glClearColor
        glClear(GL_COLOR_BUFFER_BIT);
        // The glClearColor function is a state-setting function and glClear is a state-using function

        // swap the color buffer(large buffer that contains color values for each pixel in the GLFW's window)
        // that that has been used to draw in during this iteration and show it to output screen
        glfwSwapBuffers(window);

        // checks if any events are triggered(like keyboard input, mouse movement events..)
        // update window state and calls the corresponding callbacks
        glfwPollEvents();
    }

    // clear resources

    glfwTerminate();

    return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    // check if the user has pressed the escape key(if it's not, glfwGetKey returns GLFW_RELEASE)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}