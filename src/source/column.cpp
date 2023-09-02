#include "../headers/column.h"

template <class T>
void Column::add(const T& t)
{
    mrecalculate_uniques = false;
    mitems<T>[this].push_back(t);
}

template <class T>
T Column::get(int index)
{
    return mitems<T>[this][index];
}

template <class T>
std::vector<T> Column::getColumn()
{
    return mitems<T>[this];
}

template <class T>
int Column::size()
{
    return mitems<T>[this].size();
}

void Column::setType(MType type)
{
    mrecalculate_uniques = false;
    mtype = type;
}

MType Column::getType()
{
    return mtype;
}

template <class T>
void Column::clear()
{
    mrecalculate_uniques = false;
    mitems<T>[this].clear();
}

template<>
int Column::mgetValueLength<int>(int index)
{
    return std::to_string(get<int>(index)).size();
}

template<>
int Column::mgetValueLength<double>(int index)
{
    int digit_count = 0;
    std::string numStr = std::to_string(get<double>(index));
    char element[numStr.size() + 1];
    std::copy(numStr.begin(), numStr.end(), element);

    for (char c : element)
    {
        if(c == '.')
        {
            break;
        }
        digit_count++;
    }

    return digit_count;
}

template<>
int Column::mgetValueLength<std::string>(int index)
{
    return get<std::string>(index).size();
}

template<class T>
int Column::getValueLength()
{
    int temp = 0;
    int digitCount = 0;

    for (int i = 0; i < size<T>(); i++)
    {
        int digit_count = mgetValueLength<T>(i);

        if ( digit_count > temp)
        {
            temp = digit_count;
        }
    }
    
    return temp;
}


