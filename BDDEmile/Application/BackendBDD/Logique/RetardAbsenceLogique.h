//
// Created by Emile Guillerm on 05.04.2026.
//

#ifndef BACKENDBDD_RETARDABSENCELOGIQUE_H
#define BACKENDBDD_RETARDABSENCELOGIQUE_H


#include <drogon/utils/coroutine.h>
#include "AbsenceCours.h"
#include <json/json.h>

using namespace drogon::orm;

class RetardAbsenceLogique{
public:
    static drogon::Task<Json::Value> AbsenceEleve(
    const DbClientPtr &db,
    const int32_t &idUtilisateur);
};

#endif //BACKENDBDD_RETARDABSENCELOGIQUE_H