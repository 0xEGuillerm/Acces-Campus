//
// Created by Emile Guillerm on 06.04.2026.
//

#ifndef BACKENDBDD_UTILISATEURDAO_H
#define BACKENDBDD_UTILISATEURDAO_H
#include "AbsenceCours.h"
#include "Utilisateur.h"

using namespace drogon::orm;


class UtilisateurDAO {
public:
    static drogon::Task<std::vector<drogon_model::ProjetV1::Utilisateur>> ChercherUtilisateurParNom(
    const DbClientPtr &db,
    const std::string &nom);

    static drogon::Task<std::vector<drogon_model::ProjetV1::Utilisateur>> ChercherUtilisateurParPrenom(
        const DbClientPtr &db,
        const std::string &prenom);
};


#endif //BACKENDBDD_UTILISATEURDAO_H