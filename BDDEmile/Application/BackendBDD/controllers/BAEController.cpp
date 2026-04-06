//
// Created by Emile Guillerm on 06.04.2026.
//

#include "BAEController.h"

#include <drogon/HttpController.h>
#include "resultat/StructResultat.h"
#include "Logique/BadgeLogique.h"
#include "Logique/SalleLogique.h"


using namespace drogon::orm;


drogon::Task<drogon::HttpResponsePtr> BAEController::EtatSalleBAEController(drogon::HttpRequestPtr req){
    std::string mac = req->getParameter("MAC");
    if (mac.empty())
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto etatSalle = co_await SalleLogique::EtatSalleMac(DbClientPtr, mac);
    if (etatSalle.isNull())
    {
        Json::Value MessageErreur;
        MessageErreur = "Salle introuvable ou aucun cours en cours.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(etatSalle);
    ReponseAPI->setStatusCode(drogon::k200OK);
    co_return ReponseAPI;
}


drogon::Task<drogon::HttpResponsePtr> BAEController::ScannerBadgeBAEController(drogon::HttpRequestPtr req){
    auto body = req->getJsonObject();
    if (!body || !body->isMember("uuid") || !body->isMember("MAC") || !body->isMember("heure_badge"))
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    std::string uuid = (*body)["uuid"].asString();
    std::string mac = (*body)["MAC"].asString();
    std::uint64_t heure_badge = (*body)["heure_badge"].asUInt64();
    if (uuid.empty() || mac.empty())
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await BadgeLogique::ScanneBadgeBAE(DbClientPtr, uuid, mac, heure_badge);
    if (!resultat.BoolResultat)
    {
        Json::Value MessageErreur;
        MessageErreur = resultat.MessageResultat;
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.BoolResultat);
    ReponseAPI->setStatusCode(drogon::k200OK);
    co_return ReponseAPI;
}
