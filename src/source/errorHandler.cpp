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