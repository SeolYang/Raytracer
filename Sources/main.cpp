#include <Core/CoreMinimal.h>
#include <Core/Color.h>
#include <Core/Sphere.h>
#include <Core/HittableList.h>
#include <Core/Camera.h>
#include <Core/Material.h>
#include <Core/Lambertian.h>
#include <Core/Metal.h>
#include <Core/Dielectric.h>
#include <Math/Vec3.h>
#include <Math/Ray.h>

#include <iostream>

Color RayColor(const Ray& r, const Hittable& world, int depth)
{
	if (depth <= 0)
	{
		return Color(0.0, 0.0, 0.0);
	}

	HitRecord rec;
	if (world.Hit(r, 0.001, Infinity, rec))
	{
		Ray scattered;
		Color attenuation;
		if (rec.MatPtr->Scatter(r, rec, attenuation, scattered))
		{
			return attenuation * RayColor(scattered, world, depth - 1);
		}

		return Color(1.0, 1.0, 1.0);
	}

	Vec3 unitDirection = UnitVectorOf(r.Direction);
	auto t = 0.5 * (unitDirection.y + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main()
{
	// Output Image
	constexpr double aspectRatio = 16.0 / 9.0;
	constexpr int imageWidth = 400;
	constexpr int imageHeight = static_cast<int>(imageWidth/aspectRatio);
	constexpr int imageChannels = 3; // RGB
	constexpr int samplesPerPixel = 100;
	constexpr int maximumDepth = 50;

	auto outputBuffer = std::make_unique<unsigned char[]>(imageWidth*imageHeight*imageChannels);

	// World
	auto groundMaterial = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	auto centerMaterial = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
	auto leftMaterial = std::make_shared<Dielectric>(1.5);
	auto rightMaterial = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

	HittableList world;
	world.Add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, groundMaterial));
	world.Add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, centerMaterial));
	world.Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, leftMaterial));
	world.Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.4, leftMaterial));
	world.Add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, rightMaterial));

	// Camera
	Point3 lookFrom(3.0, 3.0, 2.0);
	Point3 lookAt(0.0, 0.0, -1.0);
	Vec3 up(0.0, 1.0, 0.0);
	auto distToFocus = (lookFrom - lookAt).Length();
	auto aperture = 2.0;
	Camera cam(lookFrom, lookAt, up, 20, aspectRatio, aperture, distToFocus);

	// Render
#pragma omp parallel for schedule(dynamic, 1)
	for (int dy = imageHeight - 1; dy >= 0; --dy)
	{
		std::cerr << "\rScanlines Reamining : " << dy << ' ' << std::flush;
		for (int dx = 0; dx < imageWidth; ++dx)
		{
			Color pixelColor(0.0f, 0.0f, 0.0f);
			for (int ds = 0; ds < samplesPerPixel; ++ds)
			{
				auto u = (double(dx) + RandomDouble()) / (imageWidth - 1);
				auto v = (double(dy) + RandomDouble()) / (imageHeight - 1);
				Ray r = cam.GetRay(u, v);
				pixelColor += RayColor(r, world, maximumDepth);
			}

			size_t base = ((imageHeight - dy - 1) * imageWidth * imageChannels) + (dx * imageChannels);
			WriteColor(outputBuffer, pixelColor, base, samplesPerPixel);
		}
	}

	stbi_write_png("output.png", imageWidth, imageHeight, imageChannels, outputBuffer.get(), imageWidth * imageChannels);
	std::cerr << "\nRendering Done\n";

   return 0;
}