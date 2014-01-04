#ifndef FILE_HPP_INCLUDED
#define FILE_HPP_INCLUDED

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include "Sfgfx.hpp"
#include "Map.hpp"

#define DIR "data/"
#define SAVE_FILE "savefile"

class File
{
    public:
        File();
        ~File();
        bool save(vector<Map*> *maps);
        bool read(vector<Map*> *maps);

    /*
    public:
         struct DATA
        {
            char surname[NAME_MAX_LEN];
            char name[NAME_MAX_LEN];
            int age;                // 1-130
            int age2;               // sick age
            short int gender;           // 1: M, 0: K
            short int epileptic_focus;  // 0-9
            short int education;        // 0: low, 1: medium, 2: high, 3: vhigh
            short int domination;       // 0 - 2
            int p_hipokamp;       // 0 - 100
            int l_hipokamp;       // 0 - 100
            char test_date[WP1];
            char born_date[WP1];
            char update[20];
            short int wais[WP1];
            short int clinical_trials[ALL_CLINICAL_TRIALS];
        };
        string file_name;
        string csv_file_name;
        string pdf_file_name;
        olaData();
        ~olaData();
        bool readDb(vector <DATA*> *lp);
        bool saveDb(vector <DATA*> *lp);
        bool saveCsv(vector <DATA*> *lp,vector <string> string_epileptic_focus_tab,vector <string> string_education_tab,vector <string> string_domination_tab);
        bool savePdf(DATA* patient,vector <string> string_epileptic_focus_tab,vector <string> string_education_tab, vector <string> string_test_name_tab, vector <string> string_domination_tab);
        size_t srvSend(vector <DATA*> *lp);
        size_t srvGet(vector <DATA*> *lp);
        bool add_to_data(DATA *d, string in, size_t pos);
        string getCurrentDate(bool dots);
        string getCurrentTime();
        int getAge(char year[4]);
        void utf_to_iso2(char *src);
*/
};

#endif

