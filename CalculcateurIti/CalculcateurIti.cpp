#include <stdlib.h>
#include <iostream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
using namespace std;

const string server = "10.1.139.236";
const string username = "c2";
const string password = "mdp";

int main()
{
    sql::Driver* driver;
    sql::Connection* con;
    sql::PreparedStatement* pstmt;
    sql::ResultSet* result;

    try
    {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }

    con->setSchema("basec2");

    //select  
    pstmt = con->prepareStatement("SELECT * FROM Arret;");
    result = pstmt->executeQuery();

    while (result->next())
        printf("Reading from table=(%d, %s )\n", result->getInt(1), result->getString(2).c_str());

    delete result;
    delete pstmt;
    delete con;
    system("pause");
    return 0;
}