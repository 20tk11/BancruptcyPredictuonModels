#ifndef Column_H
#define Column_H

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

    /// @brief Type of Column
    MType mtype;

    /// @brief States if unique values should be searched again
    bool mrecalculate_uniques = false;

    /// @brief Gets max length inside of Column's vector of T
    /// @tparam T Type of Column vector
    /// @return Length of longest value of Column's vector of type T
    template<typename T>
    int mgetValueLength(int index);

public:

    ///@param t Value added to Column's vector
    ///@tparam T Type of value added to Column's vector
    ///@brief Method used to add values to Column vector of type T
    template <class T>
    void add(const T& t);

    /// @brief Get value of type T from vector in position index
    /// @tparam T Type of value
    /// @param index Position of value inside the vector
    /// @return Value of type T
    template <class T>
    T get(int index);

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
    void setType(MType type);

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

};

template<class T>
std::unordered_map<const Column*, std::vector<T>> Column::mitems;

#include "../source/column.cpp"

#endif // Column_H