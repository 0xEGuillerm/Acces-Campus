//
// Created by Emile Guillerm on 04.04.2026.
//

#include "SalleDAO.h"
#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include "models/Utilisateur.h"
#include <resultat/StructResultat.h>

using namespace drogon::orm;

//Chercher une salle à partir de l'adresse MAC
//Renvoie une liste de salle
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Salle>>> SalleDAO::ChercherSalleAdresseMACbae(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Adresse MAC
    const std::string &MAC) {
    //Mapper de salle / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Salle> mappersalle(db);
    //Variable pour le résultat / envoie du type vector salle pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Salle>> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _mac_bae de la table Classe
        resultat.donnee = co_await mappersalle.findBy(
        Criteria(drogon_model::acces_campus_bdd::Salle::Cols::_mac_bae, CompareOperator::EQ, MAC));
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

//Chercher une salle à partir de l'adresse MAC
//Renvoie une liste de salle
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Salle>>> SalleDAO::ChercherSalleAdresseMACpea(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Adresse MAC
    const std::string &MAC) {
    //Mapper de salle / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Salle> mappersalle(db);
    //Variable pour le résultat / envoie du type vector salle pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Salle>> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _mac_pea de la table Classe
        resultat.donnee = co_await mappersalle.findBy(
        Criteria(drogon_model::acces_campus_bdd::Salle::Cols::_mac_pea, CompareOperator::EQ, MAC));
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

drogon::Task<ResultatCoro<>> SalleDAO::VerifierExistanceSalle(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //nom salle
    const std::string &nomsalle) {
    //Mapper de salle / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Salle> mappersalle(db);
    //Variable pour le résultat / envoie rien pour la template (monostate)
    ResultatCoro<> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _mac_pea de la table Classe
        auto temporaire = co_await mappersalle.findBy(
        Criteria(drogon_model::acces_campus_bdd::Salle::Cols::_num_salle, CompareOperator::EQ, nomsalle));
        //Si le vecteur est rempli il y'a resultat donc existe
        if (temporaire.empty()) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = "Inconnue";
        }
        else
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



drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Salle>>> SalleDAO::ListeSalle(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db) {
    //Mapper de salle / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Salle> mappersalle(db);
    //Variable pour le résultat / envoie rien pour la template (monostate)
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Salle>> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL d'envoie de toutes les salle
        resultat.donnee = co_await mappersalle.findAll();
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