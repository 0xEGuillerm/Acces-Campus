//
// Created by Emile Guillerm on 05.04.2026.
//

#include "UtilisateurLogique.h"
#include "resultat/StructResultat.h"
#include <json/value.h>
#include <drogon/HttpController.h>
#include "dao/UtilisateurDAO.h"
#include <jwt-cpp/jwt.h>
#include <sodium.h>
#include <string>



using namespace drogon::orm;

const std::string CLEE_CHIFFREMENT_JWT = "password";

//Recherche utilisateur par prenom et/ou nom.
//Renvoie une liste d'utilisateur dans un JSON
drogon::Task<ResultatCoro<Json::Value>> UtilisateurLogique::UtilisateurParNomPrenom(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Potentiel nom à chercher
    const std::string &nom,
    //potentiel prénom a chercher
    const std::string &prenom) {
    //Creation des variable pour la réponse format JSON
    ResultatCoro<Json::Value> resultat;
    //Creation des tableau pour la reponse JSON
    Json::Value listeprenom(Json::arrayValue);
    Json::Value listenom(Json::arrayValue);
    //Verification de la présence de champ pour éviter de faire une recherche à champs vide potentiel risque d'erreur
    if (!prenom.empty()) {
        //Fonction couroutine pour la recherche avec la DAO à partir du prenom
        auto ListeUtilisateurPrenom = co_await UtilisateurDAO::ChercherUtilisateurParPrenom(db, prenom);
        //renvoie un bool false et un message d'erreur correspondant si probleme
        if (ListeUtilisateurPrenom.BoolResultat == false) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = "Erreur";
            co_return resultat;
        }
        //Ajout des donné renvoyer par le DAO dans dans le tableau JSON (ajouter en append permis par la boucle)
        for (const auto& utilisateur : ListeUtilisateurPrenom.donnee) {
            Json::Value user;
            user["id_utilisateur"] = (*utilisateur.getIdUser());
            user["prenom_utilisateur"] = utilisateur.getValueOfPrenomUser();
            user["nom_utilisateur"] = utilisateur.getValueOfNomUser();
            listeprenom.append(user);
        }
    }
    //Verification de la présence de champ pour éviter de faire une recherche à champs vide potentiel risque d'erreur
    if (!nom.empty()) {
        //Fonction couroutine pour la recherche avec la DAO à partir du nom
        auto ListeUtilisateurNom = co_await UtilisateurDAO::ChercherUtilisateurParNom(db, nom);
        //renvoie un bool false et un message d'erreur correspondant si probleme
        if (ListeUtilisateurNom.BoolResultat == false) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = "Erreur";
            co_return resultat;
        }
        //Ajout des utilisateur renvoyer par le DAO dans dans le tableau JSON (ajouter en append permis par la boucle)
        for (const auto& utilisateur : ListeUtilisateurNom.donnee) {
            //Booleen pour ne pas enregistre les doublon
            bool doublon = false;
            //Boucle dans la liste prenom pour chercher si l'id d'un nom est déja present
            for (const auto& element : listeprenom) {
                if (element["id_utilisateur"].asInt() == utilisateur.getValueOfIdUser()) {
                    doublon = true;
                    break;
                }
            }
            //Ajout des donnée si n'existe pas deja
            if (!doublon) {
                Json::Value user;
                user["id_utilisateur"] = (*utilisateur.getIdUser());
                user["prenom_utilisateur"] = utilisateur.getValueOfPrenomUser();
                user["nom_utilisateur"] = utilisateur.getValueOfNomUser();
                listenom.append(user);
            }
        }
    }
    //Renvoie une erreur si rien de trouvée
    if (listenom.empty() && listeprenom.empty()) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Aucun utilisateur trouvé";
    }
    //Ajout des deux tableau en 1 json final
    else {
        resultat.BoolResultat = true;
        //Ajout des deux tableau a la reponse JSON finale
        resultat.donnee["recherche_prenom"] = listeprenom;
        resultat.donnee["recherche_nom"] = listenom;
    }
    //Retour de la coroutines (json)
    co_return resultat;
}

//Logique pour le login d'un utilisateur sur le PSW.
//Renvoi Un token JWT dans le cas d'une réussite.
drogon::Task<ResultatCoro<Json::Value>> UtilisateurLogique::LoginPSW(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Login utiulisateur
    const std::string &login,
    //password utilisateur
    const std::string &motdepasse) {
    //Création de la structure réponse
    ResultatCoro<Json::Value> resultat;
    //Verification des champs login et renvoei d'erreur si absent
    if (login.empty()) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Pas de login";
        co_return resultat;
    }
    //Verification des champs mot de passe et renvoie d'erreur si absent
    if (motdepasse.empty()) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Pas de mot de passe";
        co_return resultat;
    }

    //Recherche de l'utilisateur correpondant au login
    auto Utilisateur = co_await UtilisateurDAO::ChercherUtilisateurParLogin(db, login);
    //Verification si un utilisateur à été trouvé
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (Utilisateur.BoolResultat == false || Utilisateur.donnee.empty()) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur";
        co_return resultat;
    }
    //Verification du mot de passe avec compariason de hash
    //Fonction fournit par libsodium
    //Prend le mot de passe à verifier et le hash stocker de l'utilisateur au format char ainsi que la taille
    //Renvoie 0 si il correspond et -1 si non
    int resultat_verif_hash = crypto_pwhash_str_verify(Utilisateur.donnee[0].getHashMdp()->c_str(), motdepasse.c_str(), motdepasse.length());
    //Verification du resultat de la verification
    if (resultat_verif_hash < 0) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Mot de passe incorrecte";
        co_return resultat;
    }
    //Création du token JWT correspondant à l'utilisateur
    auto token = jwt::create()
    //Type Json web token
    .set_type("JWT")
    //celui qui delivre le token
    .set_issuer("auth0")
    //met l'expiration du token
    .set_expires_in(std::chrono::seconds{36000})
    //Le token stock l'id de l'utilisateur
    .set_payload_claim("sub", jwt::claim(std::to_string(Utilisateur.donnee[0].getValueOfIdUser())))
    //Methode pour signer le token (avec clée de chiffrmeent et algorithme hs256)
    .sign(jwt::algorithm::hs256(CLEE_CHIFFREMENT_JWT));
    //Renvoie du resultat
    resultat.BoolResultat = true;
    resultat.donnee["token"]= token;
    co_return resultat;
}
