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
#include "resultat/StructResultat.h"
#include "Badge.h"
#include "models/Salle.h"
#include "models/Cours.h"
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include "models/Retardabsence.h"

using namespace drogon::orm;

class BadgeDAO
{
public:
    // Obtenir les infos d'un badge
    static drogon::Task<std::vector<drogon_model::ProjetV1::Utilisateur>> ChercherUtilisateurParIDBadge(
        const DbClientPtr &db,
        const std::string &uidBadge);

    //DeleteByPrimaryKey
    static drogon::Task<size_t> SupprimerBadgeParIDBadge(
        const DbClientPtr &db,
        const std::string &uidBadge);

    //FindBy
    static drogon::Task<std::vector<drogon_model::ProjetV1::Badge>> ChercherBadgeParIDBadge(
        const DbClientPtr &db,
        const std::string &uidBadge);

    //FindBy
    static drogon::Task<std::vector<drogon_model::ProjetV1::Utilisateur>> ChercherUtilisateurParIDUtilisateur(
        const DbClientPtr &db,
        const int32_t &uidUtilisateur);

    //update
    static drogon::Task<size_t> MettreAJourUser(
    const DbClientPtr &db,
    const drogon_model::ProjetV1::Utilisateur);


    static drogon::Task<std::vector<drogon_model::ProjetV1::Salle>> ChercherSalleAdresseMACbae(
        const DbClientPtr &db,
        const std::string &MAC);

    static drogon::Task<std::vector<drogon_model::ProjetV1::Salle>> ChercherSalleAdresseMACpea(
        const DbClientPtr &db,
        const std::string &MAC);

    static drogon::Task<std::vector<drogon_model::ProjetV1::Cours>> ChercherCoursParSalle(
        const DbClientPtr &db,
        const int32_t &numSalle);


    static drogon::Task<drogon_model::ProjetV1::PresenceCours> AjoutUtilisateurPresenceCours(
        const DbClientPtr &db,
        const drogon_model::ProjetV1::PresenceCours &NouvelleEntree);


    static drogon::Task<size_t> SupprimerUtilisateurAbsence(
        const DbClientPtr &db,
        const int32_t &idAbsence);

    static drogon::Task<std::vector<drogon_model::ProjetV1::AbsenceCours>> ChercherUtilisateurDansAbsencecours(
    const DbClientPtr &db,
    const int32_t &uidUtilisateur);


    static drogon::Task<drogon_model::ProjetV1::Retardabsence> AjoutRetardAbsence(
    const DbClientPtr &db,
    const drogon_model::ProjetV1::Retardabsence &NouvelleEntree);
};

#endif //BACKENDBDD_BADGEDAO_H