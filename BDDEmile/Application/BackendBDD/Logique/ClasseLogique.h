//
// Created by Emile Guillerm on 13.05.2026.
//

#ifndef BACKENDBDD_CLASSELOGIQUE_H
#define BACKENDBDD_CLASSELOGIQUE_H

#include <drogon/orm/DbClient.h>
#include <json/json.h>
#include "resultat/StructResultat.h"

#include "dao/ClasseDAO.h"

class ClasseLogique {
public:
    //renvoie toutes les classe
    //JSON liste Classe
    //nom_classe
    //id_Classe
    //peut renvoyer des erreur
    static drogon::Task<ResultatCoro<Json::Value>> ListeClasse(
        const DbClientPtr &db) ;
};


#endif //BACKENDBDD_CLASSELOGIQUE_H


