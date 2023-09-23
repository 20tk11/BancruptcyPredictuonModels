#ifndef TYPEDEF_H
#define TYPEDEF_H

#include "mtype.h"
#include <string>

template<MType T>
class Typedef
{
    public:
        using class_type = void;
};

template<>
class Typedef<MType::TYPE_DOUBLE>
{
    public:
        using class_type = double;
};

template<>
class Typedef<MType::TYPE_INTEGER>
{
    public:
        using class_type = int;
};

template<>
class Typedef<MType::TYPE_STRING>
{
    public:
        using class_type = std::string;
};

#endif // TYPEDEF_H