//
// Created by Emile Guillerm on 05.04.2026.
//

#ifndef BACKENDBDD_UTILISATEURLOGIQUE_H
#define BACKENDBDD_UTILISATEURLOGIQUE_H

#include <json/value.h>
#include <drogon/HttpController.h>
#include "dao/UtilisateurDAO.h"
#include <resultat/StructResultat.h>



using namespace drogon::orm;



//Logique en lien avec utilisateur
class UtilisateurLogique {
public:
    //Recherche utilisateur par prenom et/ou nom.
    //Renvoie une liste d'utilisateur dans un JSON
    static drogon::Task<ResultatCoro<Json::Value>> UtilisateurParNomPrenom(
        const DbClientPtr &db,
        const std::string &nom,
        const std::string &prenom);

    //Logique pour le login d'un utilisateur sur le PSW.
    //Renvoi Un token JWT en JSON dans le cas d'une réussite.
    static drogon::Task<ResultatCoro<Json::Value>> LoginPSW(
        const DbClientPtr &db,
        const std::string &login,
        const std::string &motdepasse);
};


#endif //BACKENDBDD_UTILISATEURLOGIQUE_H

