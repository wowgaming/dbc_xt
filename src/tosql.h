#pragma once
#include "stdafx.h"
#include "common.h"
#include "database.h"

class tosql
{
public:
		tosql(string fileName);
            ~tosql();

        bool convert();

private:
	    void saveFormat();
        std::string createTable();
        std::string createData();
		std::string createRow(string r);
		bool createSql(string q);
		
		void readCSV(std::string buffer,
		std::vector<std::string>& stringArray,
		std::vector<double>& doubleArray,
		std::vector<long>& longArray,
		std::vector<long>& order);

		void readCSV(std::string buffer,std::vector<std::string>& stringArray);

        std::string fName;
        std::ifstream csvInput;
        std::ofstream sqlOutput;
		int sqlCMethod;
		int sqlCInsRange;
		database *db;
		vector<string> fields;
};

