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

const int inf = 999999; //repr�sente l'infini
const int V = 126; //nombre d'arr�ts dans le r�seau
int path[V]; //chemin final
int cptPath=0;
int previous[V]; //pr�d�cesseur du sommet

const string server = "10.1.139.236";
const string username = "c2";
const string password = "mdp";

sql::Driver* driver;
sql::Connection* con;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

/// <summary>
/// methode de connexion a la bd et choix du schema
/// </summary>
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

/// <summary>
///  methode qui permet de trouver le nouvel arret non visit� avec le plus court temps de trajet
/// </summary>
/// <param name="dist">tableau des distances des arrets</param>
/// <param name="visited">tableaux des arr�ts deja visit�</param>
/// <returns>numero d'arret qui n'a pas encore �t� visit� avec le plus petit temps de trajet</returns>
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

/// <summary>
/// implementation de l'algo de dijkstra
/// </summary>
/// <param name="matrice">tableau 2D comprenant la matrice d'adjacence avec les valeurs des distances</param>
/// <param name="s">arret de depart</param>
/// <param name="f">arret d'arriv�e</param>
/// <returns>le temps entre l'arret de depart et de fin</returns>
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

/// <summary>
/// Methode qui rempli le nom des arrets dans le tableau path des arret qui sont utilis� pour le trajet demand�
/// </summary>
/// <param name="s">numero d'arret de depart</param>
/// <param name="f">numero d'arret d'arriv�e</param>
void calcPath(int s, int f) {
	int i;
	path[0] = f;
	i = 1;
	cptPath++;
	while (path[i-1] != s) {
		path[i] = previous[path[i - 1]];
		cptPath++;
		i++;
	}
	//print path

}

void initDijkstra() {

	int source, final, time;
	string sourceInput, finalInput;
	FILE* fichier = NULL;
	char ch;
	int num;
	bool dig;
	int i = 0;
	int j = 0;
	vector<vector<int>> graph(V, vector<int>(V));

	// on rempli le graph de 0 puis on y implementera les dur�e si les deux arrets se rejoignent
	for (int i = 0; i < V; i++) {
		for (int j = 0; j < V; j++) {
			graph[i][j] = 0;
		}
	}

	// on ouvre la matrice et on lit caractere par caract�re
	fichier = fopen("M.txt", "r");
	if (fichier == NULL)
	{
		printf("\nFichier impossible a ouvrir");
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
	std::cout << "Saisir l'arret de depart : " << std::endl;
	std::getline(std::cin, sourceInput);
	std::cout << "Saisir l'arret d'arrivee : " << std::endl;
	std::getline(std::cin, finalInput);

	//getnArret from user input
	try {
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
	}
	catch (exception e) {
		cout << e.what();
	}

	//numArret dans la base commence � 1 et � 0 dans l'algorithme donc on enleve 1 aux num�ros d'arret 
	final--;
	source--;

	//call algorithm
	time = dijkstra(graph, source, final);
	calcPath(source, final);

	//print result dijkstra
	std::cout << "Il faut " << time << " minutes pour aller de " << sourceInput << " a " << finalInput << "." << std::endl;
	std::cout << "Trajet : " << std::endl;

	string nomArret;
	string tmp = std::to_string(path[cptPath - 1]+1);
	string request2 = "select nomArret from Arret where nArret =" + tmp + ";";

	pstmt = con->prepareStatement(request2);
	result = pstmt->executeQuery();
	while (result->next()) {
		 nomArret = result->getString("nomArret");
	}

	int temps = graph[path[cptPath - 1]][path[cptPath - 2]];
	cout << nomArret << " --" << temps << "--> ";

	
	for (int i = cptPath-2; i>0; i--) {
		tmp = std::to_string(path[i]+1);
		request2 = "select nomArret from Arret where nArret =" + tmp + ';';
		pstmt = con->prepareStatement(request2);
		result = pstmt->executeQuery();

		while (result->next()) {
			nomArret = result->getString("nomArret");
		}
		temps = graph[path[i]][path[i-1]];
		cout << nomArret << " --" << temps << "--> ";
	}
	tmp = std::to_string(path[0]+1);
	request2 = "select nomArret from Arret where nArret =" + tmp + ';';
	pstmt = con->prepareStatement(request2);
	result = pstmt->executeQuery();

	while (result->next()) {
		nomArret = result->getString("nomArret");
	}
	cout << nomArret<<endl;
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