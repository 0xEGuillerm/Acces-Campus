//
// Created by Emile Guillerm on 05.04.2026.
//

#ifndef BACKENDBDD_RETARDABSENCEDAO_H
#define BACKENDBDD_RETARDABSENCEDAO_H

#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include "models/Utilisateur.h"
#include "Retardabsence.h"
#include <resultat/StructResultat.h>


using namespace drogon::orm;
class RetardAbsenceDAO {
public:

//Recherche un RetardAbsence à partir de l'id d'un badge
//Renvoie un vecteur de retardabsence de l'utilisateur
static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Retardabsence>>> ChercherRetardAbsenceEleve(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //id de l'utilisateur
    const int32_t &idUtilisateur);

//Ajout d'une entrée dans la table RetardAbsence
//A besoin d'un objet RetardAbsence avec les informations remplis
//insert vas ajouté une nouvelle entrée dans la table Retardabsence
//Renvoie l'objet Retardabsence ajouté
static drogon::Task<ResultatCoro<drogon_model::acces_campus_bdd::Retardabsence>> AjoutRetardAbsence(
    const DbClientPtr &db,
    const drogon_model::acces_campus_bdd::Retardabsence &NouvelleEntree);
};


#endif //BACKENDBDD_RETARDABSENCEDAO_H