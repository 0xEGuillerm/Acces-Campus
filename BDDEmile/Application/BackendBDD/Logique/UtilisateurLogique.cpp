//
// Created by Emile Guillerm on 05.04.2026.
//

#include "UtilisateurLogique.h"

#include <json/value.h>
#include <drogon/HttpController.h>
#include "dao/UtilisateurDAO.h"



using namespace drogon::orm;


//Fonction de recherche utilisateur par prenom
drogon::Task<Json::Value> UtilisateurLogique::UtilisateurParNomPrenom(
    //Parametre nécessaire pour la recherche
    const DbClientPtr &db,
    const std::string &nom,
    const std::string &prenom) {
    //Instanciation des variable pour la réponse format JSON
    Json::Value ReponseJSON;
    Json::Value listeprenom(Json::arrayValue);
    Json::Value listenom(Json::arrayValue);
    //Verification de la présence de champ pour éviter les erreurs
    if (prenom != "") {
        //Fonction couroutine pour la recherche avec la DAO
        auto ListeUtilisateurPrenom = co_await UtilisateurDAO::ChercherUtilisateurParPrenom(db, prenom);
        //Verifciation et ajout des donné fournit apres la recherche dans le JSON
        for (const auto& utilisateur : ListeUtilisateurPrenom) {
            Json::Value user;
            user["id_utilisateur"] = (*utilisateur.getIdUser());
            user["prenom_utilisateur"] = utilisateur.getValueOfPrenomUser();
            user["nom_utilisateur"] = utilisateur.getValueOfNomUser();
            listeprenom.append(user);
        }
    }
    //Verification de la présence de champ pour éviter les erreurs
    if (nom != "") {
        auto ListeUtilisateurNom = co_await UtilisateurDAO::ChercherUtilisateurParNom(db, nom);
        //Verifciation et ajout des donné fournit apres la recherche dans le JSON
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
    //Retour de la coroutines
    co_return ReponseJSON;
}
