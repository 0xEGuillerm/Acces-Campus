//
// Created by Emile Guillerm on 10.04.2026.
//

#include "ClasseDAO.h"
#include "models/Badge.h"
#include "models/Classe.h"
#include <drogon/utils/coroutine.h>
#include <drogon/orm/CoroMapper.h>
#include <drogon/orm/Criteria.h>
#include <resultat/StructResultat.h>


//Recherche le nom d'une classe à partir de l'id d'une classe
//Renvoie un objet classe
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Classe>>> ClasseDAO::ChercherClasseParID(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Id de la classe
    const int32_t &idclasse) {
    //Mapper de classe / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Classe> mapperclasse(db);
    //Variable pour le résultat / envoie du type vector Classe pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Classe>> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _id_classe de la table Classe
        resultat.donnee = co_await mapperclasse.findBy(
        Criteria(drogon_model::acces_campus_bdd::Classe::Cols::_id_classe, CompareOperator::EQ, idclasse));
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

//Recherche une classe à partir de son nom
//Renvoie un objet classe
drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Classe>>> ClasseDAO::ChercherClasseParNom(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //nom de la classe
    const std::string &nomclasse) {
    //Mapper de classe / objet <--> SQL
    CoroMapper<drogon_model::acces_campus_bdd::Classe> mapperclasse(db);
    //Variable pour le résultat / envoie du type vector Classe pour la template
    ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Classe>> resultat;
    //Gestion des erreurs avec try/catch
    try {
        //Execution commande SQL recherche dans la colonne _id_classe de la table Classe
        resultat.donnee = co_await mapperclasse.findBy(
        Criteria(drogon_model::acces_campus_bdd::Classe::Cols::_nom_classe, CompareOperator::EQ, nomclasse));
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