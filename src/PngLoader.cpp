#include "PngLoader.h"

#include <iostream>

PngLoader::PngLoader()
: m_pngPtr(nullptr),
  m_infoPtr(nullptr),
  m_endInfo(nullptr),
  m_imageData(nullptr),
  m_rowPointers(nullptr),
  m_hasAlpha(false),
  m_width(0),
  m_height(0)
{
}

PngLoader::PngLoader(const std::string fileName)
: m_pngPtr(nullptr),
  m_infoPtr(nullptr),
  m_endInfo(nullptr),
  m_imageData(nullptr),
  m_rowPointers(nullptr),
  m_hasAlpha(false),
  m_width(0),
  m_height(0)
{
	load(fileName);
}

PngLoader::~PngLoader()
{
	clear();
}

void PngLoader::load(const std::string fileName)
{
	//header for testing if it is a png
	png_byte header[8];

	//open file as binary
	FILE *fp = fopen(fileName.c_str(), "rb");
	if (!fp) {
		std::cout << " PngLoader::load(). No such file: " << fileName << std::endl;
		return;
	}

	//read the header
	fread(header, 1, 8, fp);

	//test if png
	int is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png) {
		fclose(fp);
		std::cout << " PngLoader::load(). No png file: " << fileName << std::endl;
		return;
	}

	//create png struct
	m_pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!m_pngPtr) {
		fclose(fp);
		std::cout << " PngLoader::load(). FAILED!" << std::endl;
		return;
	}

	//create png info struct
	m_infoPtr = png_create_info_struct(m_pngPtr);
	if (!m_infoPtr) {
		png_destroy_read_struct(&m_pngPtr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		std::cout << " PngLoader::load(). FAILED!" << std::endl;
		return;
	}

	//create png info struct
	m_endInfo = png_create_info_struct(m_pngPtr);
	if (!m_endInfo) {
		png_destroy_read_struct(&m_pngPtr, &m_infoPtr, (png_infopp)NULL);
		fclose(fp);
		std::cout << " PngLoader::load(). FAILED!" << std::endl;
		return;
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(m_pngPtr))) {
		png_destroy_read_struct(&m_pngPtr, &m_infoPtr, &m_endInfo);
		fclose(fp);
		std::cout << " PngLoader::load(). FAILED!" << std::endl;
		return;
	}

	//init png reading
	png_init_io(m_pngPtr, fp);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(m_pngPtr, 8);

	// read all the info up to the image data
	png_read_info(m_pngPtr, m_infoPtr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 twidth, theight;

	// get info about png
	png_get_IHDR(m_pngPtr, m_infoPtr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);

	m_hasAlpha = (color_type & PNG_COLOR_MASK_ALPHA);

	//update width and height based on png info
	m_width = twidth;
	m_height = theight;

	// Update the png info struct.
	png_read_update_info(m_pngPtr, m_infoPtr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(m_pngPtr, m_infoPtr);

	std::cout << "pngLoader: " << m_width << " " << m_height << " " << rowbytes << std::endl;


	// Allocate the image_data as a big block (e.g. to be given to opengl)
	m_imageData = new png_byte[rowbytes * m_height];
	if (!m_imageData) {
		//clean up memory and close stuff
		png_destroy_read_struct(&m_pngPtr, &m_infoPtr, &m_endInfo);
		fclose(fp);
		std::cout << " PngLoader::load(). FAILED!" << std::endl;
		return;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[m_height];
	if (!row_pointers) {
		//clean up memory and close stuff
		png_destroy_read_struct(&m_pngPtr, &m_infoPtr, &m_endInfo);
		delete[] m_imageData;
		m_imageData = nullptr;
		fclose(fp);
		
		std::cout << " PngLoader::load(). FAILED!" << std::endl;
		return;
	}
	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < m_height; ++i)
		row_pointers[m_height - 1 - i] = m_imageData + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(m_pngPtr, row_pointers);
}

unsigned char * PngLoader::imageData()
{
	return m_imageData;
}

int PngLoader::width() const
{
	return m_width;
}

int PngLoader::height() const
{
	return m_height;
}

bool PngLoader::hasAlpha() const
{
	return m_hasAlpha;
}

void PngLoader::clear()
{
	png_destroy_read_struct(&m_pngPtr, &m_infoPtr, &m_endInfo);
	delete[] m_imageData;
	delete[] m_rowPointers;
}
