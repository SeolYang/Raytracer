#pragma once
#include <Math/Vec3.h>

class Ray
{
public:
   Ray() :
      Origin(Point3(0.0, 0.0, 0.0)),
      Direction(Vec3(0.0, 0.0, 1.0))
   {
   }

   Ray(const Point3& origin, const Vec3& direction) :
      Origin(origin),
      Direction(direction)
   {
   }

   Point3 At(double t) const
   {
      return Origin + (t * Direction);
   }

public:
   Point3 Origin;
   Vec3 Direction;

};