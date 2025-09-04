#include "Circle.h"
#include <cmath>

Circle::Circle(double r) : radius(r) {}

std::string Circle::name()
{
    return "Circle with radius " + std::to_string(radius);
}

double Circle::area()
{
    return M_PI * radius * radius;
}