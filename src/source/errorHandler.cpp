#include "../headers/errorHandler.h"

int ErrorHandler::error_file_does_not_exist(std::string _file_path)
{
    std::cout << ERROR << " Can't read file - file (" << _file_path << ") does not exist in provided location";
    return 1;
}

int ErrorHandler::error_column_exists(std::string _column)
{
    std::cout << ERROR << " Can't add column - column (" << _column << ") already exists in dataframe";
    return 1;
}

int ErrorHandler::error_invalid_dataframe_dimensions(int _expected, int _size, std::string _invalid_element_err_type, int _row_index)
{
    std::cout << ERROR << " Dataframe dimensions are invalid - expected number of columns (" << _expected << "), given row has (" << _size << ") columns" << std::endl;
    
    if (_invalid_element_err_type == "ROW")
    {
        std::cout << EMPTY << " Dimensions in row (" << _row_index << ") are invalid";
    }

    else if (_invalid_element_err_type == "TYPE")
    {
        std::cout << EMPTY << " Dimensions of types are invalid";
    }
    return 1;
}

int ErrorHandler::error_invalid_element_type(std::string _element, std::string _type)
{
    std::cout << ERROR << " Can't set element - element (" << _element << ") is not valid, because element type(" << _type <<") is not valid";
    return 1;
}

int ErrorHandler::error_invalid_quantile(double _quantile)
{
    std::cout << ERROR << " Quantile exceed qiantile range of [0, 1], quantile range provided: " << _quantile;
    return 1;
}