#include <Core/CoreMinimal.h>
#include <Core/Color.h>
#include <Core/Sphere.h>
#include <Core/HittableList.h>
#include <Core/Camera.h>
#include <Core/Material.h>
#include <Core/Lambertian.h>
#include <Core/Metal.h>
#include <Core/Dielectric.h>
#include <Core/MovingSphere.h>
#include <Core/Texture.h>
#include <Core/ImageTexture.h>
#include <Core/DiffuseLight.h>
#include <Core/Rect.h>
#include <Core/Box.h>
#include <Core/Instance.h>
#include <Core/ConstantMedium.h>
#include <Core/BVHNode.h>
#include <Math/Vec3.h>
#include <Math/Ray.h>
#include <iostream>

std::unique_ptr<HittableList> RandomScene(double shutterOpen = 0.0, double shutterClose = 1.0)
{
	auto world = std::make_unique<HittableList>();
	auto groundMaterial = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
	auto checkerTexture = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	auto checkerMat = std::make_shared<Lambertian>(checkerTexture);
	world->Add(std::make_shared<Sphere>(Point3(0.0, -1000.0, 0.0), 1000.0, checkerMat));
	for (int dy = -11; dy < 11; ++dy)
	{
		for (int dx = -11; dx < 11; ++dx)
		{
			auto chooseMat = RandomDouble();
			Point3 center(static_cast<double>(dx) + 0.9 + RandomDouble(), 0.2, static_cast<double>(dy) + 0.9 * RandomDouble());
			auto randRad = RandomDouble(0.2f, 0.25f);
			if ((center - Vec3(4.0, 0.2, 0.0)).Length() > 0.9)
			{
				std::shared_ptr<Material> sphereMatPtr;
				if (chooseMat < 0.8)
				{
					// Diffuse
					auto albedo = Color::Random() * Color::Random();
					sphereMatPtr = std::make_shared<Lambertian>(albedo);
					auto center1 = center + Vec3(0.0, RandomDouble(0.0, 0.5), 0.0); // y축으로 움직임
					world->Add(std::make_shared<MovingSphere>(center, center1, shutterOpen, shutterClose, randRad, sphereMatPtr));
				}
				else if (chooseMat < 0.95)
				{
					// Metal
					auto albedo = Color::Random(0.5, 1.0);
					auto fuzz = RandomDouble(0.0, 0.5);
					auto center1 = center + Vec3(RandomDouble(0.0, 0.5), 0.0, 0.0); // x축으로 움직임
					sphereMatPtr = std::make_shared<Metal>(albedo, fuzz);
					world->Add(std::make_shared<MovingSphere>(center, center1, shutterOpen, shutterClose, randRad, sphereMatPtr));
				}
				else
				{
					// Glass
					sphereMatPtr = std::make_shared<Dielectric>(1.5);
					world->Add(std::make_shared<Sphere>(center, randRad, sphereMatPtr));
				}
			}
		}
	}

	auto dielectricMatPtr = std::make_shared<Dielectric>(1.5);
	auto lambertianMatPtr = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	auto metalMatPtr = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);

	world->Add(std::make_shared<Sphere>(Point3(0.0, 1.0, 0.0), 1.0, dielectricMatPtr));
	world->Add(std::make_shared<Sphere>(Point3(-4.0, 1.0, 0.0), 1.0, lambertianMatPtr));
	world->Add(std::make_shared<Sphere>(Point3(4.0, 1.0, 0.0), 1.0, metalMatPtr));

	return std::move(world);
}

std::unique_ptr<HittableList> TwoSpheres()
{
	auto world = std::make_unique<HittableList>();

	auto checkerTexture = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	auto checkerMat = std::make_shared<Lambertian>(checkerTexture);

	world->Add(std::make_shared<Sphere>(Point3(0.0, -10.0, 0.0), 10.0, checkerMat));
	world->Add(std::make_shared<Sphere>(Point3(0.0, 10.0, 0.0), 10.0, checkerMat));

	return std::move(world);
}

std::unique_ptr<HittableList> Earth()
{
	auto world = std::make_unique<HittableList>();

	auto earthTexture = std::make_shared<ImageTexture>("Resources/Textures/earthmap.jpg");
	auto earthMat = std::make_shared<Lambertian>(earthTexture);

	world->Add(std::make_shared<Sphere>(Point3(0.0, 0.0, 0.0), 2.0, earthMat));

	return std::move(world);
}

std::unique_ptr<HittableList> SimpleLight()
{
	auto world = std::make_unique<HittableList>();

	auto whiteTexture = std::make_shared<SolidColorTexture>(Color(1.0, 1.0, 1.0));
	auto earthTexture = std::make_shared<ImageTexture>("Resources/Textures/earthmap.jpg");

	auto whiteLambertMat = std::make_shared<Lambertian>(whiteTexture);
	auto earthLambertMat = std::make_shared<Lambertian>(earthTexture);

	world->Add(std::make_shared<Sphere>(Point3(0.0, -1000.0, 0.0), 1000.0, whiteLambertMat));
	world->Add(std::make_shared<Sphere>(Point3(0.0, 2.0, 0.0), 2.0, earthLambertMat));

	auto diffuseLightColor = std::make_shared<SolidColorTexture>(Color(4.0, 4.0, 4.0));
	auto diffuseLight = std::make_shared<DiffuseLight>(diffuseLightColor);
	world->Add(std::make_shared<Sphere>(Point3(0.0, 6.0, 0.0), 2.0, diffuseLight));
	world->Add(std::make_shared<XYRect>(3.0, 5.0, 1.0, 3.0, -2.0, diffuseLight));

	return std::move(world);
}

std::unique_ptr<HittableList> CornellBox()
{
	auto world = std::make_unique<HittableList>();

	auto redMat = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
	auto whiteMat = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	auto greenMat = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
	auto lightMat = std::make_shared<DiffuseLight>(Color(15.0, 15.0, 15.0));

	world->Add(std::make_shared<YZRect>(0.0, 555.0, 0.0, 555.0, 555.0, greenMat));
	world->Add(std::make_shared<YZRect>(0.0, 555.0, 0.0, 555.0, 0.0, redMat));
	world->Add(std::make_shared<XZRect>(213.0, 343.0, 227.0, 332.0, 554.0, lightMat));
	world->Add(std::make_shared<XZRect>(0.0, 555.0, 0.0, 555.0, 0.0, whiteMat));
	world->Add(std::make_shared<XZRect>(0.0, 555.0, 0.0, 555.0, 555.0, whiteMat));
	world->Add(std::make_shared<XYRect>(0.0, 555.0, 0.0, 555.0, 555.0, whiteMat));

	std::shared_ptr<Hittable> box0 = std::make_shared<Box>(Point3(0.0, 0.0, 0.0), Point3(165.0, 330.0, 165.0), whiteMat);
	box0 = std::make_shared<RotateY>(box0, 15.0);
	box0 = std::make_shared<Translate>(box0, Vec3(265.0, 0.0, 295.0));
	world->Add(box0);

	std::shared_ptr<Hittable> box1 = std::make_shared<Box>(Point3(0.0, 0.0, 0.0), Point3(165.0, 165.0, 165.0), whiteMat);
	box1 = std::make_shared<RotateY>(box1, -18.0);
	box1 = std::make_shared<Translate>(box1, Vec3(130.0, 0.0, 65.0));
	world->Add(box1);

	return std::move(world);
}

std::unique_ptr<HittableList> CornellBoxSmoke()
{
	auto world = std::make_unique<HittableList>();

	auto redMat = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
	auto whiteMat = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	auto greenMat = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
	auto lightMat = std::make_shared<DiffuseLight>(Color(15.0, 15.0, 15.0));

	world->Add(std::make_shared<YZRect>(0.0, 555.0, 0.0, 555.0, 555.0, greenMat));
	world->Add(std::make_shared<YZRect>(0.0, 555.0, 0.0, 555.0, 0.0, redMat));
	world->Add(std::make_shared<XZRect>(113.0, 443.0, 127.0, 432.0, 553.9, lightMat));
	world->Add(std::make_shared<XZRect>(0.0, 555.0, 0.0, 555.0, 0.0, whiteMat));
	world->Add(std::make_shared<XZRect>(0.0, 555.0, 0.0, 555.0, 555.0, whiteMat));
	world->Add(std::make_shared<XYRect>(0.0, 555.0, 0.0, 555.0, 555.0, whiteMat));

	std::shared_ptr<Hittable> box0 = std::make_shared<Box>(Point3(0.0, 0.0, 0.0), Point3(165.0, 330.0, 165.0), whiteMat);
	box0 = std::make_shared<RotateY>(box0, 15.0);
	box0 = std::make_shared<Translate>(box0, Vec3(265.0, 0.0, 295.0));
	world->Add(std::make_shared<ConstantMedium>(box0, 0.01, Color()));

	std::shared_ptr<Hittable> box1 = std::make_shared<Box>(Point3(0.0, 0.0, 0.0), Point3(165.0, 165.0, 165.0), whiteMat);
	box1 = std::make_shared<RotateY>(box1, -18.0);
	box1 = std::make_shared<Translate>(box1, Vec3(130.0, 0.0, 65.0));
	world->Add(std::make_shared<ConstantMedium>(box1, 0.01, Color(1.0, 1.0, 1.0)));

	return std::move(world);
}

std::unique_ptr<HittableList> ComplexScene()
{
	auto objects = std::make_unique<HittableList>();

	HittableList boxes0;
	auto groundMat = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

	constexpr int BoxesPerSide = 20;
	for (int dx = 0; dx < BoxesPerSide; ++dx)
	{
		for (int dz = 0; dz < BoxesPerSide; ++dz)
		{
			double w = 100.0;
			double x0 = -1000.0 + dx * w;
			double x1 = x0 + w;
			double z0 = -1000.0 + dz * w;
			double z1 = z0 + w;
			double y0 = 0.0;
			double y1 = RandomDouble(1.0, 101.0);

			boxes0.Add(std::make_shared<Box>(Point3(x0, y0, z0), Point3(x1, y1, z1), groundMat));
		}
	}

	auto bvh = std::make_shared<BVHNode>(boxes0, 0.0, 1.0);
	objects->Add(bvh);

	auto light = std::make_shared<DiffuseLight>(Color(7.0, 7.0, 7.0));
	objects->Add(std::make_shared<XZRect>(123.0, 423.0, 147.0, 412.0, 554.0, light));

	auto center0 = Point3(400.0, 400.0, 200.0);
	auto center1 = center0 + Vec3(30.0, 0.0, 0.0);
	auto movingSphereMat = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
	objects->Add(std::make_shared<MovingSphere>(center0, center1, 0.0, 1.0, 50.0, movingSphereMat));

	objects->Add(std::make_shared<Sphere>(Point3(260.0, 150.0, 45.0), 50.0, std::make_shared<Dielectric>(1.5)));
	objects->Add(std::make_shared<Sphere>(Point3(0.0, 150.0, 145.0), 50.0, std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

	auto boundary = std::make_shared<Sphere>(Point3(360.0, 150.0, 145.0), 70.0, std::make_shared<Dielectric>(1.5));
	objects->Add(boundary);
	objects->Add(std::make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
	boundary = std::make_shared<Sphere>(Point3(0.0, 0.0, 0.0), 5000.0, std::make_shared<Dielectric>(1.5));
	objects->Add(std::make_shared<ConstantMedium>(boundary, 0.0001, Color(1.0, 1.0, 1.0)));

	auto earthMat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("Resources/Textures/earthmap.jpg"));
	objects->Add(std::make_shared<Sphere>(Point3(400.0, 200.0, 400.0), 100.0, earthMat));
	auto whiteTexture = std::make_shared<SolidColorTexture>(Color(1.0, 1.0, 1.0));
	objects->Add(std::make_shared<Sphere>(Point3(220.0, 280.0, 300.0), 80.0, std::make_shared<Lambertian>(whiteTexture)));

	HittableList boxes1;
	auto whiteMat = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	int ns = 1000;
	for (int ds = 0; ds < ns; ++ds)
	{
		boxes1.Add(std::make_shared<Sphere>(Point3::Random(0.0, 165.0), 10.0, whiteMat));
	}

	objects->Add(std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<BVHNode>(boxes1, 0.0, 1.0), 15.0), Vec3(-100.0, 270.0, 395.0)));
	return std::move(objects);
}

Color RayColor(const Ray& r, const Color& background, const Hittable& world, int depth)
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
		Color emitted = rec.MatPtr->Emitted(rec.u, rec.v, rec.p);
		if (rec.MatPtr->Scatter(r, rec, attenuation, scattered))
		{
			return emitted + (attenuation * RayColor(scattered, background, world, depth - 1));
		}

		return emitted;
	}

	return background;
}

int main()
{
	// Output Image
	constexpr double aspectRatio = 1.0;
	constexpr int imageWidth = 800;
	constexpr int imageHeight = static_cast<int>(imageWidth/aspectRatio);
	constexpr int imageChannels = 3; // RGB
	constexpr int samplesPerPixel = 64;
	constexpr int maximumDepth = 50;

	auto outputBuffer = std::make_unique<unsigned char[]>(imageWidth*imageHeight*imageChannels);

	// Camera
	// Cornell
	//Point3 lookFrom(278.0, 278.0, -800.0);
	//Point3 lookAt(278.0, 278.0, 0.0);
	//Vec3 up(0.0, 1.0, 0.0);
	//auto distToFocus = 10.0;
	//auto aperture = 0.0;
	//auto shutterOpen = 0.0;
	//auto shutterClose = 1.0;
	//auto verticalFOV = 40.0;

	Point3 lookFrom(478.0, 278.0, -600.0);
	Point3 lookAt(278.0, 278.0, 0.0);
	Vec3 up(0.0, 1.0, 0.0);
	auto distToFocus = 10.0;
	auto aperture = 0.0;
	auto shutterOpen = 0.0;
	auto shutterClose = 1.0;
	auto verticalFOV = 40.0;
	Camera cam(lookFrom, lookAt, up, verticalFOV, aspectRatio, aperture, distToFocus, shutterOpen, shutterClose);

	// World
	//auto world = std::move(RandomScene(shutterOpen, shutterClose));
	//auto world = std::move(TwoSpheres());
	//auto world = std::move(SimpleLight());
	//auto world = std::move(CornellBoxSmoke());
	auto world = std::move(ComplexScene());
	Color background = Color();

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
				pixelColor += RayColor(r, background, *world, maximumDepth);
			}

			size_t base = ((imageHeight - dy - 1) * imageWidth * imageChannels) + (dx * imageChannels);
			WriteColor(outputBuffer, pixelColor, base, samplesPerPixel);
		}
	}

	stbi_write_png("output.png", imageWidth, imageHeight, imageChannels, outputBuffer.get(), imageWidth * imageChannels);
	std::cerr << "\nRendering Done\n";

   return 0;
}