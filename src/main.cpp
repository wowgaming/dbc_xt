// csvdbc.cpp : file di progetto principale.

#include "main.h"

int main(int argc, char**argv) {
	string filename,opt;

    if (argc<=1) {
		opt = "import";
		filename = "Spell";
		// return 0;
	} else {
		opt = argv[1];
		filename = argv[2];
	}

    //This repeats the program. You close console window manually by clicking "x". 
    system ("CLS"); //this will clear the screen of any text from prior run
    cin.clear(); //this will clear any values remain in cin from prior run
    string name;

    size_t found=filename.find(".");
    //if (found!=string::npos) {
        name = filename.substr(0,found);
    //}

    if (opt == "import") {
        tosql *sql = new tosql(name);
        sql->convert();
    } else {
        todbc *dbc = new todbc(name);
        dbc->convert();
    }

    system ("PAUSE");
    return 1;
}
