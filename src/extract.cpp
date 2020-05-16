#include <iostream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include "extract.h"

using namespace std;

bool check_dir(string path) {

    // Creating path for current folder
    boost::filesystem::path folder(path);

    bool dir = false;
    bool dat = false;

    // Checking if the folder has POTTER.DIR and POTTER.DAT files
    for (auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(folder), {})) {

        string file_name = entry.path().string();
        file_name = file_name.substr(path.size(), file_name.size() - 2);

        if (file_name == "POTTER.DIR") {

            dir = true;

        } else if (file_name == "POTTER.DAT") {

            dat = true;

        }

        // If POTTER.DIR and POTTER.DAT are in the folder
        if (dir && dat) {

            return true;

        }

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

void extract_dat(string path) {

    // ifstream to read the Harry Potter DIR and DAT files
    ifstream potter_dir;
    ifstream potter_dat;

    // Opening files
    potter_dir.open(path+"POTTER.DIR", ios::in | ios::binary);
    potter_dat.open(path+"POTTER.DAT", ios::in | ios::binary);

    // Getting number of files to extract
    HEADER header;
    potter_dir.read((char *) &header, sizeof(header));

    METADATA metadata;

    // Creating directories to put extracted data
    string potter_path = path+"POTTER";
    boost::filesystem::path main_potter(potter_path);
    boost::filesystem::create_directory(main_potter);

    boost::filesystem::path lang(potter_path+"/LANG");
    boost::filesystem::create_directory(lang);
    boost::filesystem::path img(potter_path+"/IMG");
    boost::filesystem::create_directory(img);
    boost::filesystem::path lev(potter_path+"/LEV");
    boost::filesystem::create_directory(lev);

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
            new_file.open(potter_path+"/LANG/"+file_name, ios::out | ios::binary);
        } else if (extension == "IMG") {
            new_file.open(potter_path+"/IMG/"+file_name, ios::out | ios::binary);
        } else {
            new_file.open(potter_path+"/LEV/"+file_name, ios::out | ios::binary);
        }
        new_file.write(mem_block, metadata.size);
        new_file.close();

        // Freeing data space to be used later for the next file
        delete[] mem_block;

        cout << file_name << " successfully extracted." << endl;

    }

    potter_dir.close();
    potter_dat.close();

}
