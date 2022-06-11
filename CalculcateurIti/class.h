#include <iostream>
#pragma once
class Arret {
private:
	char* nom;
public:
	Arret(char*);
	~Arret();
	char* getNom();
	void setNom();
};

Arret::Arret(char* nom) {
	this->nom = nom;
}

Arret::~Arret() {
	delete[] nom;
}

char* Arret::getNom() {
	return nom;
}

void Arret::setNom() {
	std::cout << "Nom de l'arret : ";
	std::cin >> nom;
}


class Troncons {
private:
	Arret A;
	Arret B;
	int temps;
public:
	Troncons(Arret, Arret, int);
	~Troncons();
	Arret getA();
	Arret getB();
	int getTemps();
	void setA();
	void setB();
	void setTemps();
};

Troncons::Troncons(Arret A, Arret B, int temps) {
	this->A = A;
	this->B = B;
	this->temps = temps;
}

Troncons::~Troncons() {

}

Arret Troncons::getA() {
	return A;
}

Arret Troncons::getB() {
	return B;
}

int Troncons::getTemps() {
	return temps;
}

void Troncons::setA() {
	std::cout << "Nom de l'arret A : ";
	std::cin >> A.getNom();
}

void Troncons::setB() {
	std::cout << "Nom de l'arret B : ";
	std::cin >> B.getNom();
}

void Troncons::setTemps() {
	std::cout << "Temps de parcours : ";
	std::cin >> temps;
}

class Ligne {
private:
	Troncons* troncons;
public:
	Ligne(Troncons*);
	~Ligne();
	Troncons* getTroncons();
	void setTroncons();
};


Ligne::Ligne(Troncons* troncons) {
	this->troncons = troncons;
}

Ligne::~Ligne() {

}

Troncons* Ligne::getTroncons() {
	return troncons;
}

void Ligne::setTroncons() {
	std::cout << "Nombre de troncons : ";
	int nbTroncons;
	std::cin >> nbTroncons;
}

class Graphes {
	private:
	Ligne* lignes;
	public:
		Graphes(Ligne*);
		~Graphes();
		Ligne* getLignes();
		void setLignes();
};

Graphes::Graphes(Ligne* lignes) {
	this->lignes = lignes;
}

Graphes::~Graphes() {

}

Ligne* Graphes::getLignes() {
	return lignes;
}

void Graphes::setLignes() {
	std::cout << "Nombre de lignes : ";
	int nbLignes;
	std::cin >> nbLignes;
}
