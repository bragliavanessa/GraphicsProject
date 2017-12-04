#include "Circle.h"

const float DEG2RAD = 3.14159/180;

Circle::Circle(float radius){}

void Circle::draw()
{
   glBegin(GL_LINE_LOOP);

   for (int i=0; i<=360; i++)
   {
      float degInRad = i*DEG2RAD;
      glVertex2f(cos(degInRad)*radius,sin(degInRad)*radius);
   }

   glEnd();
}
