#include <iostream>
#include <string>

const std::string ERROR = "[ERROR]";
const std::string ERROR_EMPTY = "       ";
const std::string WARNING = "[WARNING]";
const std::string WARNING_EMPTY = "         ";
class ErrorHandler
{
    public:

        static int error_file_does_not_exist(std::string file_path)
        {
            std::cout << ERROR << " Can't read file - file (" << file_path << ") does not exist in provided location";
            return 1;
        }

        static int error_column_exists(std::string column)
        {
            std::cout << ERROR << " Can't add column - column (" << column << ") already exists in dataframe";
            return 1;
        }

        static int error_invalid_type(std::string type)
        {
            std::cout << ERROR << " Can't set column - type (" << type << ") is not valid";
            return 1;
        }

        static int error_invalid_element_type(std::string element, std::string type)
        {
            std::cout << ERROR << " Can't set element - element (" << element << ") is not valid, because element type(" << type <<") is not valid";
            return 1;
        }

        static int error_invalid_dataframe_dimensions(int expected, int size, std::string invalid_element_err_type, int row_index = 0)
        {
            std::cout << ERROR << " Dataframe dimensions are invalid - expected number of columns (" << expected << "), given row has (" << size << ") columns" << std::endl;
            
            if (invalid_element_err_type == "ROW")
            {
                std::cout << ERROR_EMPTY << " Dimensions in row (" << row_index << ") are invalid";
            }

            else if (invalid_element_err_type == "TYPE")
            {
                std::cout << ERROR_EMPTY << " Dimensions of types are invalid";
            }
            return 1;
        }

        static int error_invalid_file()
        {
            std::cout << ERROR << " The provided file must be in CSV, JSON, or Excel format.";
            return 1;
        }
};