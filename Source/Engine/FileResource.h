#pragma once
#include "Types.h"
#include <string>
#include <fstream>

#include "Array.h"
#include "Vertex.h"

namespace Plasmium
{
    class FileResource {
    private:
        char file[256];
        uint32 size;
        FileId id = 0;

        void CreateId();

        FileResource() { }
    public:
        FileResource(const char* file);

        std::string GetFileName() { return file; }
        FileId GetId() { return id; }

        std::ifstream GetInputStream() {
            return std::ifstream(file);
        }

        std::ofstream GetOutputStream() {
            return std::ofstream(file);
        }

        bool IsNone() const { return id == 0; }
        static const FileResource& None() {
            static FileResource none;
            return none;
        }
    };
}