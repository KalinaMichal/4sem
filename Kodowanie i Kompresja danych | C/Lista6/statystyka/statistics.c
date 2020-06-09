#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include <limits.h>
#include <assert.h>
#include <stdint.h>

#define TGA_FOOTER_SIZE 26
#define TGA_HEADER_SIZE 18

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

struct PIXEL {u8 b; u8 g; u8 r; };

struct Quant {double glob; double r;double g;double b;double sglob;double sr;double sg;double sb;};

//-----------------------------------------------------------------
//------------------------- tga -----------------------------------
//-----------------------------------------------------------------

//---------------------------
#pragma pack(1)
struct COLORMAP_SPEC {u16 first_entry; u16 colormap_length; u8 colormap_entry_size; };

struct TGA_HEADER {
	u8 id_len;
	u8 type_color;
	u8 type_img;
	struct COLORMAP_SPEC specf_color_map;
	u16 X;
	u16 Y;
	u16 szerokosc;
	u16 wysokosc;
	u8 bit;
	u8 img;
};

struct TGA_FOOTER {u32 ext_offset; u32 dev_offset; char signature[16]; u8 dot; u8 zero;};

enum TGA_ERROR {TGA_OK = 0,TGA_BAD_FORMAT,TGA_OUT_OF_MEMORY,};

struct PIXEL* read_tga_image(FILE* file, size_t* size, struct TGA_HEADER* naglowek, struct TGA_FOOTER* stopka) 
{
	fseek(file, 0, SEEK_SET);
	fread(naglowek, TGA_HEADER_SIZE, 1, file);
	fseek(file, -TGA_FOOTER_SIZE, SEEK_END);
	fread(stopka, TGA_FOOTER_SIZE, 1, file);

	size_t imgsize = naglowek->szerokosc * naglowek->wysokosc;
	*size = imgsize;

	struct PIXEL* imgdata = (struct PIXEL*) malloc(imgsize * sizeof(struct PIXEL*));

	fseek(file, TGA_HEADER_SIZE, SEEK_SET);
  	fread(imgdata, 3, imgsize, file);
    
	return imgdata;
}

struct PIXEL* alloc_image_data(size_t size) {return (struct PIXEL*) malloc(size * sizeof(struct PIXEL*));}


//--------------------------------------------------------
//------------------------- liczenie ---------------------
//--------------------------------------------------------

static struct Quant measure( size_t size, struct PIXEL* poczatkowy, struct PIXEL* koncowy) 
{
	struct Quant q =(struct Quant){.glob = 0.0,.r = 0.0, .g = 0.0, .b = 0.0,.sglob = 0.0,.sr = 0.0, .sg = 0.0, .sb = 0.0};

	for (size_t i = 0; i < size; i++) {
		struct PIXEL po = poczatkowy[i]; //pixel original
		struct PIXEL pr = koncowy[i];

		double r1 = po.r - pr.r;
		double g1 = po.g - pr.g;
		double d1 = po.b - pr.b;

		q.glob += r1 * r1 + g1 * g1 + d1 * d1;
		q.sglob += po.r * po.r + po.g * po.g + po.b * po.b;

		q.r += r1 * r1;
		q.sr += po.r * po.r;

		q.g += g1 * g1;
		q.sg += po.g * po.g;

		q.b += d1 * d1;
		q.sb += po.b * po.b;
    	}

   	q.sglob = q.sglob/ q.glob;
	q.glob = q.glob/(3 * size);

	q.sr = q.sr /q.r;
	q.r = q.r /size;

	q.sg = q.sg /q.g;
	q.g = q.g /size;

	q.sb = q.sb /q.b;
	q.b = q.b /size;

	return q;
}

int main(int argc, char** argv) {
	struct TGA_HEADER naglowek;
	struct TGA_FOOTER stopka;
	size_t rozmiar;

        if (argc < 3) {
        fprintf(stderr, "Zle dane wejsciowe\n Usage: ./stats <in_file> <out_file>\n");
		return 0;
	}

	FILE* inplik = fopen(argv[1], "rb");
	FILE* out_plik = fopen(argv[2], "rb");

	if (out_plik == NULL || inplik == NULL) {
		fprintf(stderr, "Error, zly plik\n");
		return 0;
	}

	struct PIXEL* poczatkowy = read_tga_image(inplik, &rozmiar, &naglowek, &stopka);
	struct PIXEL* koncowy = read_tga_image(out_plik, &rozmiar, &naglowek, &stopka);

	struct Quant q = measure(rozmiar, poczatkowy, koncowy);
	
	printf("\n_______________________________________________\n\n");
	printf("MSE     | %.6f    | %.6f | %.6f  | %.6f\n",  q.glob,  q.r, q.g,q.b);
 	printf("SNR     | %.6f    | %.6f | %.6f | %.6f\n",q.sglob,  q.sr, q.sg, q.sb);
	printf("SNR(db) | %.6f | %.6f  | %.6f  | %.6f\n",10*log10(q.sglob),  10*log10(q.sr),10*log10(q.sg),10*log10(q.sb));
	printf("_______________________________________________\n\n");

	return 0;
}
