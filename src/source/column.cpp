#include "../headers/column.h"

template<class T>
void Column::setColumn(std::vector<T> _vec)
{
    mitems<T>[this] = _vec;
}

template <class T>
void Column::add(const T& _t)
{
    mhas_unique<T>[this] = false;
    mitems<T>[this].push_back(_t);
}

template <class T>
T Column::get(int _index)
{
    return mitems<T>[this][_index];
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

void Column::setType(MType _type)
{
    mtype = _type;
}

MType Column::getType()
{
    return mtype;
}

template <class T>
void Column::clear()
{
    mhas_unique<T>[this] = false;
    mitems<T>[this].clear();
}

template<>
int Column::mgetValueLength<int>(int _index)
{
    return std::to_string(get<int>(_index)).size();
}

template<>
int Column::mgetValueLength<double>(int _index)
{
    int _digit_count = 0;
    std::string _numStr = std::to_string(get<double>(_index));
    char _element[_numStr.size() + 1];
    std::copy(_numStr.begin(), _numStr.end(), _element);

    for (char c : _element)
    {
        if(c == '.')
        {
            break;
        }
        _digit_count++;
    }

    return _digit_count;
}

template<>
int Column::mgetValueLength<std::string>(int _index)
{
    return get<std::string>(_index).size();
}

template<class T>
int Column::getValueLength()
{
    int _temp = 0;
    int _digit_count = 0;

    for (int i = 0; i < size<T>(); i++)
    {
        int _digit_count = mgetValueLength<T>(i);

        if ( _digit_count > _temp)
        {
            _temp = _digit_count;
        }
    }
    
    return _temp;
}

template<class T>
int Column::getUniqueCount()
{
    if (!mhas_unique<T>[this])
    {
        mfindUnique<T>();
        mhas_unique<T>[this] = true;     
    }

    return mitems<T>[this].size();
}

template<class T>
std::vector<T> Column::getUniques()
{
    if (!mhas_unique<T>[this])
    {
        mfindUnique<T>();
        mhas_unique<T>[this] = true;     
    }

    return mitems<T>[this];
}

template<class T>
void Column::mfindUnique()
{
    for (int i = 0; i < size<T>(); i++)
    {
        bool _unique = true;
        T _temp = get<T>(i);
        
        for (int j = 0; j < munique<T>[this].size(); j++)
        {
            if(_temp == munique<T>[this][j])
            {
                _unique = false;
                break;
            }
        }

        if (_unique)
        {
            munique<T>[this].push_back(_temp);
        }
    }
}