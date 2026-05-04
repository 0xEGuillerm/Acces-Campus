//
// Created by Emile Guillerm on 04.04.2026.
//

#ifndef BACKENDBDD_BADGELOGIQUE_H
#define BACKENDBDD_BADGELOGIQUE_H

#include <drogon/orm/DbClient.h>
#include "models/Utilisateur.h"  // Classe générée par drogon_ctl
#include <functional>
#include <json/json.h>
#include "resultat/StructResultat.h"
#include "BadgeLogique.h"
#include "dao/ClasseDAO.h"


using namespace drogon::orm;

class BadgeLogique {
    public:

    //Obtenir les informatons d'un badge à partir de l'id
    //Renvoie un bool et un JSON avec:
    //nom_user
    //prenom_user
    //classe
    //statut (eleve, professeur, admin)
    //
    //Peut renvoyer un messsage en cas d'erreur
    static drogon::Task<ResultatCoro<Json::Value>> InformationBadge(
        //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
         const drogon::orm::DbClientPtr &db,
         //Uid Badge
         const std::string &uidBadge);

    //Supprimer un badge à partir de sont ID --> le champs seras mis à null dans la table de l'utilisateur
    //Renvoi un bool et potentiellement un message en cas d'erreur (pas de donnee)
    static drogon::Task<ResultatCoro<>> SupprimerBadge(
        //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
        const DbClientPtr &db,
        //id du badge à supprimer
        const std::string &uidBadge);

    //Creation d'un badge en base avec ajout à un utilisateur
    //renvoie un bool et potentiellement un message en cas d'erreur (pas de donnee)
    static drogon::Task<ResultatCoro<>> CreationBadge(
        //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
        const DbClientPtr &db,
        //Id de l'utilisateur
        const std::int32_t &uiduser,
        //uId du badge
        const std::string &uidBadge);

    //Modfication des info d'utilisateur à partir de sont id de badge
    //renvoie un bool et potentiellement un message en cas d'erreur (pas de donnee)
    static drogon::Task<ResultatCoro<>> ModifierInfoUtilisateur(
        //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
        const DbClientPtr &db,
        //id du badge de l'utilisateur dont il faut mdodifier les informations
        const std::string &uidBadge,
        //donnee à changer
        const Json::Value &body);

    //Verifier un badge peut importe quand si l'utilisateur à déjà eu cours dans cette salle
    //Renvoie un bool et un JSON avec:
    //nom
    //prenom
    //classe
    //
    //Peut renvoyer un messsage en cas d'erreur
    static drogon::Task<ResultatCoro<Json::Value>> VerifierBadgePEA(
        //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
        const DbClientPtr &db,
        //id badge
        const std::string &uidBadge,
        //adresse mac pea
        const std::string &mac);

    //Scanne le badge de l'utilisateur et l'ajoute au presence
    //Renvoie un bool et un JSON avec:
    //nom
    //prenom
    //classe
    //heure_arrivee
    //
    //Peut renvoyer un messsage en cas d'erreur
    static drogon::Task<ResultatCoro<Json::Value>> ScanneBadgeBAE(
        //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
        const DbClientPtr &db,
        //ID du badge
        const std::string &uidBadge,
        //Adresse MAC de la BAE
        const std::string &mac,
        //Horaire du badgeage (timestamp)
        const std::uint64_t &heure_badgage);
};
#endif //BACKENDBDD_BADGELOGIQUE_H