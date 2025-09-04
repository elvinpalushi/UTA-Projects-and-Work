#include "Shape.h"

std::string Shape::to_string()
{
    return name() + " has an area of " + std::to_string(area());
}