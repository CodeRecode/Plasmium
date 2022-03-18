#include "FileResource.h"
#include "HashTable.h"

namespace Plasmium {

    FileExentionType FileResource::GetFileExtensionType() const
    {
        const char* fileStr = file.Get();

        const char* startIndex = "";
        for (int32 index = file.Size() - 1; index >= 0; --index) {
            if (fileStr[index] == '.') {
                startIndex = &fileStr[index + 1];
                break;
            }
        }
        if (strstr(startIndex, "lvl")) {
            return FileExentionType::Level;
        }
        else if (strstr(startIndex, "ini")) {
            return FileExentionType::Config;
        }
        else if (strstr(startIndex, "fbx")) {
            return FileExentionType::FBX;
        }
        else if (strstr(startIndex, "obj")) {
            return FileExentionType::OBJ;
        }

        return FileExentionType::Unknown;
    }
}