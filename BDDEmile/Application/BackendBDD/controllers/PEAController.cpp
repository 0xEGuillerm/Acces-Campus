//
// Created by Emile Guillerm on 06.04.2026.
//

#include "PEAController.h"
#include <drogon/HttpController.h>
#include "resultat/StructResultat.h"
#include "Logique/BadgeLogique.h"


using namespace drogon::orm;

//bae/scanner_badge
//Prend l'adresse MAC du PEA
//En cas de réussite renvoie les infos de l'élèves
//En cas d'échec renvoie un code d'erreur
//Utilisation de task pour l'asynchrone
drogon::Task<drogon::HttpResponsePtr> PEAController::VerifierBadgePEAController(drogon::HttpRequestPtr req){
    std::string mac = req->getParameter("MAC");
    std::string badgeid = req->getParameter("badgeid");
    if (badgeid.empty() || mac.empty())
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse("Requête mal formulée. Veuillez vérifier les champs.");
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await BadgeLogique::VerifierBadgePEA(DbClientPtr, badgeid, mac);
    if (resultat.BoolResultat == false)
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.MessageResultat);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse((resultat.donnee));
    ReponseAPI->setStatusCode(drogon::k200OK);
    co_return ReponseAPI;
}