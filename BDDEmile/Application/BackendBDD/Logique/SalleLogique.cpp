//
// Created by Emile Guillerm on 04.04.2026.
//

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

const int VALEURMARGECOURS = 5;

using namespace drogon::orm;

drogon::Task<Json::Value> SalleLogique::EtatSalleMac(
    const DbClientPtr &db,
    const std::string &mac) {
    auto SalleListe = co_await SalleDAO::ChercherSalleAdresseMACbae(db, mac);
    if (SalleListe.empty()) {
        co_return Json::Value::null;
    }
    auto CoursListe = co_await SalleDAO::ChercherCoursParSalle(db, (*SalleListe[0].getNumSalle()));
    if (CoursListe.empty()) {
        co_return Json::Value::null;
    }
    drogon_model::ProjetV1::Cours CoursActuelle;
    trantor::Date TimestampBrut = trantor::Date::now();
    int64_t TimestampSecond = TimestampBrut.secondsSinceEpoch();
    bool trouvee = false;
    for (const auto &cours: CoursListe) {
        if (cours.getValueOfHeureDebut().secondsSinceEpoch() - VALEURMARGECOURS < TimestampSecond && cours.getValueOfHeureFin().secondsSinceEpoch() > TimestampSecond) {
            //A l'heure
            CoursActuelle = cours;
            trouvee = true;
            break;
        }
    }
    if (!trouvee) {
        co_return Json::Value::null;
    }

    auto ListeAbsent = co_await SalleDAO::ChercherUtilisateurDansAbsencecours(db, (*CoursActuelle.getIdCours()));
    if (ListeAbsent.empty()) {
        co_return Json::Value::null;
    }
    auto ListePresent = co_await SalleDAO::ChercherUtilisateurDansPresencecours(db, (*CoursActuelle.getIdCours()));
    if (ListePresent.empty()) {
        co_return Json::Value::null;
    }

    Json::Value ReponseJSON;
    ReponseJSON["nb_eleve_present"] = static_cast<int>(ListePresent.size());
    ReponseJSON["nb_eleve_absent"] = static_cast<int>(ListeAbsent.size());
    Json::Value listeAbsent(Json::arrayValue);
    for (const auto& id : ListeAbsent){
        listeAbsent.append(static_cast<int>((*id.getIdUser())));
    }
    ReponseJSON["listeAbsent"] = listeAbsent;
    co_return ReponseJSON;
    //Renvoie les id eleve
}


drogon::Task<Json::Value> SalleLogique::EtatSalleNumeroSalle(
    const DbClientPtr &db,
    const std::string &numerosalle) {
    auto CoursListe = co_await SalleDAO::ChercherCoursParSalle(db, stoi(numerosalle));
    if (CoursListe.empty()) {
        co_return Json::Value::null;
    }
    drogon_model::ProjetV1::Cours CoursActuelle;
    trantor::Date TimestampBrut = trantor::Date::now();
    int64_t TimestampSecond = TimestampBrut.secondsSinceEpoch();


    bool trouvee = false;
    for (const auto &cours: CoursListe) {
        if (cours.getValueOfHeureDebut().secondsSinceEpoch() - VALEURMARGECOURS < TimestampSecond && cours.getValueOfHeureFin().secondsSinceEpoch() > TimestampSecond) {
            //A l'heure
            CoursActuelle = cours;
            trouvee = true;
            break;
        }
    }
    if (!trouvee) {
        co_return Json::Value::null;
    }

    auto ListeAbsent = co_await SalleDAO::ChercherUtilisateurDansAbsencecours(db, (*CoursActuelle.getIdCours()));
    if (ListeAbsent.empty()) {
        co_return Json::Value::null;
    }
    auto ListePresent = co_await SalleDAO::ChercherUtilisateurDansPresencecours(db, (*CoursActuelle.getIdCours()));
    if (ListePresent.empty()) {
        co_return Json::Value::null;
    }

    Json::Value ReponseJSON;
    ReponseJSON["nb_eleve_present"] = static_cast<int>(ListePresent.size());
    ReponseJSON["nb_eleve_absent"] = static_cast<int>(ListeAbsent.size());
    Json::Value listeAbsent(Json::arrayValue);
    for (const auto& id : ListeAbsent){
        listeAbsent.append(static_cast<int>((*id.getIdUser())));
    }
    ReponseJSON["listeAbsent"] = listeAbsent;
    co_return ReponseJSON;
    //Renvoie les id eleve
}
