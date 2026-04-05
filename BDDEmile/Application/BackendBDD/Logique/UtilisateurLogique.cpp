//
// Created by Emile Guillerm on 05.04.2026.
//

#include "UtilisateurLogique.h"

#include <json/value.h>
#include <drogon/HttpController.h>
#include "dao/UtilisateurDAO.h"



using namespace drogon::orm;

static drogon::Task<Json::Value> UtilisateurParNomPrenom(
    const DbClientPtr &db,
    const std::string &nom,
    const std::string &prenom) {
    Json::Value ReponseJSON;
    Json::Value listeprenom(Json::arrayValue);
    Json::Value listenom(Json::arrayValue);
    if (prenom != "") {
        auto ListeUtilisateurPrenom = co_await UtilisateurDAO::ChercherUtilisateurParPrenom(db, prenom);
        for (const auto& utilisateur : ListeUtilisateurPrenom) {
            Json::Value user;
            user["id_utilisateur"] = (*utilisateur.getIdUser());
            user["prenom_utilisateur"] = utilisateur.getValueOfPrenomUser();
            user["nom_utilisateur"] = utilisateur.getValueOfNomUser();
            listeprenom.append(user);
        }
    }
    if (nom != "") {
        auto ListeUtilisateurNom = co_await UtilisateurDAO::ChercherUtilisateurParNom(db, nom);
        for (const auto& utilisateur : ListeUtilisateurNom) {
            Json::Value user;
            user["id_utilisateur"] = (*utilisateur.getIdUser());
            user["prenom_utilisateur"] = utilisateur.getValueOfPrenomUser();
            user["nom_utilisateur"] = utilisateur.getValueOfNomUser();
            listenom.append(user);
        }
    }
    ReponseJSON["Recherche_prenom"] = listeprenom;
    ReponseJSON["Recherche_nom"] = listenom;
    co_return ReponseJSON;
}
