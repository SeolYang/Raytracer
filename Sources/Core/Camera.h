#pragma once
#include <Core/CoreMinimal.h>
#include <Math/Ray.h>

class Camera
{
public:
   Camera(
      Point3 lookFrom,
      Point3 lookAt,
      Vec3 up,
      double verticalFov,
      double aspectRatio,
      double aperture,
      double focusDist,
      double time0 = 0.0,
      double time1 = 0.0)
   {
      // Camera
      auto theta = DegreesToRadians(verticalFov);
      auto h = std::tan(theta / 2.0);
      auto viewportHeight = 2.0 * h;
      auto viewportWidth = aspectRatio * viewportHeight;

      m_w = UnitVectorOf(lookFrom - lookAt);
      m_u = UnitVectorOf(Cross(up, m_w));
      m_v = Cross(m_w, m_u);

      m_position = lookFrom;
      m_horizontal = m_u * viewportWidth * focusDist;
      m_vertical = m_v * viewportHeight * focusDist;
      m_lowerLeftCorner = m_position - (m_horizontal / 2.0) - (m_vertical / 2.0) - (focusDist*m_w);

      m_lensRad = aperture / 2.0;

      m_time0 = time0;
      m_time1 = time1;
   }

   Ray GetRay(double s, double t) const
   {
      Vec3 rd = m_lensRad * RandomInUnitDisk();
      Vec3 offset = m_u * rd.x + m_v * rd.y;
      return Ray(m_position + offset, m_lowerLeftCorner + s * m_horizontal + t * m_vertical - m_position - offset,
         RandomDouble(m_time0, m_time1));
   }

private:
   Point3 m_position;
   Point3 m_lowerLeftCorner;
   Vec3 m_horizontal;
   Vec3 m_vertical;
   Vec3 m_u, m_v, m_w;
   double m_lensRad;
   double m_time0, m_time1;

};
