//
// Created by Emile Guillerm on 05.04.2026.
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
#include "CoursDAO.h"

static drogon::Task<std::vector<drogon_model::ProjetV1::Cours>> ChercherCoursParSalle(
    const DbClientPtr &db,
    const int32_t &salle) {
    CoroMapper<drogon_model::ProjetV1::Cours> mappercours(db);
    auto resultat = co_await mappercours.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Cours::Cols::_num_salle, CompareOperator::EQ, salle));
    co_return resultat;
}


static drogon::Task<drogon_model::ProjetV1::Cours> AjoutReservation(
    const DbClientPtr &db,
    const drogon_model::ProjetV1::Cours &NouvelleEntree) {
    CoroMapper<drogon_model::ProjetV1::Cours> mapperCours(db);
    auto resultat = co_await mapperCours.insert(NouvelleEntree);
    co_return resultat;
}
