#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <string>
#include <iostream>

const std::string ERROR = "[ERROR]";
const std::string EMPTY = "       ";

class ErrorHandler
{
    public:
    
        /// @brief Error display if file does not exist
        /// @param file_path File path of file that does not exist
        /// @return Indication that it is an error -> 1
        static int error_file_does_not_exist(std::string _file_path);

        /// @brief Error display if column exists in dataframe
        /// @param column Column name which exists
        /// @return Indication that it is an error -> 1
        static int error_column_exists(std::string _column);

        static int error_invalid_dataframe_dimensions(int _expected, int _size, std::string _invalid_element_err_type, int _row_index = 0);

        static int error_invalid_element_type(std::string _element, std::string _type);

        static int error_invalid_quantile(double _quantile);
};

#include "../source/errorHandler.cpp"

#endif // ERRORHANDLER_H