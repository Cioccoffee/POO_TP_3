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
#include <typeinfo>
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

static void readTS(ListeTrajets & catalogue, stringstream& ss) {
	string word;
	string depart;
	string arrivee;
	string transport;

	ss >> depart;
	ss >> word;
	while (word != ";") {
		depart += word;
		ss >> word;
	}
	ss >> arrivee;
	ss >> word;
	while (word != ";") {
		arrivee += word;
		ss >> word;
	}
	ss >> transport;
	ss >> word;
	while (word != "-") {
		transport += word;
		ss >> word;
	}
	catalogue.Ajouter(
			new TrajetSimple(depart.c_str(), arrivee.c_str(),
					transport.c_str()));
}

static void readTC(ListeTrajets & catalogue, stringstream& ss) {
	string word;
	string depart;
	string arrivee;
	string transport;
	ListeTrajets * lt = new ListeTrajets();
	ss >> depart;
	ss >> word;
	while (word != ";") {
		depart += word;
		ss >> word;
	}
	ss >> word;
	ss >> arrivee;
	while (word != "-") {
		ss >> arrivee;
		ss >> word;
		while (word != ";") {
			arrivee += word;
			ss >> word;
		}
		ss >> transport;
		ss >> word;
		while (word != "" && word != ";" && word != "-") {
			transport += word;
			ss >> word;
		}
		lt->Ajouter(
				new TrajetSimple(depart.c_str(), arrivee.c_str(),
						transport.c_str()));

		depart = arrivee; //copie en profondeur

	}

	catalogue.Ajouter(new TrajetCompose(lt));

}

static void readFile(ListeTrajets & catalogue, string choice, ifstream & is) {
	string ligne;
	string word;
	string type;
	string depart;
	string arrivee;
	string transport;

	int nb;
	//int nbTS;
	//int nbTC;
	if (choice == "all") {
		getline(is, ligne);
		//is.getline(ligne); sur une seule ligne now
		//on skip les deux lignes de metadata
		getline(is, ligne);
		while (ligne != "fin") {
			getline(is, ligne);
			stringstream ss(ligne);
			ss >> type;
			if (type == "TS") {
				readTS(catalogue, ss);
			} else if (type == "TC") {
				readTC(catalogue, ss);
			}

		}

	} else if (choice == "TS") {
		getline(is, ligne);
		stringstream ss(ligne);
		ss >> nb;
		if (nb != 0) {
			getline(is, ligne); //prepare while

			while (ligne != "fin") {
				getline(is, ligne);
				stringstream ss(ligne);
				ss >> type;
				if (type == "TS") {
					readTS(catalogue, ss);
				}

			}
		}

	}
	if (choice == "TC") {
		getline(is, ligne);
		//getline(is,ligne);
		stringstream ss(ligne);
		ss >> nb;
		ss >> nb;
		if (nb != 0) {

			while (ligne != "fin") {
				getline(is, ligne);
				stringstream ss(ligne);
				ss >> type;
				if (type == "TC") {
					readTC(catalogue, ss);
				}

			}
		}

	}
	if (choice == "ville") {
		//ask
		char constraint;
		cout
				<< "Souhaitez-vous imposer une ville de d�part (D), d'arriv�e (A) ou les deux (B) ?"
				<< endl;
		cout << "Veuillez saisir la lettre correspondant � votre choix" << endl;
		cin >> constraint;

		string wanted_dep = "";
		string wanted_arr = "";

		switch (constraint) {
		case 'D' /*or 'd'*/: {
			cout
					<< "Quelle doit �tre la ville de d�part des trajets � s�lectionner ?"
					<< endl;
			cin >> wanted_dep;
			break;
		}
		case 'A' /*or 'a'*/: {
			cout
					<< "Quelle doit �tre la ville d'arriv�e des trajets � s�lectionner ?"
					<< endl;
			cin >> wanted_arr;
			break;
		}
		case 'B' /*or 'b'*/: {
			cout
					<< "Quelle doit �tre la ville de d�part des trajets � s�lectionner ?"
					<< endl;
			cin >> wanted_dep;
			cout
					<< "Quelle doit �tre la ville d'arriv�e des trajets � s�lectionner ?"
					<< endl;
			cin >> wanted_arr;
			break;
		}
		}
		getline(is, ligne);
		while (ligne != "fin") {

			stringstream ss(ligne);
			ss >> type;
			ss >> depart;
			ss >> word;
			while (word != ";") {
				depart += word;
				ss >> word;
			}
			if (wanted_dep == ""
					|| (wanted_dep != "" && depart == wanted_dep)) {

				ss >> arrivee;
				ss >> word; //read next for while
				while (word != ";") {
					arrivee += word;
					ss >> word;
				}
				if (wanted_arr == ""
						|| (wanted_arr != "" && arrivee != wanted_arr)) {
					//if reached here, means we're in the right conditions

					if (type == "TS") {
						ss >> transport;
						ss >> word;
						while (word != ";") {
							transport += word;
							ss >> word;
						}
						catalogue.Ajouter(
								new TrajetSimple(depart.c_str(),
										arrivee.c_str(), transport.c_str()));

					} else if (type == "TC") {
						ListeTrajets * lt;
						while (word != "-") {
							ss >> arrivee;
							ss >> word;
							while (word != ";") {
								arrivee += word;
								ss >> word;
							}
							ss >> transport;
							ss >> word;
							while (word != "" && word != ";" && word != "-") {
								transport += word;
								ss >> word;
							}
							lt->Ajouter(
									new TrajetSimple(depart.c_str(),
											arrivee.c_str(),
											transport.c_str()));

							depart = arrivee; //copie en profondeur

						}

						catalogue.Ajouter(new TrajetCompose(lt));

					}
					getline(is, ligne);
				}
			}
		}

	}
	if (choice == "intervalle") {
		//ask for interval
		int m;
		int n;
		cout
				<< "Quel est l'indice du premier trajet à récupérer (les indices commencent à 1)?"
				<< endl;
		cin >> m;
		cout << "Quel est l'indice du dernier trajet à récupérer ?" << endl;
		cin >> n;
		getline(is, ligne);
		stringstream ss(ligne);
		int nbT;
		int a;
		ss >> nbT;
		ss >> a;
		nbT += a;
		while (n < 0 || m < 0 || n < m || n > nbT) {
			cout
					<< "Le format des indices donnés ne permet pas de constiruer un intervalle valable"
					<< endl;
			cout << "Veuillez resaissir les indices" << endl;
			cout
					<< "Quel est l'indice du premier trajet à récupérer (les indices commencent à 1)?"
					<< endl;
			cin >> m;
			cout << "Quel est l'indice du dernier trajet à récupérer ?" << endl;
			cin >> n;
		}
		if (n > nbT) {
			cout
					<< "L'indice de fin est supérieur au nombre total de trajets contenu dans le fichier."
					<< endl;
			cout << "Souhaitez-vous " << endl;
			cout << "1.importer les trajets de l'indice de départ au dernier ?"
					<< endl;
			cout << "2.renoncer ?" << endl;
			int next;
			cin >> next;
			if (next == 2)
				return;
		}
		for (int i = 1; i < m; i++) {
			getline(is, ligne);
			stringstream ss(ligne);
		}
		for (int i = m; i <= n && i <= nbT; i++) {
			getline(is, ligne);
			stringstream ss(ligne);
			ss >> type;
			if (type == "TS") {
				readTS(catalogue, ss);
			} else {
				readTC(catalogue, ss);
			}
		}

	}

}

static void load(ListeTrajets & catalogue) {
	ifstream is;

	cout
			<< "Veuillez saisir le nom du fichier à charger ou \"exit\" pour sortir: "
			<< endl;
	string filename;
	cin >> filename;
	if (filename == "exit")
		return;
//is = new ifstream(filename);
//check if exists
	is.open(filename);
	while (!is.is_open()) {
		//demander si souhaite ou mettre une option de sortie
		cout
				<< "Le fichier n'a pas pu être ouvert, veuillez saisir à nouveau le nom du fichier à charger (ou \"exit\" pour sortir): "
				<< endl;
		cin >> filename;
		if (filename == "exit")
			return;
		is.open(filename);
	}
	cout << "Veuillez choisir une option: " << endl;
	cout << "1. Charger tous les trajets " << endl;
	cout << "2. Charger uniquement les Trajets Simples " << endl;
	cout << "3. Charger uniquement les Trajets Composes " << endl;
	cout << "4. Charger un trajet en fonction du départ et/ou de l'arrivée "
			<< endl;
	cout << "5. Charger seulement une sélection de trajets " << endl;
	cout << "6. Sortir" << endl;

//demander si on veut append ou escraser
	int action;
	cin >> action;

	while (action != 6) {

		switch (action) {
		case 1: {
			//load infos from file
			readFile(catalogue, "all", is);
			break;
		}
		case 2: {
			//load infos from file
			readFile(catalogue, "TS", is);
			break;
		}
		case 3: {
			//load infos from file
			readFile(catalogue, "TC", is);
			break;
		}
		case 4: {
			//load infos from file
			readFile(catalogue, "ville", is);
			break;
		}
		case 5: {
			//load infos from file
			readFile(catalogue, "intervalle", is);
			break;
		}

		}
		cout << "Veuillez choisir une option: " << endl;
		cout << "1. Charger tous les trajets " << endl;
		cout << "2. Charger uniquement les Trajets Simples " << endl;
		cout << "3. Charger uniquement les Trajets Composes " << endl;
		cout << "4. Charger un trajet en fonction du départ et/ou de l'arrivée "
				<< endl;
		cout << "5. Charger seulement une sélection de trajets " << endl;
		cout << "6. Sortir" << endl;

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
					<< transport << " - " << "\n";

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
			os << " -" << "\n";
		}
	}

}

static void readTSCatalogue(ListeTrajets & catalogue, ofstream &os, int &nbTS) {

	for (unsigned int i = 0; i < catalogue.Taille(); i++) {

		if (catalogue.getTrajet(i)->Type().compare("TS") == 0) {

			nbTS++;
			string type = catalogue.getTrajet(i)->Type();
			const char * depart = catalogue.getTrajet(i)->Depart();
			const char * arrivee = catalogue.getTrajet(i)->Arrivee();
			const char * transport = catalogue.getTrajet(i)->Transport();

			os << type << " " << depart << " ; " << arrivee << " ; "
					<< transport << " - " << "\n";

		}
	}
}

static void readTCCatalogue(ListeTrajets & catalogue, ofstream &os, int &nbTC) {

	for (unsigned int i = 0; i < catalogue.Taille(); i++) {

		if (catalogue.getTrajet(i)->Type().compare("TC") == 0) {

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
			os << " - " << "\n";
		}
	}
}

static void readVilleCatalogue(ListeTrajets & catalogue, string dep, string arr,
		ofstream &os, int &nbTS, int &nbTC) {

	if (dep.compare("none") != 0 && arr.compare("none") != 0) {
		for (unsigned int i = 0; i < catalogue.Taille(); i++) {
			string type = catalogue.getTrajet(i)->Type();
			const char * depart = catalogue.getTrajet(i)->Depart();
			const char * arrivee = catalogue.getTrajet(i)->Arrivee();

			if (dep.compare(depart) == 0 && arr.compare(arrivee) == 0) {
				if (catalogue.getTrajet(i)->Type().compare("TS") == 0) {
					nbTS++;
					const char * transport =
							catalogue.getTrajet(i)->Transport();
					os << type << " " << depart << " ; " << arrivee << " ; "
							<< transport << " - " << "\n";
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
						os << actuel->Depart() << " ; " << actuel->Arrivee()
								<< " ; " << actuel->Transport();
						if (j != trajets->Taille() - 1)
							os << " ; ";
						if (actuelle->suivant != NULL)
							actuelle = actuelle->suivant;
					}
					os << " - " << "\n";

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
					nbTS++;
					const char * transport =
							catalogue.getTrajet(i)->Transport();
					os << type << " " << depart << " ; " << arrivee << " ; "
							<< transport << " - " << "\n";
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
						os << actuel->Depart() << " ; " << actuel->Arrivee()
								<< " ; " << actuel->Transport();
						if (j != trajets->Taille() - 1)
							os << " ; ";
						if (actuelle->suivant != NULL)
							actuelle = actuelle->suivant;
					}
					os << " - " << "\n";

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
					nbTS++;
					const char * transport =
							catalogue.getTrajet(i)->Transport();
					os << type << " " << depart << " ; " << arrivee << " ; "
							<< transport << " - " << "\n";
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
						os << actuel->Depart() << " ; " << actuel->Arrivee()
								<< " ; " << actuel->Transport();
						if (j != trajets->Taille() - 1)
							os << " ; ";
						if (actuelle->suivant != NULL)
							actuelle = actuelle->suivant;
					}
					os << " - " << "\n";

				}

			}

		}
	}
}

static void readIntervalleCatalogue(ListeTrajets & catalogue, unsigned int n,
		unsigned int m, ofstream &os, int &nbTS, int &nbTC) {

	if (n >= 1 && n <= m && m <= catalogue.Taille() + 1) {

		for (unsigned int i = n - 1; i < m; i++) {

			if (catalogue.getTrajet(i)->Type().compare("TS") == 0) {

				nbTS++;
				string type = catalogue.getTrajet(i)->Type();
				const char * depart = catalogue.getTrajet(i)->Depart();
				const char * arrivee = catalogue.getTrajet(i)->Arrivee();
				const char * transport = catalogue.getTrajet(i)->Transport();
				os << type << " " << depart << " ; " << arrivee << " ; "
						<< transport << " - " << "\n";

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
					os << actuel->Depart() << " ; " << actuel->Arrivee()
							<< " ; " << actuel->Transport();
					if (j != trajets->Taille() - 1)
						os << " ; ";
					if (actuelle->suivant != NULL)
						actuelle = actuelle->suivant;
				}
				os << " - " << "\n";
			}
		}
	}
}

static void testReadCatalogue(ListeTrajets * catalogue) {

//os.open("test.txt", std::ofstream::out | std::ofstream::app);
//	unsigned int nbTS = 0;
//	unsigned int nbTC = 0;
	ofstream os;

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

	bool choixFichier = true;

	while (choixFichier) {
		cin.clear();
		//demander le nom du fichier
		cout
				<< "Veuillez saisir le nom du fichier à charger ou \"exit\" pour sortir: "
				<< endl;
		string filename;
		cin >> filename;

		cout << filename << endl;
		if (filename == "exit" || filename == "exit ") {
			choixFichier = false;
			return;
			break;
		} else {
			//ouverture/creation d'un nouvel fichier avec le nom donné
			os.open(filename, std::ofstream::out | std::ofstream::app);
			os.seekp(0, ios_base::end);
			long taille = os.tellp();
			os.close();

			if (taille <= 0) {
				os.open(filename, std::ofstream::out);
				os << "                \n";
				choixFichier = false;
			} else {
				cout
						<< "Ce fichier existe déjà. Choisissez une des option suivantes: "
						<< endl;
				cout
						<< "1. Ecrire dans ce fichier, en ecrasant le contenu précédent"
						<< endl;

				cout << "2. Saisir le nom d'un nouvel fichier" << endl;
				cout << "3. Sortir" << endl;

				int option = 0;
				cin >> option;
				//cin.clear();
				//if (typeid(option).name
				if (option >= 1 && option <= 3) {
					switch (option) {
					case 1: {
						os.open(filename, std::ofstream::out);
						os << "                  \n";
						choixFichier = false;
						break;
					}
						int option = 0;
						cin >> option;

						if (option >= 1 && option <= 3) {
							switch (option) {
							case 1: {
								os.open(filename, std::ofstream::out);
								os << "                  \n";
								choixFichier = false;
								break;
							}

							case 2: {
								choixFichier = true;
								break;
							}
							case 3: {
								return;
								break;
							}
							}
						} else {
							cout << "Le numero saisi n'est pas valide." << endl;
							return;
						}
					}
				}
			}

			//choisir les options pour la sauvegar

			cout << "Veuillez choisir une option: " << endl;
			cout << "1. Sauvegarder tous les trajets " << endl;
			cout << "2. Sauvegarder uniquement les Trajets Simples " << endl;
			cout << "3. Sauvegarder uniquement les Trajets Composes " << endl;
			cout
					<< "4. Sauvegarder un trajet en fonction du départ et/ou de l'arrivée "
					<< endl;

		}
		cout << "5. Sauvegarder seulement une sélection de trajets " << endl;
		cout << "6. Sortir" << endl;
		cout << "5. Sauvegarder seulement une sélection de trajets " << endl;
		cout << "6. Sortir" << endl;
	}
}
//
//
//			switch (action) {
//				case 1: {
//					//load infos from file
//					load(catalogue);
//					action = 0;
//					readAllCatalogue(catalogue, os, nbTS, nbTC);
//					os << "fin";
//					os.seekp(0);
//					os << nbTS << " " << nbTC;
//					os.close();
//					readAllCatalogue(catalogue, os, nbTS, nbTC);
//					os << "fin";
//					os.seekp(0);
//					os << nbTS << " " << nbTC;
//					os.close();
//					readAllCatalogue(catalogue, os, nbTS, nbTC);
//					os << "fin";
//					os.seekp(0);
//					os << nbTS << " " << nbTC;
//					os.close();
//					break;
//				}
//				case 2: {
//
//					char depart[1024];
//					char arrivee[1024];
//					char transport[1024];
//
//					cout << "Ville de depart ?" << endl;
//					cin >> depart;
//					cout << "Ville d'arrivee ?" << endl;
//					cin >> arrivee;
//					cout << "Moyen de transport ?" << endl;
//					cin >> transport;
//
//					catalogue.Ajouter(new TrajetSimple(depart, arrivee, transport));
//					action = 0;
//					readTSCatalogue(catalogue, os, nbTS);
//					os << "fin";
//					os.seekp(0);
//					os << nbTS << " " << nbTC;
//					os.close();
//					break;
//				}
//				case 3: {
//
//					ListeTrajets * lt = new ListeTrajets;
//					int i = 0;
//					string reponse = "oui";
//
//					char arriveePrecedent[1024];
//					strcpy(arriveePrecedent, "no dest");
//
//					while (reponse.compare("oui") == 0) {
//						cout << "Saisie du trajet num " << i + 1 << endl;
//						char depart[1024];
//						char arrivee[1024];
//						char transport[1024];
//
//						if (i == 0) {
//							cout << "Ville de depart ?" << endl;
//							cin >> depart;
//						} else
//						strcpy(depart, arriveePrecedent);
//
//						cout << "Ville d'arrivee ?" << endl;
//						cin >> arrivee;
//						strcpy(arriveePrecedent, arrivee);
//						cout << "Moyen de transport ?" << endl;
//						cin >> transport;
//						lt->Ajouter(new TrajetSimple(depart, arrivee, transport));
//						cout << "Voulez-vous ajouter un autre trajet simple ? (oui / non)"
//						<< endl;
//						cin >> reponse;
//						i++;
//					}
//					catalogue.Ajouter(new TrajetCompose(lt));
//					action = 0;
//					readTCCatalogue(catalogue, os, nbTC);
//					os << "fin";
//					os.seekp(0);
//					os << nbTS << " " << nbTC;
//					os.close();
//					break;
//				}
//				case 4: {
//					string villeDepart;
//					string villeArrivee;
//					cout << "Choisissez une ville de départ ou \"none\" : ";
//					cin >> villeDepart;
//					cout << "Choisissez une ville d'arrivee ou \"none\" : ";
//					cin >> villeArrivee;
//
//					//RechercheSimple(catalogue, depart, arrivee);
//					RechercheAvancee(catalogue, depart, arrivee);
//					action = 0;
//					break;
//				}
//				case 6: {
//					//sauvegarder
//					save(catalogue);
//					action = 0;
//					readVilleCatalogue(catalogue, villeDepart, villeArrivee, os, nbTS,
//							nbTC);
//					os << "fin";
//					os.seekp(0);
//					os << nbTS << " " << nbTC;
//					os.close();
//					break;
//				}
//				case 7: {
//					//stopper l'exécution
//					return;
//					break;
//					case 5: {
//						bool choixIndices = true;
//						unsigned int n;
//						unsigned int m;
//						while (choixIndices) {
//							cout << "Indice du premier trajet: " << endl;
//							cin >> n;
//							cout << "Indice du dernier trajet: " << endl;
//							cin >> m;
//							choixIndices = false;
//							if (n < 1 || n > m || m > catalogue.Taille() + 1) {
//								cout << "Les indices saisies sont invalides." << endl;
//								cout << "1 . Choisir autres indices" << endl;
//								cout << "2 . Sortir" << endl;
//								int choix;
//								cin >> choix;
//								if (choix == 1)
//								choixIndices = true;
//								else return;
//							}
//						}

static void Menu(ListeTrajets & catalogue)
// Algorithme :
		{
	cout << "Veuillez choisir une option: " << endl;
	cout << "1. Charger des trajets depuis un fichier " << endl;
	cout << "2. Ajouter Trajet Simple " << endl;
	cout << "3. Ajouter Trajet Compose " << endl;
	cout << "4. Afficher le catalogue des trajets " << endl;
	cout << "5. Rechercher un parcours " << endl;
	cout << "6. Sauvegarder des trajets dans un fichier" << endl;
	cout << "7. Sortir" << endl;
	int action;
	cin >> action;

	while (action >= 1 && (action <= 7)) {
		switch (action) {

		case 1: {
			//load infos from file
			//load(catalogue);
			action = 0;
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
			cout << "Moyen de transport ?" << endl;
			cin >> transport;

			catalogue.Ajouter(new TrajetSimple(depart, arrivee, transport));
			action = 0;
			break;
		}

			//TC
		case 3: {

			ListeTrajets * lt = new ListeTrajets;
			int i = 0;
			string reponse = "oui";

			char arriveePrecedent[1024];
			strcpy(arriveePrecedent, "no dest");

			while (reponse.compare("oui") == 0) {
				cout << "Saisie du trajet num " << i + 1 << endl;
				char depart[1024];
				char arrivee[1024];
				char transport[1024];

				if (i == 0) {
					cout << "Ville de depart ?" << endl;
					cin >> depart;
				} else
					strcpy(depart, arriveePrecedent);

				cout << "Ville d'arrivee ?" << endl;
				cin >> arrivee;
				strcpy(arriveePrecedent, arrivee);
				cout << "Moyen de transport ?" << endl;
				cin >> transport;
				lt->Ajouter(new TrajetSimple(depart, arrivee, transport));
				cout
						<< "Voulez-vous ajouter un autre trajet simple ? (oui / non)"
						<< endl;
				cin >> reponse;
				i++;
			}
			catalogue.Ajouter(new TrajetCompose(lt));
			action = 0;
			break;
		}

			//catalogue
		case 4: {
			catalogue.Afficher();
			action = 0;
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
			action = 0;
			break;
		}
		case 6: {
			//sauvegarder
			save(catalogue);
			action = 0;
			break;
		}
		case 7: {
			//stopper l'exécution
			return;
			break;
		}
		}
		cout << "Veuillez choisir une option: " << endl;
		cout << "1. Charger des trajets depuis un fichier " << endl;
		cout << "2. Ajouter Trajet Simple " << endl;
		cout << "3. Ajouter Trajet Compose " << endl;
		cout << "4. Afficher le catalogue des trajets " << endl;
		cout << "5. Rechercher un parcours " << endl;
		cout << "6. Sauvegarder des trajets dans un fichier" << endl;
		cout << "7. Sortir" << endl;
		cin >> action;
	}

} //----- Fin de Menu
//	} else return;

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

static void testReadTS() {
	cout << "test readts" << endl;
	ListeTrajets * c = new ListeTrajets;
	stringstream ss;
	cout << "fill ss" << endl;
	ss << "Lyon ; Marseille ; train" << endl;
	string s;
//ss >> s;
//cout << s << endl;
	readTS(*c, ss);
	c->Afficher();
}

static void testReadTC() {
	cout << "test readtc" << endl;
	ListeTrajets * c = new ListeTrajets;
	stringstream ss;
	cout << "fill ss" << endl;
	ss
			<< "Lyon ; Marseille ; Monaco ; avion ; Berlin ; bateau ; Milan ; charrette ; Marseille ; licorne -"
			<< endl;
	string s;
//ss >> s;
//cout << s << endl;
	readTC(*c, ss);
	c->Afficher();
}
int main()
// Algorithme :
{
	ListeTrajets * catalogue = new ListeTrajets;

	load(*catalogue);
//testReadTC();
//testReadCatalogue(catalogue);
//Menu(*catalogue);
	testReadCatalogue(catalogue);
	Menu(*catalogue);

//save(*catalogue);
//testFile();
	delete catalogue;
	return 0;
} //----- Fin de main
