//
// Created by Emile Guillerm on 04.04.2026.
//

#ifndef BACKENDBDD_SALLELOGIQUE_H
#define BACKENDBDD_SALLELOGIQUE_H

#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include "models/Utilisateur.h"
#include <json/json.h>


using namespace drogon::orm;

class SalleLogique {
public:
    static drogon::Task<Json::Value> EtatSalleMac(
    const DbClientPtr &db,
    const std::string &mac);

    static drogon::Task<Json::Value> EtatSalleNumeroSalle(
        const DbClientPtr &db,
        const std::string &numerosalle);
};


#endif //BACKENDBDD_SALLELOGIQUE_H