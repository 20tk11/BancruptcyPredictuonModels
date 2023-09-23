#include "../headers/variableResults.h"

template<class T>
VariableResults::VariableResults(std::string _name, 
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
                                T _Mode,
                                int _ModeFreq,
                                double _ModePerc,
                                T _Mode2,
                                int _ModeFreq2,
                                double _ModePerc2)
{
    name = _name;
    feature_type = _feature_type;
    data_type = _data_type;
    not_null_count = _not_null_count;
    missing_percent = _missing_percent;
    unique_count = _unique_count;
    min = _min;
    max = _max;
    Q25 = _Q25;
    Mean = _Mean;
    Median = _Median;
    Q75 = _Q75;
    stddev = _stddev;
    Mode = _Mode;
    ModeFreq = _ModeFreq;
    ModePerc = _ModePerc;
    Mode2 = _Mode2;
    ModeFreq2 = _ModeFreq2;
    ModePerc2 = _ModePerc2;

}

template<class T>
void VariableResults::print(std::ofstream& _result_file)
{
    std::stringstream format;
    
    format << "| " << std::setw(20) << std::left << this->name;
    format << " | " << std::setw(13) << std::left << MDataTypes[this->feature_type];
    format << " | " << std::setw(11) << std::left << MDataTypes[this->data_type];
    format << " | " << std::setw(10) << std::left << this->not_null_count;
    format << " | " << std::setw(8) << std::fixed << std::setprecision(4) << std::right << this->missing_percent;
    format << " | " << std::setw(10) << std::right << this->unique_count;
    format << " | " << std::setw(12) << std::right << std::get<T>(this->min);
    format << " | " << std::setw(12) << std::right << std::get<T>(this->Q25);
    format << " | " << std::setw(12) << std::right << std::get<T>(this->Mean);
    format << " | " << std::setw(12) << std::fixed << std::setprecision(4) << std::right << std::get<T>(this->Median);
    format << " | " << std::setw(12) << std::right << std::get<T>(this->Q75);
    format << " | " << std::setw(12) << std::right << std::get<T>(this->max);
    format << " | " << std::setw(12) << std::right << std::get<T>(this->stddev);
    format << " |";

    _result_file << format.str() << std::endl; 
}

void VariableResults::printHeader(std::ofstream& _result_file)
{
    std::stringstream format;
    
    format << "| " << std::setw(20) << std::left << "Variable Name";
    format << " | " << std::setw(13) << std::left << "Feature Type";
    format << " | " << std::setw(11) << std::left << "Data Type";
    format << " | " << std::setw(10) << std::left << "Count";
    format << " | " << std::setw(8) << std::right << "Miss %";
    format << " | " << std::setw(10) << std::right << "Cardinalty";
    format << " | " << std::setw(12) << std::right << "Min";
    format << " | " << std::setw(12) << std::right << "Q25";
    format << " | " << std::setw(12) << std::right << "Mean";
    format << " | " << std::setw(12) << std::right << "Median";
    format << " | " << std::setw(12) << std::right << "Q75";
    format << " | " << std::setw(12) << std::right << "Max";
    format << " | " << std::setw(12) << std::right << "Std Dev";
    format << " |";

    _result_file << format.str() << std::endl; 
}

void VariableResults::setDataType(MDataType _type)
{
    this->data_type = _type;
}

void VariableResults::setFeatureType(MDataType _type)
{
    this->feature_type = _type;
}