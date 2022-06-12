#include <iostream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <string>
#include <fstream>
using namespace std;

const int inf = 999999; //représente l'infini
const int V = 126; //nombre d'arrêts dans le réseau
int path[V]; //chemin final
int previous[V]; //prédécesseur du sommet

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

int minDistance(int* dist, bool* visited) {
	int minVal = inf;
	int min;

	for (int i = 0; i < V; i++) {
		if (!visited[i] && dist[i] <= minVal) {
			minVal = dist[i];
			min = i;
		}
	}
	return min;
}

int dijkstra(int** matrice, int s, int f) {
	int current;
	int dist[V];
	bool visited[V];

	for (int i = 0; i < V; i++) {
		dist[i] = inf;
		visited[i] = false;
	}

	dist[s] = 0;

	for (int i = 0; i < V - 1; i++) {
		current = minDistance(dist, visited);
		if (current == f) {
			return dist[f];
		}
		visited[current] = true;

		for (int j = 0; j < V; j++) {
			if (!visited[j] && matrice[current][j] > 0 && dist[current] != inf && (dist[current] + matrice[current][j] < dist[j])) {
				dist[j] = dist[current] + matrice[current][j];
				previous[j] = current;
			}
		}
	}
}

void calcPath(int s, int f) {
	int i;
	path[0] = f;
	i = 1;
	while (path[i] != s) {
		path[i] = previous[path[i - 1]];
		i++;
	}
}

void initDijkstra() {
	int graph[V][V];
	int source, final, time;
	char* sourceInput, finalInput;

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

	for (int i = 0; i < V; i++) {
		previous[i] = NULL;
	}

	std::cout << "Saisir l'arrêt de départ : " << std::endl;
	std::cin >> sourceInput;
	std::cout << "Saisir l'arrêt d'arrivée : " << std::endl;
	std::cin >> finalInput;

	//BD assigne narret

	time = dijkstra(graph, source, final);
	calcPath(source, final);

	std::cout << "Il faut " << time << " minutes pour aller de " << sourceInput << " à " << finalInput << "." << std::endl;
	std::cout << "Trajet : " << std::endl;
	//BD
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