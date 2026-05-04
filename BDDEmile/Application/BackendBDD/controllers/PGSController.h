//
// Created by Emile Guillerm on 06.04.2026.
//

#ifndef BACKENDBDD_PGSCONTROLLER_H
#define BACKENDBDD_PGSCONTROLLER_H

#include <drogon/HttpController.h>
#include "Logique/SalleLogique.h"
#include <drogon/utils/coroutine.h>



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
    static drogon::Task<drogon::HttpResponsePtr> PlanningSallePGSController(
        drogon::HttpRequestPtr req);
    static drogon::Task<drogon::HttpResponsePtr> EtatSallePGSController(
        drogon::HttpRequestPtr req);
    static drogon::Task<drogon::HttpResponsePtr> RechercheUtilisateurPGSController(
        drogon::HttpRequestPtr req);
    static drogon::Task<drogon::HttpResponsePtr> HistoriqueBadgePGSController(
        drogon::HttpRequestPtr req);
    static drogon::Task<drogon::HttpResponsePtr> ReserverSallePGSController(
        drogon::HttpRequestPtr req);
    static drogon::Task<drogon::HttpResponsePtr> InformationBadgePGSController(
        drogon::HttpRequestPtr req,
        std::string badgeid);
    static drogon::Task<drogon::HttpResponsePtr> SupprimerBadgePGSController(
        drogon::HttpRequestPtr req,
        std::string badgeid);
    static drogon::Task<drogon::HttpResponsePtr> CreationBadgePGSController(
        drogon::HttpRequestPtr req,
        std::string badgeid);
    static drogon::Task<drogon::HttpResponsePtr> ModifierBadgePGSController(
        drogon::HttpRequestPtr req,
        std::string badgeid);
};


#endif //BACKENDBDD_PGSCONTROLLER_H
