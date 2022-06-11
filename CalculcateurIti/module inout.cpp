#include <iostream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <string>
#include <fstream>
using namespace std;

const string server = "10.1.139.236";
const string username = "c2";
const string password = "mdp";

sql::Driver* driver;
sql::Connection* con;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

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

int main() {
	initBD();
    int nArretDep;
    int nArretArr;
    string nomArretDepart;
    string nomArretArrivee;
    cout << "Donnez le nom de l'arret de depart :";
    cin >> nomArretDepart;
    cout << "Donnez le nom de l'arret d'arrivee :";
    cin >> nomArretArrivee;

    string request = "select nArret from Arret where nomArret = '" + nomArretDepart + "';";
    pstmt = con->prepareStatement(request);
	result = pstmt->executeQuery();

	while (result->next()) {
		nArretDep = result->getInt("nArret");
	}

    string request1 = "select nArret from Arret where nomArret = '" + nomArretArrivee + "';";
    pstmt = con->prepareStatement(request1);
	result = pstmt->executeQuery();

	while (result->next()) {
	    nArretArr = result->getInt("nArret");
	}

    cout << nArretDep << endl;
    cout << nArretArr << endl;

    ofstream fichier("M.txt");
    if (fichier) {
        cout << "Ouverture du fichier" << endl;
    } else
		cout << "Erreur à l'ouverture du fichier" << endl;

	delete result;
	delete pstmt;
	delete con;
	system("pause");
	return 0;
}