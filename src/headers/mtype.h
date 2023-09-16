#include <string>
#include <map>

enum MType {
    TYPE_INTEGER,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_NONE
};

std::map<MType, std::string> MTypes = {
        {MType::TYPE_INTEGER, "INTEGER"},
        {MType::TYPE_DOUBLE, "DOUBLE"},
        {MType::TYPE_STRING, "STRING"},
        {MType::TYPE_NONE, "NONE"}
    };

