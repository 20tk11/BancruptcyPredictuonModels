#include "src/headers/Model.h"
#include <iomanip>

int main()
{
    Model model;
    Dataframe c;

    //c.read_csv(file_path, true, false, ';');
    model.read_data(file_path, true, false, ';');
    c = model.get_data();
    model.display();
    c.display();
    return 0;
}

//HX_1,MX_1,SX_1,SX_2 - HEADERS
//FLOAT,FLOAT,FLOAT,STRING - TYPES





