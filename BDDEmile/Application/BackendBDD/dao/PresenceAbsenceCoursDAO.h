//
// Created by Emile Guillerm on 01.05.2026.
//

#ifndef BACKENDBDD_PRESENCEABSENCECOURS_H
#define BACKENDBDD_PRESENCEABSENCECOURS_H

#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include "models/Utilisateur.h"
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include <resultat/StructResultat.h>

using namespace drogon::orm;

class PresenceAbsenceCoursDAO {
public:

//Chercher un utilisateur dans les absencesCours (plage horaire actuelle)
//Renvoie une liste d'absence cours dans lequelle l'utilisateur est present
static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::AbsenceCours>>> ChercherUtilisateurDansAbsencecours(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //ID d'un utilisateur
    const int32_t &uidUtilisateur);

//Chercher un utilisateur dans les PresenceCours (plage horaire actuelle)
//Renvoie une liste de presence cours dans lequelle l'utilisateur est present
static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::PresenceCours>>> ChercherUtilisateurDansPresencecours(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //ID de l'utilisateur
    const int32_t &uidUtilisateur);


//Supprime l'utilisateur de table d'absence à un cours à partir de sont id
//Renvoie rien dans donnee (verifier le bool)
static drogon::Task<ResultatCoro<>> SupprimerUtilisateurAbsenceCours(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //id de l'absence à supprimer
    const int32_t &idutilisateur);

//Ajout d'une entrée dans la table AbsenceCours
//A besoin d'un objet AbsenceCours avec les informations remplis
//insert vas ajouté une nouvelle entrée dans la table AbsenceCours
//Renvoie l'objet AbsenceCours ajouté
static drogon::Task<ResultatCoro<drogon_model::acces_campus_bdd::AbsenceCours>> AjoutUtilisateurAbsenceCours(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Objet AbsenceCours avec les informations remplis
    const drogon_model::acces_campus_bdd::AbsenceCours &NouvelleEntree);


//Ajout d'une entrée dans la table PresenceCours
//A besoin d'un objet PresenceCours avec les informations remplis
//insert vas ajouté une nouvelle entrée dans la table PresenceCours
//Renvoie l'objet PresenceCours ajouté
static drogon::Task<ResultatCoro<drogon_model::acces_campus_bdd::PresenceCours>> AjoutUtilisateurPresenceCours(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Objet PresenceCours avec les informations remplis
    const drogon_model::acces_campus_bdd::PresenceCours &NouvelleEntree);


    //Supprime utilisateur de la table presence à un cours à partir de cours
//Renvoie rien dans donnee (verifier le bool)
static drogon::Task<ResultatCoro<>> SupprimerUtilisateurPresenceCours(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //id de la presence à supprimer
    const int32_t &idcours);


    //Chercher dans les absencesCours (plage horaire actuelle)
    //Renvoie une liste d'absence cours dans lequelle l'utilisateur est present
    static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::AbsenceCours>>> ChercherUtilisateurDansAbsencecoursID(
        //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
        const DbClientPtr &db,
        //ID cour
        const int32_t &idutilisateur);

};




#endif //BACKENDBDD_PRESENCEABSENCECOURS_H