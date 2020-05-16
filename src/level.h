#ifndef LEV
#define LEV
#define FILE_TYPE 4

using namespace std;

struct LEV_HEADER {
    uint32_t size;
};

struct LEV_METADATA {
    char file_name[FILE_TYPE];
    uint32_t size;
};

void extract_lev(string path);

#endif