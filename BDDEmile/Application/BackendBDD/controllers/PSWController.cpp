//
// Created by Emile Guillerm on 06.04.2026.
//

#include "PSWController.h"
#include <drogon/HttpController.h>
#include "resultat/StructResultat.h"
#include "Logique/SalleLogique.h"
#include "Logique/CoursLogique.h"
#include "Logique/RetardAbsenceLogique.h"
#include "Logique/UtilisateurLogique.h"


using namespace drogon::orm;

drogon::Task<drogon::HttpResponsePtr> PSWController::HistoriqueElevePSWController(drogon::HttpRequestPtr req){
    std::string utilisateurStr = req->getParameter("utilisateur");
    if (utilisateurStr.empty())
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    int32_t utilisateur = stoi(utilisateurStr);
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await RetardAbsenceLogique::AbsenceEleve(DbClientPtr, utilisateur);
    if (resultat.BoolResultat == false)
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.MessageResultat);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.donnee);
    ReponseAPI->setStatusCode(drogon::k200OK);
    co_return ReponseAPI;
}


drogon::Task<drogon::HttpResponsePtr> PSWController::SalleCrenauxPSWController(drogon::HttpRequestPtr req){
    std::string salle = req->getParameter("salle");
    int64_t timestamp_debut= std::stoll(req->getParameter("debut"));
    int64_t timestamp_fin= std::stoll(req->getParameter("fin"));
    if (salle.empty() || timestamp_debut == 0 || timestamp_fin == 0)
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await CoursLogique::SalleDisponible(DbClientPtr, timestamp_debut, timestamp_fin);
    if (resultat.BoolResultat == false)
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.MessageResultat);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.donnee);
    ReponseAPI->setStatusCode(drogon::k200OK);
    co_return ReponseAPI;
}

/*
drogon::Task<drogon::HttpResponsePtr> PSWController::ReserverSallePSWController(drogon::HttpRequestPtr req){
    auto body = req->getJsonObject();
    if (!body || !body->isMember("heure_debut") || !body->isMember("heure_fin") || !body->isMember("salle") || !body->isMember("utilisateur"))
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await CoursLogique::ReservationSallePSW(DbClientPtr, *body);
    if (!resultat.BoolResultat)
    {
        Json::Value MessageErreur;
        MessageErreur = resultat.MessageResultat;
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k403Forbidden);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpResponse();
    ReponseAPI->setStatusCode(drogon::k204NoContent);
    co_return ReponseAPI;
}
*/

drogon::Task<drogon::HttpResponsePtr> PSWController::PlanningSallePSWController(drogon::HttpRequestPtr req){
    std::string salle = req->getParameter("salle");
    int64_t timestamp_debut= std::stoll(req->getParameter("debut"));
    int64_t timestamp_fin= std::stoll(req->getParameter("fin"));
    if (salle.empty())
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await CoursLogique::PlanningSalle(DbClientPtr, salle, timestamp_debut, timestamp_fin);
    if (resultat.BoolResultat == false)
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.MessageResultat);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.donnee);
    ReponseAPI->setStatusCode(drogon::k200OK);
    co_return ReponseAPI;
}


drogon::Task<drogon::HttpResponsePtr> PSWController::EtatSallePSWController(drogon::HttpRequestPtr req){
    std::string salleStr = req->getParameter("salle");
    if (salleStr.empty())
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await SalleLogique::EtatSalleNumeroSalle(DbClientPtr, salleStr);
    if (resultat.BoolResultat == false)
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.MessageResultat);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.donnee);
    ReponseAPI->setStatusCode(drogon::k200OK);
    co_return ReponseAPI;
}

drogon::Task<drogon::HttpResponsePtr> PSWController::LoginPSWController(drogon::HttpRequestPtr req){
    auto body = req->getJsonObject();
    if (!body || !body->isMember("login") || !body->isMember("motdepasse"))
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    std::string login = (*body)["login"].asString();
    std::string motdepasse = (*body)["motdepasse"].asString();
    auto resultat = co_await UtilisateurLogique::LoginPSW(DbClientPtr, login, motdepasse);
    if (resultat.BoolResultat == false)
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.MessageResultat);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.donnee);
    ReponseAPI->setStatusCode(drogon::k200OK);
    co_return ReponseAPI;
}

drogon::Task<drogon::HttpResponsePtr> PSWController::ListeSalleExistante(drogon::HttpRequestPtr req){
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await SalleLogique::ListedeSalle(DbClientPtr);
    if (resultat.BoolResultat == false)
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.MessageResultat);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.donnee);
    ReponseAPI->setStatusCode(drogon::k200OK);
    co_return ReponseAPI;
}