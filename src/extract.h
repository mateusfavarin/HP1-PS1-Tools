#ifndef EXTRACT
#define EXTRACT
#define NAME_SIZE 12

#include <string>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

struct HEADER {
    uint32_t num_files;
};

struct METADATA {
    char file_name[NAME_SIZE];
    uint32_t size;
    uint32_t offset;
};

bool check_dir(fs::path);
void extract_dat(fs::path);

#endif
