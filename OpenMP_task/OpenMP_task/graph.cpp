#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <GLFW/glfw3.h>

void matrixMultiplication(int* a, int* b, int* c, int rowsA, int colsA, int colsB) {
    // Тот же код, что и в предыдущем примере
}

void benchmark(int numThreads) {
    // Здесь вы можете реализовать ваш код для измерения времени выполнения
    // в зависимости от количества параллельных потоков
    // ...

    std::this_thread::sleep_for(std::chrono::seconds(2)); // Имитация выполнения программы
}

void drawGraph(std::vector<double> data) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Performance Graph", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 0.0f, 0.0f); // Красный цвет линии
        for (int i = 0; i < data.size(); ++i) {
            glVertex2f(static_cast<float>(i), static_cast<float>(data[i]));
        }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents(); // Обработка событий ввода

        // Добавлено условие для правильного закрытия окна
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window)) {
            break;
        }
    }

    glfwTerminate();
}

int graph() {
    std::vector<double> performanceData;

    // Измерение времени выполнения для различных количеств потоков
    for (int numThreads = 1; numThreads <= 8; ++numThreads) {
        std::cout << "Benchmarking with " << numThreads << " threads" << std::endl;

        auto start = std::chrono::high_resolution_clock::now();

        // Запуск бенчмарка с заданным количеством потоков
        benchmark(numThreads);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        double executionTime = static_cast<double>(duration) / 1000.0; // Преобразование в секунды

        performanceData.push_back(executionTime);
    }

    // Отрисовка графика
    drawGraph(performanceData);

    return 0;
}