#ifndef GLYPH_METRICS_H
#define GLYPH_METRICS_H


struct GlyphMetric
{
	float ax;	// advance.x
	float ay;	// advance.y

	float bw;	// bitmap.width;
	float bh;	// bitmap.height;

	float bl;	// bitmap_left;
	float bt;	// bitmap_top;

	float tx;	// x offset of glyph in texture coordinates
	float ty;	// y offset of glyph in texture coordinates
};


#endif
