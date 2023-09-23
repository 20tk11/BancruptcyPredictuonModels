#ifndef VARIABLERESULTS_H
#define VARIABLERESULTS_H

#include <string>
#include "../../resources/constants.h"
#include <fstream>
#include "mtype.h"
#include <unordered_map>
#include <vector>
#include <tuple>
#include <variant>
struct VariableResults
{

    public:

        std::string name;
        MDataType feature_type;
        MDataType data_type;
        int not_null_count;
        double missing_percent;
        int unique_count;

        
        std::variant< double, std::string > min;
        std::variant< double, std::string > max;
        std::variant< double, std::string > Q25;
        std::variant< double, std::string > Mean;
        std::variant< double, std::string > Median;
        std::variant< double, std::string > Q75;
        std::variant< double, std::string > stddev;
        std::variant< double, std::string > Mode;
        int ModeFreq;
        double ModePerc;
        std::variant< double, std::string > Mode2;
        int ModeFreq2;
        double ModePerc2;

        /// @brief Seperator width between header, footer, and content 
        /// added up with the width of each column and multiplyed by n + 1 - (number of columns) with 2 and added up also
        static const int seperator_length = 20 + 13 + 11 + 10 + 8 + 10 + 12 + 12 + 12 + 12 + 12 + 12 + 12 + (3 * 12) + 4;

        template<class T>
        VariableResults(std::string _name,
                        MDataType _feature_type,
                        MDataType _data_type,
                        int _not_null_count,
                        double _missing_percent,
                        int _unique_count,
                        T _min,
                        T _max,
                        T _Q25,
                        T _Mean,
                        T _Median,
                        T _Q75,
                        T _stddev,
                        T Mode,
                        int ModeFreq,
                        double ModePerc,
                        T Mode2,
                        int ModeFreq2,
                        double ModePerc2);

        VariableResults() 
        {

        };

        template<class T>
        void print(std::ofstream& _result_file);

        static void printHeader(std::ofstream& _result_file);

        void setDataType(MDataType _type);

        void setFeatureType(MDataType _type);
};

#include "../source/variableResults.cpp"

#endif // VARIABLERESULTS_H