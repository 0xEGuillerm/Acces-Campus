//
// Created by Emile Guillerm on 05.04.2026.
//

#include "RetardAbsenceLogique.h"
#include <drogon/utils/coroutine.h>
#include "PresenceCours.h"
#include <json/json.h>
#include "dao/RetardAbsenceDAO.h"

using namespace drogon::orm;


//Obtenir les asence d'un élève
//Renvoie un json avec une liste comportant:
//nb_absencereatrd
//timestamp_debut_cours
//temps_retard (0 si absence)
//absence_ou_retard (bool)
drogon::Task<ResultatCoro<Json::Value>> RetardAbsenceLogique::AbsenceEleve(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //id utiulisateur
    const int32_t &idUtilisateur) {
    ResultatCoro<Json::Value> resultat;
    Json::Value listeAbsence(Json::arrayValue);

    //Recherche de toutes les absence de l'élève renvoie un vecteur d'absenceretard
    auto AbsenceTable = co_await RetardAbsenceDAO::ChercherRetardAbsenceEleve(db, idUtilisateur);
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (AbsenceTable.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur";
        co_return resultat;
    }
    //Renvoie le nombre d'absence en resultat si 0
    if (AbsenceTable.donnee.empty()) {
        resultat.BoolResultat = true;
        resultat.donnee["nb_absence"] = 0;
        co_return resultat;
    }
    //Autrement met le nombre d'absence
    else {
        resultat.donnee["nb_absence"] = static_cast<int>(AbsenceTable.donnee.size());
    }
    //Ajout de chaque absence avec lkes donne à une liste puis l'ajout au resultat final
    for (const auto& Absence : AbsenceTable.donnee){
        Json::Value AbsenceTemp;
        AbsenceTemp["timestamp_debut_cours"] = static_cast<Json::Int64>(Absence.getValueOfTimestampHeureCours().secondsSinceEpoch());
        AbsenceTemp["temps_retard"] = static_cast<Json::Int64>(Absence.getValueOfTempsRetardMin());
        AbsenceTemp["absence"] = Absence.getValueOfAbsence();
        listeAbsence.append(AbsenceTemp);
    }
    resultat.BoolResultat = true;
    resultat.donnee["listeAbsence"] = listeAbsence;
    co_return resultat;
}