#include "../headers/dataframe.h"

Dataframe::Dataframe()
{
    mcolumns.push_back("index");
    mdataframe["index"] = Column();
    mdataframe["index"].setType(MType::TYPE_INTEGER);
}

Column& Dataframe::operator[](const std::string& _key)
{
    return mdataframe[_key];
}

template<class T>
void Dataframe::addColumn(std::vector<T> _vec, std::string _column)
{
    mcolumns.push_back(_column);

    if (msize < _vec.size())
    {
        msize = _vec.size();
    }

    mdataframe[_column] = Column();
    mdataframe[_column].setColumn(_vec);

    if (std::is_same<T, double>::value)
    {
        mdataframe[_column].setType(MType::TYPE_DOUBLE);
    }
    else if (std::is_same<T, int>::value)
    {
        mdataframe[_column].setType(MType::TYPE_INTEGER);
    }
    else if (std::is_same<T, std::string>::value)
    {
        mdataframe[_column].setType(MType::TYPE_STRING);
    }
    else
    {
        mdataframe[_column].setType(MType::TYPE_NONE);
    }
}

int Dataframe::addColumn(std::string _column)
{
    for(int i = 0; i < mcolumns.size(); i++)
    {
        if(mcolumns[i] == _column)
        {
            return ErrorHandler::error_column_exists(_column);
        }
    }

    mcolumns.push_back(_column);
    mdataframe[_column] = Column();

    return 0;
}

void Dataframe::removeColumn(std::string _column)
{
    for (int i =0; i < mcolumns.size(); i++) {

        if (mcolumns[i] == _column)
        {
            mcolumns.erase(mcolumns.begin() + i - 1);
            break;
        }
    }

    auto _it = mdataframe.find(_column);
    if (_it != mdataframe.end()) {
        mdataframe.erase(_it);
    }

}

int Dataframe::readCSV(std::string _file_path, bool _headers, char _delimiter)
{
    std::string _line;
    std::vector<std::string> _dataframe_headers;

    if(!std::filesystem::exists(_file_path))
    {
        return ErrorHandler::error_file_does_not_exist(_file_path);
    }

    std::string _encoding = mdetectEncoding(_file_path);
    std::ifstream _csv_file(_file_path, std::ios::in);

    if (_encoding.find("UTF-8") == 0) 
    {
        // Skip the UTF-8 BOM (EF BB BF)
        _csv_file.seekg(3);
    }
    else if (_encoding.find("UTF-16") == 0) 
    {
        // Skip the UTF-16 BOM (FE FF or FF FE)
        _csv_file.seekg(2);
    }
    else if (_encoding.find("UTF-32") == 0) 
    {
        // Skip the UTF-32 BOM (00 00 FE FF or FF FE 00 00)
        _csv_file.seekg(4);
    }

    if (_headers)
    {
        std::getline(_csv_file, _line);

        _dataframe_headers = msplitString(_line, _delimiter);

        for(int i = 0; i < _dataframe_headers.size(); i++)
        {
            if(addColumn(_dataframe_headers[i]))
            {
                return 1;
            }
        }
    }

    return 0;
}

std::string Dataframe::mdetectEncoding(const std::string& _file_path)
{
    std::fstream _file(_file_path, std::ios::binary);

    if (!_file.is_open()) 
    {
        return "Unknown";
    }

    char _buffer[4];

    _file.read(_buffer, 4);

    if (_file.gcount() < 4) {
        return "Unknown";
    }

    if (_buffer[0] == '\xEF' && _buffer[1] == '\xBB' && _buffer[2] == '\xBF') 
    {
        return "UTF-8";
    }
    if (_buffer[0] == '\xFE' && _buffer[1] == '\xFF') 
    {
        return "UTF-16 Big Endian";
    }
    if (_buffer[0] == '\xFF' && _buffer[1] == '\xFE') 
    {
        return "UTF-16 Little Endian";
    }
    if (_buffer[0] == '\x00' && _buffer[1] == '\x00' && _buffer[2] == '\xFE' && _buffer[3] == '\xFF') 
    {
        return "UTF-32 Big Endian";
    }
    if (_buffer[0] == '\xFF' && _buffer[1] == '\xFE' && _buffer[2] == '\x00' && _buffer[3] == '\x00') 
    {
        return "UTF-32 Little Endian";
    }

    return "Unknown";
}

std::vector<std::string> Dataframe::msplitString(std::string _input, char _delimiter)
{
    std::vector<std::string> _tokens;
    std::string _token;
    char _char_array[_input.size() + 1];
    char _symbol;
    strcpy(_char_array, _input.c_str());

    for (int i = 0; i < strlen(_char_array); i++)
    {
        _symbol = _char_array[i];

        if(_delimiter == _symbol)
        {
            _tokens.push_back(_token);
            _token = "";
        }
        else
        {
            _token += _symbol;
        }

        if(strlen(_char_array) == i + 1)
        {
            _tokens.push_back(_token);
        }
    }

    return _tokens;
}