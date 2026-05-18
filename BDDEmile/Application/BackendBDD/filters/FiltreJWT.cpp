//
// Created by Emile Guillerm on 07.05.2026.
//

#include "FiltreJWT.h"

#include "jwt-cpp/traits/kazuho-picojson/defaults.h"

const std::string CLEE_CHIFFREMENT_JWT = "password";

//Filtre pour vérifier le token pour les endpoint liée au PSW (sauf login)
//Dois override les fonction native de doFilter
drogon::Task<drogon::HttpResponsePtr> FiltreJWT::doFilter(const drogon::HttpRequestPtr &req) {
    //Recuperation du token
    std::string token = req->getHeader("Authorization");
    //Verification de la presence d'un token
    if (token.empty() || token.rfind("Bearer ", 0) != 0) {
        Json::Value MessageErreur;
        MessageErreur = "Token invalide";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k401Unauthorized);
        co_return ReponseAPI;
    }
    std::string tokenverifier = token.substr(7);
    //Méthode pouvant renvoyer des erreurs donc try/catch
    try {
        //Base 64 -> string donnée
        jwt::decoded_jwt<jwt::traits::kazuho_picojson> tokendecodee = jwt::decode(tokenverifier);
        //Creation de la verification avec les bon paramètres (Créateur, algorithme)
        auto ParametreDeVerification = jwt::verify()
        .with_issuer("acces-campus")
        .allow_algorithm(jwt::algorithm::hs256{CLEE_CHIFFREMENT_JWT});
        //Verification du token de la requête
        ParametreDeVerification.verify(tokendecodee);
        //Si la fonction au dessus n'a pas lancer d'exception cela signifie que la verification c'est passé avec succès
        std::string UtilisateurIDstring = tokendecodee.get_subject();
        int32_t UtilisateurIDint = std::stoi(UtilisateurIDstring);
        req->attributes()->insert("idUtilisateur", UtilisateurIDint);
        //On return rien car on veut que la requpête atteigne le controller
        co_return nullptr;

    //Erreur dans différents cas
    } catch (const std::invalid_argument&){
        //Echec car le token n'a pas les trois partie séparé par des points (decodage)
        Json::Value MessageErreur;
        MessageErreur = "Token invalide";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k401Unauthorized);
        co_return ReponseAPI;
    } catch (const jwt::error::token_verification_exception&){
        //Echec car le token est invalide (verification)
        Json::Value MessageErreur;
        MessageErreur = "Token invalide";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k401Unauthorized);
        co_return ReponseAPI;
    } catch (const std::runtime_error&) {
        //Echec du decodage (decodage)
        Json::Value MessageErreur;
        MessageErreur = "Token invalide";
        auto ReponseAPI = drogon::HttpResponse::newHttpJsonResponse(MessageErreur);
        ReponseAPI->setStatusCode(drogon::k401Unauthorized);
        co_return ReponseAPI;
    }
}

