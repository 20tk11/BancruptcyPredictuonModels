#include "Column.h"
#include "ErrorHandler.h"
#include <map>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <limits>
#include <cstring> 
#include <sstream>

std::string TYPE__INTEGER = "INTEGER";
std::string TYPE__STRING = "STRING";
std::string TYPE__FLOAT = "FLOAT";
std::string TYPE__NONE = "NONE";

class Dataframe{

    private:

        std::map<std::string, Column> _dataframe = {
            {"index", Column()}
        };

        int _size = 0;
        std::vector<std::string> columns;

        std::string detectEncoding(const std::string& filePath) 
        {
            
            std::ifstream file(filePath, std::ios::binary);
            
            if (!file.is_open()) {
                return "Unknown";
            }

            char buffer[4];
            file.read(buffer, 4);

            if (file.gcount() < 4) {
                return "Unknown";
            }

            // Check for BOMs and return the corresponding encoding
            if (buffer[0] == '\xEF' && buffer[1] == '\xBB' && buffer[2] == '\xBF') {
                return "UTF-8";
            }
            if (buffer[0] == '\xFE' && buffer[1] == '\xFF') {
                return "UTF-16 Big Endian";
            }
            if (buffer[0] == '\xFF' && buffer[1] == '\xFE') {
                return "UTF-16 Little Endian";
            }
            if (buffer[0] == '\x00' && buffer[1] == '\x00' && buffer[2] == '\xFE' && buffer[3] == '\xFF') {
                return "UTF-32 Big Endian";
            }
            if (buffer[0] == '\xFF' && buffer[1] == '\xFE' && buffer[2] == '\x00' && buffer[3] == '\x00') {
                return "UTF-32 Little Endian";
            }

            return "Unknown";
        }

        bool isNumber(const std::string& str) {
            std::istringstream iss(str);
            double num;
            iss >> std::noskipws >> num; // Try to convert to a number
            return iss.eof() && !iss.fail();
        }

        void get_print_format(std::vector<int>& columnLengths, int& seperatorLength)
        {
            int headerLength;
            int maxValueLength;
            int init = 7;
            int length;
            seperatorLength = 1;
            for(int i = 0; i < columns.size(); i++)
            {
                headerLength = columns[i].length();
                
                maxValueLength = _dataframe[columns[i]].getLongestLength();

                if (_dataframe[columns[i]].get_type() == TYPE_FLOAT)
                {
                    maxValueLength = maxValueLength + 5;
                }

                length = init < maxValueLength ? maxValueLength : init;
                length = length < headerLength ? headerLength : length;

                columnLengths.push_back(length);
                seperatorLength += length + 3;
            }

        }

        void print_header(std::ofstream& resultFile, std::vector<int> columnLengths)
        {
            
            std::stringstream format;

            format << "|";

            for (int i = 0; i < columns.size(); i++)
            {
                if (_dataframe[columns[i]].get_type() == TYPE_INTEGER)
                {
                    format << " " << std::left << std::setw(columnLengths[i]) << columns[i] << " |";
                }
                else if(_dataframe[columns[i]].get_type() == TYPE_FLOAT)
                {
                    format << " " << std::right << std::setw(columnLengths[i]) << columns[i] << " |";
                }
                else
                {
                    format << " " << std::left << std::setw(columnLengths[i]) << columns[i] << " |";
                }
            }

            resultFile << format.str() << std::endl;
        }

        void print_content(std::ofstream& resultFile, std::vector<int> columnLengths)
        {
            for (int i = 0; i < _size; i++)
            {
                std::stringstream format;

                format << "|";

                for (int j = 0; j < columns.size(); j++)
                {
                    if (_dataframe[columns[j]].get_type() == TYPE_INTEGER)
                    {
                        format << " " << std::left << std::setw(columnLengths[j]) << _dataframe[columns[j]].get<int>(i) << " |";
                    }
                    else if(_dataframe[columns[j]].get_type() == TYPE_FLOAT)
                    {
                        if (_dataframe[columns[j]].get<double>(i) != std::numeric_limits<double>::min())
                        {
                            format << " " << std::right << std::fixed << std::setprecision(4) << std::setw(columnLengths[j]) << _dataframe[columns[j]].get<double>(i) << " |";  
                        }
                        else
                        {
                            format << " " << std::right << std::setw(columnLengths[j]) << "NULL" << " |";  
                        }
                    }
                    else
                    {
                        format << " " << std::left << std::setw(columnLengths[j] + (_dataframe[columns[j]].get<std::string>(i).length() - utf8CharacterWidth(_dataframe[columns[j]].get<std::string>(i)))) << _dataframe[columns[j]].get<std::string>(i) << " |";
                    }
                }

                resultFile << format.str() << std::endl;
            }
            
        }

        void print_footer(std::ofstream& resultFile, std::vector<int> columnLengths)
        {
            std::stringstream format;
            std::string type_identifier;

            format << "|";

            for (int j = 0; j < columns.size(); j++)
            {
                type_identifier = _dataframe[columns[j]].get_type();
                type_identifier = type_identifier == "" ? " TYPE" : type_identifier;

                if (_dataframe[columns[j]].get_type() == TYPE_INTEGER || _dataframe[columns[j]].get_type() == TYPE_STRING)
                {
                    format << " " << std::left;
                }
                else if(_dataframe[columns[j]].get_type() == TYPE_FLOAT)
                {
                    format << " " << std::right;  
                }

                format << std::setw(columnLengths[j]) << type_identifier << " |";
            }

            resultFile << format.str() << std::endl;
        }

        int utf8CharacterWidth(const std::string& str) 
        {
            int count = 0;
            for (size_t i = 0; i < str.size(); ) {
                unsigned char ch = str[i];
                if (ch < 0x80) {
                    i += 1;
                } else if (ch < 0xE0) {
                    i += 2;
                } else if (ch < 0xF0) {
                    i += 3;
                } else if (ch < 0xF8) {
                    i += 4;
                } else {
                    i += 1; // Invalid character, skip
                }
                count++;
            }
            return count;
        }

        int get_precision(double val)
        {
            char doubleChar[std::to_string(val).size() + 1];

            strcpy(doubleChar, std::to_string(val).c_str());
            
            bool afterPtr = false;
            int afterCounter = 0;
            int afterCount = 0;

            for (int i = 0; doubleChar[i] != '\0'; ++i) {

                if (afterPtr)
                {
                    afterCounter++;
                    if (doubleChar[i] != '0')
                    {
                        afterCount = afterCounter;
                    }
                }

                if (doubleChar[i] == '.')
                {
                    afterPtr = true;
                }
            }
            return afterCount;
        }

        std::vector<std::string> splitString(std::string input, char delimiter) {

            std::vector<std::string> tokens;
            std::string token;
            char charArray[input.size() + 1];
            char symbol;
            strcpy(charArray, input.c_str());

            for (int i = 0; i < strlen(charArray); i++) {
                symbol = charArray[i];
                if (delimiter == symbol)
                {
                    tokens.push_back(token);
                    token = "";
                }
                else
                {
                    token += symbol;
                }

                if(strlen(charArray) == i + 1)
                {
                    tokens.push_back(token);
                }
            }
            
            return tokens;
        }

        int add_column(std::string column)
        {
            for(int i = 0; i < columns.size(); i++)
            {
                if (columns[i] == column)
                {
                    return ErrorHandler::error_column_exists(column);
                }
            }

            columns.push_back(column);
            _dataframe[column] = Column();

            return 0;
        }

        int check_if_empty_cell(std::string element)
        {
            if (element == "" || element == "#NULL!")
            {
                return 1;
            }
            return 0;
        }

    public:

        Dataframe ()
        {
            columns.push_back("index");
            _dataframe["index"] = Column();
            _dataframe["index"].set_type(TYPE_INTEGER);
        }

        int read_csv(std::string file_path,bool headers = false, bool types = false, char delimiter = ';')
        {
            std::string line;
            std::vector<std::string> dataframe_headers;
            std::vector<std::string> types_headers;
            std::vector<std::string> valid_types = {TYPE__FLOAT, TYPE__INTEGER, TYPE__NONE, TYPE__STRING};
            std::vector<std::string> elements;

            if (!std::filesystem::exists(file_path))
            {
                return ErrorHandler::error_file_does_not_exist(file_path);
            }
            
            std::string encoding =  detectEncoding(file_path);
            std::ifstream csv_file(file_path, std::ios::in);

            if (encoding.find("UTF-8") == 0) {
                // Skip the UTF-8 BOM (EF BB BF)
                csv_file.seekg(3);
            }
            else if (encoding.find("UTF-16") == 0) {
                // Skip the UTF-16 BOM (FE FF or FF FE)
                csv_file.seekg(2);
            }
            else if (encoding.find("UTF-32") == 0) {
                // Skip the UTF-32 BOM (00 00 FE FF or FF FE 00 00)
                csv_file.seekg(4);
            }
            
            if (headers)
            {
                std::getline(csv_file, line);
                
                dataframe_headers = splitString(line, delimiter);
                
                for (int z = 0; z < dataframe_headers.size(); z++)
                {
                    if(add_column(dataframe_headers[z]))
                    {
                        return 1;
                    }
                }
            }

            if (types)
            {
                std::getline(csv_file, line);

                types_headers = splitString(line, delimiter);

                for (int z = 0; z < types_headers.size(); z++)
                {
                    
                    if(!headers)
                    {
                        columns.push_back("Column" + std::to_string(z + 1));
                        _dataframe[columns[z + 1]] = Column();
                    }

                    bool valid_type = false;
                    
                    for (int x = 0; x < valid_types.size(); x++)
                    {
                        if (valid_types[x] == types_headers[z])
                        {
                            valid_type = true;
                        }
                    }

                    if (valid_type)
                    {
                        _dataframe[columns[z + 1]].set_type(types_headers[z]);
                    }
                    else
                    {
                        return ErrorHandler::error_invalid_type(types_headers[z]);
                    }
                }

                if (types_headers.size() != columns.size() - 1)
                {
                    return ErrorHandler::error_invalid_dataframe_dimensions(columns.size() - 1, types_headers.size(), "TYPE");
                }
            }

            while(std::getline(csv_file, line))
            {
                elements = splitString(line, delimiter);

                if (elements.size() != columns.size() - 1)
                {
                    return ErrorHandler::error_invalid_dataframe_dimensions(columns.size() - 1, elements.size(), "ROW", _size);
                }
                for(int i = 0; i < elements.size(); i++)
                {
                    if(!headers && !types && _size == 0)
                    {
                        columns.push_back("Column" + std::to_string(i + 1));

                        _dataframe[columns[i + 1]] = Column();

                        if (isNumber(elements[i]))
                        {
                            _dataframe[columns[i + 1]].set_type(TYPE__FLOAT);
                        }
                        else if (check_if_empty_cell(elements[i]))
                        {
                            _dataframe[columns[i + 1]].set_type(TYPE__NONE);
                        }
                        else
                        {
                            _dataframe[columns[i + 1]].set_type(TYPE__STRING);
                        }
                    }

                    else if (!types && headers && _size == 0)
                    {
                        if (isNumber(elements[i]))
                        {
                            _dataframe[columns[i + 1]].set_type(TYPE__FLOAT);
                        }
                        else if (check_if_empty_cell(elements[i]))
                        {
                            _dataframe[columns[i + 1]].set_type(TYPE__NONE);
                        }
                        else
                        {
                            _dataframe[columns[i + 1]].set_type(TYPE__STRING);
                        }
                    }

                    if (!types)
                    {
                        if (_dataframe[columns[i + 1]].get_type() == TYPE__FLOAT)
                        {
                            if (isNumber(elements[i]))
                            {
                                _dataframe[columns[i + 1]].push_back(stod(elements[i]));
                            }
                            else if (check_if_empty_cell(elements[i]))
                            {
                                _dataframe[columns[i + 1]].push_back(std::numeric_limits<double>::min());
                            }
                            else
                            {
                                for(int y = 0; y < _dataframe[columns[i + 1]].size<double>(); y++)
                                {
                                    std::ostringstream oss;
                                    oss << std::fixed << std::setprecision(get_precision(_dataframe[columns[i + 1]].get<double>(y))) << _dataframe[columns[i + 1]].get<double>(y);
                                    if (_dataframe[columns[i + 1]].get<double>(y) == std::numeric_limits<double>::min())
                                    {
                                        _dataframe[columns[i + 1]].push_back(std::string(""));
                                    }
                                    else
                                    {
                                        _dataframe[columns[i + 1]].push_back(oss.str());
                                    }
                                }
                                _dataframe[columns[i + 1]].clear<double>();
                                _dataframe[columns[i + 1]].set_type(TYPE__STRING);
                                _dataframe[columns[i + 1]].push_back(elements[i]);
                            }
                        }
                        else if (_dataframe[columns[i + 1]].get_type() == TYPE__STRING)
                        {
                            _dataframe[columns[i + 1]].push_back(elements[i]);
                        }
                        else if (_dataframe[columns[i + 1]].get_type() == TYPE__NONE)
                        {
                            if (check_if_empty_cell(elements[i]))
                            {
                                _dataframe[columns[i + 1]].push_back(elements[i]);
                            }
                            else
                            {
                                if(isNumber(elements[i]))
                                {
                                    for(int y = 0; y < _dataframe[columns[i + 1]].size<std::string>(); y++)
                                    {
                                        _dataframe[columns[i + 1]].push_back(std::numeric_limits<double>::min());
                                    }
                                    _dataframe[columns[i + 1]].clear<std::string>();
                                    _dataframe[columns[i + 1]].set_type(TYPE__FLOAT);
                                    _dataframe[columns[i + 1]].push_back(stod(elements[i]));
                                }
                                else
                                {
                                    _dataframe[columns[i + 1]].set_type(TYPE__STRING);
                                    _dataframe[columns[i + 1]].push_back(elements[i]);
                                }
                            }
                        }
                    }
                    else
                    {
                        if (_dataframe[columns[i + 1]].get_type() == TYPE__FLOAT)
                        {
                            if (check_if_empty_cell(elements[i]))
                            {
                                _dataframe[columns[i + 1]].push_back(std::numeric_limits<double>::min());
                                
                            }
                            else if (isNumber(elements[i]))
                            {
                                _dataframe[columns[i + 1]].push_back(stod(elements[i]));
                            }
                            else
                            {
                                return ErrorHandler::error_invalid_element_type(elements[i], TYPE__STRING);
                            }
                        }
                        else if (_dataframe[columns[i + 1]].get_type() == TYPE__STRING)
                        {
                            _dataframe[columns[i + 1]].push_back(elements[i]);
                        }
                    }
                }

                _dataframe["index"].push_back(_size);
                _size++;
            }

            return 0;
        }

        int get_size()
        {
            return _size;
        }

        template<typename T>
        std::vector<T> get_column(std::string column_name)
        {
            return _dataframe[column_name].get_vector<T>();
        }

        std::vector<std::string> get_column_names()
        {
            return columns;
        }

        void display(std::string resultPath = result_path)
        {
            std::ofstream resultFile;
            std::vector<int> columnLengths;
            int seperatorLength;
            std::string seperator;

            remove(resultPath.c_str());
            resultFile.open(resultPath, std::ios::out);

            get_print_format(columnLengths, seperatorLength);

            seperator = std::string(seperatorLength, '-');

            resultFile << seperator << std::endl;
            print_header(resultFile, columnLengths);
            resultFile << seperator << std::endl;
            print_content(resultFile, columnLengths);
            resultFile << seperator << std::endl;
            print_footer(resultFile, columnLengths);
            resultFile << seperator << std::endl;

            resultFile << "  Size: " << _size;
            
        }

        void test_method()
        {
            for (int i = 1; i < columns.size(); i++)
            {
                std::cout << columns[i] << " | " << _dataframe[columns[i]].get_unique_count() << " | " << _dataframe[columns[i]].get_type() << std::endl;
            }
        }
};