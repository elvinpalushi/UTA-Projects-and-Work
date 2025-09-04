#include "Shape.h"

std::string Shape::name()
{
    return "Shape";
}

double Shape::area()
{
    return 0.0;
}

std::string Shape::to_string()
{
    return name() + " has an area of " + std::to_string(area());
}