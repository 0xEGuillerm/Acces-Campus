//
// Created by Emile Guillerm on 05.04.2026.
//

#ifndef BACKENDBDD_COURSLOGIQUE_H
#define BACKENDBDD_COURSLOGIQUE_H


#include "CoursLogique.h"
#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include "models/Utilisateur.h"
#include "resultat/StructResultat.h"

using namespace drogon::orm;


class CoursLogique {
public:
//Reserver une salle à partir des info:
//heure_debut (timestamp)
//heure_fin (timestamp)
//salle
//classe
//reserver_par (nom professeur
//professeur
//renvoie un bool et potentiellement un message en cas d'erreur (pas de donnee)
static drogon::Task<ResultatCoro<>> ReservationSallePGS(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Body contenant les info de la reservation
    const Json::Value &body);

/*
drogon::Task<ResultatCoro> CoursLogique::ReservationSallePSW(
    const DbClientPtr &db,
    const Json::Value &body);
    */


//Obtenir le planning d'une salle pour les 7 prochain jour (semaine) du time stamp
//salle
//debut (timestamp)
//fin (timestamp)
//renvoie un bool et potentiellement un message en cas d'erreur (pas de donnee)
static drogon::Task<ResultatCoro<Json::Value>> PlanningSalle(
    const DbClientPtr &db,
    const std::string &salle,
    const int64_t &timestamp_debut,
    const int64_t &timestamp_fin);



    //Obtenir les salle disponible pour une periode donnée
    //debut (timestamp)
    //fin (timestamp)
    //classe
    //reserver_par (nom professeur
    //professeur
    //renvoie un bool et potentiellement un message en cas d'erreur (pas de donnee)
    static drogon::Task<ResultatCoro<Json::Value>> SalleDisponible(
        const DbClientPtr &db,
        const int64_t &timestamp_debut,
        const int64_t &timestamp_fin);
};


#endif //BACKENDBDD_COURSLOGIQUE_H