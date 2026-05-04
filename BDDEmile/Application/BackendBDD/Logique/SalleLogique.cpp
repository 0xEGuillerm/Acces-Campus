//
// Created by Emile Guillerm on 04.04.2026.
//

#include "SalleLogique.h"
#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include "models/Utilisateur.h"
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include "models/Cours.h"
#include <json/json.h>
#include "dao/ClasseDAO.h"
#include "dao/SalleDAO.h"
#include "dao/UtilisateurDAO.h"
#include "dao/CoursDAO.h"
#include "dao/PresenceAbsenceCoursDAO.h"




using namespace drogon::orm;

//Renvoi les info du cours
//Json avec
//classe
//duree
//enseignant
//nb_absent
//nb_present
//liste_absent
//liste_present
//Renvoi "Aucun cours" si pas de cours
//peut renvoyer des erreur
drogon::Task<ResultatCoro<Json::Value>> SalleLogique::EtatSalleMac(
    const DbClientPtr &db,
    const std::string &mac) {
    //Création de la structure réponse
    ResultatCoro<Json::Value> resultat;
    //Creation des liste d'absent
    Json::Value listeAbsence(Json::arrayValue);
    Json::Value listePresent(Json::arrayValue);
    //recherche de la salle correspondant à l'adresse mac
    auto salletrouvee = co_await SalleDAO::ChercherSalleAdresseMACbae(db, mac);
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (salletrouvee.BoolResultat == false || salletrouvee.donnee.empty()) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Salle introuvable";
        co_return resultat;
    }
    //garde uniquement la salle renvoyé
    auto salle = salletrouvee.donnee[0];
    //recherche de cours dans la salle
    auto CoursListe = co_await CoursDAO::ChercherCoursParSalle(db, (*salle.getNumSalle()));
    //renvoie un bool false et un message d'erreur correspondant si probleme ou si aucun cours dans cette salle
    if (CoursListe.BoolResultat == false || CoursListe.donnee.empty()) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Pas de cours";
        co_return resultat;
    }

    //Stocker le cours qui se passe actuellement dans la salle du BAE
    drogon_model::acces_campus_bdd::Cours cour;

    //Obtention de l'horaire actuelle format int seconde
    int64_t TimestampSecond = trantor::Date::now().secondsSinceEpoch();

    //Recherche du cours qui se passe actuellement dans la salle parmis tous les cours qui ce sont passé dans cette salle (présent/passé/futur)
    bool trouvee = false;
    for (const auto &coursrecherche : CoursListe.donnee) {
        //PostgreSQL stock les timestamp en microseconde donc .secondsSinceEpoch()
        if (TimestampSecond > coursrecherche.getValueOfHeureDebut().secondsSinceEpoch() && TimestampSecond < coursrecherche.getValueOfHeureFin().secondsSinceEpoch()) {
            trouvee = true;
            cour = coursrecherche;
            break;
        }
    }
    //Verification de la recherche et envoie un bool false si il n'ya pas de cours actuellement dans la salle
    if (!trouvee) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Pas de cours";
        co_return resultat;
    }
    //recherche d'une classe avec son id
    auto classeverif = co_await ClasseDAO::ChercherClasseParID(db, cour.getValueOfIdClasse());
    //si l'id correspond à une classe existante
    if (classeverif.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur";
        co_return resultat;
    }
    //Recherche du professeur à partir d'id
    auto ProfesseurVecteur = co_await UtilisateurDAO::ChercherUtilisateurParID(db, cour.getValueOfProfesseur());
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (ProfesseurVecteur.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur";
        co_return resultat;
    }

    //Ajout des donnée du cours
    resultat.donnee["classe"] = classeverif.donnee[0].getValueOfNomClasse();
    resultat.donnee["duree"] = TimestampSecond - cour.getValueOfHeureDebut().secondsSinceEpoch();
    resultat.donnee["professeur"] = ProfesseurVecteur.donnee[0].getValueOfNomUser();
    //recherche de d'utilisateur absent lié a ce cours
    auto ListeAbsentVecteur = co_await PresenceAbsenceCoursDAO::ChercherUtilisateurDansAbsencecours(db, cour.getValueOfIdCours());
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (ListeAbsentVecteur.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur";
        co_return resultat;
    }
    //recherche de d'utilisateur present lié a ce cours
    auto ListePresentVecteur = co_await PresenceAbsenceCoursDAO::ChercherUtilisateurDansPresencecours(db, cour.getValueOfIdCours());
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (ListePresentVecteur.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur";
        co_return resultat;
    }
    resultat.donnee["nb_absent"] = static_cast<int>(ListeAbsentVecteur.donnee.size());
    resultat.donnee["nb_present"] = static_cast<int>(ListePresentVecteur.donnee.size());
    //Ajout de tout les eleve absent à la liste des élève absent
    for (const auto& Absence : ListeAbsentVecteur.donnee){
        Json::Value AbsenceTemp;
        //Recherche de l'eleve à partir d'id
        auto Eleve = co_await UtilisateurDAO::ChercherUtilisateurParID(db, Absence.getValueOfIdUser());
        //renvoie un bool false et un message d'erreur correspondant si probleme
        if (Eleve.BoolResultat == false) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = "Erreur";
            co_return resultat;
        }
        AbsenceTemp["nom"] = Eleve.donnee[0].getValueOfNomUser();
        AbsenceTemp["prenom"] = Eleve.donnee[0].getValueOfPrenomUser();
        listeAbsence.append(AbsenceTemp);
    }
    for (const auto& Present : ListePresentVecteur.donnee){
        Json::Value PresentTemp;
        //Recherche de l'eleve à partir d'id
        auto Eleve = co_await UtilisateurDAO::ChercherUtilisateurParID(db, Present.getValueOfIdUser());
        //renvoie un bool false et un message d'erreur correspondant si probleme
        if (Eleve.BoolResultat == false) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = "Erreur";
            co_return resultat;
        }
        PresentTemp["nom"] = Eleve.donnee[0].getValueOfNomUser();
        PresentTemp["prenom"] = Eleve.donnee[0].getValueOfPrenomUser();
       listePresent.append(PresentTemp);
    }
    //Ajout des liste et envoie du resultat
    resultat.donnee["listeAbsent"] = listeAbsence;
    resultat.donnee["listePresent"] = listePresent;
    resultat.BoolResultat = true;
    co_return resultat;
}

//Renvoi les info du cours
//Json avec
//classe
//duree
//enseignant
//nb_absent
//nb_present
//liste_absent
//liste_present
//Renvoi "Aucun cours" si pas de cours
//peut renvoyer des erreur
drogon::Task<ResultatCoro<Json::Value>> SalleLogique::EtatSalleNumeroSalle(
    const DbClientPtr &db,
    const std::string &salle) {

        //Création de la structure réponse
    ResultatCoro<Json::Value> resultat;
    //Creation des liste d'absent
    Json::Value listeAbsence(Json::arrayValue);
    Json::Value listePresent(Json::arrayValue);

    //recherche de cours dans la salle
    auto CoursListe = co_await CoursDAO::ChercherCoursParSalle(db, salle);
    //renvoie un bool false et un message d'erreur correspondant si probleme ou si aucun cours dans cette salle
    if (CoursListe.BoolResultat == false || CoursListe.donnee.empty()) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Pas de cours";
        co_return resultat;
    }

    //Stocker le cours qui se passe actuellement dans la salle du BAE
    drogon_model::acces_campus_bdd::Cours cour;

    //Obtention de l'horaire actuelle format int seconde
    int64_t TimestampSecond = trantor::Date::now().secondsSinceEpoch();

    //Recherche du cours qui se passe actuellement dans la salle parmis tous les cours qui ce sont passé dans cette salle (présent/passé/futur)
    bool trouvee = false;
    for (const auto &coursrecherche : CoursListe.donnee) {
        //PostgreSQL stock les timestamp en microseconde donc .secondsSinceEpoch()
        if (TimestampSecond > coursrecherche.getValueOfHeureDebut().secondsSinceEpoch() && TimestampSecond < coursrecherche.getValueOfHeureFin().secondsSinceEpoch()) {
            trouvee = true;
            cour = coursrecherche;
            break;
        }
    }
    //Verification de la recherche et envoie un bool false si il n'ya pas de cours actuellement dans la salle
    if (!trouvee) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Pas de cours";
        co_return resultat;
    }
    //recherche d'une classe avec son id
    auto classeverif = co_await ClasseDAO::ChercherClasseParID(db, cour.getValueOfIdClasse());
    //si l'id correspond à une classe existante
    if (classeverif.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur";
        co_return resultat;
    }
    //Recherche du professeur à partir d'id
    auto ProfesseurVecteur = co_await UtilisateurDAO::ChercherUtilisateurParID(db, cour.getValueOfProfesseur());
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (ProfesseurVecteur.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur";
        co_return resultat;
    }

    //Ajout des donnée du cours
    resultat.donnee["classe"] = classeverif.donnee[0].getValueOfNomClasse();
    resultat.donnee["duree"] = TimestampSecond - cour.getValueOfHeureDebut().secondsSinceEpoch();
    resultat.donnee["professeur"] = ProfesseurVecteur.donnee[0].getValueOfNomUser();
    //recherche de d'utilisateur absent lié a ce cours
    auto ListeAbsentVecteur = co_await PresenceAbsenceCoursDAO::ChercherUtilisateurDansAbsencecours(db, cour.getValueOfIdCours());
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (ListeAbsentVecteur.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur";
        co_return resultat;
    }
    //recherche de d'utilisateur present lié a ce cours
    auto ListePresentVecteur = co_await PresenceAbsenceCoursDAO::ChercherUtilisateurDansPresencecours(db, cour.getValueOfIdCours());
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (ListePresentVecteur.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur";
        co_return resultat;
    }
    resultat.donnee["nb_absent"] = static_cast<int>(ListeAbsentVecteur.donnee.size());
    resultat.donnee["nb_present"] = static_cast<int>(ListePresentVecteur.donnee.size());
    //Ajout de tout les eleve absent à la liste des élève absent
    for (const auto& Absence : ListeAbsentVecteur.donnee){
        Json::Value AbsenceTemp;
        //Recherche de l'eleve à partir d'id
        auto Eleve = co_await UtilisateurDAO::ChercherUtilisateurParID(db, Absence.getValueOfIdUser());
        //renvoie un bool false et un message d'erreur correspondant si probleme
        if (Eleve.BoolResultat == false) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = "Erreur";
            co_return resultat;
        }
        AbsenceTemp["nom"] = Eleve.donnee[0].getValueOfNomUser();
        AbsenceTemp["prenom"] = Eleve.donnee[0].getValueOfPrenomUser();
        listeAbsence.append(AbsenceTemp);
    }
    for (const auto& Present : ListePresentVecteur.donnee){
        Json::Value PresentTemp;
        //Recherche de l'eleve à partir d'id
        auto Eleve = co_await UtilisateurDAO::ChercherUtilisateurParID(db, Present.getValueOfIdUser());
        //renvoie un bool false et un message d'erreur correspondant si probleme
        if (Eleve.BoolResultat == false) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = "Erreur";
            co_return resultat;
        }
        PresentTemp["nom"] = Eleve.donnee[0].getValueOfNomUser();
        PresentTemp["prenom"] = Eleve.donnee[0].getValueOfPrenomUser();
       listePresent.append(PresentTemp);
    }
    //Ajout des liste et envoie du resultat
    resultat.donnee["listeAbsent"] = listeAbsence;
    resultat.donnee["listePresent"] = listePresent;
    resultat.BoolResultat = true;
    co_return resultat;

}


//renvoie toutes les salles
//JSON liste salle
//nom_salle
//peut renvoyer des erreur
drogon::Task<ResultatCoro<Json::Value>> SalleLogique::ListedeSalle(
    const DbClientPtr &db) {
    //Création de la structure réponse
    ResultatCoro<Json::Value> resultat;
    //Creation des liste salle
    Json::Value listesalle(Json::arrayValue);
    //obtention de toutes les salles vecteur
    auto VecteurSalle = co_await SalleDAO::ListeSalle(db);
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (VecteurSalle.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur";
        co_return resultat;
    }
    for (const auto& salle : VecteurSalle.donnee){
        Json::Value SalleTemp;
        SalleTemp["nom_salle"] = salle.getPrimaryKey();
        listesalle.append(SalleTemp);
    }
    resultat.donnee = listesalle;
    resultat.BoolResultat = true;
    co_return resultat;
}
