#pragma once
#include <Core/CoreMinimal.h>
#include <Core/Color.h>

struct HitRecord;
class Ray;
class Material
{
public:
   virtual bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;

};