#include "todbc.h"

todbc::todbc(string fileName)
{
    fName = fileName;
    string csvPath = fileName+".dbc.csv";
    csvOutput.open(csvPath);
}


todbc::~todbc(void)
{
}

bool todbc::convert() {
    string q,rs,fields;
    MYSQL_ROW row;
	vector<string> ftype;

	fields = CIniFile::GetValue(fName,"files",INI_FIELDS_FORMAT);
	if (fields.empty()) {
		cout << "missing fields file";
		return false;
	}


	ftype = common::split(fields,",","",false);
    csvOutput << fields+",\n";

    database *db = new database();
    db->connect();

    q = "SELECT * FROM "+fName;
    if (!db->query(q.c_str(),true) ) 
        return false;

    MYSQL_RES *res = db->getResult();
    if (!res) // can't happen
        return false;
	my_ulonglong rCount = res->row_count;
	unsigned int fCount = mysql_num_fields(res);
	int r=0,perc=0;
	std::cout << "Processing: " << fName << "\n";
	std::cout << "Loading:\n";
    while ((row = mysql_fetch_row(res)) != NULL) {
		r++;
		perc=(r*100)/rCount;
		if (perc % 10 == 0) // skip at each 5 units, to speed up the cycle 
			std::cout << perc << "%\r" << std::flush;

        for (uint i=0; i < fCount; i++)
        {
			
			rs = "";
			if (row[i])
				rs = row[i];

			if (ftype[i] == "str")
				rs = "\""+rs+"\"";
			else if (ftype[i] == "float") {
				size_t found = rs.find(".");
				if (found == string::npos) 
					rs += ".0";
			}

			rs += ",";

            csvOutput << rs;
        }
        csvOutput << "\n";
    }

	std::cout << "\n";
    
    //close
    db->disconnect();
    delete db;
	csvOutput.close();
    return true;
}
