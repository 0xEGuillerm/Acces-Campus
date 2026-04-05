//
// Created by Emile Guillerm on 05.04.2026.
//

#ifndef BACKENDBDD_COURSLOGIQUE_H
#define BACKENDBDD_COURSLOGIQUE_H


#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include "models/Utilisateur.h"
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include "resultat/StructResultat.h"
#include "models/Cours.h"

using namespace drogon::orm;


class CoursLogique {

    static drogon::Task<ResultatCoro> ReservationSallePGS(
    const DbClientPtr &db,
    const Json::Value &body);

    static drogon::Task<ResultatCoro> ReservationSallePSW(
    const DbClientPtr &db,
    const Json::Value &body);


    static drogon::Task<Json::Value> PlanningSalle(
    const DbClientPtr &db,
    const int32_t &salle);
};


#endif //BACKENDBDD_COURSLOGIQUE_H