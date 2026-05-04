//
// Created by Emile Guillerm on 04.04.2026.
//

#include "BadgeLogique.h"
#include <drogon/orm/DbClient.h>
#include "models/Utilisateur.h"  // Classe générée par drogon_ctl
#include <functional>
#include <json/json.h>
#include "dao/BadgeDAO.h"
#include "resultat/StructResultat.h"
#include "models/Retardabsence.h"
#include "dao/UtilisateurDAO.h"
#include "dao/ClasseDAO.h"
#include "dao/SalleDAO.h"
#include "dao/CoursDAO.h"
#include "dao/PresenceAbsenceCoursDAO.h"
#include "dao/RetardAbsenceDAO.h"

const int VALEURMARGECOURS = 5*60;



//Obtenir les informatons d'un badge à partir de l'id
//Renvoie un bool et un JSON avec:
//nom_user
//prenom_user
//classe
//statut (eleve, professeur, admin)
//
//Peut renvoyer un messsage en cas d'erreur
drogon::Task<ResultatCoro<Json::Value>> BadgeLogique::InformationBadge(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
     const drogon::orm::DbClientPtr &db,
     //Uid Badge
     const std::string &uidBadge) {
    //Création de la structure réponse
    ResultatCoro<Json::Value> resultat;
    //recherche de l'utilisateur correspondant à l'id du badge
    auto UtilisateurTrouvee = co_await BadgeDAO::ChercherUtilisateurParIDBadge(db, uidBadge);
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (UtilisateurTrouvee.donnee.empty() || UtilisateurTrouvee.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Pas d'utilisateur trouvée";
        co_return resultat;
    }
    if (UtilisateurTrouvee.donnee[0].getValueOfRoleUser() != std::string("eleve")) {
        resultat.MessageResultat = "Professeur";
    }
    else {
        //Recherche de la classe à partir d'id pour obtenir le nom de la classe
        auto classe = co_await ClasseDAO::ChercherClasseParID(db, UtilisateurTrouvee.donnee[0].getValueOfIdClasse());
        //Si pas de classe trouvee met la classe à introuvable --> la reponse est quand meme bonne
        if (classe.donnee.empty() || classe.BoolResultat == false) {
            resultat.donnee["classe"] = "Introuvable";
        }
        //Si reponse trouvee on la met
        else {
            resultat.donnee["classe"] = (UtilisateurTrouvee.donnee[0].getValueOfIdClasse());
        }
    }
    //Ajout des donnee à la réponse
    resultat.BoolResultat = true;resultat.donnee["nom_user"] = (*UtilisateurTrouvee.donnee[0].getNomUser());
    resultat.donnee["prenom_user"] = (*UtilisateurTrouvee.donnee[0].getPrenomUser());
    resultat.donnee["statut"] = (*UtilisateurTrouvee.donnee[0].getRoleUser());

    co_return resultat;
}

//Supprimer un badge à partir de sont ID --> le champs seras mis à null dans la table de l'utilisateur
//Renvoi un bool et potentiellement un message en cas d'erreur (pas de donnee)
drogon::Task<ResultatCoro<>> BadgeLogique::SupprimerBadge(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //id du badge à supprimer
    const std::string &uidBadge) {
    //Création de la structure réponse
    ResultatCoro<> resultat;
    //Suppression du badge
    auto reponseBadgeSupprimer = co_await BadgeDAO::SupprimerBadgeParIDBadge(db, uidBadge);
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (reponseBadgeSupprimer.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = reponseBadgeSupprimer.MessageResultat;
        co_return resultat;
    }
    //Si bon renvoie true
    resultat.BoolResultat = true;
    co_return resultat;
}

//Creation d'un badge en base avec ajout à un utilisateur
//renvoie un bool et potentiellement un message en cas d'erreur (pas de donnee)
drogon::Task<ResultatCoro<>> BadgeLogique::CreationBadge(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //Id de l'utilisateur
    const std::int32_t &uiduser,
    //uId du badge
    const std::string &uidBadge) {
    //Création de la structure réponse
    ResultatCoro<> resultat;
    //Recherche du badge pour verifier si il n'existe pas déjà dans la base de donnée
    //renvoie un bool false et un message d'erreur correspondant si probleme
    auto VerificationExistanceDuBadge = co_await BadgeDAO::ChercherBadgeParIDBadge(db, uidBadge);
    if (VerificationExistanceDuBadge.BoolResultat==false || !VerificationExistanceDuBadge.donnee.empty()) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Badge déja present ou erreur";
        co_return resultat;
    }
    //Recherche de l'utilisateur à partir de son id
    //renvoie un bool false et un message d'erreur correspondant si probleme
    auto TrouverUtilisateurAssocier = co_await UtilisateurDAO::ChercherUtilisateurParID(db, uiduser);
    if (TrouverUtilisateurAssocier.donnee.empty() || TrouverUtilisateurAssocier.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Utilisateur introuvable";
        co_return resultat;
    }
    //Creation du badge
    drogon_model::acces_campus_bdd::Badge Nouveaubadge;
    Nouveaubadge.setUuidBadge(uidBadge);
    //ajout du badge en base de donnée
    //renvoie un bool false et un message d'erreur correspondant si probleme
    auto resultatAjoutBadge = co_await BadgeDAO::AjoutBadge(db, Nouveaubadge);
    if (resultatAjoutBadge.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Le badge n'a pas pus etre crée";
        co_return resultat;
    }
    //Ajout du badge à l'utilisateur
    drogon_model::acces_campus_bdd::Utilisateur utilisateur = TrouverUtilisateurAssocier.donnee[0];
    utilisateur.setUuidBadge(uidBadge);
    //Liaison du badge à un utilisateur
    //renvoie un bool false et un message d'erreur correspondant si probleme
    auto resultatLiaison = co_await UtilisateurDAO::MettreAJourUser(db, utilisateur);
    //Dans le cas ou il n'a pas pus être ajouter pas besoin d'annuler l'operation precedente car on peut le supprimer à partir d'un autre endpoint api
    if (resultatLiaison.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Le badge n'a pas pus etre lié";
        co_return resultat;
    }
    resultat.BoolResultat = true;
    resultat.MessageResultat = "Ok";
    co_return resultat;
}

//Modfication des info d'utilisateur à partir de sont id de badge
//renvoie un bool et potentiellement un message en cas d'erreur (pas de donnee)
drogon::Task<ResultatCoro<>> BadgeLogique::ModifierInfoUtilisateur(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //id du badge de l'utilisateur dont il faut mdodifier les informations
    const std::string &uidBadge,
    //donnee à changer
    const Json::Value &body) {
    //Création de la structure réponse
    ResultatCoro<> resultat;
    //recherche de l'utilisateur correspondant à l'id du badge
    auto UtilisateurTrouvee = co_await BadgeDAO::ChercherUtilisateurParIDBadge(db, uidBadge);
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (UtilisateurTrouvee.donnee.empty() || UtilisateurTrouvee.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Pas d'utilisateur trouvée";
        co_return resultat;
    }
    auto utilisateurModifier = UtilisateurTrouvee.donnee[0];
    if (body.isMember("nom"))
        utilisateurModifier.setNomUser(body["nom"].asString());
    if (body.isMember("prenom"))
        utilisateurModifier.setPrenomUser(body["prenom"].asString());
    //Obtention et test de classe car je recoit un string et je dois stocker un id de classe donc
    //Verification qu'il existe en body
    if (body.isMember("classe")) {
        //recherche d'une classe avec ce nom
        auto classe = co_await ClasseDAO::ChercherClasseParNom(db, body["classe"].asString());
        //si le nom correspond à une classe existante on modifie autrement rien
        if (!classe.donnee.empty() && classe.BoolResultat == true) {
            utilisateurModifier.setIdClasse(classe.donnee[0].getValueOfIdClasse());
        }
    }
    //a modifier
    if (body.isMember("hash_mdp"))
        utilisateurModifier.setHashMdp(body["hash_mdp"].asString());
    if (body.isMember("login"))
        utilisateurModifier.setLoginUser(body["login"].asString());
    if (body.isMember("role"))
       utilisateurModifier.setRoleUser(body["role"].asString());
    //Changement des informations
    //renvoie un bool false et un message d'erreur correspondant si probleme
    auto resultatLiaison = co_await UtilisateurDAO::MettreAJourUser(db, utilisateurModifier);
    if (resultatLiaison.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Les informations n'ont pus etre mis à jour";
        co_return resultat;
    }
    resultat.BoolResultat = true;
    co_return resultat;
}

//Verifier un badge peut importe quand si l'utilisateur à déjà eu cours dans cette salle
//Renvoie un bool et un JSON avec:
//authorisee
//nom
//prenom
//classe
//
//Peut renvoyer un messsage en cas d'erreur
drogon::Task<ResultatCoro<Json::Value>> BadgeLogique::VerifierBadgePEA(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //id badge
    const std::string &uidBadge,
    //adresse mac pea
    const std::string &mac) {
    //Création de la structure réponse
    ResultatCoro<Json::Value> resultat;
    //recherche de l'utilisateur correspondant à l'id du badge
    auto UtilisateurTrouvee = co_await BadgeDAO::ChercherUtilisateurParIDBadge(db, uidBadge);
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (UtilisateurTrouvee.donnee.empty() || UtilisateurTrouvee.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Utilisateur introuvable";
        co_return resultat;
    }
    //On garde uniquement le premier
    auto utilisateur = UtilisateurTrouvee.donnee[0];
    //Si c'est un professeur ou un admin il a accès a toutes les salles donc:
    if ((*utilisateur.getRoleUser()) == "admin" || (*utilisateur.getRoleUser()) == "professeur") {
        resultat.donnee["nom"] = (*utilisateur.getNomUser());
        resultat.donnee["prenom"] = (*utilisateur.getPrenomUser());
        //Les professeur et admin n'on pas de classe donc je met leur role
        resultat.donnee["classe"] = (*utilisateur.getRoleUser());
        resultat.donnee["autorisee"] = true;
        //envoi du resultat
        co_return resultat;
    }
    //recherche de la salle correspondant à l'adresse mac
    auto salletrouvee = co_await SalleDAO::ChercherSalleAdresseMACpea(db, mac);
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (salletrouvee.BoolResultat == false || salletrouvee.donnee.empty()) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Salle introuvable";
        co_return resultat;
    }
    //On garde uniquement le premier
    auto salle = salletrouvee.donnee[0];
    //recherche de cours dans la salle
    auto CoursListe = co_await CoursDAO::ChercherCoursParSalle(db, (*salle.getNumSalle()));
    //renvoie un bool false et un message d'erreur correspondant si probleme ou si aucun cours dans cette salle
    if (CoursListe.BoolResultat == false || CoursListe.donnee.empty()) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "L'utilisateur n'as pas accès";
        co_return resultat;
    }
    //Recherche parmis tout les cours dans cette salle la classe de l'utilisateur à déjà été présente
    bool trouvee = false;
    for (const auto &cours : CoursListe.donnee) {
        if (utilisateur.getValueOfIdClasse() == cours.getValueOfIdClasse()) {
            trouvee = true;
            break;
        }
    }
    //Verification de la recherche et envoie un bool false si l'utilisateur n'a jamais eu cours dans cette salle
    if (!trouvee) {
        resultat.BoolResultat = false;
        co_return resultat;
    }
    //Ajout des reponse obtenu
    resultat.BoolResultat = true;
    resultat.donnee["nom"] = (*utilisateur.getNomUser());
    resultat.donnee["prenom"] = (*utilisateur.getPrenomUser());
    //recherche de classe pour obtenir le nom
    auto classe = co_await ClasseDAO::ChercherClasseParID(db, utilisateur.getValueOfIdClasse());
    //renvoie un bool false et un message d'erreur correspondant si probleme ou si aucun cours dans cette salle
    if (classe.BoolResultat == false || classe.donnee.empty()) {
        resultat.donnee["classe"] = "Introuvable";
    }
    else
        resultat.donnee["classe"] = classe.donnee[0].getValueOfNomClasse();
    resultat.donnee["autorisee"] = true;
    //envoi du resultat
    co_return resultat;
}

//Scanne le badge de l'utilisateur et l'ajoute au presence
//Renvoie un bool et un JSON avec:
//nom
//prenom
//classe
//heure_arrivee
//
//Peut renvoyer un messsage en cas d'erreur
drogon::Task<ResultatCoro<Json::Value>> BadgeLogique::ScanneBadgeBAE(
    //Alias d'un shared pointeur vers le client postgres (pour gère la connexion)(nécessaire pour utiliser la db au niveau du DAO)
    const DbClientPtr &db,
    //ID du badge
    const std::string &uidBadge,
    //Adresse MAC de la BAE
    const std::string &mac,
    //Horaire du badgeage (timestamp)
    const std::uint64_t &heure_badgage) {
    //Création de la structure réponse
    ResultatCoro<Json::Value> resultat;
    //recherche de l'utilisateur correspondant à l'id du badge
    auto UtilisateurTrouvee = co_await BadgeDAO::ChercherUtilisateurParIDBadge(db, uidBadge);
    //renvoie un bool false et un message d'erreur correspondant si probleme
    if (UtilisateurTrouvee.donnee.empty() || UtilisateurTrouvee.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Utilisateur introuvable";
        co_return resultat;
    }
    //On garde uniquement le premier
    auto utilisateur = UtilisateurTrouvee.donnee[0];
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
        resultat.MessageResultat = "L'utilisateur n'as pas accès";
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
        resultat.MessageResultat = "Pas de cours dans cette salle actuellement";
        co_return resultat;
    }
    //Verification de la présence de l'élève dans la table des absences lié à ce cours. Autrement cela signifie surement qu'il la déjà scanner
    auto AbsenceTable = co_await PresenceAbsenceCoursDAO::ChercherUtilisateurDansAbsencecoursID(db, utilisateur.getValueOfIdUser());
    //renvoie un bool false et un message d'erreur correspondant si probleme ou si l'utilisateur à déjà badgé
    if (AbsenceTable.donnee.empty()) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "badge déjà scanné";
        co_return resultat;
    }
    if (AbsenceTable.BoolResultat == false){
        resultat.BoolResultat = false;
        resultat.MessageResultat = AbsenceTable.MessageResultat;
        co_return resultat;
    }
    //Verification des horaire pour savoir si l'utilisateur est en retard
    //Il y'a une marge accepeter de retard de 5min
    if (heure_badgage > cour.getValueOfHeureDebut().secondsSinceEpoch() + VALEURMARGECOURS) {
        //Creation d'une nouvelle entrée dans la table retard retard
        drogon_model::acces_campus_bdd::Retardabsence CreationRetard;
        //remplissage de l'absence
        CreationRetard.setIdCours(cour.getValueOfIdCours());
        CreationRetard.setIdUser(utilisateur.getValueOfIdUser());
        //diviser par 60 car on veut des minute pas des secondes
        CreationRetard.setTempsRetardMin((heure_badgage - cour.getValueOfHeureDebut().secondsSinceEpoch())/60);
        CreationRetard.setTimestampHeureCours(cour.getValueOfHeureDebut());
        CreationRetard.setAbsence(false);
        //Ajout de la nouvelle entrée dans la table
        auto AjoutRetardAbsence = co_await RetardAbsenceDAO::AjoutRetardAbsence(db, CreationRetard);
        //renvoie un bool false et un message d'erreur correspondant si probleme
        if (AjoutRetardAbsence.BoolResultat == false) {
            resultat.BoolResultat = false;
            resultat.MessageResultat = "Erreur lors de l'enregistrement du retard";
            co_return resultat;
        }
    }
    //Creation de la nouvelle entrée dans la table presence
    drogon_model::acces_campus_bdd::PresenceCours NouvellePresence;
    NouvellePresence.setIdClasse(utilisateur.getValueOfIdClasse());
    NouvellePresence.setIdCours(cour.getValueOfIdCours());
    NouvellePresence.setIdUser(utilisateur.getValueOfIdUser());
    //Ajout de l'entrée dans la table présence
    auto AjoutPresence = co_await PresenceAbsenceCoursDAO::AjoutUtilisateurPresenceCours(db, NouvellePresence);
    //renvoie un bool false et un message d'erreur correspondant si probleme ou si l'utilisateur à déjà badgé
    if (AjoutPresence.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur lors de l'enregistrement de la presence";
        co_return resultat;
    }
    //Suppression de l'utilisateur dans la table absence
    auto SuppresionAbsence = co_await PresenceAbsenceCoursDAO::SupprimerUtilisateurAbsenceCours(db, utilisateur.getValueOfIdUser());
    if (SuppresionAbsence.BoolResultat == false) {
        resultat.BoolResultat = false;
        resultat.MessageResultat = "Erreur lors de la suppression de l'absence";
        co_return resultat;
    }
    //Creation de la réponse JSON
    resultat.BoolResultat = true;
    resultat.donnee["nom"] = (utilisateur.getValueOfNomUser());
    resultat.donnee["prenom"] = (utilisateur.getValueOfPrenomUser());
    //recherche de classe pour obtenir le nom
    auto classe = co_await ClasseDAO::ChercherClasseParID(db, utilisateur.getValueOfIdClasse());
    //renvoie un bool false et un message d'erreur correspondant si probleme ou si aucun cours dans cette salle
    if (classe.BoolResultat == false || classe.donnee.empty()) {
        resultat.donnee["classe"] = "Introuvable";
    }
    else
        resultat.donnee["classe"] = classe.donnee[0].getValueOfNomClasse();
    //est ce que je renvoie ???
    //resultat.donnee["heure_badgeage"] = heure_badgage;
    //Envoie de la réponse
    co_return resultat;
}