#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <cmath>

// Шейдеры
const char* vertexShaderSource = R"(
#version 410 core
layout (location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 410 core
out vec4 FragColor;
uniform vec4 ourColor;
void main() {
    FragColor = ourColor;
}
)";

// Обработчик ошибок
void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

// Компиляция шейдера
unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Проверка компиляции
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

// Создание шейдерной программы
unsigned int createShaderProgram() {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Проверка линковки
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    // Настройка версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(640, 480, "VBO/VAO/EBO Trapezoid", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Инициализация GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Вершины трапеции
    float vertices[] = {
        // x, y, z
        -0.6f, -0.4f, 0.0f,  // нижняя левая
         0.6f, -0.4f, 0.0f,  // нижняя правая
         0.3f,  0.4f, 0.0f,  // верхняя правая
        -0.3f,  0.4f, 0.0f   // верхняя левая
    };

    // Индексы для EBO
    unsigned int indices[] = {
        0, 1, 2,  // первый треугольник
        2, 3, 0   // второй треугольник
    };

    // Создание VBO, VAO, EBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Настройка VAO
    glBindVertexArray(VAO);

    // Настройка VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Настройка EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Настройка вершинных атрибутов
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Отвязка буферов
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Создание шейдерной программы
    unsigned int shaderProgram = createShaderProgram();

    // Основной цикл
    while (!glfwWindowShouldClose(window)) {
        // Очистка экрана
        glClearColor(1.0f, 1.0f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Использование шейдерной программы
        glUseProgram(shaderProgram);

        // Задание №1: Анимация цвета с помощью uniform переменной
        float timeValue = glfwGetTime();
        float redValue = (sin(timeValue) / 2.0f) + 0.5f;
        float greenValue = (cos(timeValue * 1.5f) / 2.0f) + 0.5f;
        float blueValue = (sin(timeValue * 2.0f) / 2.0f) + 0.5f;

        // Получение location uniform переменной и установка цвета
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

        // Отрисовка трапеции
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Обмен буферов и обработка событий
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Очистка ресурсов
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}