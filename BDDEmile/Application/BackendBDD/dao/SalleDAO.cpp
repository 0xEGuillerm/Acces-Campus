//
// Created by Emile Guillerm on 04.04.2026.
//

#include "SalleDAO.h"
#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include "models/Utilisateur.h"
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include "models/Cours.h"

using namespace drogon::orm;

drogon::Task<std::vector<drogon_model::ProjetV1::Salle>> SalleDAO::ChercherSalleAdresseMACbae(
    const DbClientPtr &db,
    const std::string &MAC) {
    CoroMapper<drogon_model::ProjetV1::Salle> mappersalle(db);
    auto resultat = co_await mappersalle.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Salle::Cols::_mac_bae, CompareOperator::EQ, MAC));
    co_return resultat;
}

drogon::Task<std::vector<drogon_model::ProjetV1::Cours>> SalleDAO::ChercherCoursParSalle(
    const DbClientPtr &db,
    const int32_t &numSalle) {
    CoroMapper<drogon_model::ProjetV1::Cours> mappercours(db);
    auto resultat = co_await mappercours.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Cours::Cols::_num_salle, CompareOperator::EQ, numSalle));
    co_return resultat;
}

drogon::Task<std::vector<drogon_model::ProjetV1::AbsenceCours>> SalleDAO::ChercherUtilisateurDansAbsencecours(
    const DbClientPtr &db,
    const int32_t &uidUtilisateur) {
    CoroMapper<drogon_model::ProjetV1::AbsenceCours> mapperabsence(db);
    auto resultat = co_await mapperabsence.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::AbsenceCours::Cols::_id_user, CompareOperator::EQ, uidUtilisateur));
    co_return resultat;
}

drogon::Task<std::vector<drogon_model::ProjetV1::PresenceCours>> SalleDAO::ChercherUtilisateurDansPresencecours(
    const DbClientPtr &db,
    const int32_t &uidUtilisateur) {
    CoroMapper<drogon_model::ProjetV1::PresenceCours> mapperPresence(db);
    auto resultat = co_await mapperPresence.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::PresenceCours::Cols::_id_user, CompareOperator::EQ, uidUtilisateur));
    co_return resultat;
}