#include "FileResource.h"

namespace Plasmium {


    FileResource::FileResource(const char* file)
    {
        strcpy_s(this->file, file);
        size = strlen(this->file);
        CreateId();
    }

    // https://stackoverflow.com/questions/8317508/hash-function-for-a-string
    void FileResource::CreateId() {
        uint32 hash = 37, index = 0;
        while (index < size) {
            hash = (hash * 54059) ^ (file[index] * 76963);
            ++index;
        }
        id = hash;
    }
}