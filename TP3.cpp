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

    cout << "Entrez vos destinations séparées par des espaces : ";
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
    // Trouver le meilleur ordre de visite des destinations et afficher.

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


