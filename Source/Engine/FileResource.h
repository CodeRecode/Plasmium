#pragma once
#include "Types.h"
#include <string>
#include <fstream>

#include "Array.h"
#include "Vertex.h"
#include "PlasString.h"

namespace Plasmium
{
    enum class FileExentionType {
        Unknown,
        Level,
        Config,
        FBX,
        OBJ,
        FileExentionTypeCount
    };
    class FileResource {
    private:
        PlasString file;
        FileResource(): file("") {}

    public:
        FileResource(const char* file) : file(file) {}

        const char* GetFileName() const { return file.Get(); }
        FileExentionType GetFileExtensionType() const;
        FileId GetId() const { return file.GetId(); }

        std::ifstream GetInputStream() {
            return std::ifstream(file.Get());
        }

        std::ofstream GetOutputStream() {
            return std::ofstream(file.Get());
        }

        static const FileResource& None() {
            static FileResource none;
            return none;
        }
        bool IsNone() const { return GetId() == None().GetId(); }
    };
}