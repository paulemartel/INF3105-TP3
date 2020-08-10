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
    //transformer le graphe en matrice
    listeEnMatrice();

    //algorithme Floyd-Warshall
    initialiserMatricePlusCourtesDistances(); //sert a rien?

    int size = _adjacences.size();
    vector<vector<int>> matriceDepart; // = matrice T
    vector<vector<int>> matriceProvisoire; // = matrice P
    //matricePlusCourtesDistances = matrice D
    //matriceProvenances = matrice pi

    matricePlusCourtesDistances = matriceAdjacences;
    initialiserMatriceProvenance();

    for(int k = 0 ; k < size ; k++){
        matriceDepart = matricePlusCourtesDistances;
        matriceProvisoire = matriceProvenances;
        initialiserMatricePlusCourtesDistances();
        for(int j = 0 ; j < size ; j++){
            for( int i = 0 ; i < size ; i++){
                int longueurTot = matriceDepart[i][j] + matriceDepart[k][j];
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
Initialise la matricePlusCourteDistance a 0
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
Initialise la matrice provenance
met le numero du noeud s'il y a
un chemin sinon met INT_MAX
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
        matricePlusCourtesDistances.push_back(nouvVecteur);
    }
}

/**
Transforme la liste des adjacence sous forme
matricielle. 
**/

vector<vector<int>>
Graphe::listeEnMatrice( void ){
    int size = _adjacences.size();


    for(int i = 0 ; i < size ; i++){
        vector<int> nouvVecteur;
        for(int j = 0 ; j < size ; j++){
            if(i==j){
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

    //au départ remplir de 0 et de infini partout


    // //afficher la liste d'incidence (DEBUG)
    // for(int m = 0 ; m < _adjacences.size() ; m++){
    //     cout<< "SIZE: " << _adjacences[m]->size()<<endl;
    //     cout<<"-------------"<<endl;
    //     for(int n = 0 ; n < _adjacences[m]->size() ; n++){
    //         cout<< _adjacences[m]->at(n)->sommetArrive<<endl;

    //     }

    // }

    //afficher matrice adjacence (DEBUG)
    // for(int o = 0 ; o < matriceAdjacences.size() ; o++){
    //     cout << "[";
    //     for(int p = 0 ; p < matriceAdjacences[o].size() ; p++){
    //         cout << matriceAdjacences[o][p] << ", ";
    //     }
    //     cout << "]" << endl;
    // }


    return matriceAdjacences;

}



/**
Affiche une matrice UTILISE POUR DEBUG
**/
// void afficherMatrice(vector<vector<int>> matrice){
//     int size = matrice[0].size();
//     for(int i = 0 ; i < size ; i++){
//         cout <<"[ ";
//         for( int j = 0 ; j < size ; j++){
//             cout<< matrice[i][j] << ", ";
//         }
//         cout << "]" << endl;
//     }
// }


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
