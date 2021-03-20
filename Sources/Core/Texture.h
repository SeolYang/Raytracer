#pragma once
#include <Math/Vec3.h>
#include <Core/Color.h>

class Texture
{
public:
   virtual Color Value(double u, double v, const Point3& p) const = 0;
};

class SolidColorTexture : public Texture
{
public:
   SolidColorTexture() = default;
   SolidColorTexture(const Color& color) :
      ColorValue(color)
   {
   }

   SolidColorTexture(double r, double g, double b) :
      ColorValue(Color(r, g, b))
   {
   }

   Color Value(double u, double v, const Point3& p) const override
   {
      return ColorValue;
   }

public:
   Color ColorValue;

};

class CheckerTexture : public Texture
{
public:
   CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd) :
      EvenAlbedo(even), OddAlbedo(odd)
   {
   }

   CheckerTexture(const Color& even, const Color& odd) :
      EvenAlbedo(std::make_shared<SolidColorTexture>(even)),
      OddAlbedo(std::make_shared<SolidColorTexture>(odd))
   {
   }

   Color Value(double u, double v, const Point3& p) const override
   {
      auto sines = std::sin(10.0 * p.x) * std::sin(10.0 * p.y) * std::sin(10.0 * p.z);
      if (sines < 0.0)
      {
         return OddAlbedo->Value(u, v, p);
      }

      return EvenAlbedo->Value(u, v, p);
   }

public:
   std::shared_ptr<Texture> EvenAlbedo;
   std::shared_ptr<Texture> OddAlbedo;

};