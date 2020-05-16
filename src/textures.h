#ifndef TEXTURES
#define TEXTURES
#include <iostream>

using namespace std;

struct TEXTURE_METADATA {
    uint8_t coord1_x;
    uint8_t coord1_y;
    uint16_t clut;
    uint8_t coord2_x;
    uint8_t coord2_y;
    uint16_t texpage;
    uint8_t coord3_x;
    uint8_t coord3_y;
    uint8_t coord4_x;
    uint8_t coord4_y;
};

struct TEXPAGE {
    int x;
    int y;
    int colors;
};

struct CLUT {
    int x;
    int y;
};

struct SQUARE {
    int x;
    int y;
    int width;
    int height;
};

uint16_t* fill_vram(ifstream* tpsx_file, int texpage_count, int offset);
int get_color_offset(uint16_t* VRAM, int tex_offset, int bit_offset, int bits);
int get_page_offset(int x, int y);
void extract_textures(string file_name);

#endif