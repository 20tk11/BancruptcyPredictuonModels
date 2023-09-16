#ifndef COLUMN_H
#define COLUMN_H

#include "mtype.h"
#include "../../resources/constants.h"
#include <vector>
#include <unordered_map>
#include <iostream>

///@brief Structure that is an vector and is able to store any type of values
///@param _items vectors where are stored any type values
///@param _type type of Column
struct Column{
private:

    /// @brief A unordered map of any type vectors
    /// @tparam T type of vector
    template<class T>
    static std::unordered_map<const Column*, std::vector<T>> mitems;

    /// @brief Indicates if Column of type T has already found all unique members inside Column
    /// @tparam T Type of Column
    template<class T>
    static std::unordered_map<const Column*, bool> mhas_unique;

    /// @brief Stored all unique member inside type T column as an vector of type T
    /// @tparam T Type of Column
    template<class T>
    static std::unordered_map<const Column*, std::vector<T>> munique;

    /// @brief Type of Column
    MType mtype;

    /// @brief Gets max length inside of Column's vector of T
    /// @tparam T Type of Column vector
    /// @return Length of longest value of Column's vector of type T
    template<typename T>
    int mgetValueLength(int _index);

    /// @brief Finds all unique members inside of type T column 
    /// @tparam T Type of Column
    template<class T>
    void mfindUnique();

    /// @brief Finds the length of longest std::string value inside column
    /// @return Length of longest value inside std::string column
    int getLongestString();

    /// @brief Finds from how much digits the largest number is made of
    /// @return Number of digits the largest integer is made of
    int getLongestInt();

    /// @brief Finds from how much digits the largest number is made of (applies precision .0000 5 symbols as digit count)
    /// @return Number of digits the largest double is made of
    int getLongestDouble();

public:

    /// @brief Used to set vector to T type Column
    /// @tparam T Type of Column vector
    /// @param _vec Vector of type T
    template<class T>
    void setColumn(std::vector<T> _vec);

    ///@param t Value added to Column's vector
    ///@tparam T Type of value added to Column's vector
    ///@brief Method used to add values to Column vector of type T
    template <class T>
    void add(const T& _t);

    /// @brief Get value of type T from vector in position index
    /// @tparam T Type of value
    /// @param index Position of value inside the vector
    /// @return Value of type T
    template <class T>
    T get(int _index);

    /// @brief Get vector from Column that is of type T
    /// @tparam T Type of vector
    /// @return Vector of type T
    template <class T>
    std::vector<T> getColumn();

    /// @brief Get size of vector of type T
    /// @tparam T Type of vector
    /// @return Size of vector of type T
    template <class T>
    int size();

    /// @brief Set type for Column
    /// @param type States type of Column
    void setType(MType _type);

    /// @brief Get the type that is set to Column
    /// @return MType that states the type of Column
    MType getType();

    /// @brief Clears the vector of type T inside mitems
    /// @tparam T Type of vector to clear
    template <class T>
    void clear();

    /// @brief Gets length of value that is of types: double, int, std::string
    /// @tparam T States the type of vector which value max length will be found
    /// @return Length of number
    template<class T>
    int getValueLength();

    /// @brief Get number of unique member inside T type of Column vector
    /// @tparam T type of Column
    /// @return Number of unique members inside Column of type T
    template<class T>
    int getUniqueCount();

    /// @brief Get all unique members inside Column of type T
    /// @tparam T Type of column
    /// @return Vector of type T with all unique values inside of type T column
    template<class T>
    std::vector<T> getUniques();

    /// @brief Finds the number of symbols the logest values is made of inside column
    /// @return Number of symbols the longest value is made of inside column
    int getLongestLength();
};

template<class T>
std::unordered_map<const Column*, std::vector<T>> Column::mitems;

template<class T>
std::unordered_map<const Column*, bool> Column::mhas_unique;

template<class T>
std::unordered_map<const Column*, std::vector<T>> Column::munique;

#include "../source/column.cpp"

#endif // COLUMN_H