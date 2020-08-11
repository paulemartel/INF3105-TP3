/*
 * TP3.cpp
 */

#include "DocumentXML.hpp"
#include "ElementXML.hpp"
#include "Graphe.hpp"
#include "Lecteur.hpp"

#include <cassert>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>

#include <climits>
#include <algorithm>

using namespace std;


Graphe *
construireGraphe( string a_nomFichier )
{
    Graphe * resultat = new Graphe();
    DocumentXML * carteXML = lireFichierXML( a_nomFichier );

    assert( nullptr != carteXML );
    ElementXML * courant = carteXML->racine();

    assert( nullptr != courant );
    courant = courant->prochainElement();

    assert( nullptr != courant );
    for( NoeudXML * contenu : * courant )
    {
        assert( nullptr != contenu );
        if( contenu->estElement() )
        {
            ElementXML * temp = (ElementXML *) contenu;

            assert( nullptr != resultat );
            assert( nullptr != temp );
            resultat->ajouterArcs(
                    stoi( temp->attribut( string( "inter_1" ) ) ),
                    stoi( temp->attribut( string( "inter_2" ) ) ),
                    stoi( temp->attribut( string( "longueur" ) ) ),
                    temp->attribut( string( "nom_rue" ) )
                    );
        }
    }
    return resultat;
}


vector< int > *
lireDestinations( istream & a_in )
{
    vector< int > * resultat = new vector< int >();
    string reponseUtilisateur;

    cout << "Entrez vos destinations s�par�es par des espaces : ";
    getline( cin, reponseUtilisateur );

    istringstream entrees( reponseUtilisateur );

    while( ! entrees.eof() )
    {
        int dest = 0;
        entrees >> dest;
        if( ! entrees.fail() ) {
            resultat->push_back( dest );
        }
    }

    return resultat;
}

/**
 * Calcule la longueur du trajet optimal pour visiter tous les lieux choisis
 * par l'utilisateur.
 * 
 * @param Graphe * carte  carte (graphe) des lieux a visiter (noeuds) et des
 *     rues les reliants (arcs)
 * @param vector< int > * destinations  liste des destinations entres par
 *     l'utilisateur
*/ 

vector <int> calculerPlusCourtChemin (Graphe * carte, 
        vector< int > * destinations) {

    int longueurPlusCourtChemin = INT_MAX;
    vector<int> noeudPlusCourtChemin;
    int longueurTemporaire = 0;

    sort(destinations->begin(), destinations->end());

    do {
        for (int i = 0; i <= destinations->size() - 2; ++i) {
            longueurTemporaire 
                += carte->matricePlusCourtesDistances
                [destinations->at(i)][destinations->at(i + 1)];
        }
        if (longueurTemporaire < longueurPlusCourtChemin) {
            longueurPlusCourtChemin = longueurTemporaire;
            // on fait une copie de la permutation actuelle
            noeudPlusCourtChemin = * destinations;
        }
        longueurTemporaire = 0;
    } while (next_permutation(destinations->begin(), destinations->end()));

    return noeudPlusCourtChemin;
}

/**
 * Affiche le meilleur trajet, en calculant d'abord le chemin le plus court 
 * entre les lieux entres par l'utilisateur, puis en affichant le trajet optimal
 * (lieux et rues en alternance).
 * 
 * @param Graphe * a_ruesMontreal  carte (graphe) des lieux a visiter (noeuds)
 *     et des rues les reliants (arcs)
 * @param vector< int > * a_destinations  trajet optimal entre les destinations
*/ 

void
afficherMeilleurTrajet(Graphe * a_ruesMontreal, vector< int > * a_destinations)
{
    // on calcule le plus court chemin
    vector<int> noeudPlusCourtChemin 
        = calculerPlusCourtChemin(a_ruesMontreal, a_destinations); 
    
    // on affiche le chemin en question
    cout << noeudPlusCourtChemin.at(0) << ", ";
    
    for (int i = 0; i <= noeudPlusCourtChemin.size() - 2; ++i) {
        
        int debut = noeudPlusCourtChemin.at(i);
        int fin = noeudPlusCourtChemin.at(i + 1);
        vector<int> noeudsIntermediaires;
        int noeudActuel;
        bool finChemin = false;

        while (finChemin == false) {
            noeudActuel = a_ruesMontreal->matriceProvenances[debut][fin];

            if (noeudActuel != debut) {
                noeudsIntermediaires.push_back(noeudActuel);
                fin = noeudActuel;
            } else {
                finChemin = true;
            }
        }
        
        if (noeudsIntermediaires.size() > 0) {

            cout << a_ruesMontreal->obtenirNomRue(debut, 
            noeudsIntermediaires.back()) << ", ";
            cout << noeudsIntermediaires.back() << ", ";

            while (noeudsIntermediaires.size() > 0) {

                if (noeudsIntermediaires.size() > 1)  {
                    cout << a_ruesMontreal->obtenirNomRue(
                        noeudsIntermediaires.back(),
                        noeudsIntermediaires.at(
                            noeudsIntermediaires.size() - 2)) << ", ";
	                cout << noeudsIntermediaires.at(
                        noeudsIntermediaires.size() - 2) << ", ";

                } else {

                    cout << a_ruesMontreal->obtenirNomRue(
                        noeudsIntermediaires.back(),
                        noeudPlusCourtChemin.at(i + 1)) << ", ";  
                    cout << noeudPlusCourtChemin.at(i + 1);

                    if (noeudPlusCourtChemin.size() > 2 
                            && noeudPlusCourtChemin.at(i + 1) 
                            != noeudPlusCourtChemin.back()) {
                        cout << ", ";
                    }
                }
                noeudsIntermediaires.pop_back();
            }
        } else {
            cout << a_ruesMontreal->obtenirNomRue(debut, fin) << ", ";
            cout << fin;
        }
    }
    cout << "." << endl;
}


int
main( int argn, char * argv [] )
{
    Graphe * ruesMontreal = construireGraphe( "map_montreal.xml" );

    //cout << *ruesMontreal << endl;

    ruesMontreal->plusCourtChemin();

    vector< int > * destinations = lireDestinations( cin );

    if( 0 < destinations->size() )
    {
        afficherMeilleurTrajet( ruesMontreal, destinations );
    }

    return 0;
}


