#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
using namespace std;

const string server = "10.1.139.236";
const string username = "c2";
const string password = "mdp";

sql::Driver* driver;
sql::Connection* con;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

vector<vector<int>> M;

void initMatrice() {
	M.resize(126);
	for (int i = 0; i < 126; i++) {
		M[i].resize(126);
		for (int j = 0; j < 126; j++) {
			M[i][j] = 0;
		}
	}
}

void initBD(){
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
}

int main()
{
    initMatrice();

    initBD();

    pstmt = con->prepareStatement("SELECT nArretA, nArretB, intervalleTemps FROM TempsTrajet;");
    result = pstmt->executeQuery();

    while (result->next()) {
        M[result->getInt(1) - 1][result->getInt(2) - 1] = result->getInt(3);
        M[result->getInt(2) - 1][result->getInt(1) - 1] = result->getInt(3);
    }

    ofstream fichier("M.txt");
    if (fichier)
    {
        for (int i = 0; i < 126; i++) {
            for (int j = 0; j < 126; j++) {
                fichier << M[i][j] << " ";
            }
            fichier << endl;
        }
        fichier.close();
    }
    else
        cout << "Impossible d'ouvrir le fichier !" << endl;

    delete result;
    delete pstmt;
    delete con;
    system("pause");
    return 0;
}