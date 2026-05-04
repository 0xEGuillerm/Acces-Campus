//
// Created by Emile Guillerm on 06.04.2026.
//

#include "UtilisateurDAO.h"
#include <drogon/orm/CoroMapper.h>
#include "Utilisateur.h"
#include <resultat/StructResultat.h>

//Recherche les utilisateur avec ce nom
//Renvoie un vecteur d'utilisateur
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>>> UtilisateurDAO::ChercherUtilisateurParNom(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //nom de l'utilisateur à chercher
    const std::string &nom) {
    //Variable pour le résultat / envoie du type vector Utilisateur pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>> resultat;
    //Mapper de Utilisateur / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Utilisateur> mapperUtilisateur(db);
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _nom_user de la table utilisateur
        resultat.donnee = co_await mapperUtilisateur.findBy(
        Criteria(drogon_model::acces_campus_bdd::Utilisateur::Cols::_nom_user, CompareOperator::EQ, nom));
        resultat.BoolResultat = true;
    } catch
        //Exception stocker dans le message du resultat
        (const DrogonDbException& e) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = e.base().what();
        }
    co_return resultat;
}

//Recherche les utilisateur avec ce prenom
//Renvoie un vecteur d'utilisateur
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>>> UtilisateurDAO::ChercherUtilisateurParPrenom(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //prenom de l'utilisateur à chercher
    const std::string &prenom) {
    //Variable pour le résultat / envoie du type vector Utilisateur pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>> resultat;
    //Mapper de Utilisateur / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Utilisateur> mapperUtilisateur(db);
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _prenom_user de la table utilisateur
        resultat.donnee = co_await mapperUtilisateur.findBy(
        Criteria(drogon_model::acces_campus_bdd::Utilisateur::Cols::_prenom_user, CompareOperator::EQ, prenom));
        resultat.BoolResultat = true;
    } catch
        //Exception stocker dans le message du resultat
        (const DrogonDbException& e) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = e.base().what();
        }
    co_return resultat;
}

//Recherche les utilisateur avec cet id
//Renvoie un vecteur d'utilisateur
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>>> UtilisateurDAO::ChercherUtilisateurParID(
//Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //id de l'utilisateur
    const std::int32_t &id) {
    //Variable pour le résultat / envoie du type vector Utilisateur pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>> resultat;
    //Mapper de Utilisateur / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Utilisateur> mapperUtilisateur(db);
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _uuid_badge de la table utilisateur
        resultat.donnee = co_await mapperUtilisateur.findBy(
        Criteria(drogon_model::acces_campus_bdd::Utilisateur::Cols::_id_user, CompareOperator::EQ, id));
        resultat.BoolResultat = true;
    } catch
        //Exception stocker dans le message du resultat
        (const DrogonDbException& e) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = e.base().what();
        }
    co_return resultat;
}


//Recherche les utilisateur avec ce login
//Renvoie un vecteur d'utilisateur
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>>> UtilisateurDAO::ChercherUtilisateurParLogin(
//Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Login de l'utilisateur
    const std::string &login) {
    //Variable pour le résultat / envoie du type vector Utilisateur pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>> resultat;
    //Mapper de Utilisateur / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Utilisateur> mapperUtilisateur(db);
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _login_user de la table utilisateur
        resultat.donnee = co_await mapperUtilisateur.findBy(
        Criteria(drogon_model::acces_campus_bdd::Utilisateur::Cols::_login_user, CompareOperator::EQ, login));
        resultat.BoolResultat = true;
    } catch
        //Exception stocker dans le message du resultat
        (const DrogonDbException& e) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = e.base().what();
        }
    co_return resultat;
}

//Modification d'une entrée dans la table utilisateur
//A besoin d'un objet utilisateur avec les informations modifié
//update vas chercher à partir de la primary key un utilisateur correspondant et vas le remplacé
//Renvoie un long du nombre d'utilisateur modifié
drogon::Task<ResultatCoro<>> UtilisateurDAO::MettreAJourUser(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Objet utilisateur avec les informations modifié
    const drogon_model::acces_campus_bdd::Utilisateur &UtilisateurModifier) {
    //Mapper de badge / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Utilisateur> mapperutilisateur(db);
    //Variable pour le résultat / envoie du type nombre de suppression pour la template
    ResultatCoro<> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL pour supprimer
        co_await mapperutilisateur.update(UtilisateurModifier);
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