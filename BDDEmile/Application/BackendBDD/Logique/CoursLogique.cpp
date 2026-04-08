//
// Created by Emile Guillerm on 05.04.2026.
//

#include "CoursLogique.h"

#include <drogon/HttpController.h>

#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include "models/Utilisateur.h"
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include "models/Cours.h"
#include "dao/CoursDAO.h"
#include "resultat/StructResultat.h"

using namespace drogon::orm;


drogon::Task<ResultatCoro> CoursLogique::ReservationSallePGS(
    const DbClientPtr &db,
    const Json::Value &body) {
    int32_t heure_debut = body["heure_debut"].asInt();
    int32_t heure_fin = body["heure_fin"].asInt();
    int32_t salle = body["salle"].asInt();
    int32_t classe = body["classe"].asInt();
    auto ListeCours = co_await CoursDAO::ChercherCoursParSalle(db, salle);
    if (ListeCours.empty()) {
        co_return ResultatCoro(false, "Salle introuvable");
    }
    trantor::Date TimestampBrut = trantor::Date::now();
    int64_t TimestampSecond = TimestampBrut.secondsSinceEpoch();
    bool trouvee = false;
    for (const auto &cours: ListeCours) {
        if ((cours.getValueOfHeureDebut().secondsSinceEpoch() < heure_debut && cours.getValueOfHeureFin().secondsSinceEpoch() > heure_debut)
            || (cours.getValueOfHeureDebut().secondsSinceEpoch() < heure_fin && cours.getValueOfHeureFin().secondsSinceEpoch() > heure_fin)
            ) {
            trouvee = true;
            break;
        }
    }
    if (!trouvee) {
        co_return ResultatCoro(false, "Ce creneau est deja occupe");
    }
    trantor::Date heureDebutTrantor (static_cast<int64_t>(heure_debut)*1000000);
    trantor::Date heureFinTrantor (static_cast<int64_t>(heure_fin)*1000000);
    drogon_model::ProjetV1::Cours Reservation;
    Reservation.setIdClasse(classe);
    Reservation.setHeureDebut(heureDebutTrantor);
    Reservation.setHeureFin(heureFinTrantor);
    Reservation.setNumSalle(salle);
    Reservation.setReservePar(0);
    auto EnregistrementCours = co_await CoursDAO::AjoutReservation(db, Reservation);
    if ((*EnregistrementCours.getIdCours()) == 0) {
        co_return ResultatCoro(false, "Erreur lors de l'enregistrement du cours");
    }
    co_return ResultatCoro(true, "Finit");
}


drogon::Task<ResultatCoro> CoursLogique::ReservationSallePSW(
    const DbClientPtr &db,
    const Json::Value &body) {
    int32_t heure_debut = body["heure_debut"].asInt();
    int32_t heure_fin = body["heure_fin"].asInt();
    int32_t salle = body["salle"].asInt();
    int32_t utilisateur = body["utilisateur"].asInt();
    auto ListeCours = co_await CoursDAO::ChercherCoursParSalle(db, salle);
    if (ListeCours.empty()) {
        co_return ResultatCoro(false, "Salle introuvable");
    }
    trantor::Date TimestampBrut = trantor::Date::now();
    int64_t TimestampSecond = TimestampBrut.secondsSinceEpoch();
    bool trouvee = false;
    for (const auto &cours: ListeCours) {
        if ((cours.getValueOfHeureDebut().secondsSinceEpoch() < heure_debut && cours.getValueOfHeureFin().secondsSinceEpoch() > heure_debut)
            || (cours.getValueOfHeureDebut().secondsSinceEpoch() < heure_fin && cours.getValueOfHeureFin().secondsSinceEpoch() > heure_fin)
            ) {
            trouvee = true;
            break;
            }
    }
    if (!trouvee) {
        co_return ResultatCoro(false, "Ce creneau est deja occupe");
    }
    trantor::Date heureDebutTrantor (static_cast<int64_t>(heure_debut)*1000000);
    trantor::Date heureFinTrantor (static_cast<int64_t>(heure_fin)*1000000);
    drogon_model::ProjetV1::Cours Reservation;
    Reservation.setIdClasse(0);
    Reservation.setHeureDebut(heureDebutTrantor);
    Reservation.setHeureFin(heureFinTrantor);
    Reservation.setNumSalle(salle);
    Reservation.setReservePar(utilisateur);
    auto EnregistrementCours = co_await CoursDAO::AjoutReservation(db, Reservation);
    if ((*EnregistrementCours.getIdCours()) == 0) {
        co_return ResultatCoro(false, "Erreur lors de l'enregistrement du cours");
    }
    co_return ResultatCoro(true, "Finit");
}

drogon::Task<Json::Value> CoursLogique::PlanningSalle(
    const DbClientPtr &db,
    const int32_t &salle) {
    auto ListeCours = co_await CoursDAO::ChercherCoursParSalle(db, salle);
    if (ListeCours.empty()) {
        co_return Json::Value::null;
    }
    Json::Value ReponseJSON;
    Json::Value listecours(Json::arrayValue);
    for (const auto& cours : ListeCours) {
        Json::Value courss;
        courss["id_cours"] = static_cast<int>(*cours.getIdCours());
        courss["heure_debut"] = static_cast<int>(((*cours.getHeureDebut()).secondsSinceEpoch()));
        courss["heure_fin"] = static_cast<int>(((*cours.getHeureFin()).secondsSinceEpoch()));
        courss["classe"] = std::to_string(*cours.getIdClasse());
        listecours.append(courss);
    }
    ReponseJSON["listeCoursPlanning"] = listecours;
    co_return ReponseJSON;
    //Renvoie les id eleve
}
