//
// Created by Emile Guillerm on 06.04.2026.
//

#include "PEAController.h"
#include <drogon/HttpController.h>
#include "resultat/StructResultat.h"
#include "Logique/BadgeLogique.h"


using namespace drogon::orm;


drogon::Task<drogon::HttpResponsePtr> PEAController::VerifierBadgePEAController(drogon::HttpRequestPtr req){
    std::string mac = req->getParameter("MAC");
    std::string badgeid = req->getParameter("badgeid");
    if (badgeid.empty() || mac.empty())
    {
        Json::Value MessageErreur;
        MessageErreur = "Requête mal formulée. Veuillez vérifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto utilisateur = co_await BadgeLogique::VerifierBadgePEA(DbClientPtr, badgeid, mac);
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse((utilisateur.BoolResultat));
    ReponseAPI->setStatusCode(drogon::k200OK);
    co_return ReponseAPI;
}