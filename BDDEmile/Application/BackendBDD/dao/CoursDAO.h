//
// Created by Emile Guillerm on 05.04.2026.
//

#ifndef BACKENDBDD_COURSDAO_H
#define BACKENDBDD_COURSDAO_H


#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include "models/Cours.h"

using namespace drogon::orm;

class CoursDAO {
public:
    static drogon::Task<std::vector<drogon_model::ProjetV1::Cours>> ChercherCoursParSalle(
    const DbClientPtr &db,
    const int32_t &salle);

    static drogon::Task<drogon_model::ProjetV1::Cours> AjoutReservation(
        const DbClientPtr &db,
        const drogon_model::ProjetV1::Cours &NouvelleEntree);

};


#endif //BACKENDBDD_COURSDAO_H