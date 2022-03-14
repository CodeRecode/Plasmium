#pragma once
#include <fstream>
#include "Types.h"

namespace Plasmium {
    class Serializable {
    protected:
        virtual uint32 GetSerializationVersion() = 0;
        virtual void SerializeJSON(std::ofstream& output) = 0;
        virtual void DeserializeJSON(std::ifstream& input) = 0;
        virtual ~Serializable() {};
    public:
        void Serialize(std::ofstream& output);
        void Deserialize(std::ifstream& input);
    };
}