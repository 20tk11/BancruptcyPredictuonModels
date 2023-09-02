#include "src/headers/column.h"
#include <iomanip>

int main()
{
    Column col;

    col.add(std::string("Petras"));
    col.add(std::string("^amfsdaas"));

    std::cout << col.get<std::string>(0) << std::endl;
    std::cout << col.get<std::string>(1) << std::endl;

    std::vector<std::string> vec = col.getColumn<std::string>();

    col.setType(MType::TYPE_INTEGER);
    std::cout << col.getValueLength<std::string>() << std::endl;
    std::cout << col.getUniqueCount<std::string>() << std::endl;
    std::cout << col.getUniqueCount<std::string>() << std::endl;
    col.add(std::string("^amfsdaas"));
    std::cout << col.getUniqueCount<std::string>() << std::endl;
    std::cout << col.getUniqueCount<std::string>() << std::endl;
    std::vector<std::string> uniques = col.getUniques<std::string>();
    return 0;
}

//HX_1,MX_1,SX_1,SX_2 - HEADERS
//FLOAT,FLOAT,FLOAT,STRING - TYPES

