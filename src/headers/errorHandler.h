#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <string>
#include <iostream>

const std::string ERROR = "[ERROR]";

class ErrorHandler
{
    public:
        static int error_file_does_not_exist(std::string file_path);
        static int error_column_exists(std::string column);
};

#include "../source/errorHandler.cpp"

#endif // COLUMN_H