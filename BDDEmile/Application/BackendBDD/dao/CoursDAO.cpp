//
// Created by Emile Guillerm on 05.04.2026.
//
#include "CoursDAO.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include "models/Utilisateur.h"
#include "models/Cours.h"
#include <resultat/StructResultat.h>

using namespace drogon::orm;


//Recherche les cours avec la Salle correspondante
//Renvoie un vecteur de Cours
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Cours>>> CoursDAO::ChercherCoursParSalle(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Id de la salle
    const std::string &salle) {
    //Mapper de classe / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Cours> mappercours(db);
    //Variable pour le résultat / envoie du type vector Cours pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Cours>> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _num_salle de la table Cours
        resultat.donnee = co_await mappercours.findBy(
        Criteria(drogon_model::acces_campus_bdd::Cours::Cols::_num_salle, CompareOperator::EQ, salle));
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


//Ajout d'une entrée dans la table Cours
//A besoin d'un objet Cours avec les informations remplis
//insert vas ajouté une nouvelle entrée dans la table Cours
//Renvoie un booleen
drogon::Task<ResultatCoro<>> CoursDAO::AjoutReservation(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Objet Cours avec les informations remplis
    const drogon_model::acces_campus_bdd::Cours &NouvelleEntree) {
    //Mapper de classe / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Cours> mapperCours(db);
    //Variable pour le résultat / envoie du type Cours pour la template
    ResultatCoro<> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL d'ajout de l'entrée
        co_await mapperCours.insert(NouvelleEntree);
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


//Recherche les cours dans l'intervalle timestamp
//Renvoie un vecteur de Cours
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Cours>>> CoursDAO::CoursIntervalleTimestamp(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Id de la salle
    const int64_t timestampdebut,
    const int64_t timestampfin) {
    //Mapper de classe / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Cours> mappercours(db);
    //Variable pour le résultat / envoie du type vector Cours pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Cours>> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _num_salle de la table Cours
        resultat.donnee = co_await mappercours.findBy((
        Criteria(drogon_model::acces_campus_bdd::Cours::Cols::_heure_fin, CompareOperator::GT, timestampdebut*1000000) &&
        Criteria(drogon_model::acces_campus_bdd::Cours::Cols::_heure_fin, CompareOperator::LT, timestampfin*1000000)) ||
        (Criteria(drogon_model::acces_campus_bdd::Cours::Cols::_heure_debut, CompareOperator::GT, timestampdebut*1000000) &&
        Criteria(drogon_model::acces_campus_bdd::Cours::Cols::_heure_debut, CompareOperator::LT, timestampfin*1000000)));
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

