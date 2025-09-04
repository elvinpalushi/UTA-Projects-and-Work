#ifndef __SHAPE_H
#define __SHAPE_H

#include <string>

class Shape
{
    public:
        virtual std::string name() = 0;
        virtual double area() = 0;
        std::string to_string();
};

#endif