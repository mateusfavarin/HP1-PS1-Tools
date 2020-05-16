#ifndef EXTRACT
#define EXTRACT
#define NAME_SIZE 12

using namespace std;

struct HEADER {
    uint32_t num_files;
};

struct METADATA {
    char file_name[NAME_SIZE];
    uint32_t size;
    uint32_t offset;
};

bool check_dir(string path);
string convert_file_name(char name[]);
void extract_dat(string path);

#endif