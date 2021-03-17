#pragma once
#include <Core/CoreMinimal.h>
#include <Math/Vec3.h>

using Color = Vec3;

static void WriteColor(std::unique_ptr<unsigned char[]>& buffer, Color color, size_t base, size_t samplesPerPixel)
{
   auto scale = 1.0 / static_cast<double>(samplesPerPixel);
   double r = std::sqrt(color.r * scale);
   double g = std::sqrt(color.g * scale);
   double b = std::sqrt(color.b * scale);

   buffer[base + 0] = static_cast<unsigned char>(256.0 * std::clamp(r, 0.0, 0.999));
   buffer[base + 1] = static_cast<unsigned char>(256.0 * std::clamp(g, 0.0, 0.999));
   buffer[base + 2] = static_cast<unsigned char>(256.0 * std::clamp(b, 0.0, 0.999));
}
