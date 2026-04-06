//
// Created by Emile Guillerm on 05.04.2026.
//

#include "RetardAbsenceLogique.h"


#include "SalleLogique.h"
#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include "models/Utilisateur.h"
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include "models/Cours.h"
#include <json/json.h>
#include "dao/SalleDAO.h"
#include "models/Retardabsence.h"
#include "dao/RetardAbsenceDAO.h"

using namespace drogon::orm;

drogon::Task<Json::Value> RetardAbsenceLogique::AbsenceEleve(
    const DbClientPtr &db,
    const int32_t &idUtilisateur) {
    auto RetardAbsenceListe = co_await RetardAbsenceDAO::ChercherRetardAbsenceEleve(db, idUtilisateur);
    if (RetardAbsenceListe.empty()) {
        co_return Json::Value::null;
    }
    Json::Value ReponseJSON;
    ReponseJSON["nb_absence"] = static_cast<int>(RetardAbsenceListe.size());
    Json::Value listeAbsence(Json::arrayValue);
    for (const auto& Absence : RetardAbsenceListe){
        Json::Value AbsenceVal;
        AbsenceVal["timestamp_debut_cours"] = static_cast<Json::Int64>((*Absence.getTimestampHeureCours()).secondsSinceEpoch());
        AbsenceVal["temps_retard"] = static_cast<Json::Int64>(Absence.getValueOfTempsRetardMin());
        AbsenceVal["absence_ou_retard"] = Absence.getValueOfAbsence();
        listeAbsence.append(AbsenceVal);
    }
    ReponseJSON["listeAbsence"] = listeAbsence;
    co_return ReponseJSON;
}