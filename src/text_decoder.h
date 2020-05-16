#ifndef TEXT_DECODER
#define TEXT_DECODER

#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

struct LANG_DATA {
    uint8_t character;
    uint8_t ignore;
};

void initialize_decoder(string * decoder);
void decode_game_text(fs::path input_name, fs::path output_name, string decoder[]);

#endif
