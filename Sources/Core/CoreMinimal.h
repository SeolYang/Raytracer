#pragma once
#include <memory>
#include <iostream>
#include <vector>
#include <random>

#define STBI_MSC_SECURE_CRT
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

constexpr double Infinity = std::numeric_limits<double>::infinity();
constexpr double Pi = 3.1415926535897932385;

inline double DegreesToRadians(double deg)
{
   return deg * (Pi / 180.0f);
}

inline double RadiansToDegrees(double rad)
{
   return rad * (180.0f / Pi);
}

inline double RandomDouble()
{
   static std::uniform_real_distribution<double> distribution(0.0, 1.0);
   static std::mt19937 gen;
   return distribution(gen);
}

inline double RandomDouble(double min, double max)
{
   return min + ((max - min) * RandomDouble());
}