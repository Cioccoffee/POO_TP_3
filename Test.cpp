/*************************************************************************
 Test  -  description
 -------------------
 début                : $DATE$
 copyright            : (C) $YEAR$ par $AUTHOR$
 e-mail               : $EMAIL$
 *************************************************************************/

//---------- Réalisation du module <Test> (fichier Test.cpp) ---------------
/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
using namespace std;
#include <iostream>

//------------------------------------------------------ Include personnel
#include "Test.h"
#include <cstring>
#include <fstream>
#include <sys/stat.h>

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

//------------------------------------------------------ Fonctions privées

static void RechercheSimple(ListeTrajets & catalogue, const char * dep,
		const char * arr)
// Algorithme :
// cherche dans le catalogue les trajets qui ont la ville de départ = param2 et ville d'arrivée = param3
		{
	bool trouve = false;
	for (unsigned int i = 0; i < catalogue.Taille(); i++) {
		const char * depart = catalogue.getTrajet(i)->Depart();
		const char * arrivee = catalogue.getTrajet(i)->Arrivee();
		if (strcmp(depart, dep) == 0 && strcmp(arrivee, arr) == 0) {
			trouve = true;
			catalogue.getTrajet(i)->Afficher();
		}
	}
	if (!trouve)
		cout << "Il n'y a pas de trajet qui corresponde !" << endl;
} //----- Fin de RechercheSimple

static void RechercheAvancee(ListeTrajets & catalogue, char * dep, char * arr)
// Algorithme :
		{
	ListeTrajets * intermede = new ListeTrajets;
	ListeTrajets * result = new ListeTrajets;

	//ajout des trajets qui partent du bon endroit
	for (unsigned int i = 0; i < catalogue.Taille(); i++) {
		if (strcmp((catalogue.getTrajet(i))->Depart(), dep) == 0) {
			intermede->Ajouter(catalogue.getTrajet(i));
		}
	}
	//recherche des correspondances et des resultats valides
	while (intermede->Taille() > 0) {

		bool retrait = false;
		for (unsigned int i = 0; i < intermede->Taille(); i++) {
			if (retrait)
				i = 0;
			retrait = false;
			//regarder si arrivee matche demande si oui => result
			if (strcmp(arr, (intermede->getTrajet(i))->Arrivee()) == 0) {
				result->Ajouter(intermede->getTrajet(i));
				intermede->Retirer(i);
				retrait = true;
			}
			//sinon regarder si arrivee matche qqch
			//et creer liste de tout ce qui matche pour ce trajet
			//=> ajouter à sa place tous ceux qui le contiennent
			else {
				ListeTrajets * correspondent = new ListeTrajets;
				for (unsigned int j = 0; j < catalogue.Taille(); j++) {
					if (strcmp((catalogue.getTrajet(j))->Depart(),
							(intermede->getTrajet(i))->Arrivee()) == 0) {
						correspondent->Ajouter(catalogue.getTrajet(j));
					}
				}
				if (correspondent->Taille() == 0) {
					intermede->Retirer(i);
					retrait = true; //on repart du début puisqu'on a changé le nombre d'éléments
				} else {
					for (unsigned int j = 0; j < correspondent->Taille(); j++) {

						ListeTrajets * aAjouter = new ListeTrajets;
						aAjouter->Ajouter(intermede->getTrajet(i));
						aAjouter->Ajouter(correspondent->getTrajet(j));
						TrajetCompose * t = new TrajetCompose(aAjouter);
						intermede->Ajouter(t);
					}
					intermede->Retirer(i);
					retrait = true; //on repart du début puisqu'on a changé le nombre d'éléments
				}
			}
		}

	}
	cout << "---------" << endl;
	cout << "Trajets possibles :" << endl;
	cout << "---------" << endl;
	result->Afficher();
	cout << "--------" << endl;
} //----- Fin de RechercheAvancee

static void read(ListeTrajets & catalogue, String choice, ifstream is) {
	String ligne;
	String word;
	int nb;
	//int nbTS;
	//int nbTC;
	switch (choice)
	{
	case "all": {
		while (is.peek() != null) {
			is.getline(ligne);
			stringstream ss(ligne);
			ss >> word;


		}
		break;
	}
	case "TS": {
		is.getline(ligne);
		stringstream ss(ligne);
		ss >> nb;
		if(nb == 0) break;
		while (is.peek() != null) {
			is.getline(ligne);
			stringstream ss(ligne);
			ss >> word;

		}
		break;
	}
	case "TC": {
		is.getline(ligne);
		is.getline(ligne);
		stringstream ss(ligne);
		ss >> nb;
		if(nb == 0) break;
		while (is.peek() != null) {
			is.getline(ligne);
			stringstream ss(ligne);
			ss >> nb;


		}
		break;
	}
	case "ville": {
		while (is.peek() != null) {
			is.getline(ligne);
			stringstream ss(ligne);

		}
		break;
	}
	case "intervalle": {
		while (is.peek() != null) {
			is.getline(ligne);
			stringstream ss(ligne);

		}
		break;
	}
	}

}

static void load(ListeTrajets & catalogue) {
	ifstream is;

	cout
			<< "Veuillez saisir le nom du fichier à charger ou \"exit\" pour sortir: "
			<< endl;
	String filename;
	cin >> filename;
	if (filename == "exit" || filename == "exit ")
		return;
	is = new ifstream(filename);
	//check if exists
	is.open(filename);
	while (!is.open()/*stat (filename, &buffer) != 0*/) {
		//demander si souhaite ou mettre une option de sortie
		cout
				<< "Le fichier n'a pas été trouvé, veuillez saisir à nouveau le nom du fichier à charger (ou \"exit\" pour sortir): "
				<< endl;
		cin >> filename;
		if (filename == "exit" || filename == "exit ")
			return;
		is.open(filename);
	}
	cout << "Veuillez choisir une option: " << endl;
	cout << "1. Charger tous les trajets " << endl;
	cout << "1. Charger uniquement les Trajets Simples " << endl;
	cout << "2. Charger uniquement les Trajets Composes " << endl;
	cout << "3. Charger un trajet en fonction du départ et/ou de l'arrivée "
			<< endl;
	//si ya pas de trajet qui correspond, printer que on a a pas trouvé "aucun trajet ne correspond à votre demande"
	cout << "4. Charger seulement une sélection de trajets " << endl;
	cout << "5. Sortir" << endl;

	//demander si on veut append ou escraser
	int action;
	cin >> action;

	while ()

		switch (action) {
		case 1: {
			//load infos from file
			read(catalogue, "all", is);
			break;
		}
		case 2: {
			//load infos from file
			read(catalogue, "TS", is);
			break;
		}
		case 3: {
			//load infos from file
			read(catalogue, "TC", is);
			break;
		}
		case 4: {
			//load infos from file
			read(catalogue, "ville", is);
			break;
		}
		case 5: {
			//load infos from file
			read(catalogue, "intervalle", is);
			break;
		}

		}

}

static void save(ListeTrajets & catalogue) {

}

static void Menu(ListeTrajets & catalogue)
// Algorithme :
		{
	cout << "Veuillez choisir une option: " << endl;
	cout << "1. Ajouter Trajet Simple " << endl;
	cout << "2. Ajouter Trajet Compose " << endl;
	cout << "3. Afficher le catalogue des trajets " << endl;
	cout << "4. Rechercher un parcours " << endl;
	cout << "5. Sortir" << endl;

	int action;
	cin >> action;

	while (action != 5
			&& (action == 1 || action == 2 || action == 3 || action == 4)) {
		switch (action) {

		case 1: {
			//load infos from file
			load();
			break;
		}
			//TS
		case 2: {

			char depart[1024];
			char arrivee[1024];
			char transport[1024];

			cout << "Ville de depart ?" << endl;
			cin >> depart;
			cout << "Ville d'arrivee ?" << endl;
			cin >> arrivee;
			while (strcmp(depart, arrivee) == 0) {
				cout
						<< "Veuillez ressaisir les villes de depart et d'arrivee de votre trajet"
						<< endl;
				cout << "Ville de depart ?" << endl;
				cin >> depart;
				cout << "Ville d'arrivee ?" << endl;
				cin >> arrivee;
			}
			cout << "Moyen de transport ?" << endl;
			cin >> transport;

			catalogue.Ajouter(new TrajetSimple(depart, arrivee, transport));

			break;
		}

			//TC

		case 3: {

			ListeTrajets * lt = new ListeTrajets;

			cout << "De combien de trajets est compose ce trajet compose ?"
					<< endl;
			int i = 0;
			int n;
			cin >> n;

			char arriveePrecedent[1024];
			strcpy(arriveePrecedent, "no dest");

			while (i < n) {
				//while(arrivee d'avant != depart de maintenant) => sinon on peut le remplir pour lui
				cout << "Saisie du trajet num " << i + 1 << endl;

				char depart[1024];
				char arrivee[1024];
				char transport[1024];

				cout << "Ville de depart ?" << endl;
				cin >> depart;

				cout << "Ville d'arrivee ?" << endl;
				cin >> arrivee;

				while ((strcmp(depart, arrivee) == 0)
						|| ((strcmp(depart, arriveePrecedent) != 0) && (i != 0))) {
					cout
							<< "Veuillez ressaisir les villes de depart et d'arrivee de votre trajet"
							<< endl;
					cout << "Ville de depart ?" << endl;
					cin >> depart;
					cout << "Ville d'arrivee ?" << endl;
					cin >> arrivee;
				}
				cout << "Moyen de transport ?" << endl;
				cin >> transport;

				strcpy(arriveePrecedent, arrivee);
				lt->Ajouter(new TrajetSimple(depart, arrivee, transport));

				i++;
			}
			TrajetCompose * tc = new TrajetCompose(lt);

			catalogue.Ajouter(tc);

			//delete lt;

			break;
		}

			//catalogue
		case 4: {
			catalogue.Afficher();
			break;
		}

			//search
		case 5: {
			//appel m�thode de recherche
			char depart[1024];
			char arrivee[1024];
			cout << "Quelle est votre ville de d�part ?" << endl;
			cin >> depart;
			cout << "Quelle est votre destination ?" << endl;
			cin >> arrivee;

			//RechercheSimple(catalogue, depart, arrivee);
			RechercheAvancee(catalogue, depart, arrivee);

			break;
		}
		case 6: {
			//sauvegarder
			save(catalogue);
			break;
		}
		case 7:
			//stopper l'exécution
			break;
		}
		cout << "Choisissez une option: " << endl;
		cout << "1. Charger des trajets depuis un fichier " << endl;
		cout << "2. Ajouter Trajet Simple " << endl;
		cout << "3. Ajouter Trajet Compose " << endl;
		cout << "4. Afficher le catalogue des trajets " << endl;
		cout << "5. Rechercher un parcours " << endl;
		cout << "6. Sauvegarder des trajets dans un fichier" << endl;
		cout << "7. Sortir" << endl;
		cin >> action;
	} //----- Fin de Menu

}

int main()
// Algorithme :
{
	ListeTrajets * catalogue = new ListeTrajets;
	Menu(*catalogue);
	delete catalogue;
	return 0;
} //----- Fin de main
