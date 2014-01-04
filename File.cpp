#include "File.hpp"
using namespace std;

File::File()
{
    //file_name = DEFAULT_FILE_NAME;
}

File::~File()
{

}

bool File::read(vector<Map*> *maps)
{
/*
    ifstream file;
    file.open(file_name.c_str(),ios::in|ios::binary);

    if(file.is_open())
    {
        DATA *temp;
        while(!file.eof())
        {
            temp = (DATA*) new DATA;
            if(file.read((char*) temp, sizeof(DATA))!=0)
                lp->push_back(temp);
*/
            /*
            file.read((char*) temp->surname,sizeof(char[30]));
            file.read((char*) temp->name,sizeof(char[30]));
            file.read((char*) &temp->age,sizeof(int));
            file.read((char*) &temp->gender,sizeof(short int));
            file.read((char*) &temp->epileptic_focus,sizeof(short int));
            file.read((char*) &temp->education,sizeof(short int));
            file.read((char*) temp->test_date,sizeof(char[11]));
            file.read((char*) temp->born_date,sizeof(char[11]));
            lp->push_back(temp);
            temp = NULL;
            */
 /*
        }
        file.close();
        return true;
    }
    else return false;
*/
    return false;
}

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, Object &vm, const unsigned int version)
{
    ar & vm.move_speed;
}

} // namespace serialization
} // namespace boost

bool File::save(vector<Map*> *maps)
{
    ofstream file;
    string path = DIR;
    path += SAVE_FILE;

    file.open(path.c_str(),ios::out|ios::binary);
    if(file.is_open())
    {

        //stringstream sss;
        //boost::archive::binary_oarchive oa(file);
        boost::archive::text_oarchive oa(file);
        oa << *(*(*maps)[0]->objects)[0];

        //for(unsigned int ii=0;ii<(unsigned int)maps->size();ii++)
        //    file.write((char*)(*maps)[ii], sizeof(Map));
            //oa << *(*maps)[ii];

        //reinterpret_cast<char *>(myobject)
        file.close();
        return true;
    }
    else return false;
}

