#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "extract.h"

using namespace std;
namespace fs = std::filesystem;

bool check_dir(fs::path file_path) {

    // Creating path for current folder
    fs::path folder(file_path);

    bool dir = false;
    bool dat = false;

    // Checking if the folder has POTTER.DIR and POTTER.DAT files
    for (const auto& entry : fs::directory_iterator(folder)) {

        string file_name = entry.path().filename().string();

        if (file_name == "POTTER.DIR") {
            dir = true;
        } else if (file_name == "POTTER.DAT") {
            dat = true;
        }

    }

    // If POTTER.DIR and POTTER.DAT are in the folder
    if (dir && dat) {
        return true;
    }
    return false;
}

string convert_file_name(char name[]) {

    string file_name;

    for (int i = 0; i < NAME_SIZE; i++) {

        // Search for end of string
        if (name[i] == 0x00) {
            break;
        }

        // Adds character to the string
        file_name += name[i];
    }

    return file_name;
}

void extract_dat(fs::path file_path) {

    // ifstream to read the Harry Potter DIR and DAT files
    ifstream potter_dir;
    ifstream potter_dat;

    // Opening files
    potter_dir.open(file_path / "POTTER.DIR", ios::in | ios::binary);
    potter_dat.open(file_path / "POTTER.DAT", ios::in | ios::binary);

    // Getting number of files to extract
    HEADER header;
    potter_dir.read((char *) &header, sizeof(header));

    METADATA metadata;

    // Creating directories to put extracted data
    fs::path potter_path = file_path / "POTTER";
    fs::path main_potter = potter_path;
    fs::create_directory(main_potter);

    fs::path lang(potter_path / "LANG");
    fs::create_directory(lang);
    fs::path img(potter_path / "IMG");
    fs::create_directory(img);
    fs::path lev(potter_path / "LEV");
    fs::create_directory(lev);

    // Memory block to hold variable size of data to read from the POTTER.DAT
    char * mem_block;

    cout << "Extracting POTTER.DAT to POTTER/..." << endl;

    // Reading every single file information
    for (int i = 0; i < header.num_files; i++) {

        // Reading file metadata
        potter_dir.read((char *) &metadata, sizeof(metadata));

        // Creating a block of memory to read the data from POTTER.DAT
        mem_block = new char[metadata.size];

        // Adjusting pointer to read the data from
        potter_dat.seekg(metadata.offset, ios::beg);
        potter_dat.read(mem_block, metadata.size);

        // Converting filename to a string
        string file_name = convert_file_name(metadata.file_name);
        string extension = file_name.substr(file_name.size() - 3);

        // Creating extracted file
        ofstream new_file;
        if (extension == "BIN") {
            new_file.open(potter_path / "LANG" / file_name, ios::out | ios::binary);
        } else if (extension == "IMG") {
            new_file.open(potter_path / "IMG" / file_name, ios::out | ios::binary);
        } else {
            new_file.open(potter_path / "LEV" / file_name, ios::out | ios::binary);
        }
        new_file.write(mem_block, metadata.size);
        new_file.close();

        // Freeing data space to be used later for the next file
        delete[] mem_block;

        cout << file_name << " successfully extracted." << endl;

    }

}
