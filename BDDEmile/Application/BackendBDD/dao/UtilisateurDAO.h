//
// Created by Emile Guillerm on 06.04.2026.
//

#ifndef BACKENDBDD_UTILISATEURDAO_H
#define BACKENDBDD_UTILISATEURDAO_H


#include "AbsenceCours.h"
#include "Utilisateur.h"
#include <resultat/StructResultat.h>

using namespace drogon::orm;


class UtilisateurDAO {
public:
    //Recherche les utilisateur avec ce nom
    //Renvoie un vecteur d'utilisateur
    static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>>> ChercherUtilisateurParNom(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //nom de l'utilisateur à chercher
    const std::string &nom);

    //Recherche les utilisateur avec ce prenom
    //Renvoie un vecteur d'utilisateur
    static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>>> ChercherUtilisateurParPrenom(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //prenom de l'utilisateur à chercher
    const std::string &prenom);

    //Recherche les utilisateur avec cet id
    //Renvoie un vecteur d'utilisateur
    static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>>> ChercherUtilisateurParID(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //id de l'utilisateur
    const std::int32_t &id);

    //Recherche les utilisateur avec ce login
    //Renvoie un vecteur d'utilisateur
    static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Utilisateur>>> ChercherUtilisateurParLogin(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Login de l'utilisateur
    const std::string &login);

    //Modification d'une entrée dans la table utilisateur
    //A besoin d'un objet utilisateur avec les informations modifié
    //update vas chercher à partir de la primary key un utilisateur correspondant et vas le remplacé
    //Renvoie un long du nombre d'utilisateur modifié
    static drogon::Task<ResultatCoro<>> MettreAJourUser(
        //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
        const DbClientPtr &db,
        //Objet utilisateur avec les informations modifié
        const drogon_model::acces_campus_bdd::Utilisateur &UtilisateurModifier);
};


#endif //BACKENDBDD_UTILISATEURDAO_H