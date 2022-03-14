#pragma once
#include "Level.h"
#include "FileResource.h"
#include "Serialization.h"

namespace Plasmium {
    class StaticLevel : public Level, protected Serializable {
    private:
        FileResource file;

        uint32 GetSerializationVersion() { return 1; }
        void SerializeJSON(std::ofstream& output);
        void DeserializeJSON(std::ifstream& input);

    public:
        StaticLevel(FileResource file) : file(file) {}
        void Load() override;
        void Unload() override;
    };
}