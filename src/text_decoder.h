#ifndef TEXT_DECODER
#define TEXT_DECODER

using namespace std;

struct LANG_DATA {
    uint8_t character;
    uint8_t ignore;
};

void initialize_decoder(string * decoder);
void decode_game_text(string input_name, string output_name, string * decoder);

#endif