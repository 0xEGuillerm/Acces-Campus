//
// Created by Emile Guillerm on 04.04.2026.
//

#ifndef BACKENDBDD_SALLELOGIQUE_H
#define BACKENDBDD_SALLELOGIQUE_H

#include "SalleLogique.h"
#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include "models/Utilisateur.h"
#include <json/json.h>
#include "dao/SalleDAO.h"


using namespace drogon::orm;

class SalleLogique {
public:
    //Renvoi les info du cours à partir de l'adresse mac du bae
    //Json avec
    //classe
    //duree
    //enseignant
    //nb_absent
    //nb_present
    //liste_absent
    //liste_present
    //Renvoi "Aucun cours" si pas de cours
    //peut renvoyer des erreur
    static drogon::Task<ResultatCoro<Json::Value>> EtatSalleMac(
    const DbClientPtr &db,
    const std::string &mac);

    //Renvoi les info du cours à partir d'une salle
    //Json avec
    //classe
    //duree
    //enseignant
    //nb_absent
    //nb_present
    //liste_absent
    //liste_present
    //Renvoi "Aucun cours" si pas de cours
    //peut renvoyer des erreur
    static drogon::Task<ResultatCoro<Json::Value>> EtatSalleNumeroSalle(
    const DbClientPtr &db,
    const std::string &salle);

    //renvoie toutes les salles
    //JSON liste salle
    //nom_salle
    //peut renvoyer des erreur
    static drogon::Task<ResultatCoro<Json::Value>> ListedeSalle(
        const DbClientPtr &db);
};


#endif //BACKENDBDD_SALLELOGIQUE_H