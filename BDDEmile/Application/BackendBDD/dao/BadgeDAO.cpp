//
// Created by Emile Guillerm on 30.03.2026.
//

#include "BadgeDAO.h"
#include "models/Badge.h"
#include "models/Utilisateur.h"
#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include <resultat/StructResultat.h>

//Recherche un utilisateur à partir de l'id d'un badge
//Renvoie un vecteur d'utilisateur
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>>> BadgeDAO::ChercherUtilisateurParIDBadge(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //uid badge
    const std::string &uidBadge) {
    //Mapper de utilisateur / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Utilisateur> mapperuser(db);
    //Variable pour le résultat / envoie du type vector Utilisateur pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _uuid_badge de la table utilisateur
        resultat.donnee = co_await mapperuser.findBy(
        Criteria(drogon_model::acces_campus_bdd::Utilisateur::Cols::_uuid_badge, CompareOperator::EQ, uidBadge));
        resultat.BoolResultat = true;
    } catch
        //Exception stocker dans le message du resultat
        (const DrogonDbException& e) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = e.base().what();
        }
    //Envoie du résultat
    co_return resultat;
}

//Supprime un badge à partir de l'id
//Renvoie rien dans donnee (verifier le bool)
drogon::Task<ResultatCoro<>> BadgeDAO::SupprimerBadgeParIDBadge(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //uid badge
    const std::string &uidBadge) {
    //Mapper de badge / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Badge> mapperbadge(db);
    //Variable pour le résultat / envoie du type nombre de suppression pour la template (rien monostate)
    ResultatCoro<> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL pour supprimer
        co_await mapperbadge.deleteByPrimaryKey(uidBadge);
        resultat.BoolResultat = true;
    } catch
        //Exception stocker dans le message du resultat
        (const DrogonDbException& e) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = e.base().what();
        }
    //Envoie du résultat
    co_return resultat;
}

//Recherche un badge à partir de l'id d'un badge
//Renvoie un objet badge
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Badge>>> BadgeDAO::ChercherBadgeParIDBadge(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //uid badge
    const std::string &uidBadge){
    //Mapper de badge / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Badge> mapperbadge(db);
    //Variable pour le résultat / envoie du type vector badge pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Badge>> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _uuid_badge de la table badge
        resultat.donnee = co_await mapperbadge.findBy(
        Criteria(drogon_model::acces_campus_bdd::Badge::Cols::_uuid_badge, CompareOperator::EQ, uidBadge));
        resultat.BoolResultat = true;
    } catch
        //Exception stocker dans le message du resultat
        (const DrogonDbException& e) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = e.base().what();
        }
    //Envoie du résultat
    co_return resultat;
}

//Ajout d'une entrée dans la table Badge
//A besoin d'un objet Badge avec les informations remplis
//insert vas ajouté une nouvelle entrée dans la table Badge
//Renvoie l'objet Badge ajouté
drogon::Task<ResultatCoro<drogon_model::acces_campus_bdd::Badge>> BadgeDAO::AjoutBadge(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Objet Badge avec les informations remplis
    const drogon_model::acces_campus_bdd::Badge &NouvelleEntree) {
    //Mapper de classe / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Badge> mapperCours(db);
    //Variable pour le résultat / envoie du type Badge pour la template
    ResultatCoro<drogon_model::acces_campus_bdd::Badge> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL d'ajout de l'entrée
        resultat.donnee = co_await mapperCours.insert(NouvelleEntree);
        resultat.BoolResultat = true;
    } catch
        //Exception stocker dans le message du resultat
        (const DrogonDbException& e) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = e.base().what();
        }
    //Envoie du résultat
    co_return resultat;
}














