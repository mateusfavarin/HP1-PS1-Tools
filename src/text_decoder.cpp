#include <fstream>
#include <string>
#include <filesystem>
#include "text_decoder.h"

using namespace std;
namespace fs = std::filesystem;

void initialize_decoder(string * decoder) {

    decoder[0x0] = "\n";

    for (int i = 0x1; i < 0x20; i++) {
        decoder[i] = "";
    }

    decoder[0x20] = " ";
    decoder[0x21] = "!";
    decoder[0x22] = "\"";
    decoder[0x23] = u8"🇷2️⃣";
    decoder[0x24] = u8"⚡";
    decoder[0x25] = "%";
    decoder[0x26] = u8"⚡";
    decoder[0x27] = "'";
    decoder[0x28] = "(";
    decoder[0x29] = ")";
    decoder[0x2A] = "*";
    decoder[0x2B] = "+";
    decoder[0x2C] = ",";
    decoder[0x2D] = "-";
    decoder[0x2E] = ".";
    decoder[0x2F] = "/";

    char j = '0';
    for (int i = 0x30; i < 0x3A; i++) {
        decoder[i] = j;
        j++;
    }

    decoder[0x3A] = ":";
    decoder[0x3B] = ";";
    decoder[0x3C] = u8"🔲";
    decoder[0x3D] = "=";
    decoder[0x3E] = u8"🇽";
    decoder[0x3F] = "?";
    decoder[0x40] = u8"🎮";

    char letter = 'A';
    for (int i = 0x41; i < 0x5B; i++) {
        decoder[i] = letter;
        letter += 1;
    }

    decoder[0x5B] = u8"🔺";
    decoder[0x5C] = "\n";
    decoder[0x5D] = u8"🅾️";
    decoder[0x5E] = u8"🇱1️⃣";
    decoder[0x5F] = u8"🇷1️⃣";

    letter = 'a';
    for (int i = 0x60; i < 0x7A; i++) {
        decoder[i] = letter;
        letter += 1;
    }

    decoder[0x7A] = "|";
    decoder[0x7B] = u8"➡️";
    decoder[0x7C] = u8"8️⃣";
    decoder[0x7D] = u8"⬅️";
    decoder[0x7E] = u8"¡";
    decoder[0x7F] = u8"©";
    decoder[0x80] = u8"ª";
    decoder[0x81] = u8"º";
    decoder[0x82] = "'";
    decoder[0x83] = u8"¾";
    decoder[0x84] = u8"¿";
    decoder[0x85] = u8"À";
    decoder[0x86] = u8"Á";
    decoder[0x87] = u8"Ã";
    decoder[0x88] = u8"Ä";
    decoder[0x89] = u8"Å";
    decoder[0x8A] = u8"Æ";
    decoder[0x8B] = u8"С";
    decoder[0x8C] = u8"È";
    decoder[0x8D] = u8"É";
    decoder[0x8E] = u8"Ê";
    decoder[0x8F] = u8"Í";
    decoder[0x90] = u8"Ó";
    decoder[0x91] = u8"Ö";
    decoder[0x92] = u8"Ø";
    decoder[0x93] = u8"Ù";
    decoder[0x94] = u8"Ú";
    decoder[0x95] = u8"Ü";
    decoder[0x96] = u8"β";
    decoder[0x97] = u8"à";
    decoder[0x98] = u8"á";
    decoder[0x99] = u8"â";
    decoder[0x9A] = u8"ã";
    decoder[0x9B] = u8"ä";
    decoder[0x9C] = u8"å";
    decoder[0x9D] = u8"æ";
    decoder[0x9E] = u8"с";
    decoder[0x9F] = u8"è";
    decoder[0xA0] = u8"é";
    decoder[0xA1] = u8"ê";
    decoder[0xA2] = u8"ë";
    decoder[0xA3] = u8"ì";
    decoder[0xA4] = u8"í";
    decoder[0xA5] = u8"ĩ";
    decoder[0xA6] = u8"ï";
    decoder[0xA7] = u8"ñ";
    decoder[0xA8] = u8"ò";
    decoder[0xA9] = u8"ó";
    decoder[0xAA] = u8"ô";
    decoder[0xAB] = u8"õ";
    decoder[0xAC] = u8"ö";
    decoder[0xAD] = u8"ø";
    decoder[0xAE] = u8"ù";
    decoder[0xAF] = u8"ú";
    decoder[0xB0] = u8"û";
    decoder[0xB1] = u8"ü";
    decoder[0xB2] = u8"-";
    decoder[0xB3] = u8"'";

    for (int i = 0xB4; i < 0x100; i++) {
        decoder[i] = " ";
    }
}

void decode_game_text(fs::path input_name, fs::path output_name, string decoder[]) {

    // Reading language file and creating output file
    ifstream lang_file(input_name, ios::in | ios::binary);
    ofstream output_file(output_name, ios::out);

    output_file << "\xEF\xBB\xBF"; // UTF-8 BOM

    // Creating data block to store the characters
    LANG_DATA blk;

    do {
        // Read character and write decoded character to text file
        lang_file.read(reinterpret_cast<char*>(&blk), sizeof(blk));
        output_file << decoder[blk.character];
    } while ((lang_file.good()));
}
