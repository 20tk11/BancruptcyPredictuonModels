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
    mfindUnique<T>();

    return munique<T>[this].size();
}

template<class T>
std::vector<T> Column::getUniques()
{
    mfindUnique<T>();

    return munique<T>[this];
}

template<class T>
void Column::mfindUnique()
{
    if (!mhas_unique<T>[this])
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
        mhas_unique<T>[this] = true;     
    }
    
}

int Column::getLongestLength()
{
    if (this->mtype == MType::TYPE_DOUBLE)
    {
        return this->getLongestDouble();
    }
    else if (this->mtype == MType::TYPE_INTEGER)
    {
        return this->getLongestInt();
    }
    else
    {
        return this->getLongestString();
    }

    return 1;
}

int Column::getLongestString()
{
    int temp = 0;
    int digit_count;

    for(int i = 0; i < this->size<std::string>(); i++)
    {
        digit_count = this->get<std::string>(i).size();

        if (digit_count > temp)
        {
            temp = digit_count;
        }
    }

    return temp;
}

int Column::getLongestInt()
{
    int temp = 0;
    int digit_count;
    std::string num_str;

    for(int i = 0; i < this->size<int>(); i++)
    {
        std::string num_str = std::to_string(this->get<int>(i));
        digit_count = num_str.length();

        if(digit_count > temp)
        {
            temp = digit_count;
        }
    }

    return temp;
}

int Column::getLongestDouble()
{
    int temp = 0;
    int digit_count;

    for(int i = 0; i < this->size<double>(); i++)
    {
        digit_count = 0;

        std::string temp_el = std::to_string(this->get<double>(i));
        char element[temp_el.size() + 1];

        std::copy(temp_el.begin(), temp_el.end(), element);

        for (char c : element)
        {
            
            if (c == '.')
            {
                break;
            }

            digit_count++;
        }

        if (digit_count > temp)
        {
            temp = digit_count;
        }
    }

    return temp;
}

std::array<MDataType, 2> Column::getDataType()
{
    std::array<MDataType, 2> types;
    double unique_p;
    if (this->mtype == MType::TYPE_STRING)
    {
        unique_p = this->getUniqueCount<std::string>();
        unique_p = unique_p / this->size<std::string>() * 100;

        types[0] = MDataType::CATEGORICAL;

        if (this->getUniqueCount<std::string>() == 2)
        {
            types[1] = MDataType::BINARY;
        }
        else if(this->getUniqueCount<std::string>() <= 10)
        {
            types[1] = MDataType::CATEGORICAL;
        }
        else
        {
            types[1] = MDataType::TEXTUAL;
        }
    }
    else if (this->mtype == MType::TYPE_DOUBLE)
    {
        unique_p = this->getUniqueCount<double>();
        unique_p = unique_p / this->size<double>() * 100;

        types[0] = MDataType::CONTINUOUS;
        if (unique_p <= 10 )
        {
            types[1] = MDataType::INTERVAL;
        }
        else
        {
            types[1] = MDataType::NUMERIC;
        }
    }

    return types;
}

template<class T>
int Column::notNull()
{
    int count = 0;
    T value;

    for (int i = 0; i < this->size<T>(); i++)
    {
        value = this->get<T>(i);

        if (!Format::ccheckNull<T>(value))
        {
            count++;
        }
    }

    return count;
}


/// @brief TODO max min with flag
/// @tparam T 
/// @return 
template<class T>
T Column::max()
{
    T max = this->get<T>(0);

    for (int i = 1; i < this->size<T>(); i++)
    {
        if (max < this->get<T>(i))
        {
            max = this->get<T>(i);
        }
    }

    return max;
}

template<class T>
T Column::min()
{
    T min = this->get<T>(0);

    for (int i = 1; i < this->size<T>(); i++)
    {
        if (min > this->get<T>(i))
        {
            min = this->get<T>(i);
        }
    }

    return min;
}

template<class T>
T Column::quantile(double _quantile)
{
    if (_quantile > 1 || _quantile < 0)
    {
        ErrorHandler::error_invalid_quantile(_quantile);
    }

    std::vector<T> quantile_vec(this->getColumn<T>());

    std::sort(quantile_vec.begin(), quantile_vec.end());

    quantile_vec.erase(std::remove_if(quantile_vec.begin(), quantile_vec.end(), [](T x) {
        return Format::ccheckNull(x);
    }), quantile_vec.end());
    
    int n = quantile_vec.size();
    int index = static_cast<int>(_quantile * (n - 1));

    return quantile_vec[index];
}

template<class T>
T Column::stdDev(double _mean)
{
    std::vector<T> stddev_vec(this->getColumn<T>());
    stddev_vec.erase(std::remove_if(stddev_vec.begin(), stddev_vec.end(), [](T x) {
        return Format::ccheckNull(x);
    }), stddev_vec.end());

    T stddev = pow(stddev_vec[0] - _mean, 2);

    for (int i = 1; i < stddev_vec.size(); i++)
    {
        stddev += (stddev_vec[i] - _mean) * (stddev_vec[i] - _mean);
    }

    return sqrt(stddev / (this->size<T>() - 1));
}

template<class T>
T Column::Mean()
{
    std::vector<T> mean_vec(this->getColumn<T>());
    mean_vec.erase(std::remove_if(mean_vec.begin(), mean_vec.end(), [](T x) {
        return Format::ccheckNull(x);
    }), mean_vec.end());

    T mean = mean_vec[0];

    for (int i = 1; i < mean_vec.size(); i++)
    {
        mean += mean_vec[i];
    }

    return mean / mean_vec.size();
}

template<class T>
std::vector<std::pair<std::string, int>> Column::modes()
{
    std::vector<T> mode_vec(this->getColumn<T>());
    mode_vec.erase(std::remove_if(mode_vec.begin(), mode_vec.end(), [](T x) {
        return Format::ccheckNull(x);
    }), mode_vec.end());

    std::map<T, int> mode_map;

    
    for (int i = 0; i < mode_vec.size(); i++)
    {
        mode_map[mode_vec[i]]++;
    }
    
    std::vector<std::pair<std::string, int>> key_value_pairs;

    for (const auto& pair : mode_map) {
        key_value_pairs.push_back(pair);
    }

    // Sort the vector of pairs based on the integer values
    std::sort(key_value_pairs.begin(), key_value_pairs.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // Create a new map from the sorted vector of pairs

    return key_value_pairs;
}