//
// Created by Emile Guillerm on 06.04.2026.
//

#include "PGSController.h"

#include <drogon/HttpController.h>
#include "resultat/StructResultat.h"
#include "Logique/BadgeLogique.h"
#include "Logique/SalleLogique.h"
#include "Logique/CoursLogique.h"
#include "Logique/RetardAbsenceLogique.h"
#include "Logique/UtilisateurLogique.h"


using namespace drogon::orm;


drogon::Task<drogon::HttpResponsePtr> PGSController::PlanningSallePGSController(drogon::HttpRequestPtr req){
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
    auto utilisateurs = co_await UtilisateurLogique::UtilisateurParNomPrenom(DbClientPtr, nom, prenom);
    if (utilisateurs.isNull())
    {
        Json::Value MessageErreur;
        MessageErreur = "Aucun utilisateur trouve.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(utilisateurs);
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
    int32_t idUtilisateur = stoi(uuidUserStr);
    auto DbClientPtr = drogon::app().getDbClient();
    auto historique = co_await RetardAbsenceLogique::AbsenceEleve(DbClientPtr, idUtilisateur);
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


drogon::Task<drogon::HttpResponsePtr> PGSController::ReserverSallePGSController(drogon::HttpRequestPtr req){
    auto body = req->getJsonObject();
    if (!body || !body->isMember("heure_debut") || !body->isMember("heure_fin") || !body->isMember("salle") || !body->isMember("classe"))
    {
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    auto DbClientPtr = drogon::app().getDbClient();
    auto resultat = co_await CoursLogique::ReservationSallePGS(DbClientPtr, *body);
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
    auto utilisateur = co_await BadgeLogique::InformationBadge(DbClientPtr, badgeid);
    if (utilisateur.isNull())
    {
        Json::Value MessageErreur;
        MessageErreur = "Badge inconnu.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(utilisateur);
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
    if (!resultat.BoolResultat)
    {
        Json::Value MessageErreur;
        MessageErreur = resultat.MessageResultat;
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
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
    if (!resultat.BoolResultat)
    {
        Json::Value MessageErreur;
        MessageErreur = resultat.MessageResultat;
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    auto ReponseAPI = drogon::HttpResponse::newHttpResponse();
    ReponseAPI->setStatusCode(drogon::k204NoContent);
    co_return ReponseAPI;
}
