//
// Created by Emile Guillerm on 05.04.2026.
//

#ifndef BACKENDBDD_RETARDABSENCEDAO_H
#define BACKENDBDD_RETARDABSENCEDAO_H

#include "RetardAbsenceDAO.h"
#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include "models/Utilisateur.h"
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include "Retardabsence.h"
#include "models/Cours.h"

using namespace drogon::orm;
class RetardAbsenceDAO {
public:
    static drogon::Task<std::vector<drogon_model::ProjetV1::Retardabsence>> ChercherRetardAbsenceEleve(
    const DbClientPtr &db,
    const std::string &idUtilisateur);
};


#endif //BACKENDBDD_RETARDABSENCEDAO_H