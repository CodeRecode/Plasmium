#include "PlasString.h"
#include <string>
#include "HashTable.h"

namespace Plasmium {
    PlasString::PlasString(const char* string)
    {
        strcpy_s(this->string, string);
        length = strlen(this->string);
        id = HashFunc<const char *>(this->string);
    }
}