// csvdbc.cpp : file di progetto principale.

#include "main.h"

int main(int argc, char**argv) {
	string filename,opt;

    if (argc<=1) {
		cout << "Enter command: import/export\n";
		cin >> opt;
		cout << "Enter filename\n";
		cin >> filename;
		// return 0;
	} else {
		opt = argv[1];
		filename = argv[2];
	}

	if (opt == "export" && (filename == "" || filename == "*")){
		MYSQL_ROW tables;
		MYSQL_RES *result;
		my_ulonglong nb_tables;

		database *db = new database();
		db->connect();
		result=mysql_list_tables(db->getConnection(),NULL);
		if (result==NULL)
			printf("Error: no tables\n");
		else {
			nb_tables=result->row_count;
		}

		tables=mysql_fetch_row(result);
		delete db;
		for (unsigned int i=0;i<nb_tables;i++) {
			todbc *dbc = new todbc(tables[i]);
			dbc->convert();
		}
	} else {
		vector<string> files = common::split(filename,",","",false);

		for (vector<string>::iterator it = files.begin(); it != files.end(); it++) {
			string name;

			size_t found=it->find(".");
			//if (found!=string::npos) {
				name = it->substr(0,found);
			//}

			if (opt == "import") {
				tosql *sql = new tosql(name);
				sql->convert();
			} else {
				todbc *dbc = new todbc(name);
				dbc->convert();
			}
		}
	}

	cout << "Process completed .. press any key to quit";
	_getch();
	exit(1);
}
