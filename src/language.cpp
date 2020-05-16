#include <iostream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include "language.h"
#include "text_decoder.h"

using namespace std;

void extract_lang(string path) {

    // Decode string array using the game's dictionary
    string decoder[0x100];
    initialize_decoder(decoder);

    // Store path to POTTER/LANG/
    boost::filesystem::path lang_folder;

    cout << "Extracting texts to POTTER/LANG/TXT/..." << endl;

    // Assigning path to path specified by user in the main
    lang_folder = boost::filesystem::path(path);

    // Create path to store the extracted output
    string output_folder = path + "TXT";

    boost::filesystem::path out(output_folder);
    boost::filesystem::create_directory(out);

    // Get every single language file in the POTTER/LANG/
    for (auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(lang_folder), {})) {

        // Converting file path to string
        string file_name = entry.path().string();
        file_name = file_name.substr(path.size(), file_name.size() - (path.size() + 4));

        // Skipping if the file is the folder to hold the extracted content
        if (file_name == "TXT") {

            continue;
            
        }

        // Decoding in game strings to a text file
        decode_game_text(path+file_name+".BIN", output_folder+"/"+file_name+".txt", decoder);

    }

    cout << "Texts successfully extracted." << endl;

}
