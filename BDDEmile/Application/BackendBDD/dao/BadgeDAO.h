//
// Created by Emile Guillerm on 30.03.2026.
//

#ifndef BACKENDBDD_BADGEDAO_H
#define BACKENDBDD_BADGEDAO_H

#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>
#include "models/Utilisateur.h"  // Classe générée par drogon_ctl
#include <functional>
#include <json/json.h>

#include "Badge.h"

using namespace drogon::orm;

class BadgeDAO
{
public:
    // Obtenir les infos d'un badge
    void InformationBadge(
        const DbClientPtr &db,
        const std::int32_t &uid,
        //Appel la fonction BadgeObtenu(callback)
        std::function<void(const Json::Value &body)> &&InformationObtenu,
        std::function<void(const DrogonDbException &)> &&Erreur);

    // Crée un nouvel utilisateur
    void CreeBadge(
        const DbClientPtr &db,
        const std::int32_t &uiduser,
        const std::int32_t &uidbadge,
        std::function<void(bool, const std::string&)> &&BadgeCree,
        std::function<void(const DrogonDbException &)> &&Erreur);

    void SupprimerBadge(
        const DbClientPtr &db,
        const std::int32_t &uid,
        std::function<void(const drogon_model::ProjetV1::Badge &)> &&BadgeSupprimer,
        std::function<void(const DrogonDbException &)> &&Erreur);

    void ModifierBadge(
        //La connexion
        const DbClientPtr &db,
        //Uid Badge
        const std::int32_t &uid,
        //Body des champs à modifier peut etre vide donc tester les champs
        const Json::Value &body,
        //callback reussite / Erreur
        std::function<void(bool, const std::string&)> &&ModificationReussit,
        std::function<void(const DrogonDbException &)> &&ErreurVide);

    void VerifierBadge(
        const DbClientPtr &db,
        const std::int32_t &uid,
        const std::string &MAC,
        std::function<void(const drogon_model::ProjetV1::Badge &)> &&BadgeVerifierResultat,
        std::function<void(const DrogonDbException &)> &&Erreur);
};

#endif //BACKENDBDD_BADGEDAO_H