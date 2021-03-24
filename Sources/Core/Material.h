#pragma once
#include <Core/CoreMinimal.h>
#include <Core/Color.h>

struct HitRecord;
class Ray;
class Material
{
public:
   virtual Color Emitted(double u, double v, const Point3& p) const { return Color(); }
   virtual bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;

};