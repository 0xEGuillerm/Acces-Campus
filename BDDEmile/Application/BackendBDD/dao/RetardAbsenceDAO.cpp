//
// Created by Emile Guillerm on 05.04.2026.
//

#include "RetardAbsenceDAO.h"
#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include "models/Utilisateur.h"
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include "Retardabsence.h"
#include "models/Cours.h"

using namespace drogon::orm;

static drogon::Task<std::vector<drogon_model::ProjetV1::Retardabsence>> ChercherRetardAbsenceEleve(
    const DbClientPtr &db,
    const std::string &idUtilisateur) {
    CoroMapper<drogon_model::ProjetV1::Retardabsence> mapperRetardAbsence(db);
    auto resultat = co_await mapperRetardAbsence.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Retardabsence::Cols::_id_user, CompareOperator::EQ, idUtilisateur));
    co_return resultat;
}