#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "language.h"
#include "text_decoder.h"

using namespace std;
namespace fs = std::filesystem;

void extract_lang(fs::path file_path) {

    // Decode string array using the game's dictionary
    string decoder[0x100];
    initialize_decoder(decoder);

    // Store path to POTTER/LANG/
    fs::path lang_folder;

    cout << "Extracting texts to POTTER/LANG/TXT/..." << endl;

    // Assigning path to path specified by user in the main
    lang_folder = file_path;

    // Create path to store the extracted output
    fs::path out = file_path / "TXT";
    fs::create_directory(out);

    // Get every single language file in the POTTER/LANG/
    for (const auto& entry : fs::directory_iterator(lang_folder)) {

        // Converting file path to string
        string file_name = entry.path().stem().string();

        // Skipping if the file is the folder to hold the extracted content
        if (file_name == "TXT") {
            continue;
        }

        // Decoding in game strings to a text file
        auto input_file = fs::path(file_path / (file_name + ".BIN"));
        auto output_file = fs::path(out / (file_name + ".txt"));
        decode_game_text(input_file, output_file, decoder);
    }

    cout << "Texts successfully extracted." << endl;

}
