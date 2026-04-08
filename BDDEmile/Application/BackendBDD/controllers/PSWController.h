//
// Created by Emile Guillerm on 06.04.2026.
//

#ifndef BACKENDBDD_PSWCONTROLLER_H
#define BACKENDBDD_PSWCONTROLLER_H

#include <drogon/HttpController.h>
#include "resultat/StructResultat.h"
#include "Logique/SalleLogique.h"
#include "Logique/CoursLogique.h"
#include "Logique/RetardAbsenceLogique.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include <json/json.h>


using namespace drogon::orm;

class PSWController : public drogon::HttpController<PSWController>{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(PSWController::HistoriqueElevePSWController, "/psw/historique_eleve", drogon::HttpMethod::Get);
    ADD_METHOD_TO(PSWController::SalleCrenauxPSWController, "/psw/salle_crenaux", drogon::HttpMethod::Get);
    ADD_METHOD_TO(PSWController::ReserverSallePSWController, "/psw/reserver_salle", drogon::HttpMethod::Post);
    ADD_METHOD_TO(PSWController::PlanningSallePSWController, "/psw/planning", drogon::HttpMethod::Get);
    ADD_METHOD_TO(PSWController::EtatSallePSWController, "/psw/etat_salle", drogon::HttpMethod::Get);
    METHOD_LIST_END
    drogon::Task<drogon::HttpResponsePtr> HistoriqueElevePSWController(
        drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> SalleCrenauxPSWController(
        drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> ReserverSallePSWController(
        drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> PlanningSallePSWController(
        drogon::HttpRequestPtr req);
    drogon::Task<drogon::HttpResponsePtr> EtatSallePSWController(
        drogon::HttpRequestPtr req);
};


#endif //BACKENDBDD_PSWCONTROLLER_H
