//
// Created by Emile Guillerm on 30.03.2026.
//

#include "BadgeDAO.h"
#include "models/Badge.h"
#include "models/Utilisateur.h"
#include "models/Salle.h"
#include "models/Cours.h"
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include <chrono>
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include "resultat/StructResultat.h"
#include "models/Retardabsence.h"


static drogon::Task<std::vector<drogon_model::ProjetV1::Utilisateur>> ChercherUtilisateurParBadge(
    const DbClientPtr &db,
    const std::string &uidBadge) {
    CoroMapper<drogon_model::ProjetV1::Utilisateur> mapperuser(db);
    auto resultat = co_await mapperuser.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Utilisateur::Cols::_uuid_badge, CompareOperator::EQ, uidBadge));
    co_return resultat;
}

static drogon::Task<size_t> SupprimerBadgeParIDBadge(
    const DbClientPtr &db,
    const std::string &uidBadge) {
    CoroMapper<drogon_model::ProjetV1::Badge> mapperbadge(db);
    auto resultat = co_await mapperbadge.deleteByPrimaryKey(uidBadge);
    co_return resultat;
}


static drogon::Task<std::vector<drogon_model::ProjetV1::Badge>> ChercherBadgeParIDBadge(
    const DbClientPtr &db,
    const std::string &uidBadge){
    CoroMapper<drogon_model::ProjetV1::Badge> mapperbadge(db);
    auto resultat = co_await mapperbadge.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Badge::Cols::_uuid_badge, CompareOperator::EQ, uidBadge));
    co_return resultat;
}

//FindBy
static drogon::Task<std::vector<drogon_model::ProjetV1::Utilisateur>> ChercherUtilisateurParIDUtilisateur(
    const DbClientPtr &db,
    const int32_t &uidUtilisateur) {
    CoroMapper<drogon_model::ProjetV1::Utilisateur> mapperutilisateur(db);
    auto resultat = co_await mapperutilisateur.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Utilisateur::Cols::_id_user, CompareOperator::EQ, uidUtilisateur));
    co_return resultat;
}

//update
static drogon::Task<size_t> MettreAJourUser(
    const DbClientPtr &db,
    const drogon_model::ProjetV1::Utilisateur &UtilisateurModifier) {
    CoroMapper<drogon_model::ProjetV1::Utilisateur> mapperutilisateur(db);
    auto resultat = co_await mapperutilisateur.update(UtilisateurModifier);
    co_return resultat;
    }

static drogon::Task<std::vector<drogon_model::ProjetV1::Salle>> ChercherSalleAdresseMACbae(
    const DbClientPtr &db,
    const std::string &MAC) {
    CoroMapper<drogon_model::ProjetV1::Salle> mappersalle(db);
    auto resultat = co_await mappersalle.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Salle::Cols::_mac_bae, CompareOperator::EQ, MAC));
    co_return resultat;
}

static drogon::Task<std::vector<drogon_model::ProjetV1::Salle>> ChercherSalleAdresseMACpea(
    const DbClientPtr &db,
    const std::string &MAC) {
    CoroMapper<drogon_model::ProjetV1::Salle> mappersalle(db);
    auto resultat = co_await mappersalle.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Salle::Cols::_mac_pea, CompareOperator::EQ, MAC));
    co_return resultat;
}

static drogon::Task<std::vector<drogon_model::ProjetV1::Cours>> ChercherCoursParSalle(
    const DbClientPtr &db,
    const int32_t &numSalle) {
    CoroMapper<drogon_model::ProjetV1::Cours> mappercours(db);
    auto resultat = co_await mappercours.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Cours::Cols::_num_salle, CompareOperator::EQ, numSalle));
    co_return resultat;
}


static drogon::Task<std::vector<drogon_model::ProjetV1::AbsenceCours>> ChercherUtilisateurDansAbsencecours(
    const DbClientPtr &db,
    const int32_t &uidUtilisateur) {
    CoroMapper<drogon_model::ProjetV1::AbsenceCours> mapperabsence(db);
    auto resultat = co_await mapperabsence.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::AbsenceCours::Cols::_id_user, CompareOperator::EQ, uidUtilisateur));
    co_return resultat;
}


static drogon::Task<drogon_model::ProjetV1::PresenceCours> AjoutUtilisateurPresenceCours(
    const DbClientPtr &db,
    const drogon_model::ProjetV1::PresenceCours &NouvelleEntree) {
    CoroMapper<drogon_model::ProjetV1::PresenceCours> mapperPresence(db);
    auto resultat = co_await mapperPresence.insert(NouvelleEntree);
    co_return resultat;
}


static drogon::Task<size_t> SupprimerUtilisateurAbsence(
    const DbClientPtr &db,
    const int32_t &idAbsence) {
    CoroMapper<drogon_model::ProjetV1::AbsenceCours> mapperabsence(db);
    auto resultat = co_await mapperabsence.deleteByPrimaryKey(idAbsence);
    co_return resultat;
}

static drogon::Task<drogon_model::ProjetV1::Retardabsence> AjoutRetardAbsence(
    const DbClientPtr &db,
    const drogon_model::ProjetV1::Retardabsence &NouvelleEntree) {
    CoroMapper<drogon_model::ProjetV1::Retardabsence> mapperRetardAbsence(db);
    auto resultat = co_await mapperRetardAbsence.insert(NouvelleEntree);
    co_return resultat;
}
