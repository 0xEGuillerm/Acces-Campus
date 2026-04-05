//
// Created by Emile Guillerm on 06.04.2026.
//

#include "UtilisateurDAO.h"

#include "Utilisateur.h"

static drogon::Task<std::vector<drogon_model::ProjetV1::Utilisateur>> ChercherUtilisateurParNom(
    const DbClientPtr &db,
    const std::string &nom) {
    CoroMapper<drogon_model::ProjetV1::Utilisateur> mapperUtilisateur(db);
    auto resultat = co_await mapperUtilisateur.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Utilisateur::Cols::_nom_user, CompareOperator::EQ, nom));
    co_return resultat;
}

static drogon::Task<std::vector<drogon_model::ProjetV1::Utilisateur>> ChercherUtilisateurParPrenom(
    const DbClientPtr &db,
    const std::string &prenom) {
    CoroMapper<drogon_model::ProjetV1::Utilisateur> mapperUtilisateur(db);
    auto resultat = co_await mapperUtilisateur.findBy(
        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Utilisateur::Cols::_prenom_user, CompareOperator::EQ, prenom));
    co_return resultat;
}