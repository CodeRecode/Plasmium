#include "Serialization.h"

namespace Plasmium {
    void Serializable::Serialize(std::ofstream& output) {
        SerializeJSON(output);
    }

    void Serializable::Deserialize(std::ifstream& input) {
        DeserializeJSON(input);
    }
}