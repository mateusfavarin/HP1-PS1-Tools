#include <iostream>
#include <string>
#include <filesystem>
#include "extract.h"
#include "language.h"
#include "level.h"

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

    // Setting default folder as the folder where the program was called
    fs::path folder = "./";

    // If user specified folder when running the program
    if (argc > 1) {
        folder = argv[1];
    }

    // If the choosen directory has POTTER.DIR and POTTER.DAT
    if (check_dir(folder)) {

        extract_dat(folder);
        extract_lang(folder / "POTTER/LANG");
        extract_lev(folder / "POTTER/LEV");

        cout << "Extraction successfully completed." << endl;

    } else {

        cout << "ERROR: Couldn't find POTTER.DIR and POTTER.DAT in the specified folder. Please extract those files from the game ROM and make sure that they're in the specified folder." << endl;

    }

    return 0;
}
