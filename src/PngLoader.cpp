#include "PngLoader.h"

#include <iostream>

#define PNG_DEBUG 3

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


	for (int x = 0; x < m_width; ++x)
	{
		for (int y = 0; y < m_height; ++y)
		{
			int idx = 3*(y*m_width + x);
			std::cout << int(m_imageData[idx]) << " " << int(m_imageData[idx + 1]) << " " << int(m_imageData[idx+2]) << std::endl;
		}
	}
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



void PngLoader::read_png_file(char* file_name)
{
	png_byte header[8];    // 8 is the maximum size that can be checked

	/* open file and test for it being a png */
	FILE *fp = fopen(file_name, "rb");
	if (!fp)
		abort_("[read_png_file] File %s could not be opened for reading", file_name);
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8))
		abort_("[read_png_file] File %s is not recognized as a PNG file", file_name);


	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		abort_("[read_png_file] png_create_read_struct failed");

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[read_png_file] png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during init_io");

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	widthTest = png_get_image_width(png_ptr, info_ptr);
	heightTest = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);


	/* read file */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during read_image");

	row_pointers = (png_bytep*)malloc(sizeof(png_bytep)* heightTest);
	for (y = 0; y<heightTest; y++)
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));

	png_read_image(png_ptr, row_pointers);

	fclose(fp);
}


void PngLoader::write_png_file(char* file_name)
{
	/* create file */
	FILE *fp = fopen(file_name, "wb");
	if (!fp)
		abort_("[write_png_file] File %s could not be opened for writing", file_name);


	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		abort_("[write_png_file] png_create_write_struct failed");

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[write_png_file] png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during init_io");

	png_init_io(png_ptr, fp);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing header");

	png_set_IHDR(png_ptr, info_ptr, widthTest, heightTest,
		bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing bytes");

	png_write_image(png_ptr, row_pointers);


	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during end of write");

	png_write_end(png_ptr, NULL);

	/* cleanup heap allocation */
	for (y = 0; y<heightTest; y++)
		free(row_pointers[y]);
	free(row_pointers);

	fclose(fp);
}


void PngLoader::process_file(void)
{
	if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB)
		abort_("[process_file] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA "
		"(lacks the alpha channel)");

	if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA)
		abort_("[process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (%d) (is %d)",
		PNG_COLOR_TYPE_RGBA, png_get_color_type(png_ptr, info_ptr));

	for (y = 0; y<heightTest; y++) {
		png_byte* row = row_pointers[y];
		for (x = 0; x<widthTest; x++) {
			png_byte* ptr = &(row[x * 4]);
			printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
				x, y, ptr[0], ptr[1], ptr[2], ptr[3]);

			/* set red value to 0 and green value to the blue one */
			ptr[0] = 0;
			ptr[1] = ptr[2];
		}
	}
}

void PngLoader::abort_(const char * s, ...)
{
	//va_list args;
	//va_start(args, s);
	//vfprintf(stderr, s, args);
	//fprintf(stderr, "\n");
	//va_end(args);
	//abort();
}

//
//int main(int argc, char **argv)
//{
//	if (argc != 3)
//		abort_("Usage: program_name <file_in> <file_out>");
//
//	read_png_file(argv[1]);
//	process_file();
//	write_png_file(argv[2]);
//
//	return 0;
//}

bool PngLoader::loadPngImage(const char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) 
{
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	int color_type, interlace_type;
	FILE *fp;

	if ((fp = fopen(name, "rb")) == NULL)
		return false;

	/* Create and initialize the png_struct
	* with the desired error handler
	* functions.  If you want to use the
	* default stderr and longjump method,
	* you can supply NULL for the last
	* three parameters.  We also supply the
	* the compiler header file version, so
	* that we know if the application
	* was compiled with a compatible version
	* of the library.  REQUIRED
	*/
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		NULL, NULL, NULL);

	if (png_ptr == NULL) {
		fclose(fp);
		return false;
	}

	/* Allocate/initialize the memory
	* for image information.  REQUIRED. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;
	}

	/* Set error handling if you are
	* using the setjmp/longjmp method
	* (this is the normal method of
	* doing things with libpng).
	* REQUIRED unless you  set up
	* your own error handlers in
	* the png_create_read_struct()
	* earlier.
	*/
	if (setjmp(png_jmpbuf(png_ptr))) {
		/* Free all of the memory associated
		* with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		/* If we get here, we had a
		* problem reading the file */
		return false;
	}

	/* Set up the output control if
	* you are using standard C streams */
	png_init_io(png_ptr, fp);

	/* If we have already
	* read some of the signature */
	png_set_sig_bytes(png_ptr, sig_read);

	/*
	* If you have enough memory to read
	* in the entire image at once, and
	* you need to specify only
	* transforms that can be controlled
	* with one of the PNG_TRANSFORM_*
	* bits (this presently excludes
	* dithering, filling, setting
	* background, and doing gamma
	* adjustment), then you can read the
	* entire image (including pixels)
	* into the info structure with this
	* call
	*
	* PNG_TRANSFORM_STRIP_16 |
	* PNG_TRANSFORM_PACKING  forces 8 bit
	* PNG_TRANSFORM_EXPAND forces to
	*  expand a palette into RGB
	*/
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

	png_uint_32 width, height;
	int bit_depth;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		&interlace_type, NULL, NULL);

	outHasAlpha = (color_type == PNG_COLOR_TYPE_RGBA);
	outWidth = width;
	outHeight = height;

	unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	*outData = (unsigned char*)malloc(row_bytes * outHeight);

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	for (int i = 0; i < outHeight; i++) {
		// note that png is ordered top to
		// bottom, but OpenGL expect it bottom to top
		// so the order or swapped
		memcpy(*outData + (row_bytes * (outHeight - 1 - i)), row_pointers[i], row_bytes);
	}

	/* Clean up after the read,
	* and free any memory allocated */
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	/* Close the file */
	fclose(fp);

	/* That's it */
	return true;
}