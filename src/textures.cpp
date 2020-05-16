#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include <boost/filesystem.hpp>
#include "textures.h"

using namespace std;

uint16_t* fill_vram(ifstream* tpsx_file, int vram_size, int offset) {

    // Creating memory block to fill the PSX VRAM
    uint16_t* VRAM = new uint16_t[vram_size];
    int vram_length = 0;
    
    // Correcting offset to start reading from the start of the textures
    uint32_t vram_offset;

    // Getting file size
    tpsx_file->seekg(0, ios::end);
    int file_size = tpsx_file->tellg();

    tpsx_file->seekg(offset, ios::beg);
    tpsx_file->read((char *) &vram_offset, sizeof(vram_offset));

    offset += (vram_offset + 2) * 4;
    tpsx_file->seekg(offset, ios::beg);

    // Running texture decompression algorithm
    int16_t cmd;
    uint16_t pixel;
    while (vram_length < vram_size) {

        // Checking if file is good to read
        if (sizeof(cmd) + tpsx_file->tellg() >= file_size) {

            return VRAM;
        }

        tpsx_file->read((char *) &cmd, sizeof(cmd));
        

        // If the command is less than zero
        if (cmd < 0) {

            // Copy the same 2bytes value cmd times
            cmd *= -1;

            // Checking if file is good to read
            if (sizeof(pixel) + tpsx_file->tellg() >= file_size) {

                return VRAM;
            }

            tpsx_file->read((char *) &pixel, sizeof(pixel));

            for (int i = 0; i < cmd; i++) {
                VRAM[vram_length] = pixel;
                vram_length += 1;

                if (vram_length >= vram_size) {

                    return VRAM;
                }
            }

        // If the command is non negative
        } else {

            // Get the next 2bytes cmd times
            for (int i = 0; i < cmd; i++) {

                // Checking if file is good to read
                if (sizeof(pixel) + tpsx_file->tellg() >= file_size) {

                    return VRAM;
                }

                tpsx_file->read((char *) &pixel, sizeof(pixel));
                VRAM[vram_length] = pixel;
                vram_length += 1;

                if (vram_length >= vram_size) {

                    return VRAM;
                }
            }
        }
    }

    return VRAM;
}

int get_color_offset(uint16_t* VRAM, int tex_offset, int bit_offset, int bits) {

    // Read postion of the color offset
    int offset = VRAM[tex_offset];
    
    if (bits == 4) {
        // If the texture uses 4 bit colors

        offset = offset >> (4 * bit_offset);
        offset = offset & 0xF;

    } else if (bits == 8) {
        // If the texture uses 8 bit colors

        offset = offset >> (8 * bit_offset);
        offset = offset & 0xFF;
    }

    return offset;

}

int get_page_offset(int x, int y) {

    // Return the offset where the coordinates are store in VRAM
    return (x % 256) + ((y % 256) * 256) + (((((int) (x / 256)) * 2) + ((int) (y / 256))) * 65536);
}

void extract_textures(string file_name) {

    // Open texture file
    ifstream tpsx_file(file_name, ios::in | ios::binary);

    // Reading flag that has information about the textures offset
    uint32_t tpsx_flag;
    tpsx_file.read((char *) &tpsx_flag, sizeof(tpsx_flag));

    // Calculating offset based on the flag
    int offset = 0x4;
    uint32_t start_offset = 0x0;

    if ((tpsx_flag & 0x14) != 0) {

        if ((tpsx_flag & 4) == 0) {

            start_offset = 0x44;

        } else {

            start_offset = 0x24;
        }

        offset += 0x800 + start_offset;
    }

    // Updating offset to the start of texture metadata
    tpsx_file.seekg(offset, ios::beg);

    // Getting information about the texture metadata
    int texture_count;
    int vram_size;

    tpsx_file.read((char *) &texture_count, sizeof(texture_count));
    tpsx_file.read((char *) &vram_size, sizeof(vram_size));
    vram_size *= 0x10000;

    TEXTURE_METADATA metadata;
    int metadata_offset = offset + 0x8;
    offset = metadata_offset + (texture_count * sizeof(metadata));
    texture_count -= 0x10;

    uint16_t* VRAM = fill_vram(&tpsx_file, vram_size, offset);

    // Reading texture metadata
    tpsx_file.seekg(metadata_offset, ios::beg);
    TEXPAGE texpage;
    CLUT clut;
    SQUARE texture;
    int palette;
    int tex_start;
    int tex_offset;
    int bit_offset;
    uint16_t* raw_texture;
    int index;
    int raw_size;

    // Creating directory for the textures
    file_name = file_name.substr(0, file_name.size() - 4);
    file_name += "/Textures";
    boost::filesystem::path tex_path(file_name);
    boost::filesystem::create_directory(tex_path);

    for (int i = 0; i < texture_count; i++) {

        tpsx_file.read((char *) &metadata, sizeof(metadata));

        // Decrypting texpage
        texpage.x = ((metadata.texpage & 0xF) * 64) - 512;
        texpage.y = ((metadata.texpage & 0x1F) >> 4) * 256;
        texpage.colors = (metadata.texpage & 0x1FF) >> 7;

        // Decrypting clut
        clut.x = ((metadata.clut & 0x3F) * 16) - 512;
        clut.y = metadata.clut >> 6;

        // Setting dimensions of the texture
        texture.x = min(min(metadata.coord1_x, metadata.coord2_x), min(metadata.coord3_x, metadata.coord4_x));
        texture.y = min(min(metadata.coord1_y, metadata.coord2_y), min(metadata.coord3_y, metadata.coord4_y));
        texture.width = 1 + max(max(metadata.coord1_x, metadata.coord2_x), max(metadata.coord3_x, metadata.coord4_x)) - texture.x;
        texture.height = 1 + max(max(metadata.coord1_y, metadata.coord2_y), max(metadata.coord3_y, metadata.coord4_y)) - texture.y;

        // Creating memory space for the texture
        raw_size = texture.width * texture.height;
        raw_texture = new uint16_t[raw_size];
        index = 0;

        // Setting up the palette and texture start coordinates in VRAM
        palette = get_page_offset(clut.x, clut.y);
        tex_start = get_page_offset(texpage.x, texpage.y + texture.y);

        // Getting raw texture
        if (texpage.colors == 0) {
            // 4 bit colors

            tex_start += texture.x / 4;
            bit_offset = texture.x % 4;

            for (int i = 0; i < texture.height; i++) {

                // Updating texture height
                tex_offset = tex_start + i * 256;

                for (int j = 0; j < texture.width; j++) {

                    // Getting the color location in VRAM
                    int color = palette + get_color_offset(VRAM, tex_offset, bit_offset, 4);

                    // Handling segment fault
                    if (color >= vram_size) {
                        
                        goto stop;
                    }

                    // Getting the color of the pixel
                    raw_texture[index] = VRAM[color];
                    index += 1;


                    // Updating bit offset
                    bit_offset += 1;
                    bit_offset %= 4;

                    // Checking if you read every bit of the word
                    if (bit_offset == 0) {

                        // Updating texture width
                        tex_offset += 1;
                    }    

                }

            }

        } else if (texpage.colors == 1) {
            // 8 bit colors

            tex_start += texture.x / 2;
            bit_offset = texture.x % 2;

            for (int i = 0; i < texture.height; i++) {

                // Updating texture height
                tex_offset = tex_start + i * 256;

                for (int j = 0; j < texture.width; j++) {

                    // Getting the color location in VRAM
                    int color = palette + get_color_offset(VRAM, tex_offset, bit_offset, 8);

                    // Handling segment fault
                    if (color >= vram_size) {
                        
                        goto stop;
                    }

                    // Getting the color of the pixel
                    raw_texture[index] = VRAM[color];
                    index += 1;


                    // Updating bit offset
                    bit_offset += 1;
                    bit_offset %= 2;

                    // Checking if you read every bit of the word
                    if (bit_offset == 0) {

                        // Updating texture width
                        tex_offset += 1;
                    }
       
                }
                
            }

        } else {
            // 16 bit colors

            tex_start += texture.x;

            for (int i = 0; i < texture.height; i++) {

                // Updating texture height
                tex_offset = tex_start + i * 256;

                for (int j = 0; j < texture.width; j++) {

                    // Handling segment fault
                    if (tex_offset >= vram_size) {

                        goto stop;
                    }

                    // Getting the color of the pixel
                    raw_texture[index] = VRAM[tex_offset];
                    index += 1;


                    // Updating texture width
                    tex_offset += 1;

                }
                
            }

        }

        // Label to stop for inner loop and jump to foor outter loop
        stop:        

        // Creating PPM file for the texture
        ofstream new_file(file_name+"/"+to_string(i)+"_"+to_string(texture.width)+"x"+to_string(texture.height)+".ppm", ios::out);
        
        // PPM Header
        new_file << "P3" << "\n" << texture.width << " " << texture.height << "\n" << 32 << "\n";
        
        // Writing RGB colors to file
        for (int i = 0; i < raw_size; i++) {

            new_file << (raw_texture[i] & 0x1F) << " ";
            new_file << ((raw_texture[i] >> 5) & 0x1F) << " ";
            new_file << ((raw_texture[i] >> 10) & 0x1F) << "\n";

        }

        new_file.close();

        // Freeing texture memory to be used in the next iteration
        delete[] raw_texture;
        
    }

    tpsx_file.close();
}
