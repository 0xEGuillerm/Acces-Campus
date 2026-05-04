//
// Created by Emile Guillerm on 30.03.2026.
//

#ifndef BACKENDBDD_BADGEDAO_H
#define BACKENDBDD_BADGEDAO_H

#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>
#include "models/Utilisateur.h"
#include <functional>
#include "Badge.h"
#include <resultat/StructResultat.h>

using namespace drogon::orm;

class BadgeDAO
{
public:
//Recherche un utilisateur à partir de l'id d'un badge
//Renvoie un vecteur d'utilisateur
static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>>> ChercherUtilisateurParIDBadge(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //uid badge
    const std::string &uidBadge);

//Supprime un badge à partir de l'id
//Renvoie le nombre d'élément supprimer
static drogon::Task<ResultatCoro<>> SupprimerBadgeParIDBadge(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //uid badge
    const std::string &uidBadge);

//Recherche un badge à partir de l'id d'un badge
//Renvoie un vecteur de badge
static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Badge>>> ChercherBadgeParIDBadge(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //uid badge
    const std::string &uidBadge);

//Ajout d'une entrée dans la table Badge
//A besoin d'un objet Badge avec les informations remplis
//insert vas ajouté une nouvelle entrée dans la table Badge
//Renvoie l'objet Badge ajouté
static drogon::Task<ResultatCoro<drogon_model::acces_campus_bdd::Badge>> AjoutBadge(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Objet Badge avec les informations remplis
    const drogon_model::acces_campus_bdd::Badge &NouvelleEntree);
};

#endif //BACKENDBDD_BADGEDAO_H