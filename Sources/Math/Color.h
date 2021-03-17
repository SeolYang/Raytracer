#pragma once
#include <Core/CoreMinimal.h>
#include <Math/Vec3.h>

using Color = Vec3;

static void WriteColor(std::unique_ptr<unsigned char[]>& buffer, Color color, size_t base)
{
   buffer[base + 0] = static_cast<unsigned char>(255.999 * color.r);
   buffer[base + 1] = static_cast<unsigned char>(255.999 * color.g);
   buffer[base + 2] = static_cast<unsigned char>(255.999 * color.b);
}
