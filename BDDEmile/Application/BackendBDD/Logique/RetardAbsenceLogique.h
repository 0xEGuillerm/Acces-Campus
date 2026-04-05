//
// Created by Emile Guillerm on 05.04.2026.
//

#ifndef BACKENDBDD_RETARDABSENCELOGIQUE_H
#define BACKENDBDD_RETARDABSENCELOGIQUE_H



#include "SalleLogique.h"
#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include "models/Utilisateur.h"
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include "models/Cours.h"
#include <json/json.h>
#include "dao/SalleDAO.h"
#include "models/Retardabsence.h"
#include "dao/RetardAbsenceDAO.h"

using namespace drogon::orm;

class RetardAbsenceLogique{
public:
    static drogon::Task<Json::Value> AbsenceEleve(
    const DbClientPtr &db,
    const int32_t &idUtilisateur);
};

#endif //BACKENDBDD_RETARDABSENCELOGIQUE_H