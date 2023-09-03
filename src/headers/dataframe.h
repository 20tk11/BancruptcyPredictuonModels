#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <map>
#include "column.h"
#include <type_traits>
#include <filesystem>
#include "errorHandler.h"
#include <fstream>
#include <cstring> 

/// @brief Dataframe object that used to store data as columns, each columns has different names and specific types
/// currently works best with integer, double and std::string types
class Dataframe
{

    private:

        /// @brief Stores Columns by index key
        std::map<std::string, Column> mdataframe =
        {
            {"index", Column()}
        };

        /// @brief Indicates the size of Dataframe columns
        int msize = 0;

        /// @brief Stores all of the keys of the columns that are stored inside an dataframe
        std::vector<std::string> mcolumns;

        /// @brief Detects encoding indicated int the starting bytes of file
        /// @param _file_path Path to file that is being read
        /// @return Type of encoding used in file
        std::string mdetectEncoding(const std::string& _file_path);

        /// @brief Splits string into vector by indicated delimited
        /// @param _input String to split
        /// @param _delimiter Indicator by which string is splitted
        /// @return Vector of elements splitted from input string
        std::vector<std::string> msplitString(std::string _input, char _delimiter);

    public:

        /// @brief Constructor that initializes dataframe with one column index that indicates row index in columns inside dataframe 
        Dataframe();

        /// @brief Used to get Column with array index like indicator
        /// @param _key Index/key of dictionary member
        /// @return Column of name _key
        Column& operator[](const std::string& _key);

        /// @brief Adds column to dataframe as key _column and vector of type T
        /// @tparam T Type of vector added to dataframe as Column
        /// @param _vec Vector added to Column as type T
        /// @param _column Column name that is added to vector
        template<class T>
        void addColumn(std::vector<T> _vec, std::string _column);


        int addColumn(std::string _column);

        /// @brief Removes Column from dataframe
        /// @param _column Columns name that will be removed
        void removeColumn(std::string _column);

        /// @brief Read an CSV file
        /// @param file_path Path to file
        /// @return Indicates if an error occured or not: 1 -> error, 0 -> no error
        int readCSV(std::string _file_path, bool _headers = false, char _delimiter = ';');
};

#include "../source/dataframe.cpp"

#endif // DATAFRAME_H