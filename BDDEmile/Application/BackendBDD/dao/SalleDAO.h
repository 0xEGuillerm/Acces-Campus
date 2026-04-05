//
// Created by Emile Guillerm on 04.04.2026.
//

#ifndef BACKENDBDD_SALLEDAO_H
#define BACKENDBDD_SALLEDAO_H

#include "SalleDAO.h"
#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include "models/Utilisateur.h"
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include "models/Cours.h"

using namespace drogon::orm;

class SalleDAO {
public:
    static drogon::Task<std::vector<drogon_model::ProjetV1::Salle>> ChercherSalleAdresseMACbae(
    const DbClientPtr &db,
    const std::string &MAC);

    static drogon::Task<std::vector<drogon_model::ProjetV1::Cours>> ChercherCoursParSalle(
        const DbClientPtr &db,
        const int32_t &numSalle);

    static drogon::Task<std::vector<drogon_model::ProjetV1::AbsenceCours>> ChercherUtilisateurDansAbsencecours(
        const DbClientPtr &db,
        const int32_t &uidUtilisateur);

    static drogon::Task<std::vector<drogon_model::ProjetV1::PresenceCours>> ChercherUtilisateurDansPresencecours(
        const DbClientPtr &db,
        const int32_t &uidUtilisateur);
};


#endif //BACKENDBDD_SALLEDAO_H