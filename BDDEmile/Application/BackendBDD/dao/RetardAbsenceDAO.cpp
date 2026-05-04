//
// Created by Emile Guillerm on 05.04.2026.
//

#include "RetardAbsenceDAO.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include "AbsenceCours.h"
#include "Retardabsence.h"
#include <resultat/StructResultat.h>


using namespace drogon::orm;


//Recherche un RetardAbsence à partir de l'id d'un badge
//Renvoie un vecteur de retardabsence de l'utilisateur
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Retardabsence>>> RetardAbsenceDAO::ChercherRetardAbsenceEleve(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //id de l'utilisateur
    const int32_t &idUtilisateur) {
    //Mapper de classe / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Retardabsence> mapperRetardAbsence(db);
    //Variable pour le résultat / envoie du type vector Retardabsence pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Retardabsence>> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _id_user de la table Retardabsence
        resultat.donnee = co_await mapperRetardAbsence.findBy(
        Criteria(drogon_model::acces_campus_bdd::Retardabsence::Cols::_id_user, CompareOperator::EQ, idUtilisateur));
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

//Ajout d'une entrée dans la table RetardAbsence
//A besoin d'un objet RetardAbsence avec les informations remplis
//insert vas ajouté une nouvelle entrée dans la table Retardabsence
//Renvoie l'objet Retardabsence ajouté
drogon::Task<ResultatCoro<drogon_model::acces_campus_bdd::Retardabsence>> RetardAbsenceDAO::AjoutRetardAbsence(
    const DbClientPtr &db,
    const drogon_model::acces_campus_bdd::Retardabsence &NouvelleEntree) {
    //Mapper de classe / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Retardabsence> mapperRetardAbsence(db);
    //Variable pour le résultat / envoie du type Retardabsence pour la template
    ResultatCoro<drogon_model::acces_campus_bdd::Retardabsence> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL d'ajout de l'entrée
        resultat.donnee = co_await mapperRetardAbsence.insert(NouvelleEntree);
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