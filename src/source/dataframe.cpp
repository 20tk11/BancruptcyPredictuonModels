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

template<class T>
std::vector<T> Dataframe::getColumn(std::string _column_name)
{
    return this->mdataframe[_column_name].getColumn<T>();
}

std::vector<std::string> Dataframe::getColumnNames()
{
    return this->mcolumns;
}

int Dataframe::getColumnCount()
{
    return this->mcolumns.size() - 1;
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

int Dataframe::readCSV(std::string _file_path, bool _headers, bool _types, char _delimiter)
{
    std::string line;
    std::vector<std::string> dataframe_headers;
    std::vector<std::string> types_headers;
    std::vector<std::string> elements;

    if(!std::filesystem::exists(_file_path))
    {
        return ErrorHandler::error_file_does_not_exist(_file_path);
    }

    std::string encoding = mdetectEncoding(_file_path);
    std::ifstream _csv_file(_file_path, std::ios::in);

    if (encoding.find("UTF-8") == 0) 
    {
        // Skip the UTF-8 BOM (EF BB BF)
        _csv_file.seekg(3);
    }
    else if (encoding.find("UTF-16") == 0) 
    {
        // Skip the UTF-16 BOM (FE FF or FF FE)
        _csv_file.seekg(2);
    }
    else if (encoding.find("UTF-32") == 0) 
    {
        // Skip the UTF-32 BOM (00 00 FE FF or FF FE 00 00)
        _csv_file.seekg(4);
    }

    if (_headers)
    {
        std::getline(_csv_file, line);

        dataframe_headers = msplitString(line, _delimiter);

        for(int i = 0; i < dataframe_headers.size(); i++)
        {
            if(addColumn(dataframe_headers[i]))
            {
                return 1;
            }
        }
    }
    
    if (_types)
    {
        std::getline(_csv_file, line);
        
        types_headers = msplitString(line, _delimiter);
        for (int i = 0; i < types_headers.size(); i++)
        {
            if (!_headers)
            {
                std::cout << "|" << this->mcolumns[i] << "|" << " " << std::endl;
                this->addColumn("Column" + std::to_string(i + 1));
            }

            this->mdataframe[this->mcolumns[i+1]].setType(this->findType(types_headers[i]));
        }

        if (types_headers.size() != this->mcolumns.size() - 1)
        {
            return ErrorHandler::error_invalid_dataframe_dimensions(this->mcolumns.size() - 1, types_headers.size(), "TYPE");
        }
    }

    while(std::getline(_csv_file, line))
    {
        elements = this->msplitString(line, _delimiter);

        if (elements.size() != this->mcolumns.size() - 1)
        {
            return ErrorHandler::error_invalid_dataframe_dimensions(this->mcolumns.size() - 1, elements.size(), "ROW", this->msize);
        }

        for(int i = 0; i < elements.size(); i++)
        {
            if(!_headers && !_types && this->msize == 0)
            {
                this->addColumn("Column" + std::to_string(i + 1));
            }
            if (!_types && this->msize == 0)
            {
                if (Format::cisNumber(elements[i]))
                {
                    this->mdataframe[this->mcolumns[i + 1]].setType(MType::TYPE_DOUBLE);
                }
                else if (Format::ccheckNull(elements[i]))
                {
                    this->mdataframe[this->mcolumns[i + 1]].setType(MType::TYPE_NONE);
                }
                else
                {
                    this->mdataframe[this->mcolumns[i + 1]].setType(MType::TYPE_STRING);
                }
            }

            if (!_types)
            {
                if (this->mdataframe[this->mcolumns[i + 1]].getType() == MType::TYPE_DOUBLE)
                {
                    if (Format::cisNumber(elements[i]))
                    {
                        this->mdataframe[this->mcolumns[i + 1]].add(stod(elements[i]));
                    }
                    else if (Format::ccheckNull(elements[i]))
                    {
                        this->mdataframe[this->mcolumns[i + 1]].add(std::numeric_limits<double>::min());
                    }
                    else
                    {
                        for(int y = 0; y < this->mdataframe[this->mcolumns[i + 1]].size<double>(); y++)
                        {
                            std::ostringstream oss;
                            oss << std::fixed << std::setprecision(this->mgetPrecision(this->mdataframe[this->mcolumns[i + 1]].get<double>(y))) << this->mdataframe[this->mcolumns[i + 1]].get<double>(y);
                            if (this->mdataframe[this->mcolumns[i + 1]].get<double>(y) == std::numeric_limits<double>::min())
                            {
                                this->mdataframe[this->mcolumns[i + 1]].add(std::string(""));
                            }
                            else
                            {
                                this->mdataframe[this->mcolumns[i + 1]].add(oss.str());
                            }
                        }
                        this->mdataframe[this->mcolumns[i + 1]].clear<double>();
                        this->mdataframe[this->mcolumns[i + 1]].setType(MType::TYPE_STRING);
                        this->mdataframe[this->mcolumns[i + 1]].add(elements[i]);
                    }
                }
                else if (this->mdataframe[this->mcolumns[i + 1]].getType() == MType::TYPE_INTEGER)
                {
                    if (Format::cisNumber(elements[i]))
                    {
                        this->mdataframe[this->mcolumns[i + 1]].add(stoi(elements[i]));
                    }
                    else if (Format::ccheckNull(elements[i]))
                    {
                        this->mdataframe[this->mcolumns[i + 1]].add(std::numeric_limits<int>::min());
                    }
                    else
                    {
                        for(int y = 0; y < this->mdataframe[this->mcolumns[i + 1]].size<int>(); y++)
                        {
                            if (this->mdataframe[this->mcolumns[i + 1]].get<int>(y) == std::numeric_limits<int>::min())
                            {
                                this->mdataframe[this->mcolumns[i + 1]].add(std::string(""));
                            }
                            else
                            {
                                this->mdataframe[this->mcolumns[i + 1]].add(std::to_string(this->mdataframe[this->mcolumns[i + 1]].get<int>(y)));
                            }
                        }
                        this->mdataframe[this->mcolumns[i + 1]].clear<int>();
                        this->mdataframe[this->mcolumns[i + 1]].setType(MType::TYPE_STRING);
                        this->mdataframe[this->mcolumns[i + 1]].add(elements[i]);
                    }
                }
                else if (this->mdataframe[this->mcolumns[i + 1]].getType() == MType::TYPE_STRING)
                {
                    this->mdataframe[this->mcolumns[i + 1]].add(elements[i]);
                }
                else if (this->mdataframe[this->mcolumns[i + 1]].getType() == MType::TYPE_NONE)
                {
                    if (Format::ccheckNull(elements[i]))
                    {
                        this->mdataframe[this->mcolumns[i + 1]].add(elements[i]);
                    }
                    else
                    {
                        if(Format::cisNumber(elements[i]))
                        {
                            for(int y = 0; y < this->mdataframe[this->mcolumns[i + 1]].size<std::string>(); y++)
                            {
                                this->mdataframe[this->mcolumns[i + 1]].add(std::numeric_limits<double>::min());
                            }
                           this->mdataframe[this->mcolumns[i + 1]].clear<std::string>();
                            this->mdataframe[this->mcolumns[i + 1]].setType(MType::TYPE_DOUBLE);
                            this->mdataframe[this->mcolumns[i + 1]].add(stod(elements[i]));
                        }
                        else
                        {
                            this->mdataframe[this->mcolumns[i + 1]].setType(MType::TYPE_STRING);
                            this->mdataframe[this->mcolumns[i + 1]].add(elements[i]);
                        }
                    }
                }
            }
            else
            {
                if (this->mdataframe[this->mcolumns[i + 1]].getType() == MType::TYPE_DOUBLE)
                {
                    if (Format::ccheckNull(elements[i]))
                    {
                        this->mdataframe[this->mcolumns[i + 1]].add(std::numeric_limits<double>::min());
                        
                    }
                    else if (Format::cisNumber(elements[i]))
                    {
                        this->mdataframe[this->mcolumns[i + 1]].add(stod(elements[i]));
                    }
                    else
                    {
                        return ErrorHandler::error_invalid_element_type(elements[i], MTypes[MType::TYPE_STRING]);
                    }
                }
                else if (this->mdataframe[this->mcolumns[i + 1]].getType() == MType::TYPE_STRING)
                {
                    this->mdataframe[this->mcolumns[i + 1]].add(elements[i]);
                }
                else if (this->mdataframe[this->mcolumns[i + 1]].getType() == MType::TYPE_INTEGER)
                {
                    if (Format::ccheckNull(elements[i]))
                    {
                        this->mdataframe[this->mcolumns[i + 1]].add(std::numeric_limits<int>::min());
                        
                    }
                    else if (Format::cisNumber(elements[i]))
                    {
                        this->mdataframe[this->mcolumns[i + 1]].add(stoi(elements[i]));
                    }
                    else
                    {
                        return ErrorHandler::error_invalid_element_type(elements[i], MTypes[MType::TYPE_STRING]);
                    }
                }
                
            }

        }

        this->mdataframe["index"].add(this->msize);
        this->msize++;
    }
    return 0;
}

int Dataframe::readTSV(std::string _file_path, bool _headers, bool _types)
{
    return this->readCSV(_file_path, _headers, _types, '\t');
}

MType Dataframe::findType(std::string _value)
{
    MType _key;
    for (const auto& pair : MTypes) {
        if (_value == pair.second)
        {
            return pair.first;
        }
    }

    return MType::TYPE_NONE;
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
            if (_symbol != '\r' && _symbol != '\n')
            {
                _token += _symbol;
            }
        }
    }
    
    _tokens.push_back(_token);

    return _tokens;
}

void Dataframe::mgetPrintFormat(std::vector<int>& _column_lengths, int& _seperator_length)
{
    int header_length;
    int max_value_length;
    int init = 7;
    int length;
    _seperator_length = 1;

    for (int i = 0; i < this->mcolumns.size(); i++)
    {
        header_length = this->mcolumns[i].length();
    
        max_value_length = this->mdataframe[this->mcolumns[i]].getLongestLength();

        if (this->mdataframe[this->mcolumns[i]].getType() == MType::TYPE_DOUBLE)
        {
            max_value_length = max_value_length + 5;
        }

        length = init < max_value_length ? max_value_length : init;
        length = length < header_length ? header_length : length;

        _column_lengths.push_back(length);
        _seperator_length += length + 3;
    }
}

void Dataframe::display(std::string _result_path)
{
    std::ofstream result_file;
    std::vector<int> column_lengths;
    int seperator_length;
    std::string seperator;

    remove(_result_path.c_str());
    result_file.open(_result_path, std::ios::out);

    this->mgetPrintFormat(column_lengths, seperator_length);

    seperator = std::string(seperator_length, '-');

    result_file << seperator << std::endl;
    this->mprintHeader(result_file, column_lengths);
    result_file << seperator << std::endl;
    this->mprintContent(result_file, column_lengths);
    result_file << seperator << std::endl;
    this->mprintFooter(result_file, column_lengths);
    result_file << seperator << std::endl;

    result_file << "  Size: " << this->msize;

    result_file.close();
}

void Dataframe::mprintHeader(std::ofstream& _result_file, std::vector<int> _column_lengths)
{
    std::stringstream format;

    format << "|";

    for (int i = 0; i < this->mcolumns.size(); i++)
    {
        format << " ";

        if(this->mdataframe[this->mcolumns[i]].getType() == MType::TYPE_DOUBLE)
        {
            format << std::right;
        }
        else
        {
            format << std::left;
        }

        format << std::setw(_column_lengths[i]) << this->mcolumns[i] << " |";
    }

    _result_file << format.str() << std::endl;
}

void Dataframe::mprintFooter(std::ofstream& _result_file, std::vector<int> _column_lengths)
{
    std::stringstream format;
    std::string type_identifier;

    format << "|";

    for (int i = 0; i < this->mcolumns.size(); i++)
    {
        type_identifier = MTypes[this->mdataframe[this->mcolumns[i]].getType()];
        type_identifier = type_identifier == "" ? " TYPE" : type_identifier;
        
        format << " ";

        if (this->mdataframe[this->mcolumns[i]].getType() == MType::TYPE_DOUBLE)
        {
            format << std::right;
        }
        else
        {
            format << std::left;
        }

        format << std::setw(_column_lengths[i]) << type_identifier << " |";
    }

    _result_file << format.str() << std::endl;
}

void Dataframe::mprintContent(std::ofstream& _result_file, std::vector<int> _column_lengths)
{
    for (int i = 0; i < this->msize; i++)
    {
        std::stringstream format;

        format << "|";

        for (int j = 0; j < this->mcolumns.size(); j++)
        {
            format << " ";

            //std::cout << "Index: " << i << " Column: " << this->mcolumns[j] << std::endl;
            if(this->mdataframe[this->mcolumns[j]].getType() == MType::TYPE_DOUBLE)
            {
                format << std::right;

                if (this->mdataframe[this->mcolumns[j]].get<double>(i) != std::numeric_limits<double>::min())
                {
                    format << std::fixed << std::setprecision(4) << std::setw(_column_lengths[j]) 
                           << this->mdataframe[this->mcolumns[j]].get<double>(i);
                }
                else
                {
                    format << std::setw(_column_lengths[j]) << "NULL";  
                } 
            }
            else
            {
                format << std::left;

                if(this->mdataframe[this->mcolumns[j]].getType() == MType::TYPE_INTEGER)
                {
                    if (this->mdataframe[this->mcolumns[j]].get<int>(i) != std::numeric_limits<int>::min())
                    {
                        format << std::setw(_column_lengths[j]) << this->mdataframe[this->mcolumns[j]].get<int>(i);
                    }
                    else
                    {
                        format << std::setw(_column_lengths[j]) << "NULL"; 
                    } 
                }
                else
                {
                    format << std::setw(_column_lengths[j] + (this->mdataframe[this->mcolumns[j]].get<std::string>(i).length() - 
                        Format::cutf8CharacterWidth(this->mdataframe[this->mcolumns[j]].get<std::string>(i)))) << this->mdataframe[this->mcolumns[j]].get<std::string>(i);
                }
            } 

            format << " |";
        }

        _result_file << format.str() << std::endl;
    }
}

int Dataframe::mgetPrecision(double _val)
{
    char double_char[std::to_string(_val).size() + 1];

    strcpy(double_char, std::to_string(_val).c_str());
    
    bool after_ptr = false;
    int after_counter = 0;
    int after_count = 0;

    for (int i = 0; double_char[i] != '\0'; ++i) {

        if (after_ptr)
        {
            after_counter++;
            if (double_char[i] != '0')
            {
                after_count = after_counter;
            }
        }

        if (double_char[i] == '.')
        {
            after_ptr = true;
        }
    }

    return after_count;
}

int Dataframe::size()
{
    return this->size();
}

void Dataframe::analyseVariables()
{
    std::array<MDataType, 2> types;
    double unique_count;
    double null_percent;
    int not_null_count;

    for (int i = 1; i < this->mcolumns.size(); i++)
    {
        types = this->mdataframe[this->mcolumns[i]].getDataType();

        

        if (this->mdataframe[this->mcolumns[i]].getType() == MType::TYPE_DOUBLE)
        {
            unique_count = this->mdataframe[this->mcolumns[i]].getUniqueCount<double>();
            not_null_count = this->mdataframe[this->mcolumns[i]].notNull<double>();
            null_percent = not_null_count;
            null_percent = null_percent / this->mdataframe[this->mcolumns[i]].size<double>() * 100;
            double mean = this->mdataframe[this->mcolumns[i]].Mean<double>();

            VariableResults variable_results(this->mcolumns[i], types[0], types[1], not_null_count, 
                                                    null_percent, unique_count, 
                                                    this->mdataframe[this->mcolumns[i]].min<double>(),
                                                    this->mdataframe[this->mcolumns[i]].max<double>(),
                                                    this->mdataframe[this->mcolumns[i]].quantile<double>(0.25),
                                                    mean,
                                                    this->mdataframe[this->mcolumns[i]].quantile<double>(0.5),
                                                    this->mdataframe[this->mcolumns[i]].quantile<double>(0.75),
                                                    this->mdataframe[this->mcolumns[i]].stdDev<double>(mean),
                                                    "", "", "", "", "", "");

            mvariable_results[this->mcolumns[i]] = variable_results;
        }
        else if (this->mdataframe[this->mcolumns[i]].getType() == MType::TYPE_STRING)
        {
            unique_count = this->mdataframe[this->mcolumns[i]].getUniqueCount<std::string>();
            not_null_count = this->mdataframe[this->mcolumns[i]].notNull<std::string>();
            null_percent = not_null_count;
            null_percent = null_percent / this->mdataframe[this->mcolumns[i]].size<std::string>() * 100;

            std::vector<std::pair<std::string, int>> modes = this->mdataframe[this->mcolumns[i]].modes<std::string>();
            
            std::cout << modes[0].first << " " << modes[0].second << std::endl;
            std::cout << modes[1].first << " " << modes[1].second << std::endl << std::endl;

            VariableResults variable_results(this->mcolumns[i], types[0], types[1], not_null_count, null_percent, unique_count, 
                                                    "", "", "", "", "", "", "", modes[0].first, "", modes[0].second, modes[1].first, "", modes[1].second);

            mvariable_results[this->mcolumns[i]] = variable_results;

            
        }
    }
}

void Dataframe::displayVariableResults(std::string _result_path)
{
    std::ofstream result_file;
    std::string seperator;

    remove(_result_path.c_str());
    result_file.open(_result_path, std::ios::out);

    seperator = std::string(VariableResults::seperator_length , '-');

    result_file << seperator << std::endl;
    VariableResults::printHeader(result_file);
    result_file << seperator << std::endl;

    for (int i = 1; i < this->mcolumns.size(); i++)
    {
        if (this->mdataframe[this->mcolumns[i]].getType() == MType::TYPE_DOUBLE)
        {            
            mvariable_results[this->mcolumns[i]].print<double>(result_file);
        }
        else if (this->mdataframe[this->mcolumns[i]].getType() == MType::TYPE_STRING)
        {
            mvariable_results[this->mcolumns[i]].print<std::string>(result_file);
        }
    }

    result_file << seperator << std::endl;

    result_file.close();
}

void Dataframe::setVariableFeature(std::string _column, MDataType _type)
{
    this->mvariable_results[_column].setFeatureType(_type);
}

void Dataframe::setVariableDataType(std::string _column, MDataType _type)
{
    this->mvariable_results[_column].setDataType(_type);
}