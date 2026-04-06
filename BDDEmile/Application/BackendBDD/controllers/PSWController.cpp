//
// Created by Emile Guillerm on 06.04.2026.
//

#include "PSWController.h"

#include <drogon/HttpController.h>
#include "resultat/StructResultat.h"
#include "Logique/SalleLogique.h"
#include "Logique/CoursLogique.h"
#include "Logique/RetardAbsenceLogique.h"


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
    auto historique = co_await RetardAbsenceLogique::AbsenceEleve(DbClientPtr, utilisateur);
    if (historique.isNull())
    {
        Json::Value MessageErreur;
        MessageErreur = "Aucun historique trouve.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(historique);
    ReponseAPI->setStatusCode(drogon::k200OK);
    co_return ReponseAPI;
}


drogon::Task<drogon::HttpResponsePtr> PSWController::SalleCrenauxPSWController(drogon::HttpRequestPtr req){
    std::string salleStr = req->getParameter("salle");
    if (salleStr.empty())
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    int32_t salle = stoi(salleStr);
    auto DbClientPtr = drogon::app().getDbClient();
    auto planning = co_await CoursLogique::PlanningSalle(DbClientPtr, salle);
    if (planning.isNull())
    {
        Json::Value MessageErreur;
        MessageErreur = "Aucun cours trouve pour cette salle.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(planning);
    ReponseAPI->setStatusCode(drogon::k200OK);
    co_return ReponseAPI;
}


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


drogon::Task<drogon::HttpResponsePtr> PSWController::PlanningSallePSWController(drogon::HttpRequestPtr req){
    std::string salleStr = req->getParameter("salle");
    if (salleStr.empty())
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    int32_t salle = stoi(salleStr);
    auto DbClientPtr = drogon::app().getDbClient();
    auto planning = co_await CoursLogique::PlanningSalle(DbClientPtr, salle);
    if (planning.isNull())
    {
        Json::Value MessageErreur;
        MessageErreur = "Aucun cours trouve pour cette salle.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(planning);
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
    auto etatSalle = co_await SalleLogique::EtatSalleNumeroSalle(DbClientPtr, salleStr);
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
