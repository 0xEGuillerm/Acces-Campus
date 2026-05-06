//
// Created by Emile Guillerm on 06.05.2026.
//

#ifndef BACKENDBDD_GESTIONCOURSTEMPORELLE_H
#define BACKENDBDD_GESTIONCOURSTEMPORELLE_H

#include "GestionCoursTemporelle.h"
#include <drogon/utils/coroutine.h>
#include "dao/UtilisateurDAO.h"



class GestionCoursTemporelle {
public:
    //Verification qu'un cour commence et ajout
static drogon::Task<> traiterCoursADemarrer();

//Verification qu'un cour commence est finit et suppression + ajout absence
static drogon::Task<> traiterCoursACloturer();
};


#endif //BACKENDBDD_GESTIONCOURSTEMPORELLE_H