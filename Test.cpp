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
#include <sstream>
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

	while (ss.peek() != null) {
		ss >> word; //throw the separator
		ss >> arrivee;
		while (ss.peek() != ";") {
			ss >> word;
			arrivee += word;
		}
		ss >> word; //throw the separator
		ss >> transport;
		while (ss.peek() != null && ss.peek() != ";") {
			ss >> word;
			transport += word;
		}
		lt->Ajouter(new TrajetSimple(depart, arrivee, transport));

		strcpy(depart, arrivee);
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
		<< "Souhaitez-vous imposer une ville de d�part (D), d'arriv�e (A) ou les deux (B) ?"
		<< endl;
		cout << "Veuillez saisir la lettre correspondant � vtre choix" << endl;
		cin >> constraint;

		String wanted_dep;
		String wanted_arr;

		switch (constraint) {
		case 'D' or 'd': {
			cout
			<< "Quelle doit �tre la ville de d�part des trajets � s�lectionner ?"
			<< endl;
			cin >> wanted_dep;
		}
		case 'A' or 'a': {
			cout
			<< "Quelle doit �tre la ville d'arriv�e des trajets � s�lectionner ?"
			<< endl;
			cin >> wanted_arr;
		}
		case 'B' or 'b': {
			cout
			<< "Quelle doit �tre la ville de d�part des trajets � s�lectionner ?"
			<< endl;
			cin >> wanted_dep;
			cout
			<< "Quelle doit �tre la ville d'arriv�e des trajets � s�lectionner ?"
			<< endl;
			cin >> wanted_arr;
		}
		}
		while (is.peek() != null) {
			is.getline(ligne);
			stringstream ss(ligne);
			ss >> type;
			ss >> depart;
			while (ss.peek() != ";") {
				ss >> word;
				depart += word;
			}
			if (wanted_dep != null && depart != wanted_dep)
				break;
			ss >> word; //throw the separator
			ss >> arrivee;
			while (ss.peek() != ";") {
				ss >> word;
				depart += word;
			}
			if (wanted_arr != null && arrivee != wanted_arr)
				break;
			//if reached here, means we're in the right conditions
			if (type == "TS") {
				ss >> word; //throw the separator
				while (ss.peek() != null) {
					ss >> word;
					depart += word;
				}
				catalogue->Ajouter(
						new TrajetSimple(depart, arrivee, transport));
			} else if (type == "TC") {
				//completer
			}

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

static void readAllCatalogue(ListeTrajets & catalogue, ofstream &os, int & nbTS,
		int & nbTC) {

	for (unsigned int i = 0; i < catalogue.Taille(); i++) {

		if (catalogue.getTrajet(i)->Type().compare("TS") == 0) {

			nbTS++;
			string type = catalogue.getTrajet(i)->Type();
			const char * depart = catalogue.getTrajet(i)->Depart();
			const char * arrivee = catalogue.getTrajet(i)->Arrivee();
			const char * transport = catalogue.getTrajet(i)->Transport();
			os << type << " " << depart << " ; " << arrivee << " ; "
					<< transport << "\n";

		} else {

			nbTC++;
			string type = catalogue.getTrajet(i)->Type();
			os << type << " ";

			unsigned int j = 0;
			TrajetCompose *tc =
					dynamic_cast<TrajetCompose *>(catalogue.getTrajet(i));
			ListeTrajets * trajets = tc->Trajets();
			CelluleTrajet * actuelle = trajets->Tete();
			Trajet * actuel = actuelle->valeur;

			for (j = 0; j < trajets->Taille(); j++) {
				actuel = actuelle->valeur;
				os << actuel->Depart() << " ; " << actuel->Arrivee() << " ; "
						<< actuel->Transport();
				if (j != trajets->Taille() - 1)
					os << " ; ";
				if (actuelle->suivant != NULL)
					actuelle = actuelle->suivant;
			}
			os << "\n";
		}
	}
	//os<<nbTC;
	//os.seekp(0,ios::beg);
	//os.put('1');
	//os.write("TC ",3);
	//os<<nbTC;
	//	os<<"TS "<<nbTS<<"\n";
	//os<<"TC "<<nbTC<<"\n";
	//os.close();
}

static void readTSCatalogue(ListeTrajets & catalogue, ofstream &os) {

	for (unsigned int i = 0; i < catalogue.Taille(); i++) {

		if (catalogue.getTrajet(i)->Type().compare("TS") == 0) {

			//nbTS++;
			string type = catalogue.getTrajet(i)->Type();
			const char * depart = catalogue.getTrajet(i)->Depart();
			const char * arrivee = catalogue.getTrajet(i)->Arrivee();
			const char * transport = catalogue.getTrajet(i)->Transport();

			os << type << " " << depart << " ; " << arrivee << " ; "
					<< transport << "\n";

		}
	}
}

static void readTCCatalogue(ListeTrajets & catalogue, ofstream &os) {

	for (unsigned int i = 0; i < catalogue.Taille(); i++) {

		if (catalogue.getTrajet(i)->Type().compare("TC") == 0) {

			//nbTC++;
			string type = catalogue.getTrajet(i)->Type();
			os << type << " ";

			unsigned int j = 0;
			TrajetCompose *tc =
					dynamic_cast<TrajetCompose *>(catalogue.getTrajet(i));
			ListeTrajets * trajets = tc->Trajets();
			CelluleTrajet * actuelle = trajets->Tete();
			Trajet * actuel = actuelle->valeur;

			for (j = 0; j < trajets->Taille(); j++) {
				actuel = actuelle->valeur;
				os << actuel->Depart() << " ; " << actuel->Arrivee() << " ; "
						<< actuel->Transport();
				if (j != trajets->Taille() - 1)
					os << " ; ";
				if (actuelle->suivant != NULL)
					actuelle = actuelle->suivant;
			}
			os << "\n";
		}
	}
}

static void readVilleCatalogue(ListeTrajets & catalogue, string dep, string arr,
		ofstream &os) {

	if (dep.compare("none") != 0 && arr.compare("none") != 0) {
		for (unsigned int i = 0; i < catalogue.Taille(); i++) {
			string type = catalogue.getTrajet(i)->Type();
			const char * depart = catalogue.getTrajet(i)->Depart();
			const char * arrivee = catalogue.getTrajet(i)->Arrivee();

			if (dep.compare(depart) == 0 && arr.compare(arrivee) == 0) {
				if (catalogue.getTrajet(i)->Type().compare("TS") == 0) {
					const char * transport =
							catalogue.getTrajet(i)->Transport();
					os << type << " " << depart << " ; " << arrivee << " ; "
							<< transport << "\n";
				} else {
					string type = catalogue.getTrajet(i)->Type();
					os << type << " ";

					unsigned int j = 0;
					TrajetCompose *tc =
							dynamic_cast<TrajetCompose *>(catalogue.getTrajet(i));
					ListeTrajets * trajets = tc->Trajets();
					CelluleTrajet * actuelle = trajets->Tete();
					Trajet * actuel = actuelle->valeur;

					for (j = 0; j < trajets->Taille(); j++) {
						actuel = actuelle->valeur;
						os << actuel->Depart() << " ; " << actuel->Arrivee()
										<< " ; " << actuel->Transport();
						if (j != trajets->Taille() - 1)
							os << " ; ";
						if (actuelle->suivant != NULL)
							actuelle = actuelle->suivant;
					}
					os << "\n";

				}

			}

		}
	} else if (dep.compare("none") != 0 && arr.compare("none") == 0) {
		for (unsigned int i = 0; i < catalogue.Taille(); i++) {
			string type = catalogue.getTrajet(i)->Type();
			const char * depart = catalogue.getTrajet(i)->Depart();
			const char * arrivee = catalogue.getTrajet(i)->Arrivee();

			if (dep.compare(depart) == 0) {
				if (catalogue.getTrajet(i)->Type().compare("TS") == 0) {
					const char * transport =
							catalogue.getTrajet(i)->Transport();
					os << type << " " << depart << " ; " << arrivee << " ; "
							<< transport << "\n";
				} else {
					string type = catalogue.getTrajet(i)->Type();
					os << type << " ";

					unsigned int j = 0;
					TrajetCompose *tc =
							dynamic_cast<TrajetCompose *>(catalogue.getTrajet(i));
					ListeTrajets * trajets = tc->Trajets();
					CelluleTrajet * actuelle = trajets->Tete();
					Trajet * actuel = actuelle->valeur;

					for (j = 0; j < trajets->Taille(); j++) {
						actuel = actuelle->valeur;
						os << actuel->Depart() << " ; " << actuel->Arrivee()
										<< " ; " << actuel->Transport();
						if (j != trajets->Taille() - 1)
							os << " ; ";
						if (actuelle->suivant != NULL)
							actuelle = actuelle->suivant;
					}
					os << "\n";

				}

			}

		}
	} else if (dep.compare("none") == 0 && arr.compare("none") != 0) {
		for (unsigned int i = 0; i < catalogue.Taille(); i++) {
			string type = catalogue.getTrajet(i)->Type();
			const char * depart = catalogue.getTrajet(i)->Depart();
			const char * arrivee = catalogue.getTrajet(i)->Arrivee();

			if (arr.compare(arrivee) == 0) {
				if (catalogue.getTrajet(i)->Type().compare("TS") == 0) {
					const char * transport =
							catalogue.getTrajet(i)->Transport();
					os << type << " " << depart << " ; " << arrivee << " ; "
							<< transport << "\n";
				} else {
					string type = catalogue.getTrajet(i)->Type();
					os << type << " ";

					unsigned int j = 0;
					TrajetCompose *tc =
							dynamic_cast<TrajetCompose *>(catalogue.getTrajet(i));
					ListeTrajets * trajets = tc->Trajets();
					CelluleTrajet * actuelle = trajets->Tete();
					Trajet * actuel = actuelle->valeur;

					for (j = 0; j < trajets->Taille(); j++) {
						actuel = actuelle->valeur;
						os << actuel->Depart() << " ; " << actuel->Arrivee()
										<< " ; " << actuel->Transport();
						if (j != trajets->Taille() - 1)
							os << " ; ";
						if (actuelle->suivant != NULL)
							actuelle = actuelle->suivant;
					}
					os << "\n";

				}

			}

		}
	}
}

static void readIntervalleCatalogue(ListeTrajets & catalogue, unsigned int n,
		unsigned int m, ofstream &os) {

	if (n >= 1 && n <= m && m <= catalogue.Taille() + 1) {

		for (unsigned int i = n - 1; i < m; i++) {

			if (catalogue.getTrajet(i)->Type().compare("TS") == 0) {

				//nbTS++;
				string type = catalogue.getTrajet(i)->Type();
				const char * depart = catalogue.getTrajet(i)->Depart();
				const char * arrivee = catalogue.getTrajet(i)->Arrivee();
				const char * transport = catalogue.getTrajet(i)->Transport();
				os << type << " " << depart << " ; " << arrivee << " ; "
						<< transport << "\n";

			} else {

				//nbTC++;
				string type = catalogue.getTrajet(i)->Type();
				os << type << " ";

				unsigned int j = 0;
				TrajetCompose *tc =
						dynamic_cast<TrajetCompose *>(catalogue.getTrajet(i));
				ListeTrajets * trajets = tc->Trajets();
				CelluleTrajet * actuelle = trajets->Tete();
				Trajet * actuel = actuelle->valeur;

				for (j = 0; j < trajets->Taille(); j++) {
					actuel = actuelle->valeur;
					os << actuel->Depart() << " ; " << actuel->Arrivee()
									<< " ; " << actuel->Transport();
					if (j != trajets->Taille() - 1)
						os << " ; ";
					if (actuelle->suivant != NULL)
						actuelle = actuelle->suivant;
				}
				os << "\n";
			}
		}
	}

}

static void testReadCatalogue(ListeTrajets * catalogue) {
	ofstream os;
	//os.open("test.txt", std::ofstream::out | std::ofstream::app);
	//	unsigned int nbTS = 0;
	//	unsigned int nbTC = 0;

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

	//string * type = catalogue->getTrajet(0)->Type();
	//cout<<catalogue->getTrajet(1)->Type()<<endl;

	//readAllCatalogue(*catalogue, os);
	//delete ts1;
}

static void save(ListeTrajets & catalogue) {
	ofstream os;
	int nbTS = 0;
	int nbTC = 0;

	//demander le nom du fichier
	cout << "Veuillez saisir le nom du fichier à charger ou \"exit\" pour sortir: "
			<< endl;
	string filename;
	cin >> filename;
	if (filename == "exit" || filename == "exit ")
		return;

	//ouverture/creation d'un nouvel fichier avec le nom donné

	if (!os.is_open()) {
		os.open(filename, std::ofstream::out | std::ofstream::app);
		//os << "           \n";
		//os << "           \n";
	} else {
		os.open(filename, std::ofstream::out | std::ofstream::app);
		//		string ligne;
		//		string s;
		//		os.read(ligne,15);
		//		stringstream ss(ligne);
		//		ss>> s >> nbTS;
		//		cout<<"jkdsnfsz"<<nbTS;

	}

	//demander si on veut append ou escraser

	//choisir les options pour la sauvegar

	cout << "Veuillez choisir une option: " << endl;
	cout << "1. Sauvegarder tous les trajets " << endl;
	cout << "2. Sauvegarder uniquement les Trajets Simples " << endl;
	cout << "3. Sauvegarder uniquement les Trajets Composes " << endl;
	cout << "4. Sauvegarder un trajet en fonction du départ et/ou de l'arrivée "
			<< endl;

	//si ya pas de trajet qui correspond, printer que on a a pas trouvé "aucun trajet ne correspond à votre demande"
	cout << "5. Sauvegarder seulement une sélection de trajets " << endl;
	cout << "6. Sortir" << endl;

	//	string line;
	//	cin.getline(line,20);
	//	cout<<line;
	//	stringstream ss(line);
	//	int val;
	//	string s;
	//
	//	ss>>val>>s;
	//
	//	cout<< val;
	//	cout<<s;
	//	ss<<100;
	//	ss>>action;
	//	ss<<action;

	// verifier si action et bon !!!
	int action;
	cin >> action;

	switch (action) {
	case 1: {
		readAllCatalogue(catalogue, os, nbTS, nbTC);
		//os.close();
		//os.open(filename, std::ofstream::out);
		//		os.seekp(0);
		//		os << "TS " << nbTS << "\n";
		//		os << "TC " << nbTC << "\n";
		//os.close();
		break;
	}
	case 2: {
		readTSCatalogue(catalogue, os);
		break;
	}
	case 3: {
		readTCCatalogue(catalogue, os);
		break;
	}
	case 4: {
		string villeDepart;
		string villeArrivee;
		cout << "Choisissez une ville de départ ou \"none\" : ";
		cin >> villeDepart;
		cout << "Choisissez une ville d'arrivee ou \"none\" : ";
		cin >> villeArrivee;

		readVilleCatalogue(catalogue, villeDepart, villeArrivee, os);
		break;
	}
	case 5: {
		unsigned int n;
		cout << "Indice du premier trajet: " << endl;
		cin >> n;
		unsigned int m;
		cout << "Indice du dernier trajet: " << endl;
		cin >> m;
		readIntervalleCatalogue(catalogue, n, m, os);
		break;
	}
	}

	//os.close();
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

	while (action != 5 && (action == 1 || action == 2 || action == 3 || action == 4)) {
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

			cout << "De combien de trajets est compose ce trajet compose ?" << endl;
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
		}

		//catalogue
		case 4:
		{
			catalogue.Afficher();
			break;
		}
		//search
		case 5:
		{
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
		case 6:
		{
			//sauvegarder
			save(catalogue);
			break;
		}
		case 7:
		{
			//stopper l'exécution

			break;
		}
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

static void testFile() {
	ofstream os;
	os.open("test.txt", std::ofstream::out);

	os << "bbbbbbbbbbbbbbb\n";
	os << "ccccccccccccccc\n";

	os.seekp(0, ios::beg);
	os.write(" sam", 4);
	os.close();

	os.open("test.txt", std::ofstream::out | std::ofstream::app);

	os << "bbbbbbbbbbbbbbb\n";
}

int main()
// Algorithme :
{
	ListeTrajets * catalogue = new ListeTrajets;

	//Menu(*catalogue);
	testReadCatalogue(catalogue);
	save(*catalogue);
	//testFile();
	delete catalogue;
	return 0;
} //----- Fin de main
