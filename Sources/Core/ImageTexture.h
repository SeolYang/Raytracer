#pragma once
#include <Core/Texture.h>

namespace ImageTextureConstants
{
   constexpr int BytesPerPixel = 3;
}

class ImageTexture : public Texture
{
public:
   ImageTexture() = default;
   ImageTexture(const std::string_view& fileName)
   {
      auto componentsPerPixel = ImageTextureConstants::BytesPerPixel;
      m_data = stbi_load(fileName.data(), &m_width, &m_height, &componentsPerPixel, componentsPerPixel);
      if (m_data != nullptr)
      {
         m_bytesPerScanline = ImageTextureConstants::BytesPerPixel * m_width;
      }
      else
      {
         std::cerr << "ERROR: Could not load texture image file from '" << fileName.data() << "'. \n";
      }
   }

   virtual ~ImageTexture()
   {
      delete m_data;
   }

   Color Value(double u, double v, const Point3& p) const override
   {
      if (m_data == nullptr)
      {
         return Color();
      }

      u = std::clamp(u, 0.0, 1.0);
      v = 1.0 - std::clamp(v, 0.0, 1.0);

      auto dx = static_cast<int>(u * m_width);
      auto dy = static_cast<int>(v * m_height);

      if (dx >= m_width)
      {
         dx = m_width - 1;
      }
      if (dy >= m_height)
      {
         dy = m_height - 1;
      }

      const auto colorScale = 1.0 / 255.0;
      auto pixel = &m_data[dy * m_bytesPerScanline + dx * ImageTextureConstants::BytesPerPixel];
      return Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
   }

private:
   unsigned char* m_data = nullptr;
   int m_width = 0, m_height = 0;
   int m_bytesPerScanline = 0;

};
