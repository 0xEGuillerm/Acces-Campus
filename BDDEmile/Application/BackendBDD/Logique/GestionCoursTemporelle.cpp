//
// Created by Emile Guillerm on 06.05.2026.
//

#include "GestionCoursTemporelle.h"
#include <drogon/HttpController.h>
#include <drogon/utils/coroutine.h>
#include "models/Utilisateur.h"
#include "models/Cours.h"
#include "dao/CoursDAO.h"
#include "dao/PresenceAbsenceCoursDAO.h"
#include "dao/UtilisateurDAO.h"
#include "resultat/StructResultat.h"
#include "dao/RetardAbsenceDAO.h"






//Verification qu'un cour commence et ajout
drogon::Task<> GestionCoursTemporelle::traiterCoursADemarrer() {
    auto db = drogon::app().getDbClient();
    //Obtention de l'horaire actuelle format int seconde
    int64_t TimestampMinuteDebut = (trantor::Date::now().microSecondsSinceEpoch()/60000000)*60000000;
    int64_t TimestampMinuteFin = TimestampMinuteDebut + 60000000;
    auto ListeCours = co_await CoursDAO::CoursDebutIntervalleMinute(db, TimestampMinuteDebut, TimestampMinuteFin);
    if (!ListeCours.donnee.empty()) {
        for (const auto &coursAjout : ListeCours.donnee) {
            //recherche de liste utilisateur correspondant à l'id classe
            auto Utilisateurliste = co_await UtilisateurDAO::ChercherUtilisateurParClasse(db, coursAjout.getValueOfIdClasse());
            //renvoie un bool false et un message d'erreur correspondant si probleme
            if (!Utilisateurliste.donnee.empty() && Utilisateurliste.BoolResultat == true) {
                for (const auto &utilisateur : Utilisateurliste.donnee) {
                    drogon_model::acces_campus_bdd::AbsenceCours EntreeAbsence;
                    EntreeAbsence.setIdClasse(coursAjout.getValueOfIdClasse());
                    EntreeAbsence.setIdCours(coursAjout.getValueOfIdCours());
                    EntreeAbsence.setIdUser(utilisateur.getValueOfIdUser());
                    co_await PresenceAbsenceCoursDAO::AjoutUtilisateurAbsenceCours(db, EntreeAbsence);
                }
            }
        }
    }
    co_return;
}

//Verification qu'un cour commence est finit et suppression + ajout absence
drogon::Task<> GestionCoursTemporelle::traiterCoursACloturer() {
    auto db = drogon::app().getDbClient();
    //Obtention de l'horaire actuelle format int seconde
    int64_t TimestampMinuteDebut = (trantor::Date::now().microSecondsSinceEpoch()/60000000)*60000000;
    int64_t TimestampMinuteFin = TimestampMinuteDebut + 60000000;
    //Obtention de tout les cours qui finissent
    auto ListeCours = co_await CoursDAO::CoursFinIntervalleMinute(db, TimestampMinuteDebut, TimestampMinuteFin);
    //Si pas de cours rien
    if (!ListeCours.donnee.empty()) {
        //Boucle pour manipuler chaque cours 1 a 1
        for (const auto &coursAjout : ListeCours.donnee) {
            //Ajout de l'absence
            //recherche de tous les éleves dans la table absence avec l'id du cours en question
            auto ListeAbsence = co_await PresenceAbsenceCoursDAO::ChercherUtilisateurDansAbsencecours(db, coursAjout.getValueOfIdCours());
            if (!ListeAbsence.donnee.empty() && ListeAbsence.BoolResultat == true) {
                for (const auto &Absence : ListeAbsence.donnee) {
                    //Creation d'une nouvelle entrée dans la table retard retard
                    drogon_model::acces_campus_bdd::Retardabsence CreationRetard;
                    //remplissage de l'absence
                    CreationRetard.setIdCours(coursAjout.getValueOfIdCours());
                    CreationRetard.setIdUser(Absence.getValueOfIdUser());
                    //diviser par 60 car on veut des minute pas des secondes
                    CreationRetard.setTempsRetardMin(0);
                    CreationRetard.setTimestampHeureCours(coursAjout.getValueOfHeureDebut());
                    CreationRetard.setAbsence(true);
                    //Ajout de la nouvelle entrée dans la table
                    auto AjoutRetardAbsence = co_await RetardAbsenceDAO::AjoutRetardAbsence(db, CreationRetard);
                }
            }
            //Suppresion des table presence et absence
            co_await PresenceAbsenceCoursDAO::SupprimerUtilisateurAbsenceCoursIDcour(db , coursAjout.getValueOfIdCours());
            co_await PresenceAbsenceCoursDAO::SupprimerUtilisateurPresenceCours(db , coursAjout.getValueOfIdCours());

        }
    }
    co_return;
}