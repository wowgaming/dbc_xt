#pragma once
#pragma comment(lib, "libmysql")
#pragma comment(lib, "mysqlclient")

#include "my_global.h"
#include "mysql.h"
#include "stdafx.h"
#include ".\IniFile.h"

class database
{
public:
    database(void);
    ~database(void);

    bool connect();
    void disconnect();
    bool query(const char* q,bool storeresult = false);
	MYSQL *getConnection() { return connection; };
    MYSQL_RES *getResult();

private:
    MYSQL *connection, mysql;
    MYSQL_RES *result;
    MYSQL_ROW row;
    int query_state;

    std::string confFile;
    std::string password,user,host,dbname;
    uint32 port;
};

