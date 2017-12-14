/*************************************************************************
Trajet  -  description
-------------------
d�but                : $DATE$
copyright            : (C) $YEAR$ par $AUTHOR$
e-mail               : $EMAIL$
*************************************************************************/

//---------- Realisation de la classe <Trajet> (fichier Trajet.cpp) ------------
//---------------------------------------------------------------- INCLUDE
//-------------------------------------------------------- Include syst�me
using namespace std;
#include <iostream>
#include <cstring>

//------------------------------------------------------ Include personnel
#include "TrajetCompose.h"


//------------------------------------------------------------- Constantes

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Methodes publiques
//

void TrajetCompose::Afficher() const
// Algorithme :
{
	unsigned int i = 0;
	CelluleTrajet * actuelle = trajets->Tete();

	Trajet * actuel = actuelle->valeur;

	for (i = 0; i < trajets->Taille(); i++)
	{
		actuel = actuelle->valeur;

		//Affichage trajet

		if(i!=0) cout << " - ";
		actuel->Afficher();
		if (actuelle->suivant != NULL) actuelle = actuelle->suivant;
	}

} //----- Fin de Afficher

const char * TrajetCompose::Depart() const
// Algorithme :
//
{
	CelluleTrajet * teteTrajets = trajets->Tete();
	Trajet * trajetEnTete = teteTrajets->valeur;
	return trajetEnTete->Depart();
} //----- Fin de Depart

const char * TrajetCompose::Arrivee() const
// Algorithme :
//
{
	CelluleTrajet * queueTrajets = trajets->Queue();
	Trajet * trajetEnQueue = queueTrajets->valeur;
	return trajetEnQueue->Arrivee(); //choisi la méthode adaptée au type (TS ou TC)
} //----- Fin de Arrivee


ListeTrajets * TrajetCompose::Trajets()  const
// Algorithme :
//
{
	return trajets;
} //----- Fin de Trajets

  //------------------------------------------------- Surcharge d'operateurs
  
  //-------------------------------------------- Constructeurs - destructeur

string TrajetCompose:: Type() const
{
	return "TC";
}


TrajetCompose::TrajetCompose()
// Algorithme :
{
#ifdef MAP
	//cout << "Appel au constructeur de <Trajet>" << endl;
#endif
} //----- Fin de Trajet

TrajetCompose::TrajetCompose(const TrajetCompose & t)
// Algorithme :
{
#ifdef MAP
	//cout << "Appel au constructeur de <Trajet>" << endl;
#endif
	trajets = t.trajets;
} //----- Fin de Trajet

TrajetCompose::TrajetCompose(ListeTrajets * lt)
// Algorithme :
{
	trajets =lt;
#ifdef MAP
	//cout << "Appel au constructeur de <Trajet>" << endl;
#endif
} //----- Fin de Trajet

TrajetCompose::~TrajetCompose()
// Algorithme :
{
	delete trajets;
#ifdef MAP
	//cout << "Appel au destructeur de <Trajet>" << endl;
#endif

} //----- Fin de ~Trajet

  //------------------------------------------------------------------ PRIVE

  //----------------------------------------------------- Methodes protegeees


