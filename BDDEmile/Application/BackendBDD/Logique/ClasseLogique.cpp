//
// Created by Emile Guillerm on 13.05.2026.
//

#include "ClasseLogique.h"
#include <drogon/orm/DbClient.h>
#include "models/Utilisateur.h"  // Classe générée par drogon_ctl
#include <json/json.h>
#include "resultat/StructResultat.h"
#include "dao/ClasseDAO.h"

//renvoie toutes les classe
//JSON liste Classe
//nom_classe
//id_Classe
//peut renvoyer des erreur
drogon::Task<ResultatCoro<Json::Value>> ClasseLogique::ListeClasse(
    const drogon::orm::DbClientPtr &db) {
    //Création de la structure réponse
    ResultatCoro<Json::Value> resultat;
    //Creation des liste Classe
    Json::Value listeClasse(Json::arrayValue);
    //obtention de toutes les Classe vecteur
    auto VecteurClasse = co_await ClasseDAO::ListeClasse(db);
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (VecteurClasse.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur";
        co_return resultat;
    }
    for (const auto& classe : VecteurClasse.donnee){
        Json::Value ClasseTemp;
        ClasseTemp["nom_classe"] = classe.getValueOfNomClasse();
        ClasseTemp["id_classe"] = classe.getValueOfIdClasse();
        listeClasse.append(ClasseTemp);
    }
    resultat.donnee = listeClasse;
    resultat.BoolResultat = true;
    co_return resultat;
}