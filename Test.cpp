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

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

//------------------------------------------------------ Fonctions privées

static void RechercheSimple(ListeTrajets & catalogue, const char * dep,const char * arr) 
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
}//----- Fin de RechercheSimple

static void RechercheAvancee(ListeTrajets & catalogue,char * dep, char * arr) 
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
		for (unsigned int i = 0; i < intermede->Taille(); i++) 
		{
			if (retrait)
				i = 0;
			retrait = false;
			//regarder si arrivee matche demande si oui => result
			if (strcmp(arr, (intermede->getTrajet(i))->Arrivee()) == 0) 
			{
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
	cout << "Trajets possibles :"<<endl;
	cout << "---------" << endl;
	result->Afficher();
	cout << "--------" << endl;
}//----- Fin de RechercheAvancee

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
		//TS
		case 1: {

			char depart[1024];
			char arrivee[1024];
			char transport[1024];

			cout << "Ville de depart ?" << endl;
			cin >> depart;
			cout << "Ville d'arrivee ?" << endl;
			cin >> arrivee;
			while (strcmp(depart, arrivee) == 0) {
				cout<< "Veuillez ressaisir les villes de depart et d'arrivee de votre trajet"<< endl;
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

		case 2: {

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
		case 3: {
			catalogue.Afficher();
			break;
		}

			//search
		case 4: {
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

		case 5:
			//stopper l'exécution
			break;
		}
		cout << "Choisissez une option: " << endl;
		cout << "1. Ajouter Trajet Simple " << endl;
		cout << "2. Ajouter Trajet Compose " << endl;
		cout << "3. Afficher le catalogue des trajets " << endl;
		cout << "4. Rechercher un parcours " << endl;
		cout << "5. Sortir" << endl;
		cin >> action;
	}//----- Fin de Menu

}

int main()
// Algorithme :
{
	ListeTrajets * catalogue = new ListeTrajets;
	Menu(*catalogue);
	delete catalogue;
	return 0;
}//----- Fin de main
