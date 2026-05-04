//
// Created by Emile Guillerm on 01.05.2026.
//

#include "PresenceAbsenceCoursDAO.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include "models/Cours.h"
#include <resultat/StructResultat.h>

//Chercher dans les absencesCours (plage horaire actuelle)
//Renvoie une liste d'absence cours dans lequelle l'utilisateur est present
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::AbsenceCours>>> PresenceAbsenceCoursDAO::ChercherUtilisateurDansAbsencecours(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //ID cour
    const int32_t &idcours) {
    //Mapper de classe / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::AbsenceCours> mapperabsence(db);
    //Variable pour le résultat / envoie du type vector AbsenceCours pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::AbsenceCours>> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _id_cours de la table AbsenceCours
        resultat.donnee = co_await mapperabsence.findBy(
        Criteria(drogon_model::acces_campus_bdd::AbsenceCours::Cols::_id_cours, CompareOperator::EQ, idcours));
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

//Chercher dans les PresenceCours (plage horaire actuelle)
//Renvoie une liste de presence cours dans lequelle l'utilisateur est present
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::PresenceCours>>> PresenceAbsenceCoursDAO::ChercherUtilisateurDansPresencecours(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //ID de l'utilisateur
    const int32_t &idcours) {
    //Mapper de classe / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::PresenceCours> mapperPresence(db);
    //Variable pour le résultat / envoie du type vector PresenceCours pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::PresenceCours>> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _id_cours de la table PresenceCours
        resultat.donnee = co_await mapperPresence.findBy(
        Criteria(drogon_model::acces_campus_bdd::PresenceCours::Cols::_id_cours, CompareOperator::EQ, idcours));
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


//Supprime l'utilisateur de table d'absence à un cours
//Renvoie rien dans donnee (verifier le bool)
drogon::Task<ResultatCoro<>> PresenceAbsenceCoursDAO::SupprimerUtilisateurAbsenceCours(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //id de l'absence à supprimer
    const int32_t &idutilisateur) {
    //Mapper de badge / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::AbsenceCours> mapperabsence(db);
    //Variable pour le résultat / envoie du type nombre de suppression pour la template (rien monostate)
    ResultatCoro<> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL pour supprimer
        co_await mapperabsence.deleteBy(
        Criteria(drogon_model::acces_campus_bdd::AbsenceCours::Cols::_id_user, CompareOperator::EQ, idutilisateur));
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

//Ajout d'une entrée dans la table AbsenceCours
//A besoin d'un objet AbsenceCours avec les informations remplis
//insert vas ajouté une nouvelle entrée dans la table AbsenceCours
//Renvoie l'objet AbsenceCours ajouté
drogon::Task<ResultatCoro<drogon_model::acces_campus_bdd::AbsenceCours>> PresenceAbsenceCoursDAO::AjoutUtilisateurAbsenceCours(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Objet AbsenceCours avec les informations remplis
    const drogon_model::acces_campus_bdd::AbsenceCours &NouvelleEntree) {
    //Mapper de badge / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::AbsenceCours> mapperabsencecours(db);
    //Variable pour le résultat / envoie du type absencecours pour la template
    ResultatCoro<drogon_model::acces_campus_bdd::AbsenceCours> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL d'ajout de l'entrée
        resultat.donnee = co_await mapperabsencecours.insert(NouvelleEntree);
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


//Ajout d'une entrée dans la table PresenceCours
//A besoin d'un objet PresenceCours avec les informations remplis
//insert vas ajouté une nouvelle entrée dans la table PresenceCours
//Renvoie l'objet PresenceCours ajouté
drogon::Task<ResultatCoro<drogon_model::acces_campus_bdd::PresenceCours>> PresenceAbsenceCoursDAO::AjoutUtilisateurPresenceCours(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Objet PresenceCours avec les informations remplis
    const drogon_model::acces_campus_bdd::PresenceCours &NouvelleEntree) {
    //Mapper de badge / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::PresenceCours> mapperPresenceCours(db);
    //Variable pour le résultat / envoie du type absencecours pour la template
    ResultatCoro<drogon_model::acces_campus_bdd::PresenceCours> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL d'ajout de l'entrée
        resultat.donnee = co_await mapperPresenceCours.insert(NouvelleEntree);
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


//Supprime utilisateur de la table presence à un cours à partir de cours
//Renvoie rien dans donnee (verifier le bool)
drogon::Task<ResultatCoro<>> PresenceAbsenceCoursDAO::SupprimerUtilisateurPresenceCours(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //id de la presence à supprimer
    const int32_t &idcours) {
    //Mapper de badge / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::PresenceCours> mapperpresence(db);
    //Variable pour le résultat / envoie du type nombre de suppression pour la template (rien monostate)
    ResultatCoro<> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL pour supprimer
        co_await mapperpresence.deleteBy(
        Criteria(drogon_model::acces_campus_bdd::PresenceCours::Cols::_id_cours, CompareOperator::EQ, idcours));

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


//Chercher dans les absencesCours (plage horaire actuelle)
//Renvoie une liste d'absence cours dans lequelle l'utilisateur est present
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::AbsenceCours>>> PresenceAbsenceCoursDAO::ChercherUtilisateurDansAbsencecoursID(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //ID cour
    const int32_t &idutilisateur) {
    //Mapper de classe / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::AbsenceCours> mapperabsence(db);
    //Variable pour le résultat / envoie du type vector AbsenceCours pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::AbsenceCours>> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _id_user de la table AbsenceCours
        resultat.donnee = co_await mapperabsence.findBy(
        Criteria(drogon_model::acces_campus_bdd::AbsenceCours::Cols::_id_user, CompareOperator::EQ, idutilisateur));
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

