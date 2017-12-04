#ifndef CIRCLE_H
#define CIRCLE_H

#include <QtOpenGL>
#include "Point2.h"
#include "Point3.h"

class Circle
{
public:
    Circle(float radius);
    void draw();

private:
    float radius;
};

#endif // CIRCLE_H
