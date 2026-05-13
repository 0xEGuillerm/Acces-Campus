//
// Created by Emile Guillerm on 06.04.2026.
//

#include "PGSController.h"

#include <drogon/HttpController.h>
#include "resultat/StructResultat.h"
#include "Logique/BadgeLogique.h"
#include "Logique/ClasseLogique.h"
#include "Logique/SalleLogique.h"
#include "Logique/CoursLogique.h"
#include "Logique/RetardAbsenceLogique.h"
#include "Logique/UtilisateurLogique.h"


using namespace drogon::orm;


drogon::Task<drogon::HttpResponsePtr> PGSController::PlanningSallePGSController(drogon::HttpRequestPtr req){
    std::string salleStr = req->getParameter("salle");
    int64_t timestamp_debut;
    int64_t timestamp_fin;

    try {
        timestamp_debut= std::stoll(req->getParameter("debut"));
        timestamp_fin= std::stoll(req->getParameter("fin"));

    } catch (const std::invalid_argument&) {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    if (salleStr.empty() || timestamp_debut == 0 || timestamp_fin == 0)
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await CoursLogique::PlanningSalle(DbClientPtr, salleStr, timestamp_debut, timestamp_fin);
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


drogon::Task<drogon::HttpResponsePtr> PGSController::EtatSallePGSController(drogon::HttpRequestPtr req){
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


drogon::Task<drogon::HttpResponsePtr> PGSController::RechercheUtilisateurPGSController(drogon::HttpRequestPtr req){
    std::string prenom = req->getParameter("prenom");
    std::string nom = req->getParameter("nom");
    if (prenom.empty() && nom.empty())
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await UtilisateurLogique::UtilisateurParNomPrenom(DbClientPtr, nom, prenom);
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


drogon::Task<drogon::HttpResponsePtr> PGSController::HistoriqueBadgePGSController(drogon::HttpRequestPtr req){
    std::string uuidUserStr = req->getParameter("uuid_user");
    if (uuidUserStr.empty())
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    int32_t idUtilisateur;
    try {
        idUtilisateur = stoi(uuidUserStr);
    } catch (const std::invalid_argument&) {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await RetardAbsenceLogique::AbsenceEleve(DbClientPtr, idUtilisateur);
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


drogon::Task<drogon::HttpResponsePtr> PGSController::ReserverSallePGSController(drogon::HttpRequestPtr req){
    auto body = req->getJsonObject();
    if (!body || !body->isMember("heure_debut") || !body->isMember("heure_fin") || !body->isMember("salle") || !body->isMember("classe") || !body->isMember("professeur"))
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await CoursLogique::ReservationSallePGS(DbClientPtr, *body);
    if (resultat.BoolResultat == false)
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.MessageResultat);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpResponse();
    ReponseAPI->setStatusCode(drogon::k204NoContent);
    co_return ReponseAPI;
}


drogon::Task<drogon::HttpResponsePtr> PGSController::InformationBadgePGSController(drogon::HttpRequestPtr req, std::string badgeid){
    if (badgeid.empty())
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await BadgeLogique::InformationBadge(DbClientPtr, badgeid);
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


drogon::Task<drogon::HttpResponsePtr> PGSController::SupprimerBadgePGSController(drogon::HttpRequestPtr req, std::string badgeid){
    if (badgeid.empty())
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await BadgeLogique::SupprimerBadge(DbClientPtr, badgeid);
    if (resultat.BoolResultat == false)
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.MessageResultat);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpResponse();
    ReponseAPI->setStatusCode(drogon::k204NoContent);
    co_return ReponseAPI;
}


drogon::Task<drogon::HttpResponsePtr> PGSController::CreationBadgePGSController(drogon::HttpRequestPtr req, std::string badgeid){
    auto body = req->getJsonObject();
    if (badgeid.empty() || !body || !body->isMember("uuid_user"))
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    int32_t uuidUser = (*body)["uuid_user"].asInt();
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await BadgeLogique::CreationBadge(DbClientPtr, uuidUser, badgeid);
    if (resultat.BoolResultat == false)
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.MessageResultat);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpResponse();
    ReponseAPI->setStatusCode(drogon::k204NoContent);
    co_return ReponseAPI;
}


drogon::Task<drogon::HttpResponsePtr> PGSController::ModifierBadgePGSController(drogon::HttpRequestPtr req, std::string badgeid){
    auto body = req->getJsonObject();
    if (badgeid.empty() || !body)
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await BadgeLogique::ModifierInfoUtilisateur(DbClientPtr, badgeid, *body);
    if (resultat.BoolResultat == false)
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.MessageResultat);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpResponse();
    ReponseAPI->setStatusCode(drogon::k204NoContent);
    co_return ReponseAPI;
}

drogon::Task<drogon::HttpResponsePtr> PGSController::ListeSalleExistante(drogon::HttpRequestPtr req){
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

drogon::Task<drogon::HttpResponsePtr> PGSController::ListeProfesseur(drogon::HttpRequestPtr req){
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await UtilisateurLogique::ListeProfesseur(DbClientPtr);
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

drogon::Task<drogon::HttpResponsePtr> PGSController::ListeClasse(drogon::HttpRequestPtr req){
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await ClasseLogique::ListeClasse(DbClientPtr);
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