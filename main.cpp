#include "src/headers/dataframe.h"
#include <iomanip>
#include "resources/constants.h"

int main()
{
    Dataframe c = Dataframe();

    c.readTSV(file_path, true);
    c.display();
    c.analyseVariables();

    c.setVariableFeature("Id", MDataType::CATEGORICAL);
    c.setVariableDataType("Id", MDataType::ORDINAL);
    c.setVariableDataType("YearBuilt", MDataType::INTERVAL);

    c.displayVariableResults();

    return 0;
}

//HX_1,MX_1,SX_1,SX_2 - HEADERS
//FLOAT,FLOAT,FLOAT,STRING - TYPES

