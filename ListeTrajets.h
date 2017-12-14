/*************************************************************************
 ListeTrajets  -  description
 -------------------
 début                : 18 oct. 2017
 copyright            : (C) $YEAR$ par Caraiman and Gallé
 e-mail               : ocaraiman
 *************************************************************************/

//---------- Interface de la classe <ListeTrajets> (fichier Xxx.h) ----------------
#if ! defined ( ListeTrajets_H )
#define ListeTrajets_H
#include "Trajet.h"
#include "TrajetSimple.h"

//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//structure représentant un élément de la liste
// valeur : contient le trajet inseré dans la liste
// suivant : pointeur vers l'élément suivant de la liste, null si c'est le dernier élément

struct CelluleTrajet {
	Trajet * valeur;
	CelluleTrajet *suivant;
};

//------------------------------------------------------------------------
// Rôle de la classe <ListeTrajets>
// représente la structure de données utilisée pour stocker les trajet
// contient des méthodes permettant l'ajout, la suppression des trajets
//
//------------------------------------------------------------------------

class ListeTrajets {
//----------------------------------------------------------------- PUBLIC

public:
//----------------------------------------------------- Méthodes publiques
	// type Méthode ( liste des paramètres );
	// Mode d'emploi :
	//
	// Contrat :
	//

	ListeTrajets();
	// Mode d'emploi :
	// crée la tete de la liste et l'initialise (le dernier élement pointe vers la tete)
	// Contrat :
	//

	ListeTrajets(const ListeTrajets &uneListe);

	virtual void Afficher() const;
	// Mode d'emploi :
	// affiche le contenu de la liste
	// Contrat :
	//
	//
	virtual void Ajouter(Trajet * val);
	// Mode d'emploi :
	// param : trajet qu'on veut ajouter
	// ajoute un trajet à la liste

	CelluleTrajet * Tete() const;
	// Mode d'emploi :
	//retourne un pointeur vers le premier element de la liste
	// Contrat :
	//

	CelluleTrajet * Queue() const;
	// Mode d'emploi :
	//retourne un pointeur vers le dernier element de la liste
	// Contrat :
	//
	//

	unsigned int Taille();
	// Mode d'emploi :
	// retourne la taille actuelle de la liste
	// Contrat :
	//
	//

	Trajet * getTrajet(unsigned int i);
	// Mode d'emploi :
	// param i : l'indice du trajet auquel on veut accéder
	// retourne le trajet qui se trouve à l'indice i de la liste
	// Contrat :
	//
	//

	void Retirer(unsigned int i);
	// Mode d'emploi :
	// param i : l'indice du trajet pour lequel on veut avoir le nom
	// rétire le trajet se trouvant à la position i dans la liste
	// renvoie 0 si le trajet d'indice i est retiré
	// Contrat :
	//
	//

	virtual ~ListeTrajets();
	// Mode d'emploi :
	// détruit tous les éléments de la liste
	// Contrat :
	//

//------------------------------------------------------------------ PRIVE

protected:
//----------------------------------------------------- Méthodes protégées

//----------------------------------------------------- Attributs protégés

	unsigned int taille; //taille de la liste
	CelluleTrajet *tete; //pointeur vers le premier élément de la liste
	CelluleTrajet * dernierElement; //pointeur vers le dernier element de la liste

};

//-------------------------------- Autres définitions dépendantes de <Xxx>

#endif // XXX_H
