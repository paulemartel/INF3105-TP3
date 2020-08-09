/*
 * Graphe.cpp
 */

#include "Graphe.hpp"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>


using namespace std;



Graphe::Graphe( void )
{

}


Graphe::~Graphe( void )
{
    // Puisque _adjacences n'est pas un pointeur il est automatiquement desalloue.
    // Le destructeur de vector s'occupe d'appeler les destructeurs sur chacun de
    // ses elements.
    // Il n'est donc pas utile de desallouer les elements.
}


void
Graphe::ajouterArcs( int a_sommet1, int a_sommet2, int a_longueur, string a_nom )
{
    int tailleMinimum = a_sommet1 < a_sommet2 ? a_sommet2 : a_sommet1;
    while( _adjacences.size() <= tailleMinimum )
    {
        _adjacences.push_back( new vector< Arc * >() );
    }

    assert( 0 <= a_sommet1 && a_sommet1 < _adjacences.size() );
    assert( nullptr != _adjacences[a_sommet1] );
    _adjacences[a_sommet1]->push_back( new Arc( a_sommet2, a_longueur, a_nom ) );

    assert( 0 <= a_sommet2 && a_sommet2 < _adjacences.size() );
    assert( nullptr != _adjacences[a_sommet2] );
    _adjacences[a_sommet2]->push_back( new Arc( a_sommet1, a_longueur, a_nom ) );
}


void
Graphe::plusCourtChemin( void )
{

}


ostream &
operator <<( ostream & a_out, Graphe const & a_graphe )
{
    int i = 0;
    int j = 0;

    for( i = 0; i < a_graphe._adjacences.size(); ++ i )
    {
        a_out << i << " : ";
        for( j = 0; j < a_graphe._adjacences[i]->size(); ++ j )
        {
            if( 0 != j )
            {
                a_out << ", ";
            }
            a_out << *( a_graphe._adjacences[i]->at(j) );
        }
        a_out << endl;
    }

    return a_out;
}
