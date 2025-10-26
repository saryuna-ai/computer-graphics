#include <glew.h>
#include <glfw3.h>
#include <iostream>

void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

void drawTrapezoid() {
    glBegin(GL_QUADS);

    glColor3f(0.5f, 0.5f, 1.0f);

    glVertex2f(-0.6f, -0.4f);
    glVertex2f(0.6f, -0.4f);

    glVertex2f(0.3f, 0.4f);
    glVertex2f(-0.3f, 0.4f);

    glEnd();
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Trapezoid Example", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawTrapezoid();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}