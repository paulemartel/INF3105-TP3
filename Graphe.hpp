/*
 * Graphe.hpp
 *
 */

#ifndef TP3_GRAPHE_HPP_
#define TP3_GRAPHE_HPP_


#include "Arc.hpp"
#include "Graphe.hpp"

#include <iostream>
#include <vector>

using namespace std;

class Graphe
{
private:
    vector< vector< Arc * > * > _adjacences;
    vector<vector<int>> matriceAdjacences;

    vector<vector<int>> listeEnMatrice( void );
    void initialiserMatricePlusCourtesDistances( void );
    void initialiserMatriceProvenance( void );
public:
    Graphe( void );
    virtual ~Graphe( void );

    string obtenirNomRue (int sommet, int arc);

    void ajouterArcs( int a_sommet1, int a_sommet2, int a_longueur, string a_nom );
    void plusCourtChemin( void );
    //void afficherMatrice(&vector<vector<int>> matrice);
    
    vector<vector<int>> matricePlusCourtesDistances;
    vector<vector<int>> matriceProvenances;

    friend ostream & operator <<( ostream &, Graphe const & );
};


#endif /* TP2_GRAPHE_HPP_ */
