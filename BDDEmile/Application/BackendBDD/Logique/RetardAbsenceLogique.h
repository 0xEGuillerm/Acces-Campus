//
// Created by Emile Guillerm on 05.04.2026.
//

#ifndef BACKENDBDD_RETARDABSENCELOGIQUE_H
#define BACKENDBDD_RETARDABSENCELOGIQUE_H


#include <drogon/utils/coroutine.h>
#include <json/json.h>
#include "dao/RetardAbsenceDAO.h"

using namespace drogon::orm;

class RetardAbsenceLogique{
public:
    //Obtenir les asence d'un élève
    //Renvoie un json avec une liste comportant:
    //nb_absencereatrd
    //timestamp_debut_cours
    //temps_retard (0 si absence)
    //absence_ou_retard (bool)
    static drogon::Task<ResultatCoro<Json::Value>> AbsenceEleve(
        //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
        const DbClientPtr &db,
        //id utiulisateur
        const int32_t &idUtilisateur);
};

#endif //BACKENDBDD_RETARDABSENCELOGIQUE_H