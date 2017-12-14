/*************************************************************************
 TrajetSimple  -  description
 -------------------
 début                : 18 oct. 2017
 copyright            : (C) $YEAR$ par Caraiman and Gallé
 e-mail               : ocaraiman
 *************************************************************************/

//---------- Interface de la classe <TrajetSimple> (fichier Trajet.h) ----------------
#if ! defined ( TrajetSimple_H )
#define TrajetSimple_H

//--------------------------------------------------- Interfaces utilisées
#include "Trajet.h"
#include <string>

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types


//------------------------------------------------------------------------
// Rôle de la classe <TrajetSimple>
// classe qui sert à la construction des trajets simples, avec une ville de départ, une ville d'arrivée et un moyen de transport
// classe déscendante de la classe Trajet
//------------------------------------------------------------------------

class TrajetSimple : public Trajet
{
//----------------------------------------------------------------- PUBLIC

public:
//----------------------------------------------------- Méthodes publiques
	// type Méthode ( liste des paramètres );
	// Mode d'emploi :
	//
	// Contrat :
	//

	TrajetSimple(const char * depart, const char * arrivee, const char * transport);
	// Mode d'emploi :
	// constructeur de TrajetSimple : initialise les attributs de classe
	// param 1 : pointeur vers une chaine de caract constante représentant la ville de départ
	// param 2 : pointeur vers une chaine de caract constante représentant la ville d'arrivée
	// param 1 : pointeur vers une chaine de caract constante représentant le moyen de transport
	// Contrat :
	//les paramètres ont une taille maximale égale à 1024 à cause de l'ajout par le menu (dans Test.cpp)

	TrajetSimple(const TrajetSimple & t);
	// Mode d'emploi :
	// constructeur de copie
	// param : référence d'un objet de type TrajetSimple
	// Contrat :
	//

	virtual void Afficher() const;
	// Mode d'emploi :
	// methode pour l'affichage de la ville de départ, d'arrivée et du moyen de transport
	// Contrat :
	//

	const char * Depart() const;
	// Mode d'emploi :
	// méthode qui retourne la ville de départ
	// Contrat :
	//
	//

	const char * Arrivee() const;
	// Mode d'emploi :
	// méthode qui retourne la ville d'arrivée
	// Contrat :
	//
	//

	const char * Transport() const;
	// Mode d'emploi :
	// méthode qui retourne le moyen de transport
	// Contrat :
	//
	//

	string Type() const;

	virtual ~TrajetSimple();
	// Mode d'emploi :
	//
	// Contrat :
	//


//------------------------------------------------------------------ PRIVE

protected:
//----------------------------------------------------- Méthodes protégées

//----------------------------------------------------- Attributs protégés


	char * villeDepart;
	char * villeArrivee;
	char * moyenTransport;



};

//-------------------------------- Autres définitions dépendantes de <Xxx>

#endif // TrajetSimple_H
