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
#include "resultat/StructResultat.h"

using namespace drogon::orm;

class BadgeLogique {
public:
drogon::Task<Json::Value> InformationBadge(
     //Gère les connexion avecc la DB (TCP, nombre de connexion, répartition, etc)
     const drogon::orm::DbClientPtr &db,
     //Uid Badge
     const std::string &uidBadge);

drogon::Task<ResultatCoro> SupprimerBadge(
    const DbClientPtr &db,
    const std::string &uidBadge);


drogon::Task<ResultatCoro> CreationBadge(
    const DbClientPtr &db,
    const std::int32_t &uiduser,
    const std::string &uidBadge);

drogon::Task<ResultatCoro> ModifierInfoUtilisateur(
    const DbClientPtr &db,
    const std::string &uidBadge,
    const Json::Value &body);

drogon::Task<ResultatCoro> VerifierBadgePEA(
    const DbClientPtr &db,
    const std::string &uidBadge,
    const std::string &mac);

drogon::Task<ResultatCoro> ScanneBadgeBAE(
const DbClientPtr &db,
const std::string &uidBadge,
const std::string &mac,
const std::uint64_t &heure_badgage);
};
#endif //BACKENDBDD_BADGELOGIQUE_H