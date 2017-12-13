/*************************************************************************
 TrajetSimple  -  description
 -------------------
 début                : $DATE$
 copyright            : (C) $YEAR$ par $AUTHOR$
 e-mail               : $EMAIL$
 *************************************************************************/

//---------- Réalisation de la classe <TrajetSimple> (fichier TrajetSimple.cpp) ------------
//---------------------------------------------------------------- INCLUDE
//-------------------------------------------------------- Include système
using namespace std;
#include <iostream>
#include <cstring>

//------------------------------------------------------ Include personnel
#include "TrajetSimple.h"

//------------------------------------------------------------- Constantes

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
//

void TrajetSimple::Afficher() const
// Algorithme :
{
	cout << "Trajet ";
	cout << "de " << villeDepart << " à " << villeArrivee << " en "<< moyenTransport << endl;
} //----- Fin de Méthode

const char * TrajetSimple::Depart() const
// Algorithme :
{
	return villeDepart;
} //----- Fin de Méthode

const char * TrajetSimple::Arrivee() const
// Algorithme :
{
	return villeArrivee;
} //----- Fin de Méthode

const char * TrajetSimple::Transport() const
// Algorithme :
//
{
	return moyenTransport;
} //----- Fin de Methode


String TrajetSimple:: Type() const
{
	return "TS";
}

//------------------------------------------------- Surcharge d'opérateurs

//-------------------------------------------- Constructeurs - destructeur

TrajetSimple::TrajetSimple(const char * depart, const char * arrivee,
		const char * transport)
// Algorithme :
		{
#ifdef MAP
	//cout << "Appel au constructeur de <TrajetSimple>" << endl;
#endif

	villeDepart = new char[strlen(depart) + 1];
	strcpy(villeDepart, depart);

	villeArrivee = new char[strlen(arrivee) + 1];
	strcpy(villeArrivee, arrivee);

	moyenTransport = new char[strlen(transport) + 1];
	strcpy(moyenTransport, transport);

} //----- Fin de TrajetSimple

TrajetSimple::TrajetSimple(const TrajetSimple & t)
// Algorithme :
		{
#ifdef MAP
	//cout << "Appel au constructeur de <TrajetSimple>" << endl;
#endif

	villeDepart = new char[strlen(t.villeDepart) + 1];
	strcpy(villeDepart, t.villeDepart);

	villeArrivee = new char[strlen(t.villeArrivee) + 1];
	strcpy(villeArrivee, t.villeArrivee);

	moyenTransport = new char[strlen(t.moyenTransport) + 1];
	strcpy(moyenTransport, t.moyenTransport);

} //----- Fin de TrajetSimple

TrajetSimple::~TrajetSimple()
// Algorithme :
{
	delete[] villeDepart;
	delete[] villeArrivee;
	delete[] moyenTransport;

#ifdef MAP
	//cout << "Appel au destructeur de <TrajetSimple>" << endl;
#endif

} //----- Fin de ~TrajetSimple

//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées

