//
// Created by Emile Guillerm on 05.04.2026.
//

#ifndef BACKENDBDD_UTILISATEURLOGIQUE_H
#define BACKENDBDD_UTILISATEURLOGIQUE_H

#include <json/value.h>
#include <drogon/HttpController.h>
#include "dao/UtilisateurDAO.h"



using namespace drogon::orm;




class UtilisateurLogique {
public:
    static drogon::Task<Json::Value> UtilisateurParNomPrenom(
        const DbClientPtr &db,
        const std::string &nom,
        const std::string &prenom);
};


#endif //BACKENDBDD_UTILISATEURLOGIQUE_H

