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

        /// @brief Gets print format: column size, and seperator count between content, header and footer
        /// @param _column_Lengths Width for each column
        /// @param _seperator_length Number of symbols needed to seperate header, content and footer seactions
        void mgetPrintFormat(std::vector<int>& _column_Lengths, int& _seperator_length);

        /// @brief Prints Header section
        /// @param _result_file File where results are printed
        /// @param _column_lengths Width for each column
        void mprintHeader(std::ofstream& _result_file, std::vector<int> _column_lengths);

        /// @brief Prints Footer section
        /// @param _result_file File where results are printed
        /// @param _column_lengths Width for each column
        void mprintFooter(std::ofstream& _result_file, std::vector<int> _column_lengths);

        /// @brief Checks if value is number
        /// @param _str Textual value
        /// @return True if it is number and false if not
        bool misNumber(const std::string& _str);

        /// @brief Checks if value is null
        /// @param _element Textual value
        /// @return True if the value is null and false if not
        bool mcheckNull(std::string _element);

        /// @brief Prints Content section
        /// @param _result_file File where results are printed
        /// @param _column_lengths Width for each column
        void mprintContent(std::ofstream& _result_file, std::vector<int> _column_lengths);
    
        /// @brief Finds size of value in utf8 character set
        /// @param _str Textual value
        /// @return Size of textual value in UTF8 char set
        int mutf8CharacterWidth(const std::string& _str);

        /// @brief Finds precision of double value
        /// @param _val Numeric value
        /// @return Precision of numeric value
        int mgetPrecision(double _val);

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

        /// @brief Adds column to dataframe
        /// @param _column Column name
        /// @return If column was added succesfully return 0 and 1 otherwise
        int addColumn(std::string _column);

        /// @brief Gets vector of Column of type T where the values of type T are stored
        /// @tparam T Type of column
        /// @param _column_name Column name
        /// @return Vector of type T with Column values
        template<class T>
        std::vector<T> getColumn(std::string _column_name);

        /// @brief Return vector of column names inside dataframe
        /// @return Vector of type std::string with all column names inside dataframe
        std::vector<std::string> getColumnNames();

        /// @brief Return the number of columns inside dataframe, with index column being excluded
        /// @return Number of columns inside dataframe
        int getColumnCount();
        
        /// @brief Removes Column from dataframe
        /// @param _column Columns name that will be removed
        void removeColumn(std::string _column);
        
        /// @brief Read an CSV file
        /// @param file_path Path to file
        /// @param _headers Indicates if file has specified columns
        /// @param _types Indicates if file has specified column types
        /// @param _delimiter Indicates what seperator used to seperate columns in rows
        /// @return Indicates if an error occured or not: 1 -> error, 0 -> no error
        int readCSV(std::string _file_path, bool _headers = false, bool _types = false, char _delimiter = ';');

        /// @brief Read an TSV file, uses readCSV method to read an .tsv file by using delimited of tab -> '\t'
        /// @param _file_path Path to file
        /// @param _headers Indicates if file has specified columns
        /// @param _types Indicates if file has specified column types
        /// @return Indicates if an error occured or not: 1 -> error, 0 -> no error
        int readTSV(std::string _file_path, bool _headers = false, bool _types = false);

        /// @brief Finds if given type name exists
        /// @param _value Type name
        /// @return Returns enum of type
        MType findType(std::string _value);

        /// @brief Prints dataframe data to specified file or into default location path
        /// @param _result_path file path where to write results
        void display(std::string _result_path = result_path);

        /// @brief Returns the size of dataframe (row count)
        /// @return Number of rows inside dataframe
        int size();


};

#include "../source/dataframe.cpp"

#endif // DATAFRAME_H