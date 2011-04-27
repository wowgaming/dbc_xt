#include "tosql.h"

using namespace std;

tosql::tosql(string fileName)
{
    fName = fileName;
	sqlCMethod = atoi(CIniFile::GetValue("sql.create.method","options",INI_CONF_FILE).c_str());
	sqlCInsRange = atoi(CIniFile::GetValue("sql.create.insertrange","options",INI_CONF_FILE).c_str());
    string csvPath = fileName+".dbc.csv";
    string sqlPath = fileName+".sql";
    csvInput.open(csvPath);
	if (sqlCMethod>0)
		sqlOutput.open(sqlPath,ios::out);
	db = new database();
}

bool tosql::convert() {
    string q;

    if (!csvInput)
        return false;

    saveFormat();

    db->connect();

    // drop the table
    std::string drop = "DROP TABLE  IF EXISTS `"+fName+"`; \n";
    createSql(drop);

    // create the table
    createTable();
    // fill with data
    createData();

    //close
    db->disconnect();
    delete db;
	if (sqlOutput)
		sqlOutput.close();
	csvInput.close();
    return 0;
}

string tosql::createTable() {
    string createStr,colType,row;
    int col = 0;
    csvInput.seekg (0, ios::beg); // go to the first line
    getline(csvInput, row);

    fields = common::split(row,",","",false);

    createStr += "CREATE TABLE `"+fName+"` (\n";

    string opt = ""; // standard options
   
    uint32 i = 0;
    for( vector<string>::iterator it = fields.begin(); it != fields.end(); ++it ){
        createStr += "  col"+common::convertInt(i);
        i++;

        if ( it->compare("long") == 0) {
            createStr += " int(11) "+opt+" NOT NULL DEFAULT '0' COMMENT 'type long'";
        } else if ( it->compare("str") == 0) {
            createStr += " longtext "+opt+" NOT NULL COMMENT 'type string'";
        } else if ( it->compare("flags") == 0) {
            createStr += " longtext "+opt+" NOT NULL COMMENT 'type flags'";
        } else if ( it->compare("None") == 0) {
            createStr += " longtext "+opt+" COMMENT 'type None'";
        } else if ( it->compare("float") == 0) {
            createStr += " double "+opt+" NOT NULL DEFAULT '0' COMMENT 'type float'";
        } else 
            return "";

        if (i < fields.size())
            createStr += ",";

        createStr += "\n";

    }

    createStr += ") ENGINE=MyISAM DEFAULT CHARSET=utf8;\n";

	createSql(createStr);

    return createStr;
}


string tosql::createData() {
    std::string createStr,row;

    csvInput.seekg (0,ios::beg);
    getline(csvInput,row); //go to the second line - hack 

    int rCount = 0,insCount = 0;
	createStr = "INSERT INTO `"+fName+"` VALUES \n";

    while(getline(csvInput,row)) // To get you all the lines.
    {
		if (row.size() > 0 ) {
			if (sqlCInsRange > 0 && sqlCInsRange == insCount) {
				createStr += ";\n";
				createSql(createStr);
				createStr.clear();
				createStr += "INSERT INTO `"+fName+"` VALUES \n";
				insCount = 0;
			} else {
				insCount++;
				if (rCount > 0)
					createStr += ",\n";
			}

			createStr += "(";
			createStr += createRow(row); // remove latest comma
			createStr += ")";
		}

        rCount++;
		// fast test
		//if (rCount>100)
		//	break;
    }

	createStr += ";\n";
	createSql(createStr);
	createStr.clear();

	return  createStr;
}

std::string tosql::createRow(string r) {
	
	std::string row;
    uint32 i = 0;

	std::string temp;
	unsigned long start=0;
	unsigned long end=0;
	bool isString = false;
	while (end<=r.size()-1) {
		
		//skip comma check in string type
		if(!isString && r[start]=='\"')
			isString = true;

		if (isString) {
			end++;
			if ( r.substr(end,2) != "\",")
				continue;
			else {
				end++;
				isString = false;
			}
		}

		//check fields ( in case of string type, it's true)
		if (r[end]!=',') {
			end++;
			continue;
		} 
		
		temp.assign(r,start,end-start);

		if (i>0)
			row += ",";

		if (fields[i] == "None")
			row += "NULL"; //"\"\"";
		else if (fields[i] == "flags")
			row += "\""+temp+"\"";
		else if (fields[i] == "str") {
			string fixed;
			fixed = temp.substr(1,temp.size()-2); // remove ""
			if (fixed != "") {
				char *escaped = new char[(strlen(fixed.c_str()) * 2) + 1];
				mysql_real_escape_string(db->getConnection(),escaped,fixed.c_str(),strlen(fixed.c_str()));
				//common::find_and_replace(fixed,"\"","\\\"");
				fixed = escaped;
				row += "\""+fixed+"\"";
			} else
				row += temp;
		}
		else
			row += temp;

		i++;
		
		end++;
		start=end;
	}

	if (i!=fields.size()) {
		printf("Out of range warning. col count: %d, values processed: %d",fields.size(),i);
		//return "";
	}

	return row;
}

void tosql::saveFormat() {
    std::string row;
    csvInput.seekg (0, ios::beg);
    getline(csvInput, row); // store 


    // Create a new file 
	if (!common::fexists(INI_FIELDS_FORMAT))
		CIniFile::Create(INI_FIELDS_FORMAT);
    // Create a Section called MySection, and a key/value of MyKey=MyValue
    CIniFile::SetValue(fName,row.substr(0,row.size()-1).c_str(),"files",INI_FIELDS_FORMAT);
    // Get the value of MyKey in section MySection and store it in s
    CIniFile::Sort(INI_FIELDS_FORMAT,false);
    return;
}

bool tosql::createSql(string q) {
	if (sqlCMethod == DATABASE && !db->query(q.c_str()))
		return false;
    
	if (sqlCMethod == SQL)
		sqlOutput << q.c_str();
	return true;
}


/******************************************************************************/
/* void readCSV()
 * 
 * Function to read a line from a comma-separated value text file and interpret 
 * the entries.  Assumes that there are three potential data types - string, 
 * long/integer, and double/float.  The entries are stored in separate vectors
 * that are passed to the function by reference.
 * 
 * Arguments:
 * 
 * 		string buffer		- buffer row from csv file
 * 
 * vector<string>& stringArray	-vector of strings in which to store the strings
 * 							that are found in the line
 * 
 * vector<double>& doubleArray	-vector of doubles in which to store the doubles
 * 							or floats that are found in the line
 * 
 * vector<long>& longArray	-vector of longs in which to store the longs or ints
 * 							that are found in the line
 * 
 * vector<long>& order		-vector of longs in which to store the ordering of
 * 							elements in the original line.  0=long, 1=double, 
 * 							2=string.
 * 
 * The ordering of the original elements can be reconstructed and outputted 
 * using:
 * 
 * 	vector<string>::iterator striter=stringArray.begin();
	vector<double>::iterator dbliter=doubleArray.begin();
	vector<long>::iterator longiter=longArray.begin();
	for (long i=0; i<order.size(); ++i) {
		if (order.at(i)==0) {
			cout << *longiter << endl;
			longiter++;
		} else if (order.at(i)==1) {
			cout << *dbliter << endl;
			dbliter++;
		} else if (order.at(i)==2) {
			cout << *striter << endl;
			striter++;
		}
	}
 * 
 * The function stores entries in the line from the CSV file based their data
 * type, which is decided with a few simple rules:
 * 
 * If an entry contains only numerical characters, it is considered to be an 
 * integer (stored as long integers).  Floating point numbers (stored as 
 * doubles) must consist entirely of numerical characters, except for a single 
 * period located anywhere within the entry.  Any entry containing more than one
 * period, or any other non-numerical characters, is stored as a string (sorry, 
 * no support for hex numbers).
 * 
 * Dependencies:  requires the string, vector, and fstream standard C++ headers.
 */
void tosql::readCSV(std::string buffer,
		std::vector<std::string>& stringArray,
		std::vector<double>& doubleArray,
		std::vector<long>& longArray,
		std::vector<long>& order) {
		
		std::vector<std::string> strArr;
		readCSV(buffer,strArr);
	
		for( vector<string>::iterator it = strArr.begin(); it != strArr.end(); ++it ){
			//	run through each character of the token to determine if it is a 
			//	string, floating-point, or integer, based on number of periods and
			//	presence of non-numeric characters

			int containsPeriod=0;
			int containsAlpha=0;
			std::string temp = it->c_str();
		
			for (long i=0; i<(long)it->size(); ++i) {
				if (temp[i]=='.') {
					//	token contains a '.' character - either a string or a floating point
					containsPeriod++;
				} else if ( temp[i]<48 || 57<temp[i] ) {
					//	token contains non-numeric characters - string (sorry no hex)
					containsAlpha++;
				}
			}
		
		
			//	based on alphabetical/numerical content and the presence of period/
			//	decimals, determine whether the entry is an integer, floating-point
			//	number, or a string
			if (containsAlpha==0 && containsPeriod==0) {
				longArray.push_back(atoi(temp.c_str()));
				order.push_back(0);
			
			} else if (containsPeriod==1 && containsAlpha==0) {
				doubleArray.push_back(atof(temp.c_str()));
				order.push_back(1);
			} else {
				stringArray.push_back(temp);
				order.push_back(2);
			}

		}
}

void tosql::readCSV(std::string buffer,std::vector<std::string>& stringArray) {
	
	std::string temp;
	unsigned long start=0;
	unsigned long end=0;
	while (end<=buffer.size()) {
		
		//	increment end counter
		end++;
		
		if (end<buffer.size() && buffer[end]!=',') {
			continue;
		}
		
		temp.assign(buffer,start,end-start);
		stringArray.push_back(temp);

		
		end++;
		start=end;
	}
}
