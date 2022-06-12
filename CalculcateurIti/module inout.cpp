#include <iostream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <ctype.h>
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

void initBD() {
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

int minDistance(int dist[], bool visited[]) {
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

int dijkstra(vector<vector<int>> matrice, int s, int f) {
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

	//int graph[V][V];
	int source, final, time;
	string sourceInput, finalInput;


	//init graph
	FILE* fichier = NULL;
	char ch;
	int num;
	bool dig;
	int i = 0;
	int j = 0;
	vector<vector<int>> graph(V, vector<int>(V));
	for (int i = 0; i < V; i++) {
		for (int j = 0; j < V; j++) {
			graph[i][j] = 0;
		}
	}
	fichier = fopen("M.txt", "r");
	if (fichier == NULL)
	{
		printf("\nFichier impossible à ouvrir");
	}
	else
	{
		while ((ch = fgetc(fichier)) != EOF)
		{
			dig = isdigit(ch);
			if (dig) {
				num = ch - '0';
			}
			if (ch == '\n')
			{
				i++;
				j = 0;
			}
			if (dig && num > 0)
			{
				graph[i][j] = num;
			}
			if (dig)
			{
				j++;
			}
		}
	}
	

	//initPrevious
	for (int i = 0; i < V; i++) {
		previous[i] = NULL;
	}

	//user input
	std::cout << "Saisir l'arrêt de départ : " << std::endl;
	std::getline(std::cin, sourceInput);
	std::cout << "Saisir l'arrêt d'arrivée : " << std::endl;
	std::getline(std::cin, finalInput);


	//getnArret from user input
	string request = "select nArret from Arret where nomArret = '" + sourceInput + "';";
	pstmt = con->prepareStatement(request);
	result = pstmt->executeQuery();

	while (result->next()) {
		source = result->getInt("nArret");
	}

	string request1 = "select nArret from Arret where nomArret = '" + finalInput + "';";
	pstmt = con->prepareStatement(request1);
	result = pstmt->executeQuery();

	while (result->next()) {
		final = result->getInt("nArret");
	}

	//call algorithm
	time = dijkstra(graph, source, final);
	calcPath(source, final);

	//print result
	std::cout << "Il faut " << time << " minutes pour aller de " << sourceInput << " à " << finalInput << "." << std::endl;
	std::cout << "Trajet : " << std::endl;
	//BD trajet to do
}

int main() {
	initBD();
	initDijkstra();

	delete result;
	delete pstmt;
	delete con;
	system("pause");
	return 0;
	
}