#ifndef LEV
#define LEV
#define FILE_TYPE 4

#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

struct LEV_HEADER {
    uint32_t size;
};

struct LEV_METADATA {
    char file_name[FILE_TYPE];
    uint32_t size;
};

void extract_lev(fs::path);

#endif
