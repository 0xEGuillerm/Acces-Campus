//
// Created by Emile Guillerm on 06.04.2026.
//

#include "BAEController.h"

#include <drogon/HttpController.h>
#include "resultat/StructResultat.h"
#include "Logique/BadgeLogique.h"
#include "Logique/SalleLogique.h"


using namespace drogon::orm;

//bae/etat_salle
//Prend l'adresse MAC de la salle dans laquelle ce trouve la BAE
//En cas de réussite renvoie les infos de l'élèves
//En cas d'échec renvoie un code d'erreur
//Utilisation de task pour l'asynchrone
drogon::Task<drogon::HttpResponsePtr> BAEController::EtatSalleBAEController(drogon::HttpRequestPtr req){
    std::string mac = req->getParameter("MAC");
    //Vérifie la présence d'une adresse MAC
    if (mac.empty())
    {
        //Création du message d'erreur au format JSON
        Json::Value MessageErreur;
        //Ecriture du message conformément à la documentation
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        //Fabrication de la réponse HTTP avec le message JSON
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        //Mettre le statut de la réponse HTTP à 404
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        //Retour de la réponse Asynchrone
        co_return ReponseAPI;
    }
    //Obtient l'id de la connexion
    auto DbClientPtr = drogon::app().getDbClient();
    //Envoie des informations à la couche logique pour traiter la requête
    //Recois un JSON
    auto etatSalle = co_await SalleLogique::EtatSalleMac(DbClientPtr, mac);
    //Verifie le résultat pour renvoyer une erreur même si vide
    if (etatSalle.BoolResultat == false)
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(etatSalle.MessageResultat);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    //Création de la réponse HTTP
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(etatSalle.donnee);
    //Mise en place du code d'erreur à 200 OK
    ReponseAPI->setStatusCode(drogon::k200OK);
    //Envoie de la réponse
    co_return ReponseAPI;
}

//bae/scanner_badge
//Prend l'adresse MAC de la salle dans laquelle il y'a le scanne
//En cas de réussite renvoie les infos de l'élèves
//En cas d'échec renvoie un code d'erreur
//Utilisation de task pour l'asynchrone
drogon::Task<drogon::HttpResponsePtr> BAEController::ScannerBadgeBAEController(drogon::HttpRequestPtr req){
    //Récuperation des informations dans le body de la requête HTTP
    auto body = req->getJsonObject();
    //Verification que les champs correspondant bien a ce qui est attendu
    if (!body || !body->isMember("uuid") || !body->isMember("MAC") || !body->isMember("heure_badge"))
    {
        //Autrement erreur
        Json::Value MessageErreur;
        MessageErreur = "Requete mal formulee. Veuillez verifier les champs.";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k400BadRequest);
        co_return ReponseAPI;
    }
    //Recupération des champs dans des variables pour pouvoir les traitées facilement
    std::string uuid = (*body)["uuid"].asString();
    std::string mac = (*body)["MAC"].asString();
    std::uint64_t heure_badge = (*body)["heure_badge"].asUInt64();
    //Création de la connexion vers la db
    auto DbClientPtr = drogon::app().getDbClient();
    //Appel de la logique avec la méthode correspondante en envoyant l'uid du badge et l'adresse mac du BAE
    auto resultat = co_await BadgeLogique::ScanneBadgeBAE(DbClientPtr, uuid, mac, heure_badge);
    if (resultat.BoolResultat == false)
    {
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.MessageResultat);
        ReponseAPI->setStatusCode(drogon::k404NotFound);
        co_return ReponseAPI;
    }
    //Création de la réponse HTTP avec les informations de l'utilisateur qui à scanner renvoyer par la méthode badgeLogique
    auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(resultat.donnee);
    ReponseAPI->setStatusCode(drogon::k200OK);
    co_return ReponseAPI;
}
