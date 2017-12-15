/*************************************************************************
 Test  -  description
 -------------------
 d√©but                : $DATE$
 copyright            : (C) $YEAR$ par $AUTHOR$
 e-mail               : $EMAIL$
 *************************************************************************/

//---------- R√©alisation du module <Test> (fichier Test.cpp) ---------------
/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include syst√®me
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

//------------------------------------------------------ Fonctions priv√©es

static void RechercheSimple(ListeTrajets & catalogue, const char * dep,
		const char * arr)
// Algorithme :
// cherche dans le catalogue les trajets qui ont la ville de d√©part = param2 et ville d'arriv√©e = param3
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
			//=> ajouter √† sa place tous ceux qui le contiennent
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
					retrait = true; //on repart du d√©but puisqu'on a chang√© le nombre d'√©l√©ments
				} else {
					for (unsigned int j = 0; j < correspondent->Taille(); j++) {

						ListeTrajets * aAjouter = new ListeTrajets;
						aAjouter->Ajouter(intermede->getTrajet(i));
						aAjouter->Ajouter(correspondent->getTrajet(j));
						TrajetCompose * t = new TrajetCompose(aAjouter);
						intermede->Ajouter(t);
					}
					intermede->Retirer(i);
					retrait = true; //on repart du d√©but puisqu'on a chang√© le nombre d'√©l√©ments
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

static void readTS(ListeTrajets & catalogue, stringstream ss) {
	String word;
	String depart;
	String arrivee;
	String transport;

	ss >> depart;
	while (ss.peek() != ";") {
		ss >> word;
		depart += word;
	}
	ss >> word; //throw the separator
	ss >> arrivee;
	while (ss.peek() != ";") {
		ss >> word;
		arrivee += word;
	}
	ss >> word; //throw the separator
	ss >> transport;
	while (ss.peek() != null) {
		ss >> word;
		transport += word;
	}
	catalogue->Ajouter(new TrajetSimple(depart, arrivee, transport));
	//do I have to explicitely cast from string to char * ??????????????
}

static void readTC(ListeTrajets & catalogue, stringstream ss) {
	String word;
	String depart;
	String arrivee;
	String transport;
	ListeTrajets * lt = new ListeTrajets();
	ss >> depart;
	while (ss.peek() != ";") {
		ss >> word;
		depart += word;
	}


	while (ss.peek()!=null) {
		ss >> word; //throw the separator
		ss >> arrivee;
		while (ss.peek() != ";") {
			ss >> word;
			arrivee += word;
		}
		ss >> word; //throw the separator
		ss >> transport;
		while (ss.peek()!=null && ss.peek() != ";") {
			ss >> word;
			transport += word;
		}
		lt->Ajouter(new TrajetSimple(depart, arrivee, transport));

		strcpy(depart,arrivee);
	}

	catalogue->Ajouter(new TrajetCompose(lt));

}

static void read(ListeTrajets & catalogue, String choice, ifstream is) {
	String ligne;
	String word;
	String type;

	int nb;
	//int nbTS;
	//int nbTC;
	switch (choice) {
	case "all": {
		is.getline(ligne);
		is.getline(ligne);
		//on skip les deux lignes de metadata
		while (is.peek() != null) {
			is.getline(ligne);
			stringstream ss(ligne);
			ss >> type;
			if (type == "TS") {
				readTS(catalogue, ss);
			} else if (type == "TC") {
				readTC(catalogue, ss);
			}

		}
		break;
	}
	case "TS": {
		is.getline(ligne);
		stringstream ss(ligne);
		ss >> nb;
		if (nb == 0)
			break;
		is.getline(ligne); //skip metadata about TC

		while (is.peek() != null) {
			is.getline(ligne);
			stringstream ss(ligne);
			ss >> type;
			if (type == "TS") {
				readTS(catalogue, ss);
			}

		}
		break;
	}
	case "TC": {
		is.getline(ligne);
		is.getline(ligne);
		stringstream ss(ligne);
		ss >> nb;
		if (nb == 0)
			break;
		while (is.peek() != null) {
			is.getline(ligne);
			stringstream ss(ligne);
			ss >> type;
			if (type == "TC") {
				readTC(catalogue, ss);
			}

		}
		break;
	}
	case "ville": {
		//ask
		char constraint;
		cout
				<< "Souhaitez-vous imposer une ville de dÈpart (D), d'arrivÈe (A) ou les deux (B) ?"
				<< endl;
		cout << "Veuillez saisir la lettre correspondant ‡ vtre choix" << endl;
		cin >> constraint;

		String wanted_dep;
		String wanted_arr;

		switch (constraint) {
		case 'D' or 'd': {
			cout
					<< "Quelle doit Ítre la ville de dÈpart des trajets ‡ sÈlectionner ?"
					<< endl;
			cin >> wanted_dep;
		}
		case 'A' or 'a': {
			cout
					<< "Quelle doit Ítre la ville d'arrivÈe des trajets ‡ sÈlectionner ?"
					<< endl;
			cin >> wanted_arr;
		}
		case 'B' or 'b': {
			cout
					<< "Quelle doit Ítre la ville de dÈpart des trajets ‡ sÈlectionner ?"
					<< endl;
			cin >> wanted_dep;
			cout
					<< "Quelle doit Ítre la ville d'arrivÈe des trajets ‡ sÈlectionner ?"
					<< endl;
			cin >> wanted_arr;
		}
		}
		while (is.peek() != null) {
			is.getline(ligne);
			stringstream ss(ligne);
			ss >> type;
			ss >> depart;
			if (wanted_dep == null || depart != wanted_dep)
				break;
			ss >> arrivee;
			if (wanted_arr == null || arrivee != wanted_arr)
				break;

		}
		break;
	}
	case "intervalle": {
		//ask for interval
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
			<< "Veuillez saisir le nom du fichier √† charger ou \"exit\" pour sortir: "
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
				<< "Le fichier n'a pas √©t√© trouv√©, veuillez saisir √† nouveau le nom du fichier √† charger (ou \"exit\" pour sortir): "
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
	cout << "3. Charger un trajet en fonction du d√©part et/ou de l'arriv√©e "
			<< endl;
	//si ya pas de trajet qui correspond, printer que on a a pas trouv√© "aucun trajet ne correspond √† votre demande"
	cout << "4. Charger seulement une s√©lection de trajets " << endl;
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

static void readCatalogue(ListeTrajets & catalogue, string choice, ofstream &os)
{
	//os.open("test.txt",std::ofstream::out | std::ofstream::app);

	for (unsigned int i = 0; i < catalogue.Taille(); i++) {
		const char * depart = catalogue.getTrajet(i)->Depart();
		const char * arrivee = catalogue.getTrajet(i)->Arrivee();
		//const char * transport = catalogue.getTrajet(i)->Transport();

		os << depart;

	}

}

static void testReadCatalogue(ListeTrajets * catalogue)
{
	ofstream os;
	os.open("test.txt",std::ofstream::out | std::ofstream::app);

	TrajetSimple *ts1 = new TrajetSimple("ty", "B", "MT");
	Trajet * ts2 = new TrajetSimple("B", "C", "MT2");
	TrajetSimple *ts3 = new TrajetSimple("ty", "C", "MT3");
	TrajetSimple *ts4 = new TrajetSimple("C", "K", "MT4");
	TrajetSimple *ts5 = new TrajetSimple("C", "E", "MT5");
	TrajetSimple *ts6 = new TrajetSimple("E", "K", "MT6");

	ListeTrajets * lt2 = new ListeTrajets;
	lt2->Ajouter(ts1);
	lt2->Ajouter(ts2);
	TrajetCompose * tc1 = new TrajetCompose(lt2);

	catalogue->Ajouter(tc1);
	catalogue->Ajouter(ts3);
	catalogue->Ajouter(ts4);
	catalogue->Ajouter(ts5);
	catalogue->Ajouter(ts6);

	catalogue->Afficher();

	readCatalogue(*catalogue,"all",os);
}

static void save(ListeTrajets & catalogue) {
	ofstream os;

	//demander le nom du fichier
	cout
	<< "Veuillez saisir le nom du fichier √† charger ou \"exit\" pour sortir: "
	<< endl;
	string filename;
	cin >> filename;
	if (filename == "exit" || filename == "exit ")
	return;

	//ouverture/creation d'un nouvel fichier avec le nom donn√©
	os.open(filename,std::ofstream::out | std::ofstream::app);

	//demander si on veut append ou escraser

	//choisir les options pour la sauvegar

	cout << "Veuillez choisir une option: " << endl;
	cout << "1. Charger tous les trajets " << endl;
	cout << "1. Charger uniquement les Trajets Simples " << endl;
	cout << "2. Charger uniquement les Trajets Composes " << endl;
	cout << "3. Charger un trajet en fonction du d√©part et/ou de l'arriv√©e "
	<< endl;
	//si ya pas de trajet qui correspond, printer que on a a pas trouv√© "aucun trajet ne correspond √† votre demande"
	cout << "4. Charger seulement une s√©lection de trajets " << endl;
	cout << "5. Sortir" << endl;

	int action;
	cin >> action;

//	switch (action)
//	{
//		case 1: {
//			//load infos from file
//			read(catalogue, "all", os);
//			break;
//		}
//		case 2: {
//			//load infos from file
//			read(catalogue, "TS", os);
//			break;
//		}
//		case 3: {
//			//load infos from file
//			read(catalogue, "TC", os);
//			break;
//		}
//		case 4: {
//			//load infos from file
//			read(catalogue, "ville", os);
//			break;
//		}
//		case 5: {
//			//load infos from file
//			read(catalogue, "intervalle", os);
//			break;
//		}
//
//	}

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
				load(catalogue);
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
				//appel mÔøΩthode de recherche
				char depart[1024];
				char arrivee[1024];
				cout << "Quelle est votre ville de dÔøΩpart ?" << endl;
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
			//stopper l'ex√©cution
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
	testReadCatalogue(catalogue);
	delete catalogue;
	return 0;
} //----- Fin de main
