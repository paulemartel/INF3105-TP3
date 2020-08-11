/*
 * Graphe.cpp
 */

#include "Graphe.hpp"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>
#include <climits>


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

/**
 * Permet d'acceder au nom des rues (arcs) de l'exterieur
 * de la classe.
 *
 * @param int sommet lieu de depart de l'arc
 * @param arc la rue en question
 * @return nomRue le nom de la rue
**/
string Graphe::obtenirNomRue (int sommet, int arc) {
    string nomRue;
    for (auto unArc : * _adjacences[sommet]) {
        if (unArc->sommetArrive == arc) {
            nomRue = unArc->nom;
        }
    }
    return nomRue;
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

/**
* Fait le calcul de la matrice des plus courts chemins et de
* celle des provenances, puis stocke les resultats
* dans matricePlusCourtesDistances et matriceProvisoire.
**/
void
Graphe::plusCourtChemin( void )
{
    //transformer le graphe en matrice
    listeEnMatrice();

    //algorithme Floyd-Warshall
    int size = _adjacences.size();
    vector<vector<int>> matriceDepart;
    vector<vector<int>> matriceProvisoire;

    matricePlusCourtesDistances = matriceAdjacences;
    initialiserMatriceProvenance();

    for(int k = 0 ; k < size ; k++){
        matriceDepart = matricePlusCourtesDistances;
        matriceProvisoire = matriceProvenances;
        initialiserMatricePlusCourtesDistances();
        for(int j = 0 ; j < size ; j++){
            for( int i = 0 ; i < size ; i++){
                int longueurTot;
                if(matriceDepart[i][k] == INT_MAX 
                    || matriceDepart[k][j] == INT_MAX){
                    longueurTot = INT_MAX;
                } else{
                    longueurTot = matriceDepart[i][k] + matriceDepart[k][j];
                }
                if(longueurTot < matriceDepart[i][j]){
                    matricePlusCourtesDistances[i][j] = longueurTot;
                    matriceProvenances[i][j] = matriceProvisoire[k][j];
                } else{
                    matricePlusCourtesDistances[i][j] = matriceDepart[i][j];
                }
            }
        }
    }
}



/**
* Initialise la matricePlusCourteDistance a 0.
**/
void
Graphe::initialiserMatricePlusCourtesDistances( void ){
    int size = _adjacences.size();
    for(int i = 0 ; i < size ; i++){
        vector<int> nouvVecteur;
        for(int j = 0 ; j < size ; j++){
            nouvVecteur.push_back(0);
        }
        matricePlusCourtesDistances.push_back(nouvVecteur);

    }
}

/**
* Initialise la matrice des provenances avec le numero du noeud s'il y a
* un chemin, sinon l'initialise a infini.
**/
void
Graphe::initialiserMatriceProvenance( void ){
    int size = _adjacences.size();
    for(int i = 0 ; i < size ; i++){
        vector<int> nouvVecteur;
        for(int j = 0 ; j < size ; j++){
            if(matriceAdjacences[i][j] != INT_MAX){
                nouvVecteur.push_back(i);
            } else{
                nouvVecteur.push_back(INT_MAX);
            }
        }
        matriceProvenances.push_back(nouvVecteur);
    }
}

/**
* Transforme la liste des adjacences sous forme matricielle. 
*
* @return vector<vector<int>> matriceAdjacences la matrice des adjacences
**/
vector<vector<int>>
Graphe::listeEnMatrice( void ){
    int size = _adjacences.size();

    //initialiser le tout a infini
    for(int i = 0 ; i < size ; i++){
        vector<int> nouvVecteur;
        for(int j = 0 ; j < size ; j++){
            if(i == j){
                nouvVecteur.push_back(0);

            } else{
                nouvVecteur.push_back(INT_MAX);
            }
        }
        matriceAdjacences.push_back(nouvVecteur);
    }

    //changer les partie de la matrice
    for(int k = 0 ; k <_adjacences.size() ; k++){
        for(int m = 0 ; m < _adjacences[k]->size() ; m++){
            int sommet = _adjacences[k]->at(m)->sommetArrive;
            int longueurArc = _adjacences[k]->at(m)->longueur;
            matriceAdjacences[k][sommet] = longueurArc;
        }
    }

    return matriceAdjacences;

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
