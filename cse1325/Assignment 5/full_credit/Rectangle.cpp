#include "Rectangle.h"
#include <iostream>

Rectangle::Rectangle(double h, double w) : height(h), width(w) {}

std::string Rectangle::name()
{
    return "Rectangle with dimenstions " + std::to_string(height) + " by " + std::to_string(width);
}

double Rectangle::area()
{
    return height * width;
}