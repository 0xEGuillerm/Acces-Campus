//
// Created by Emile Guillerm on 10.04.2026.
//

#ifndef BACKENDBDD_CLASSEDAO_H
#define BACKENDBDD_CLASSEDAO_H

#include "ClasseDAO.h"
#include "models/Utilisateur.h"
#include "models/Classe.h"
#include <drogon/utils/coroutine.h>
#include <resultat/StructResultat.h>


using namespace drogon::orm;

class ClasseDAO {
public:
    //Recherche le nom d'une classe à partir de l'id d'une classe
    //Renvoie un objet classe
    static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Classe>>> ChercherClasseParID(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Id de la classe
    const int32_t &idclasse);

    //Recherche une classe à partir de son nom
    //Renvoie un objet classe
    static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Classe>>> ChercherClasseParNom(
        //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
        const DbClientPtr &db,
        //nom de la classe
        const std::string &nomclasse);
};

#endif //BACKENDBDD_CLASSEDAO_H