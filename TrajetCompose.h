/*************************************************************************
 TrajetCompose  -  description
 -------------------
 début                : 18 oct. 2017
 copyright            : (C) $YEAR$ par Caraiman and Gallé
 e-mail               : ocaraiman
 *************************************************************************/

//---------- Interface de la classe <TrajetCompose> (fichier Xxx.h) ----------------
#if ! defined ( TrajetCompose_H )
#define TrajetCompose_H

//--------------------------------------------------- Interfaces utilisées
#include "Trajet.h"
#include "ListeTrajets.h" 
#include <cstring>

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//TYPEDEF IF NECESSARY

//------------------------------------------------------------------------
// Rôle de la classe <TrajetCompose>
// classe descendante de la classe Trajet
// sert à la construction des trajets composés de plusieurs trajets simples
//
//------------------------------------------------------------------------

class TrajetCompose: public Trajet {
//----------------------------------------------------------------- PUBLIC

public:
//----------------------------------------------------- Méthodes publiques

	TrajetCompose();
	// Mode d'emploi :
	//crée un TrajetCompose vide, dont la liste n'est pas initialisée
	// Contrat :
	//

	TrajetCompose(const TrajetCompose & t);
	// Mode d'emploi :
	// constructeur de copie
	// param : référence d'un objet de type TrajetCompose
	// Contrat :
	//

	TrajetCompose(ListeTrajets * lt);
	// Mode d'emploi :
	// constructeur qui copie la liste en parametre dans la liste en attribut
	// param : pointeur vers un objet de type ListeTrajets contenant des trajets
	// Contrat :
	//

	virtual void Afficher() const;
	// Mode d'emploi :
	// affichage du trajet composé avec les villes de départ et d'arrivée associées
	// Contrat :
	//
	//

	const char * Depart() const;
	// Mode d'emploi :
	// méthode qui retourne la ville de départ
	// Contrat :
	//
	///

	const char * Arrivee() const;
	// Mode d'emploi :
	// méthode qui retourne la ville d'arrivée
	// Contrat :
	//
	//

	ListeTrajets * Trajets() const;
	// Mode d'emploi :
	// méthode qui retourne le moyen de transport
	// Contrat :
	//
	//

	String Type() const;

	virtual ~TrajetCompose();
	// Mode d'emploi :
	//
	// Contrat :
	//

//------------------------------------------------------------------ PRIVE

protected:
//----------------------------------------------------- Méthodes protégées

//----------------------------------------------------- Attributs protégés

	ListeTrajets * trajets; //liste des trajets simples ou composées

};

//-------------------------------- Autres définitions dépendantes de <Xxx>

#endif // TrajetCompose_H
