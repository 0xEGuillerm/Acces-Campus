//
// Created by Emile Guillerm on 05.04.2026.
//

#ifndef BACKENDBDD_COURSDAO_H
#define BACKENDBDD_COURSDAO_H


#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include "models/Cours.h"
#include <resultat/StructResultat.h>

using namespace drogon::orm;

class CoursDAO {
public:

    //Recherche les cours avec la Salle correspondante
    //Renvoie un vecteur de Cours
    static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Cours>>> ChercherCoursParSalle(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Id de la salle
    const std::string &salle);

    //Modification d'une entrée dans la table Cours
    //A besoin d'un objet Cours avec les informations remplis
    //insert vas ajouté une nouvelle entrée dans la table Cours
    //Renvoie l'objet Cours ajouté
    static drogon::Task<ResultatCoro<>> AjoutReservation(
        //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
        const DbClientPtr &db,
        //Objet Cours avec les informations remplis
        const drogon_model::acces_campus_bdd::Cours &NouvelleEntree);


    //Recherche les cours dans l'intervalle timestamp
    //Renvoie un vecteur de Cours
    static drogon::Task<ResultatCoro<std::vector<drogon_model::acces_campus_bdd::Cours>>> CoursIntervalleTimestamp(
        //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
        const DbClientPtr &db,
        //Id de la salle
        const int64_t timestampdebut,
        const int64_t timestampfin);
};


#endif //BACKENDBDD_COURSDAO_H