//
// Created by Emile Guillerm on 04.04.2026.
//

#ifndef BACKENDBDD_BADGELOGIQUE_H
#define BACKENDBDD_BADGELOGIQUE_H

#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>
#include "models/Utilisateur.h"  // Classe générée par drogon_ctl
#include <functional>
#include <json/json.h>

class BadgeLogique {
public:
    //Renvoie un json des informations d'un utilisateur
    drogon::Task<Json::Value> InformationBadge(
        //Gère les connexion vaec la DB (TCP, nombre de connexion, répartition, etc)
        const drogon::orm::DbClientPtr &db,
        const std::string &uidBadge);

    // Crée un nouvel utilisateur
    void CreeBadge(
        const drogon::orm::DbClientPtr &db,
        const std::int32_t &uiduser,
        const std::string &uidbadge);

    void SupprimerBadge(
        const drogon::orm::DbClientPtr &db,
        const std::string &uid);

    void ModifierBadge(
        //La connexion
        const drogon::orm::DbClientPtr &db,
        //Uid Badge
        const std::string &uid,
        //Body des champs à modifier peut etre vide donc tester les champs
        const Json::Value &body);

    void VerifierBadge(
        const drogon::orm::DbClientPtr &db,
        const std::string &uid,
        const std::string &MAC);

    void ScannerBadge(
        const drogon::orm::DbClientPtr &db,
        const std::string &uid,
        const std::string &MAC);
};
#endif //BACKENDBDD_BADGELOGIQUE_H