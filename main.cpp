#include "src/headers/dataframe.h"
#include <iomanip>
#include "resources/constants.h"

int main()
{
    // Column col;

    // col.add(std::string("Petras"));
    // col.add(std::string("^amfsdaas"));

    // std::cout << col.get<std::string>(0) << std::endl;
    // std::cout << col.get<std::string>(1) << std::endl;

    // std::vector<std::string> vec = col.getColumn<std::string>();

    // col.setType(MType::TYPE_INTEGER);
    // std::cout << col.getValueLength<std::string>() << std::endl;
    // std::cout << col.getUniqueCount<std::string>() << std::endl;
    // std::cout << col.getUniqueCount<std::string>() << std::endl;
    // col.add(std::string("^amfsdaas"));
    // std::cout << col.getUniqueCount<std::string>() << std::endl;
    // std::cout << col.getUniqueCount<std::string>() << std::endl;
    // std::vector<std::string> uniques = col.getUniques<std::string>();
    
    Dataframe c = Dataframe();

    // std::vector<double> _float;
    // _float.push_back(5.5676);
    // _float.push_back(5141.5676);
    // _float.push_back(514141.5676);

    // //c.addColumn(_float, "Column1");
    // c.addColumn(_float, "Column1");
    // //c["Column1"].add(5.6519);
    // std::cout << c["Column1"].get<double>(0) << std::endl;
    // std::cout << c["Column1"].getType() << std::endl;
    // c.removeColumn("Column1");

    // std::cout << c["Column1"].get<double>(0) << std::endl;

    c.readTSV(file_path, true);
    c.display();

    return 0;
}

//HX_1,MX_1,SX_1,SX_2 - HEADERS
//FLOAT,FLOAT,FLOAT,STRING - TYPES

