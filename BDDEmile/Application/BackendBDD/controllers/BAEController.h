//
// Created by Emile Guillerm on 06.04.2026.
//

#ifndef BACKENDBDD_BAECONTROLLER_H
#define BACKENDBDD_BAECONTROLLER_H

#include <drogon/HttpController.h>
#include "Logique/SalleLogique.h"
#include <drogon/utils/coroutine.h>



using namespace drogon::orm;

class BAEController : public drogon::HttpController<BAEController>{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(BAEController::EtatSalleBAEController, "/bae/etat_salle", drogon::HttpMethod::Get);
    ADD_METHOD_TO(BAEController::ScannerBadgeBAEController, "/bae/scanner_badge", drogon::HttpMethod::Post);
    METHOD_LIST_END
    static drogon::Task<drogon::HttpResponsePtr> EtatSalleBAEController(
        drogon::HttpRequestPtr req);
    static drogon::Task<drogon::HttpResponsePtr> ScannerBadgeBAEController(
        drogon::HttpRequestPtr req);
};


#endif //BACKENDBDD_BAECONTROLLER_H
