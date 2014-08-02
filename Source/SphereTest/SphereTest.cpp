//include the engine code
#include "../RTEngine/RTEngine.hpp"
#include "../RTEngine/PhongMaterial.hpp"
#include "../RTEngine/Sphere.hpp"
#include "../RTEngine/Plane.hpp"
#include "../RTEngine/SphericalLight.hpp"
#include <cstdio>
#include <png.h>

using namespace RealRT;

const int Width = 2560;
const int Height = 1600;

void writeBufferAsPng(std::string fileName, unsigned char *buffer, int width, int height)
{
	std::FILE *fh = std::fopen(fileName.c_str(), "wb");

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);

	png_init_io(png_ptr, fh);

	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);

	png_bytepp rowPointers = new png_bytep[height];
	for(int i = 0; i < height; i++)
		rowPointers[i] = buffer + (width * i * 3);

	png_write_image(png_ptr, rowPointers);

	png_write_end(png_ptr, NULL);

	delete rowPointers;

	std::fclose(fh);
}

int main(int argc, char **argv)
{
	std::shared_ptr<RTEngine> engine = RTEngine::Instantiate(Width, Height);

	std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>(DiffuseRed,Vector3D(-5.0,0.0,2.0));
	std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>(DiffuseGreen,Vector3D(0.0,4.0,0.0),0.5);
	std::shared_ptr<Sphere> s3 = std::make_shared<Sphere>(DiffuseGreen,Vector3D(0.0,2.0,0.0),0.5);
	std::shared_ptr<Sphere> s4 = std::make_shared<Sphere>(DiffuseGreen,Vector3D(0.0,0.0,0.0),0.5);
	std::shared_ptr<Sphere> s5 = std::make_shared<Sphere>(ReflectiveBlue,Vector3D(5.0,0.0,2.0));

	std::shared_ptr<Sphere> lens = std::make_shared<Sphere>(Water, Vector3D(0.0, 1.0, -3.0), 1.0);

	std::shared_ptr<Plane> floorobj = std::make_shared<Plane>(Mirror,Vector3D(0.0,1.0,0.0),3.0);

	std::shared_ptr<SphericalLight> l1 = std::make_shared<SphericalLight>(WhiteLight,Vector3D(-9.5,6.0,0.0),0.1);
	std::shared_ptr<SphericalLight> l2 = std::make_shared<SphericalLight>(WhiteLight,Vector3D(0.0,6.0,-2.0),0.1);
	std::shared_ptr<SphericalLight> l3 = std::make_shared<SphericalLight>(WhiteLight,Vector3D(9.5,6.0,0.0),0.1);

	engine->AddWorldObject(floorobj);

	engine->AddWorldObject(s1);
	engine->AddWorldObject(s2);
	engine->AddWorldObject(s3);
	engine->AddWorldObject(s4);
	engine->AddWorldObject(s5);

	engine->AddWorldObject(lens);

	engine->AddWorldObject(l1);
	engine->AddWorldObject(l2);
	engine->AddWorldObject(l3);

	engine->CalculateScene();

	unsigned char *rendered = engine->Screen();

	std::string output = "out.png";
	if(argc > 1)
		output = argv[1];

	writeBufferAsPng(output, rendered, Width, Height);

	return 0;
}
