#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <thread> // std::this_thread::sleep_for
#include <chrono>
#include <vector>
#include "square.hpp"
#include "shaderprogram.hpp"

float length = 0.08;
bool isCollide;
// noktalara ait koordinat bilgileri.
float vertices[] = {
    -length / 2, length / 2, 0.0f,
    -length / 2, -length / 2, 0.0f,
    length / 2, -length / 2, 0.0f,

    -length / 2, length / 2, 0.0f,
    length / 2, -length / 2, 0.0f,
    length / 2, length / 2, 0.0f

};

std::vector<Square *> snakeList;

// OpenGL nesnelerinin id değerlerini tutacak olan değişkenler

unsigned int VBO;
unsigned int VAO;

bool isSnakeDead()
{
    // Creating the variables for simplicity
    float headX = snakeList[0]->getPosition().x;
    float headY = snakeList[0]->getPosition().y;

    for (int i = snakeList.size() - 1; i > 0; i--)
    {
        float currentX = snakeList[i]->getPosition().x;
        float currentY = snakeList[i]->getPosition().y;

        if (headX == currentX && headY == currentY)
        {
            // The head collides with the current body part
            return true;
        }
    }

    return false;
}
void moveSnake()
{
    if (!isCollide)
        for (auto next : snakeList)
        {
            next->move();
        }
    for (int i = snakeList.size() - 1; i > 0; i--)
    {
        snakeList[i]->setDirection(snakeList[i - 1]->getDirection());
        if (isSnakeDead())
        {
            std::cout << "caripsti";
            isCollide = true;
        }
    }
}

void drawSnake(ShaderProgram &program) // referans olarak gönderdik
{
    for (auto next : snakeList)
    {
        program.setVec3("uMove", next->getPosition()); // next adres olduğu için -> kullanarak fonk. çağırıyoruz.
        program.setVec4("uColor", next->getColor());
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
void addToSnakeTail()
{
    int elementCount = snakeList.size();
    if (elementCount == 0)
    {
        snakeList.push_back(new Square(0.0f, 0.0f, length));
    }
    else
    {
        Square *lastSquare = snakeList[elementCount - 1];
        glm::vec3 pos = lastSquare->getPosition();
        Square::DIRECTION dir = lastSquare->getDirection();

        switch (dir)
        {
        case Square::DIR_RIGHT:
            pos -= glm::vec3(length, 0.0f, 0.0f);
            break;
        case Square::DIR_LEFT:
            pos += glm::vec3(length, 0.0f, 0.0f);
            break;
        case Square::DIR_UP:
            pos -= glm::vec3(0.0f, length, 0.0f);
            break;
        case Square::DIR_DOWN:
            pos += glm::vec3(0.0f, length, 0.0f);
            break;
        }
        Square *newSquare = new Square(pos.x, pos.y, length);
        newSquare->setDirection(dir);
        snakeList.push_back(newSquare);
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_ESCAPE)
        glfwTerminate();
    if (action == GLFW_PRESS)
    {
        if (snakeList.size() != 0)
        {
            Square *first = snakeList[0];
            if (key == GLFW_KEY_A)
                first->setDirection(Square::DIR_LEFT);
            if (key == GLFW_KEY_D)
                first->setDirection(Square::DIR_RIGHT);
            if (key == GLFW_KEY_S)
                first->setDirection(Square::DIR_DOWN);
            if (key == GLFW_KEY_W)
                first->setDirection(Square::DIR_UP);
        }
        if (key == GLFW_KEY_SPACE)
            addToSnakeTail();
    }
}

int main(int argc, char **argv)
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(800, 800, "İLk mainn", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Pencere Olusturulamadi" << std::endl;

        glfwTerminate();

        return -1;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    addToSnakeTail();
    addToSnakeTail();
    addToSnakeTail();
    addToSnakeTail();

    ShaderProgram program;
    program.attachShader("../../ders4/shaders/simplevs.glsl", GL_VERTEX_SHADER);
    program.attachShader("../../ders4/shaders/simplefs.glsl", GL_FRAGMENT_SHADER);
    program.link();

    program.addUniform("uMove");
    program.addUniform("uColor");

    // vertex array object oluşturuluyor
    glGenVertexArrays(1, &VAO);
    // vertex buffer object oluşuruluyor
    glGenBuffers(1, &VBO);

    // vertex array object aktif edildi.
    glBindVertexArray(VAO);
    // vertex buffer nesnesi aktif edildi.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // vertex buffer'a nokta bilgileri ytollanıyor
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // gönderilen vertex'e ait özellikler etiketleniyor
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    //özellik  etiket i aktif ediliyor.
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        // oluşturulacak resim başlangıç rengine boyanıyor
        glClearColor(0.0f, 0.4f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //çizimde kullanılacak olan program nesnesi aktif ediliyor
        program.use();

        //çizimde kullanılacak olan Vertex array object aktif ediliyor
        glBindVertexArray(VAO);

        drawSnake(program);
        moveSnake();

        std::this_thread::sleep_for(std::chrono::milliseconds(70));
        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}