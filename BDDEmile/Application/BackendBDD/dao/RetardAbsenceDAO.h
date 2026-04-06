//
// Created by Emile Guillerm on 05.04.2026.
//

#ifndef BACKENDBDD_RETARDABSENCEDAO_H
#define BACKENDBDD_RETARDABSENCEDAO_H

#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include "models/Utilisateur.h"
#include "Retardabsence.h"


using namespace drogon::orm;
class RetardAbsenceDAO {
public:
    static drogon::Task<std::vector<drogon_model::ProjetV1::Retardabsence>> ChercherRetardAbsenceEleve(
    const DbClientPtr &db,
    const int32_t &idUtilisateur);
};


#endif //BACKENDBDD_RETARDABSENCEDAO_H