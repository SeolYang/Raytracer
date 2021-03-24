#pragma once
#include <Core/Material.h>
#include <Core/Texture.h>

class DiffuseLight : public Material
{
public:
   DiffuseLight(std::shared_ptr<Texture> emit) :
      m_emit(emit)
   {
   }

   DiffuseLight(const Color& emit) :
      DiffuseLight(std::make_shared<SolidColorTexture>(emit))
   {
   }

   bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
   {
      return false;
   }

   Color Emitted(double u, double v, const Point3& p) const override
   {
      if (m_emit != nullptr)
      {
         return m_emit->Value(u, v, p);
      }

      return Material::Emitted(u, v, p);
   }

private:
   std::shared_ptr<Texture> m_emit;

};