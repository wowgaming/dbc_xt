#include "database.h"


database::database(void)
{
    host = CIniFile::GetValue("host","mysql",INI_CONF_FILE);
    password = CIniFile::GetValue("password","mysql",INI_CONF_FILE);
    user = CIniFile::GetValue("user","mysql",INI_CONF_FILE);
    dbname = CIniFile::GetValue("database","mysql",INI_CONF_FILE);
    port = atoi(CIniFile::GetValue("port","mysql",INI_CONF_FILE).c_str());
}

database::~database(void)
{
}

bool database::connect() {
    mysql_init(&mysql);
    //connection = mysql_real_connect(&mysql,"host","user","password","database",0,0,0);
    connection = mysql_real_connect(&mysql,host.c_str(),user.c_str(),password.c_str(),dbname.c_str(),port,0,0);
    if (!connection) 
        printf("ERROR: %s \n",mysql_error(&mysql));

    return connection != NULL;
}

void database::disconnect() {
   // mysql_free_result(result);
   // mysql_close(connection);
    return;
}

bool database::query(const char* q,bool storeresult) {
    query_state = mysql_query(connection, q);
    if (query_state !=0) 
        printf("ERROR: %s \n",mysql_error(connection));
    else if (storeresult)
        result = mysql_store_result(connection);

    return query_state == 0;
}

MYSQL_RES *database::getResult() {
    return result;
}