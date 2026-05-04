//
// Created by Emile Guillerm on 05.04.2026.
//

#include "CoursLogique.h"
#include <drogon/HttpController.h>
#include "models/Salle.h"
#include <drogon/utils/coroutine.h>
#include "models/Utilisateur.h"
#include "dao/ClasseDAO.h"
#include "models/Cours.h"
#include "dao/CoursDAO.h"
#include "dao/SalleDAO.h"
#include "dao/UtilisateurDAO.h"
#include "resultat/StructResultat.h"

using namespace drogon::orm;

//Reserver une salle à partir des info:
//heure_debut (timestamp)
//heure_fin (timestamp)
//salle
//classe
//reserver_par (nom professeur
//professeur
//renvoie un bool et potentiellement un message en cas d'erreur (pas de donnee)
drogon::Task<ResultatCoro<>> CoursLogique::ReservationSallePGS(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Body contenant les info de la reservation
    const Json::Value &body) {
    //Création de la structure réponse
    ResultatCoro<> resultat;
    //Verifications des champs
    bool champ_manquant = false;
    if (!body.isMember("heure_debut"))
        champ_manquant = true;
    if (!body.isMember("heure_fin"))
        champ_manquant = true;
    if (!body.isMember("salle"))
        champ_manquant = true;
    if (!body.isMember("classe"))
        champ_manquant = true;
    if (!body.isMember("professeur"))
        champ_manquant = true;
    if (champ_manquant == true) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Un champ est manquant";
        co_return resultat;
    }
    //enregistrement des info dans des variables
    int64_t heure_debut = body["heure_debut"].asInt64();
    int64_t heure_fin = body["heure_fin"].asInt64();
    std::string salle = body["salle"].asString();
    std::string classe = body["classe"].asString();
    std::string reserve_par;
    if (!body.isMember("reserve_par"))
        reserve_par = "ADMIN";
    else
        reserve_par = body["professeur"].asString();
    std::string professeur = body["professeur"].asString();
    //Verification des informations:
    //Trouver la salle
    //Verifier que la salle n'a pas de cours aux horaires actuelle
    //Verifier que la classe existe
    //Verifier le professeur existe

    //verification que la salle existe/ bonne
    auto salletrouvee = co_await SalleDAO::VerifierExistanceSalle(db, salle);
    if (salletrouvee.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Salle introuvable";
        co_return resultat;
    }
    //recherche de cours dans la salle
    auto CoursListe = co_await CoursDAO::ChercherCoursParSalle(db, salle);
    //renvoie un bool false et un message d'erreur correspondant si probleme ou si aucun cours dans cette salle
    if (CoursListe.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur";
        co_return resultat;
    }
    //Recherche parmis tout les cours si les horaire fournit rentre dans la plage horaire d'un cours de la salle et donc impossible de reservé
    bool trouvee = false;

    for (const auto &cour : CoursListe.donnee) {
        //Si L'heure de debut de la reservation n'est pas entre l'heure du debut et l'heure de fin du cours
        if (cour.getValueOfHeureDebut().secondsSinceEpoch() < heure_fin && cour.getValueOfHeureFin().secondsSinceEpoch() > heure_debut) {
            trouvee = true;
            break;
        }
    }
    //Renvoie d'une erreur si un cours est present aux horaire
    if (trouvee) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Creneau occupe";
        co_return resultat;
    }

    //recherche d'une classe avec ce nom
    auto classeverif = co_await ClasseDAO::ChercherClasseParNom(db, classe);
    //si le nom correspond à une classe existante on modifie autrement rien
    if (classeverif.donnee.empty() || classeverif.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Classe inexistante";
        co_return resultat;
    }
    //recherche d'un professeur avec son nom
    auto profverif = co_await UtilisateurDAO::ChercherUtilisateurParNom(db, professeur);
    //si le nom correspond à une classe existante on modifie autrement rien
    if (profverif.donnee.empty() || profverif.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Professeur inexistante";
        co_return resultat;
    }
    //Drogon à besoin d'un trantor pour stocker un timestamp
    //il le stock en micro seconde donc x1 000 000
    trantor::Date heureDebutTrantor (heure_debut*1000000);
    trantor::Date heureFinTrantor (heure_fin*1000000);
    //Creation de l'entre reservation en bdd
    drogon_model::acces_campus_bdd::Cours Reservation;
    Reservation.setIdClasse(classeverif.donnee[0].getValueOfIdClasse());
    Reservation.setHeureDebut(heureDebutTrantor);
    Reservation.setHeureFin(heureFinTrantor);
    Reservation.setNumSalle(salle);
    if (reserve_par == "ADMIN")
        Reservation.setReservePar(0);
    else
        Reservation.setReservePar(profverif.donnee[0].getValueOfIdUser());
    Reservation.setProfesseur(profverif.donnee[0].getValueOfIdUser());
    //Enregistrement en base de donnée
    auto EnregistrementCours= co_await CoursDAO::AjoutReservation(db, Reservation);
    //si le nom correspond à une classe existante on modifie autrement rien
    if (EnregistrementCours.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur enregistrement cours";
        co_return resultat;
    }
    resultat.BoolResultat = true;
    //Envoi du resultat
    co_return resultat;
}

/*
drogon::Task<ResultatCoro> CoursLogique::ReservationSallePSW(
    const DbClientPtr &db,
    const Json::Value &body) {
    int32_t heure_debut = body["heure_debut"].asInt();
    int32_t heure_fin = body["heure_fin"].asInt();
    std::string salle = body["salle"].asString();
    int32_t utilisateur = body["utilisateur"].asInt();
    auto ListeCours = co_await CoursDAO::ChercherCoursParSalle(db, salle);
    if (ListeCours.empty()) {
        co_return ResultatCoro(false, "Salle introuvable");
    }
    trantor::Date TimestampBrut = trantor::Date::now();
    int64_t TimestampSecond = TimestampBrut.secondsSinceEpoch();
    bool trouvee = false;
    for (const auto &cours: ListeCours) {
        if ((cours.getValueOfHeureDebut().secondsSinceEpoch() < heure_debut && cours.getValueOfHeureFin().secondsSinceEpoch() > heure_debut)
            || (cours.getValueOfHeureDebut().secondsSinceEpoch() < heure_fin && cours.getValueOfHeureFin().secondsSinceEpoch() > heure_fin)
            ) {
            trouvee = true;
            break;
            }
    }
    if (!trouvee) {
        co_return ResultatCoro(false, "Ce creneau est deja occupe");
    }
    trantor::Date heureDebutTrantor (static_cast<int64_t>(heure_debut)*1000000);
    trantor::Date heureFinTrantor (static_cast<int64_t>(heure_fin)*1000000);
    drogon_model::acces_campus_bdd::Cours Reservation;
    Reservation.setIdClasse(0);
    Reservation.setHeureDebut(heureDebutTrantor);
    Reservation.setHeureFin(heureFinTrantor);
    Reservation.setNumSalle(salle);
    Reservation.setReservePar(utilisateur);
    auto EnregistrementCours = co_await CoursDAO::AjoutReservation(db, Reservation);
    if ((*EnregistrementCours.getIdCours()) == 0) {
        co_return ResultatCoro(false, "Erreur lors de l'enregistrement du cours");
    }
    co_return ResultatCoro(true, "Finit");
}
*/

//Obtenir le planning d'une salle pour les 7 prochain jour (semaine) du time stamp
//salle
//debut (timestamp)
//renvoie un bool et potentiellement un message en cas d'erreur (pas de donnee)
drogon::Task<ResultatCoro<Json::Value>> CoursLogique::PlanningSalle(
    const DbClientPtr &db,
    const std::string &salle,
    const int64_t &timestamp_debut,
    const int64_t &timestamp_fin) {
    //Création de la structure réponse
    ResultatCoro<Json::Value> resultat;
    //Stocker la liste de cours qui correspondant aux critere horaire
    Json::Value listecours(Json::arrayValue);
    //recherche de cours dans la salle
    auto CoursListe = co_await CoursDAO::ChercherCoursParSalle(db, salle);
    //renvoie un bool false et un message d'erreur correspondant si probleme ou si aucun cours dans cette salle
    if (CoursListe.BoolResultat == false || CoursListe.donnee.empty()) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "La salle n'existe pas";
        co_return resultat;
    }
    //Recherche parmis tout les cours si les horaire fournit rentre dans la plage horaire d'un cours de la salle et donc impossible de reservé
    for (const auto &cour : CoursListe.donnee) {
        //Si L'heure de debut du cours et l'heure de fin du cours fais partie du creneau de timestamp
        if ((cour.getValueOfHeureDebut().secondsSinceEpoch() > timestamp_debut && cour.getValueOfHeureFin().secondsSinceEpoch() < timestamp_fin)){
            Json::Value CoursTemporaire;
            CoursTemporaire["id_cours"] = cour.getValueOfIdCours();
            CoursTemporaire["heure_debut"] = cour.getValueOfHeureDebut().secondsSinceEpoch();
            CoursTemporaire["heure_fin"] = cour.getValueOfHeureFin().secondsSinceEpoch();
            //recherche d'une classe avec son id
            auto classeverif = co_await ClasseDAO::ChercherClasseParID(db, cour.getValueOfIdClasse());
            //si l'id correspond à une classe existante on modifie autrement rien
            if (classeverif.donnee.empty() || classeverif.BoolResultat == false) {
                CoursTemporaire["classe"] = "inconnue";
            }
            else
                CoursTemporaire["classe"] = classeverif.donnee[0].getValueOfNomClasse();
            listecours.append(CoursTemporaire);
        }
    }
    //Ajout de la liste et envoie resultat
    resultat.BoolResultat = true;
    resultat.donnee = listecours;
    co_return resultat;
}


//Obtenir les salle disponible pour une periode donnée
//debut (timestamp)
//fin (timestamp)
//classe
//reserver_par (nom professeur
//professeur
//renvoie un bool et potentiellement un message en cas d'erreur (pas de donnee)
drogon::Task<ResultatCoro<Json::Value>> CoursLogique::SalleDisponible(
    const DbClientPtr &db,
    const int64_t &timestamp_debut,
    const int64_t &timestamp_fin) {
    //Création de la structure réponse
    ResultatCoro<Json::Value> resultat;
    //Stocker la liste de cours qui correspondant aux critere horaire
    Json::Value listesalle(Json::arrayValue);
    //recherche de cours dans la salle
    auto ListeSalle = co_await SalleDAO::ListeSalle(db);
    //renvoie un bool false et un message d'erreur correspondant si probleme ou si aucun cours dans cette salle
    if (ListeSalle.BoolResultat == false) {
        resultat.BoolResultat = false;
        co_return resultat;
    }
    auto ListeCour = co_await CoursDAO::CoursIntervalleTimestamp(db, timestamp_debut, timestamp_fin);
    //renvoie un bool false et un message d'erreur correspondant si probleme ou si aucun cours dans cette salle
    if (ListeCour.BoolResultat == false) {
        resultat.BoolResultat = false;
        co_return resultat;
    }
    //Recherche des salle qui ne sont pas presente dans liste de cours de l'intervalle
    for (const auto &salle : ListeSalle.donnee) {
        bool occupee = false;
        for (const auto &cour : ListeCour.donnee) {
            if (salle.getPrimaryKey() == cour.getValueOfNumSalle()) {
                occupee = true;
                break;
            }
        }
        if (!occupee) {
            Json::Value salleTemporaire;
            salleTemporaire["id_salle"] = salle.getPrimaryKey();
            listesalle.append(salleTemporaire);
        }
    }
    //Ajout de la liste et envoie resultat
    resultat.BoolResultat = true;
    resultat.donnee = listesalle;
    co_return resultat;
}
