#pragma once
#include <Math/Vec3.h>

class Ray
{
public:
   Ray()
   {
   }

   Ray(const Point3& origin, const Vec3& direction, double time = 0.0) :
      Origin(origin),
      Direction(direction),
      Time(time)
   {
   }

   Point3 At(double t) const
   {
      return Origin + (t * Direction);
   }

public:
   Point3 Origin;
   Vec3 Direction;
   double Time = 0.0;

};