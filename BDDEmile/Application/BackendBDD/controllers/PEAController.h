//
// Created by Emile Guillerm on 06.04.2026.
//

#ifndef BACKENDBDD_PEACONTROLLER_H
#define BACKENDBDD_PEACONTROLLER_H

#include <drogon/HttpController.h>
#include "resultat/StructResultat.h"
#include "Logique/BadgeLogique.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include <json/json.h>


using namespace drogon::orm;

class PEAController : public drogon::HttpController<PEAController>{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(PEAController::VerifierBadgePEAController, "/pea/verif_badge", drogon::HttpMethod::Get);
    METHOD_LIST_END
drogon::Task<drogon::HttpResponsePtr> VerifierBadgePEAController(
    drogon::HttpRequestPtr req);
};


#endif //BACKENDBDD_PEACONTROLLER_H
