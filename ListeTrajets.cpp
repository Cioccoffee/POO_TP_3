/*************************************************************************
 ListeTrajets  -  description
 -------------------
 début                : $DATE$
 copyright            : (C) $YEAR$ par $AUTHOR$
 e-mail               : $EMAIL$
 *************************************************************************/

//---------- Réalisation de la classe <ListeTrajets> (fichier ListeTrajets.cpp) ------------
//---------------------------------------------------------------- INCLUDE
//-------------------------------------------------------- Include système
using namespace std;
#include <iostream>

//------------------------------------------------------ Include personnel
#include "ListeTrajets.h"

//------------------------------------------------------------- Constantes

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
//

void ListeTrajets::Afficher() const
// Algorithme :
// parcours de chaque élément de la liste et son affichage
{
	if (taille == 0) {
		cout << "Liste vide" << endl;
	} else {
		CelluleTrajet *elCourant = tete;
		while (elCourant->suivant != NULL) {
			(elCourant->valeur)->Afficher();
			elCourant = elCourant->suivant;
		}

		//Dernier element dont le suivant est null
		(elCourant->valeur)->Afficher();

	}

} //----- Fin de Méthode

void ListeTrajets::Ajouter(Trajet *val) {
//	Trajet * trajet = new Trajet();
//	trajet = val;

	if (taille == 0) {

		//tete->valeur=new Trajet;
		tete->valeur = val;

		//////////
		tete->suivant = NULL;
		dernierElement = tete;
		///////

	} else {
		CelluleTrajet *nouvelElement = new CelluleTrajet;
		//nouvelElement->valeur = new Trajet;
		nouvelElement->valeur = val;
		nouvelElement->suivant = NULL;

		dernierElement->suivant = nouvelElement;
		dernierElement = nouvelElement;

	}

	//if(tete->suivant == dernierElement) cout<<"pizda masii"<<endl;

	taille++;

} //----- Fin de Ajouter

CelluleTrajet * ListeTrajets::Tete() const {
	return tete;
} //----- Fin de Tete

CelluleTrajet * ListeTrajets::Queue() const {
	return dernierElement;
} //----- Fin de Queue

unsigned int ListeTrajets::Taille() {
	return taille;
} //----- Fin de Taille

Trajet * ListeTrajets::getTrajet(unsigned int i) {
	unsigned int j = 0;

	if (taille == 0 || i > taille) {
		return NULL;
	} else {
		CelluleTrajet *elCourant = tete;
		while ((j < i) && (elCourant->suivant != NULL)) {
			//(elCourant->valeur)->Afficher();
			elCourant = elCourant->suivant;
			j++;
		}

		//Dernier element dont le suivant est null
		return elCourant->valeur;
	}
}

void ListeTrajets::Retirer(unsigned int i)
// Algorithme :
		{

	//condition existence du trajet
	if (taille == 0 || i > taille) {
	}

	else if (i == 0) {
		if (taille != 1) {
			tete = tete->suivant;
		} else {
			tete->valeur = NULL;
		}

	} else {
		CelluleTrajet * precedent = tete;
		for (unsigned int j = 0; (j < i - 1) && (precedent->suivant != NULL);
				j++) {
			//if (precedent->suivant != NULL)
			precedent = precedent->suivant;
		}

		CelluleTrajet * aRetirer;

		if (precedent->suivant != NULL) {
			aRetirer = precedent->suivant;

			if (aRetirer != dernierElement) {
				precedent->suivant = aRetirer->suivant;
			} else if (i == (taille - 1)) {
				dernierElement = precedent;
				precedent->suivant = NULL; //nouvelle queue
			}

		}
	}
	taille--;

} //----- Fin de Retirer

ListeTrajets::ListeTrajets()
// Algorithme :
{
#ifdef MAP
	//cout << "Appel au constructeur de <ListeTrajets>" << endl;
#endif

	tete = new CelluleTrajet;
	tete->valeur = NULL;
	tete->suivant = dernierElement;

	taille = 0;

} //----- Fin de ListeTrajets

ListeTrajets::ListeTrajets(const ListeTrajets &uneListe) {
	//ListeTrajets * resultat = new ListeTrajets;
	taille=0;

	if (uneListe.taille > 0) {

	tete = new CelluleTrajet;
		tete->valeur = NULL;
		tete->suivant = dernierElement;
		//taille = 0;

		//tete = uneListe.Tete();

		CelluleTrajet * courant;
		CelluleTrajet * courantACopier = uneListe.tete;

		while (taille < uneListe.taille) {
			this->Ajouter(courantACopier->valeur);
			taille++;
		}


		/*if (uneListe.taille == 1) {
			tete = new CelluleTrajet;
			tete->valeur = new Trajet;
			tete->valeur = courantACopier->valeur;
			tete->suivant = NULL;
			dernierElement = tete;
			taille++;
		} else {
			while (courantACopier->suivant != NULL) {
				if (taille == 0) {
					tete = new CelluleTrajet;
					tete->valeur = new Trajet;
					tete->valeur = courantACopier->valeur;
					tete->suivant = courantACopier->suivant;
					taille++;
				} else {
					courant = new CelluleTrajet;
					courant->valeur = new Trajet;

					courant->valeur = courantACopier->valeur;
					courant->suivant = courantACopier->suivant;
					taille++;
				}
				//courant = courant->suivant;
				courantACopier = courantACopier->suivant;

			}

			dernierElement = new CelluleTrajet;
			dernierElement->valeur = courantACopier->valeur;
			dernierElement->suivant = NULL;

			//dernierElement->suivant->Afficher();
			taille++;
		}
		*/

	}

}

ListeTrajets::~ListeTrajets()
// Algorithme :
{
#ifdef MAP
	//cout << "Appel au destructeur de <ListeTrajets>" << endl;
#endif

//	dernierElement->valeur->Afficher();
//	tete->valeur->Afficher();
	CelluleTrajet *elSupprime = tete;
	CelluleTrajet *elSupprimeSuiv = elSupprime;

	while (elSupprime->suivant != NULL) {
		elSupprimeSuiv = elSupprime->suivant;

		delete elSupprime->valeur;
		delete elSupprime;
		elSupprime = elSupprimeSuiv;

	}
	delete elSupprime->valeur;
	delete elSupprime;

} //----- Fin de ~ListeTrajets

//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées

