#ifndef MTYPE_H
#define MTYPE_H

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

enum MDataType {
    CATEGORICAL,
    CONTINUOUS,
    NUMERIC,
    INTERVAL,
    ORDINAL,
    BINARY,
    TEXTUAL,
    NONE
};

std::map<MDataType, std::string> MDataTypes = {
        {MDataType::CATEGORICAL, "CATEGORICAL"},
        {MDataType::CONTINUOUS, "CONTINUOUS"},
        {MDataType::NUMERIC, "NUMERIC"},
        {MDataType::INTERVAL, "INTERVAL"},
        {MDataType::ORDINAL, "ORDINAL"},
        {MDataType::TEXTUAL, "TEXTUAL"},
        {MDataType::BINARY, "BINARY"},
        {MDataType::NONE, ""}
    };
#endif // MTYPE_H