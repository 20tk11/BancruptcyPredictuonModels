#include "Dataframe.h"

class Model
{
    private:

        Dataframe _dataframe;

        std::string get_extension(std::string file_path)
        {
            char charArray[file_path.size() + 1];
            strcpy(charArray, file_path.c_str());
            std::string extension = "";

            for (int i = strlen(charArray) - 1; i >= 0; i--)
            {
                extension = charArray[i] + extension;

                if (charArray[i] == '.')
                {
                    return extension;
                }
            }

            return extension;
        }

    public:

        void set_data(Dataframe dataframe)
        {
            _dataframe = dataframe;
        }    

        void read_data(std::string file_path, bool headers = false, bool types = false, char delimiter = ';')
        {
            std::string extension =  get_extension(file_path);

            if (extension == ".csv")
            {
                _dataframe.read_csv(file_path, headers, types, delimiter); 
            }
            else
            {
                ErrorHandler::error_invalid_file();
                return;
            }
            
        }

        // when returning and displaying dataframe get error: Segmentation failt (code dumped)
        Dataframe get_data()
        {
            return _dataframe;
        }

        void display()
        {
            _dataframe.display();
        }

        void prepare_data()
        {
            //
        }
};