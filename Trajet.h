/*************************************************************************
 Trajet  -  description
 -------------------
 début                : 18 oct. 2017
 copyright            : (C) $YEAR$ par Caraiman and Gallé
 e-mail               : ocaraiman
 *************************************************************************/

//---------- Interface de la classe <Trajet> (fichier Xxx.h) ----------------
#if ! defined ( Trajet_H )
#define Trajet_H
//using namespace std::string;
#include <string>

//--------------------------------------------------- Interfaces utilisées
//#include "ListeTrajets.h"
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//------------------------------------------------------------------------
// Rôle de la classe <Trajet>
// classe mère de TrajetSimple et TrajetCompose
// définit le comportement général des ses classes filles
//------------------------------------------------------------------------

class Trajet {
//----------------------------------------------------------------- PUBLIC

public:
//----------------------------------------------------- Méthodes publiques

	Trajet();
	// Mode d'emploi :
	//
	// Contrat :
	//

	Trajet(Trajet * t);
	// Mode d'emploi :
	//
	// Contrat :
	//

	virtual void Afficher() const;
	// Mode d'emploi :
	//
	// Contrat :
	//
	//

	virtual const char * Depart() const;
	// Mode d'emploi :
	// méthode qui retourne la ville de départ
	// Contrat :
	//
	//

	virtual const char * Arrivee() const;
	// Mode d'emploi :
	// méthode qui retourne la ville d'arrivée
	// Contrat :
	//
	//

	virtual const char * Transport() const;
	// Mode d'emploi :
	// méthode qui retourne le moyen de transport
	// Contrat :
	//
	//
	virtual string Type() const;

//	virtual ListeTrajets * Trajets() const;

	virtual ~Trajet();
	// Mode d'emploi :
	//
	// Contrat :
	//

//------------------------------------------------------------------ PRIVE

protected:
	const char * villeDepart;
	const char * villeArrivee;
	const char * transport;
	//ListeTrajets * trajets;

//----------------------------------------------------- Méthodes protégées

//----------------------------------------------------- Attributs protégés

};

//-------------------------------- Autres définitions dépendantes de <Trajet>

#endif // Trajet_H
