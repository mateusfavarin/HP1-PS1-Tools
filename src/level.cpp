#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "text_decoder.h"
#include "textures.h"
#include "level.h"

using namespace std;
namespace fs = std::filesystem;

void extract_lev(fs::path file_path) {

    cout << "Extracting level files..." << endl;

    string decoder[0x100];
    initialize_decoder(decoder);

    // Creating path to POTTER/LEV/
    fs::path lev_folder = file_path;

    // Iterating every file in POTTER/LEV/
    for (const auto& entry : fs::directory_iterator(lev_folder)) {

        // Manipulating each file name
        string entry_name = entry.path().filename().string();
        string entry_extension = entry.path().extension().string();

        // This extractor doesn't support non WAD files
        if (entry_extension != "WAD") {
            continue;
        }

        char first_char = entry_name[0];

        // This extractor only supports level files
        if (first_char != 'T') {
            continue;
        }

        // Creating folder for extracted files
        entry_name = entry_name.substr(0, entry_name.size() - 4);
        fs::path output_path = file_path / entry_name;

        fs::path out(output_path);
        fs::create_directory(out);

        // Opening file in read mode
        ifstream lev_file(output_path.string() + ".WAD", ios::in | ios::binary);

         // Reading header
        LEV_HEADER header;
        lev_file.read((char *) &header, sizeof(header));

        LEV_METADATA metadata;
        char * mem_block;

        for (int i = 0; i < 7; i++) {

            // Reading metadata
            lev_file.read((char *) &metadata, sizeof(metadata));

            // Converting file name to a string
            string file_type;
            for (int i = FILE_TYPE - 1; i > -1; i--) {

                // End name on blank space
                if (metadata.file_name[i] == ' ') {
                    break;
                }

                // Add character (big endian) to name
                file_type += metadata.file_name[i];
            }

            // Creating memory block to read the chunk in the WAD
            mem_block = new char[metadata.size];
            lev_file.read(mem_block, metadata.size);

            // Creating new file
            fs::path file_name = output_path / file_type;
            ofstream new_file(file_name, ios::out | ios::binary);
            new_file.write(mem_block, metadata.size);

            // If this is a texture ile
            if (file_type == "TPSX") {

                cout << "Extracting textures..." << endl;

                // Extract all textures in PPM file format
                string file_name_str = file_name.string();
                extract_textures(file_name_str);

            }

            // If this is a game text file
            if (file_type == "LPSX") {

                // Creating folder for the text file
                fs::path text_path = output_path / "Text";
                fs::path text_dir(text_path);
                fs::create_directory(text_dir);

                cout << "Extracting texts..." << endl;

                // Decoding in game strings to a text file
                decode_game_text(file_name, text_path / "LPSX.txt", decoder);
            }


            // Freeing data space to be used later for the next file
            delete[] mem_block;

        }

        cout << entry_name + ".WAD successfully extracted." << endl;

    }

}
