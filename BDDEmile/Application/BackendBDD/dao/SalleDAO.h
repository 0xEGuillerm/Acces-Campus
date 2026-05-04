//
// Created by Emile Guillerm on 04.04.2026.
//

#ifndef BACKENDBDD_SALLEDAO_H
#define BACKENDBDD_SALLEDAO_H

#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include "models/Utilisateur.h"
#include <resultat/StructResultat.h>

using namespace drogon::orm;

class SalleDAO {
public:

//Chercher une salle à partir de l'adresse MAC
//Renvoie une liste de salle
static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Salle>>> ChercherSalleAdresseMACbae(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Adresse MAC
    const std::string &MAC);

//Chercher une salle à partir de l'adresse MAC
//Renvoie une liste de salle
static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Salle>>> ChercherSalleAdresseMACpea(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Adresse MAC
    const std::string &MAC);

    static drogon::Task<ResultatCoro<>> VerifierExistanceSalle(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //nom salle
    const std::string &nomsalle);

    static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Salle>>> ListeSalle(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db);
};


#endif //BACKENDBDD_SALLEDAO_H