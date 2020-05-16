#include <iostream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include "text_decoder.h"
#include "textures.h"
#include "level.h"

using namespace std;

void extract_lev(string path) {

cout << "Extracting level files..." << endl;

    string decoder[0x100];
    initialize_decoder(decoder);

    // Creating path to POTTER/LEV/
    boost::filesystem::path lev_folder(path);

    // Iterating every file in POTTER/LEV/
    for (auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(lev_folder), {})) {

        // Manipulating each file name
        string entry_name = entry.path().string();
        entry_name = entry_name.substr(path.size(), entry_name.size() - path.size());
        string entry_extension = entry_name.substr(entry_name.size() - 3);

        // This extractor doesn't support non WAD files
        if (entry_extension != "WAD") {
            continue;
        }

        string first_char = entry_name.substr(0, 1);

        // This extractor only supports level files
        if (first_char != "T") {
            continue;
        }

        // Creating folder for extracted files
        entry_name = entry_name.substr(0, entry_name.size() - 4);
        string output_path = path+entry_name;

        boost::filesystem::path out(output_path);
        boost::filesystem::create_directory(out);

        // Opening file in read mode
        ifstream lev_file(output_path+".WAD", ios::in | ios::binary);

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
            string file_name = output_path+"/"+file_type;
            ofstream new_file(file_name, ios::out | ios::binary);
            new_file.write(mem_block, metadata.size);
            new_file.close();

            // If this is a texture ile
            if (file_type == "TPSX") {

                cout << "Extracting textures..." << endl;

                // Extract all textures in PPM file format
                extract_textures(file_name);

            }

            // If this is a game text file
            if (file_type == "LPSX") {

                // Creating folder for the text file
                string text_path = output_path+"/Text";
                boost::filesystem::path text_dir(text_path);
                boost::filesystem::create_directory(text_dir);

                cout << "Extracting texts..." << endl;

                // Decoding in game strings to a text file
                decode_game_text(file_name, text_path+"/LPSX.txt", decoder);
            }


            // Freeing data space to be used later for the next file
            delete[] mem_block;

        }

        lev_file.close();

        cout << entry_name + ".WAD successfully extracted." << endl;

    }

}
