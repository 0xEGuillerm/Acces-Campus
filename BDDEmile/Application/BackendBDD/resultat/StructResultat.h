//
// Created by Emile Guillerm on 04.04.2026.
//

#ifndef BACKENDBDD_STRUCTRESULTAT_H
#define BACKENDBDD_STRUCTRESULTAT_H

#include <string>
#include <variant>

//Structure de réponse dans mon programme avec:
//Un booleen pour signaler la bonne/mauvaise execution !! A verifier avant d'utiliser donnée !!
//Un message pour remonter les erreur
//Les potentielle donnée a faire remonté
//Utilisation de template car on dois remonter des info différente ex:
//vector<Utilisateur>
//Inisialiser à monostate par défaut car dans le cas d'une erreur on dois remonter l'info et donc on dois avoir un T
//monostate -> type vide -> 1 octet vide
template<typename T = std::monostate>
struct ResultatCoro {
    bool BoolResultat = false;
    std::string MessageResultat;
    T donnee;
};



#endif //BACKENDBDD_STRUCTRESULTAT_H