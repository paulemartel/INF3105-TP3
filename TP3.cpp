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

#include <limits> // VOIR SI PERMIS
#include <algorithm> // idem

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


void
afficherMeilleurTrajet( Graphe * a_ruesMontreal, vector< int > * a_destinations )
{
    // CALCUL DU PLUS COURT CHEMIN
    int longueurPlusCourtChemin = numeric_limits<int>::max();
    vector<int> noeudPlusCourtChemin;
    int longueurTemporaire = 0;

    sort(a_destinations->begin(), a_destinations->end());

    do {
        for (int i = 0; i < a_destinations->size() - 2; ++i) {
            longueurTemporaire 
                += a_ruesMontreal->matricePlusCourtesDistances
                [a_destinations->at(i)][a_destinations->at(i + 1)];
        }
        if (longueurTemporaire < longueurPlusCourtChemin) {
            longueurPlusCourtChemin = longueurTemporaire;
            // on fait une copie de la permutation actuelle
            noeudPlusCourtChemin = * a_destinations;
        }
        longueurTemporaire = 0;

    } while (next_permutation(a_destinations->begin(), a_destinations->end()));

    // AFFICHAGE DUDIT CHEMIN

    cout << a_destinations->at(0) << ", ";

    for (int i = 0; i <= a_destinations->size() - 2; ++i) {
        
        int debut = a_destinations->at(i);
        int fin = a_destinations->at(i + 1);
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
        
        cout << a_ruesMontreal->obtenirNomRue(a_destinations->at(i), 
                noeudsIntermediaires.back()) << ", ";
        //noeudsIntermediaires.pop_back();
        while (noeudsIntermediaires.size() > 0) {
            cout << noeudsIntermediaires.back() << ", ";
            cout << a_ruesMontreal->obtenirNomRue(noeudsIntermediaires.back(),
                noeudsIntermediaires.at(noeudsIntermediaires.size() - 2)) << ", ";
            noeudsIntermediaires.pop_back();
        }
        cout << a_ruesMontreal->obtenirNomRue(noeudsIntermediaires.back(), 
                a_destinations->at(i + 1));
    }

    cout << a_destinations->back() << "." << endl;
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


