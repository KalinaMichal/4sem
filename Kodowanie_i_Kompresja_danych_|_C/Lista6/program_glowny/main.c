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
struct PIXEL_DIFF {s16 r; s16 g; s16 b; };

//-----------------------------------------------------------------------------------------------------------------
//---------------------------- kwantyzacja ------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------


struct SPLIT_B { struct PIXEL_DIFF high; struct PIXEL low;};
struct RESULT_QUANTYSIZE {struct PIXEL* cen_l; u8* rep_l; struct PIXEL_DIFF* cen_h; u8* repr_hi; };

void initial_representatnts_getter(struct PIXEL* input, size_t size, int kwanty, struct PIXEL* cntr) 
{
	srand((u32)time(NULL));
	for (int i = 0; i < kwanty; i++) {u32 idx = (u32)rand() % size;cntr[i] = input[idx];}
}

u32 metric(struct PIXEL x, struct PIXEL y) {return (u32) abs((int)x.r - (int)y.r) + (u32) abs((int)x.g - (int)y.g) + (u32) abs((int)x.b - (int)x.b);}

u8 best_fit(struct PIXEL pixel, int kwanty, struct PIXEL* cen) {
	int min_id = 0;
	u32 min = UINT_MAX;
	for (int i = 0; i < kwanty; i++) if (metric(pixel, cen[i]) < min){min_id = i;min = metric(pixel, cen[i]);}
	return (u8) min_id;
}

u8 clamp(int a) {
	if (a>255) return 255;
	if (a<0) return 0;
	else return (u8)a;
}

struct PIXEL* quantize(size_t rozmiar, int bity, struct PIXEL* input, u8* quant_ids) {
	const int kwant = 1 << bity;
	const int MAX_tmp = 256;

	struct PIXEL* retr = (struct PIXEL*) calloc((size_t)kwant, sizeof(struct PIXEL));
	initial_representatnts_getter(input, rozmiar, kwant, retr);
	for (size_t i = 0; i < rozmiar; i++) quant_ids[i] = best_fit(input[i], kwant, retr);

	for (int i = 0; i < MAX_tmp; i++) {
	//get centroits
		u64* r = calloc((size_t)kwant, sizeof(u64));
   		u64* g = calloc((size_t)kwant, sizeof(u64));
    		u64* b = calloc((size_t)kwant, sizeof(u64));
    		u64* s = calloc((size_t)kwant, sizeof(u64));

    		for (size_t i = 0; i < rozmiar; i++) {
        		u8 id = quant_ids[i];
        
        		r[id] += input[i].r;
        		g[id] += input[i].g;
        		b[id] += input[i].b;

        		++s[id];
	    	}

	    	for (int i = 0; i < kwant; i++) {
			if (s[i] == 0) {retr[i] = (struct PIXEL){0, 0, 0};break;}

			retr[i] = (struct PIXEL){.r = (u8)(r[i] / s[i]),.g = (u8)(g[i] / s[i]),.b = (u8)(b[i] / s[i]),};
	    	}

		free(s);free(r);free(b);free(g); //free
		for (size_t i = 0; i < rozmiar; i++) quant_ids[i] = best_fit(input[i], kwant, retr);
	}

	return retr;
}

static u32 diff_metric(struct PIXEL_DIFF a, struct PIXEL_DIFF b) {return (u32)abs(a.r - b.r) + (u32)abs(a.g - b.g) + (u32)abs(a.b - b.b);}

static u8 best_fit_diff(struct PIXEL_DIFF pixel, int kwanty, struct PIXEL_DIFF* centroids) {
	u32 min = UINT_MAX;
	int min_id = 0;
	for (int i = 0; i < kwanty; i++) if (diff_metric(pixel, centroids[i]) < min) {min = diff_metric(pixel, centroids[i]); min_id = i;}
	return (u8) min_id;
}

struct PIXEL_DIFF* diff_quantize(size_t rozmiar, int bits, struct PIXEL_DIFF* input, u8* quant_ids) 
{
	const int kwant = 1 << bits;
	const int MAX_tmp = 256;

	struct PIXEL_DIFF* centroids = (struct PIXEL_DIFF*) calloc((size_t)kwant, sizeof(struct PIXEL_DIFF));

	srand((u32)time(NULL));
	for (int i = 0; i < kwant; i++) {u32 idx = (u32)rand() % rozmiar; centroids[i] = input[idx];}//get init diff representants
	for (size_t i = 0; i < rozmiar; i++) quant_ids[i] = best_fit_diff(input[i], kwant, centroids);

	for (int i = 0; i < MAX_tmp; i++) {
	//getting centroids diff
		s64* s = calloc((size_t)kwant, sizeof(s64));
		s64* r = calloc((size_t)kwant, sizeof(s64));
    		s64* g = calloc((size_t)kwant, sizeof(s64));
		s64* b = calloc((size_t)kwant, sizeof(s64));

		for (size_t i = 0; i < rozmiar; i++) {
   			s[quant_ids[i]]++;
        		r[quant_ids[i]] += input[i].r;
        		g[quant_ids[i]] += input[i].g;
        		b[quant_ids[i]] += input[i].b;	
    		}

    		for (int i = 0; i < kwant; i++) {
        		if (s[i] == 0) {
            			centroids[i] = (struct PIXEL_DIFF){0, 0, 0};
            			break;
        		}
			centroids[i] = (struct PIXEL_DIFF){.r = (s16)(r[i] / s[i]),.g = (s16)(g[i] / s[i]),.b = (s16)(b[i] / s[i]),};
		}
	    	free(r);free(g);free(b);free(s); //free
	//----
		for (size_t i = 0; i < rozmiar; i++) quant_ids[i] = best_fit_diff(input[i], kwant, centroids);
	}

	return centroids;
}

//-----------------------------------------------------------------
//------------------------- OBSŁUGA PLIKU -------------------------
//-----------------------------------------------------------------

struct FileAdapter { u8 byte; u8 bits; FILE* file;};

static u8 read_bit(struct FileAdapter* file) {
	if (file->bits == 0) {
		int c = fgetc(file->file);

		file->byte = (u8) c;
		file->bits = 8;
	}
	u8 bit = (file->byte & 0x80) >> 7;
	file->byte <<= 1;
	--file->bits;

	return bit;
}

static void write_bit(struct FileAdapter* file, u8 bit) {
	file->byte <<= 1;
	if (bit)	++file->byte;
	++file->bits;

	if (file->bits >= 8) {
		fputc(file->byte, file->file);
		file->bits = 0;
		file->byte = 0;
	}
}

void write_bits( u16 value, int bits,struct FileAdapter* file) {
	for (int i = bits-1; i >= 0; --i) {
		u8 bit = (u8)((value & 1 << i) >> i);
		write_bit(file, bit);
	}
}

struct FileAdapter fromFILE(FILE* file) {return (struct FileAdapter) {.byte = 0,.bits = 0,.file = file,};}

u16 read_bits(struct FileAdapter* file, int bits) {
	u16 value = 0;
	for (int i = 0; i < bits; i++) {
		u8 bit = read_bit(file);
		value <<= 1;
		if (bit) value++;
	}
	return value;
}

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
//---------------------------

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

//--------------------------------------------------------------------------
//------------------------- compress help ----------------------------------
//--------------------------------------------------------------------------

//pixel operacje
struct PIXEL pixel_dodaj(struct PIXEL l, struct PIXEL_DIFF r) { return (struct PIXEL){.r = clamp((int)l.r + (int)r.r),.g = clamp((int)l.g + (int)r.g),.b = clamp((int)l.b + (int)r.b)};}

struct PIXEL pixel_odejm(struct PIXEL l, struct PIXEL_DIFF r) {return (struct PIXEL) {.r = clamp((int)l.r - (int)r.r),.g = clamp((int)l.g - (int)r.g),.b = clamp((int)l.b - (int)r.b)};}

static struct PIXEL_DIFF pixel_high(struct PIXEL l, struct PIXEL r) {return (struct PIXEL_DIFF) {.r = (s16)(((int)l.r - (int)r.r) / 2),.g = (s16)(((int)l.g - (int)r.g) / 2),.b = (s16)(((int)l.b - (int)r.b) / 2)};}

static struct PIXEL pixel_low(struct PIXEL l, struct PIXEL r) {return (struct PIXEL) {.r = (u8)((((int)l.r + (int)r.r) / 2) % 256),.g = (u8)((((int)l.g + (int)r.g) / 2) % 256),.b = (u8)((((int)l.b + (int)r.b) / 2) % 256)};}

//----------------

struct RESULT_QUANTYSIZE quantize_bands(size_t size, struct SPLIT_B* band_data,  int bity) {
	size_t new_size = size/2;
	u8* out_buffer = malloc(new_size);
	struct PIXEL* buffer = alloc_image_data(new_size);

    	for (size_t i = 1; i < new_size; i++) buffer[i] = band_data[i].low;
    
	struct PIXEL* centroids = quantize(new_size, bity, buffer, out_buffer);

	for (size_t i = 1; i < new_size; i++) band_data[i].low = centroids[out_buffer[i]]; 

	free(buffer);

	struct PIXEL_DIFF* bufferHi = (struct PIXEL_DIFF*) 
        malloc(size * sizeof(struct PIXEL_DIFF));
	u8* out_bufferHi = malloc(new_size);

	for (size_t i = 1; i < new_size; i++) bufferHi[i] = band_data[i].high;

	struct PIXEL_DIFF* centroidsHi = diff_quantize(new_size, bity, bufferHi, out_bufferHi);

	for (size_t i = 1; i < new_size; i++) band_data[i].high = centroidsHi[out_bufferHi[i]]; 

	free(bufferHi);

	return (struct RESULT_QUANTYSIZE) {
		.cen_l = centroids,
		.rep_l = out_buffer,

		.cen_h = centroidsHi,
		.repr_hi = out_bufferHi,
	};
}

s16 diff_decode(u16 enc) {
	if (enc % 2 == 1) return (enc+1) / 2;
	return -(enc / 2);
}

u16 diff_encode(s16 diff) {
	if (diff > 0) return (u16)(2*diff-1);
	return (u16)(-2*diff);
}

//-----------------------------------------------------------------------
//------------------------- compress  -----------------------------------
//-----------------------------------------------------------------------

static void compress(u16 szerokosc, u16 wysokosc,struct RESULT_QUANTYSIZE results, u8 bits, FILE* file_out) 	
{
    	struct FileAdapter plik = fromFILE(file_out);
    	int wejscia = 1 << bits;

    	write_bits(szerokosc, 16, &plik );
   	write_bits( wysokosc, 16, &plik);
    	write_bits( bits, 8, &plik);

    	for (int i = 0; i < wejscia; ++i) {
        	write_bits(results.cen_l[i].r, 8, &plik);
        	write_bits(results.cen_l[i].g, 8, &plik );
        	write_bits(results.cen_l[i].b, 8, &plik);
    	}

    	for 	(int i = 0; i < wejscia; ++i) {
        	write_bits(diff_encode(results.cen_h[i].r), 9, &plik);
        	write_bits(diff_encode(results.cen_h[i].g), 9, &plik);
        	write_bits(diff_encode(results.cen_h[i].b), 9, &plik);
    	}

    	for (int i = 0; i < (szerokosc * wysokosc / 2); ++i) {
        	write_bits(results.rep_l[i], bits, &plik);
		write_bits(results.repr_hi[i], bits, &plik);
	}

	void pomTmp(struct FileAdapter* plik) {while (plik->bits > 0) write_bit(plik, 0);}
	pomTmp(&plik);
}

//----------------------------------------------------------------------
//------------------------- decompess  ---------------------------------
//----------------------------------------------------------------------

static void decompress(FILE* infile, FILE* file_out) {

	struct FileAdapter plik = fromFILE(infile);

	u16 szerokosc = read_bits(&plik, 16);
	u16 wysokosc = read_bits(&plik, 16);
	u8 bits = (u8)read_bits(&plik, 8);

	size_t size = szerokosc * wysokosc;
	size_t wejscia = 1 << bits;

	u8* rep_l = malloc((size / 2));    
	u8* repr_hi = malloc((size / 2));

	struct PIXEL* cen_l = (struct PIXEL*) malloc((size / 2) * sizeof(struct PIXEL));
	struct PIXEL_DIFF* cen_h = (struct PIXEL_DIFF*) malloc((size / 2) * sizeof(struct PIXEL_DIFF));
    	struct PIXEL* output_data = alloc_image_data(size);
	struct TGA_HEADER naglowek = {0, 0,2,{0, 0, 0}, 0,0,szerokosc,wysokosc,24, 0};
	struct TGA_FOOTER stopka = *((struct TGA_FOOTER*)"");

	for (size_t i = 0; i < wejscia; ++i) {
		cen_l[i].r = (u8)read_bits(&plik, 8);
		cen_l[i].g = (u8)read_bits(&plik, 8);
		cen_l[i].b = (u8)read_bits(&plik, 8);
	}

	for (size_t i = 0; i < wejscia; ++i) {
		cen_h[i].r = diff_decode(read_bits(&plik, 9));
		cen_h[i].g = diff_decode(read_bits(&plik, 9));
		cen_h[i].b = diff_decode(read_bits(&plik, 9));
	}

	for (size_t i = 0; i < (size / 2); ++i) {
		rep_l[i] = (u8)read_bits(&plik, bits);
		repr_hi[i] = (u8)read_bits(&plik, bits);
	}

   	for (size_t i = 1; i < (size / 2); i++) {
		struct PIXEL low = cen_l[rep_l[i]];
		struct PIXEL_DIFF high = cen_h[repr_hi[i]];

		output_data[2*i] = pixel_dodaj(low, high);
		output_data[2*i-1] = pixel_odejm(low, high);
	}

	void save_tga(FILE* plik,struct PIXEL* imgdata) 
	{
		fseek(plik, 0, SEEK_SET);
		fwrite(&naglowek, TGA_HEADER_SIZE, 1, plik);
		fwrite(imgdata, sizeof(struct PIXEL), size, plik);
		fwrite(&stopka, TGA_FOOTER_SIZE, 1, plik);
	}

	save_tga(file_out, output_data);   
}

//----------------------------------------------------------------------
//---------------------------- main  -----------------------------------
//----------------------------------------------------------------------

int main(int argc, char** argv) {
	_Bool option = 0;//1 - compress | 0 - decompress
	size_t size;
	struct TGA_HEADER naglowek;
	struct TGA_FOOTER stopka;
	int bits=-1; 

	if(!strcmp(argv[1],"compress"))option=1;
	else if(!strcmp(argv[1],"decompress"))option =0;
	else {         
		fprintf(stderr, "Podano zle dane!\nUsage: ./main <compress/decompress> <input_file> <output_file> <bits> \n <bits> tylko w przypadku compress\n");
		return 0;
	}

	if (argc < option+3) {
		fprintf(stderr, "Za malo danych wejsciowych!\nUsage: ./main <compress/decompress> <input_file> <output_file> <bits> \n<bits> tylko w przypadku compress\n");
		return 0;
	}

	FILE* file_in = fopen(argv[2], "rb");
	if (file_in == NULL) {
		fprintf(stderr, "Error, zly plik wejscia\n");
		return 0;
	}

	FILE* file_out = fopen(argv[3], "wb");

	if(option == 1){
		sscanf(argv[4], "%d", &bits);
		if(bits<0){printf("zle dane\n"); return 0;}

    		if (bits > 7 || bits <1) {
        		fprintf(stderr, "Zla liczba bitow: max: 7 | min: 1\n");
        		return 0;
    		} 
	
		struct PIXEL* img_data = read_tga_image(file_in, &size, &naglowek, &stopka);	
		struct SPLIT_B* band_data = (struct SPLIT_B*) calloc(size, sizeof(struct SPLIT_B));

		//dzielenie pasm
			band_data[0].low = img_data[0];
			band_data[0].high = (struct PIXEL_DIFF){0,0,0};

			for (size_t i = 1; i < size/2; i++) {
				band_data[i].low = pixel_low(img_data[2*i], img_data[2*i-1]);
				band_data[i].high = pixel_high(img_data[2*i], img_data[2*i-1]);
			}

		struct RESULT_QUANTYSIZE results = quantize_bands(size, band_data, bits);
		compress(naglowek.szerokosc, naglowek.wysokosc, results, (u8)bits, file_out);
	}
	else	decompress(file_in, file_out);

	return 0;
}




