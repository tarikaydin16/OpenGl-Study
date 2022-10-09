#ifndef SQUARE_HPP
#define SQUARE_HPP
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
class Square
{
public:
    enum DIRECTION
    {
        DIR_LEFT = 1,
        DIR_RIGHT = -1,
        DIR_UP = 2,
        DIR_DOWN = -2
    };

    Square(float x, float y, float length);
    glm ::vec4 getColor();
    glm ::vec3 getPosition();
    void setDirection(DIRECTION dir);
    DIRECTION getDirection();
    void move();

private:
    glm ::vec4 m_Color;
    glm ::vec3 m_Position;
    float m_Length;
    DIRECTION m_Direction;
};

#endif