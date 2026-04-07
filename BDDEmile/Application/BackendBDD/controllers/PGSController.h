//
// Created by Emile Guillerm on 06.04.2026.
//

#ifndef BACKENDBDD_PGSCONTROLLER_H
#define BACKENDBDD_PGSCONTROLLER_H

#include <drogon/HttpController.h>
#include "resultat/StructResultat.h"
#include "Logique/BadgeLogique.h"
#include "Logique/SalleLogique.h"
#include "Logique/CoursLogique.h"
#include "Logique/RetardAbsenceLogique.h"
#include "Logique/UtilisateurLogique.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include <json/json.h>


using namespace drogon::orm;

class PGSController : public drogon::HttpController<PGSController>{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(PGSController::PlanningSallePGSController, "/pgs/planning", drogon::HttpMethod::Get);
    ADD_METHOD_TO(PGSController::EtatSallePGSController, "/pgs/etat_salle", drogon::HttpMethod::Get);
    ADD_METHOD_TO(PGSController::RechercheUtilisateurPGSController, "/pgs/recherche_utilisateur", drogon::HttpMethod::Get);
    ADD_METHOD_TO(PGSController::HistoriqueBadgePGSController, "/pgs/historique_badge", drogon::HttpMethod::Get);
    ADD_METHOD_TO(PGSController::ReserverSallePGSController, "/pgs/reserver_salle", drogon::HttpMethod::Post);
    ADD_METHOD_TO(PGSController::InformationBadgePGSController, "/pgs/badges/{badgeid}", drogon::HttpMethod::Get);
    ADD_METHOD_TO(PGSController::SupprimerBadgePGSController, "/pgs/badges/{badgeid}", drogon::HttpMethod::Delete);
    ADD_METHOD_TO(PGSController::CreationBadgePGSController, "/pgs/badges/{badgeid}", drogon::HttpMethod::Post);
    ADD_METHOD_TO(PGSController::ModifierBadgePGSController, "/pgs/badges/{badgeid}", drogon::HttpMethod::Put);
    METHOD_LIST_END
    drogon::Task<drogon::HttpResponsePtr> PlanningSallePGSController(
        drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> EtatSallePGSController(
        drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> RechercheUtilisateurPGSController(
        drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> HistoriqueBadgePGSController(
        drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> ReserverSallePGSController(
        drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> InformationBadgePGSController(
        drogon::HttpRequestPtr req,
        std::string badgeid);
    drogon::Task<drogon::HttpResponsePtr> SupprimerBadgePGSController(
        drogon::HttpRequestPtr req,
        std::string badgeid);
    drogon::Task<drogon::HttpResponsePtr> CreationBadgePGSController(
        drogon::HttpRequestPtr req,
        std::string badgeid);
    drogon::Task<drogon::HttpResponsePtr> ModifierBadgePGSController(
        drogon::HttpRequestPtr req,
        std::string badgeid);
};


#endif //BACKENDBDD_PGSCONTROLLER_H
