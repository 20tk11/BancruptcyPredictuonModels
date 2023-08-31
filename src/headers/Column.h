#include "../../resources/constants.h"
#include <vector>
#include <unordered_map>
#include <iostream>
struct Column{
private:

    template<class T>
    static std::unordered_map<const Column*, std::vector<T>> items;
    std::string _type;

public:

    template <class T>
    void push_back(const T& _t)
    {
        items<T>[this].push_back(_t);
    }

    template <class T>
    void clear()
    {
        items<T>[this].clear();
    }

    template <class T>
    T get(int index)
    {
        return items<T>[this][index];
    }

    template <class T>
    std::vector<T> get_vector()
    {
        return items<T>[this];
    }

    template <class T>
    int size()
    {
        return items<T>[this].size();
    }

    void set_type(std::string type)
    {
        _type = type;
    }

    std::string get_type()
    {
        return _type;
    }

    int getLongestLength()
    {
        //
        
        if(get_type() == TYPE_FLOAT)
        {
            return get_number_length();
        }
        else if(get_type() == TYPE_INTEGER)
        {
            return getLongestInteger();
        }
        else
        {
            return getLongestString();
        }

        return 0;
    }

    int getLongestString()
    {
        int temp = 0;
        int digitCount;

        for(int i = 0; i < size<std::string>(); i++)
        {
            digitCount = get<std::string>(i).size();

            if(digitCount > temp)
            {
                temp = digitCount;
            }
        }

        return temp;
    }

    int getLongestInteger()
    {
        int temp = 0;
        int digitCount;
        std::string numStr;

        for(int i = 0; i < size<int>(); i++)
        {
            std::string numStr = std::to_string(get<int>(i));
            digitCount = numStr.length();

            if(digitCount > temp)
            {
                temp = digitCount;
            }
        }

        return temp;
    }

    int get_number_length()
    {
        int temp = 0;
        int digitCount = 0;
        
        for (int i = 0; i < size<double>(); i++)
        {
            digitCount = 0;

            std::string tempel = std::to_string(get<double>(i));
            char element[tempel.size() + 1];
            std::copy(tempel.begin(), tempel.end(), element);

            for (char c : element)
            {
                
                if (c == '.')
                {
                    break;
                }
                digitCount++;
            }

            if (digitCount > temp)
            {
                temp = digitCount;
            }
        }
        
        return temp;
    }

    template<typename T>
    int unique_count()
    {
        std::vector<T> temp;
        bool unique;
        int count = 0;

        for (int i = 0; i < size<T>(); i++)
        {
            unique = true;

            for (int j = 0; j < temp.size(); j++)
            {
                if (get<T>(i) == temp[j])
                {
                    unique = false;
                    break;
                }
            }

            if (unique)
            {
                temp.push_back(get<T>(i));
                count++;
            }
        }

        return count;
    }

    int get_unique_count()
    {
        if (get_type() == TYPE_FLOAT)
        {
            return unique_count<double>();
        }
        else
        {
            return unique_count<std::string>();
        }
    }
};

template<class T>
std::unordered_map<const Column*, std::vector<T>> Column::items;