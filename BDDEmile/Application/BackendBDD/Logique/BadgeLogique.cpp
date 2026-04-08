//
// Created by Emile Guillerm on 04.04.2026.
//

#include "BadgeLogique.h"
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>
#include "models/Utilisateur.h"  // Classe générée par drogon_ctl
#include <functional>
#include <json/json.h>
#include "dao/BadgeDAO.h"
#include "resultat/StructResultat.h"
#include "models/Retardabsence.h"

const int VALEURMARGECOURS = 5;



//Fonction logique pour obtenir les informatons d'un badge
drogon::Task<Json::Value> BadgeLogique::InformationBadge(
     //Gère les connexion avecc la DB (TCP, nombre de connexion, répartition, etc)
     const drogon::orm::DbClientPtr &db,
     //Uid Badge
     const std::string &uidBadge) {
    //Execute et stock la commande base de donnée sous forme d'un utilisateur
    auto UtilisateurTrouvee = co_await BadgeDAO::ChercherUtilisateurParIDBadge(db, uidBadge);
    if (UtilisateurTrouvee.empty()) {
        co_return Json::Value::null;
    }
    co_return UtilisateurTrouvee[0].toJson();
}

drogon::Task<ResultatCoro> BadgeLogique::SupprimerBadge(
    const DbClientPtr &db,
    const std::string &uidBadge) {
    ResultatCoro ResultatSupprimerBadge;
    auto NbBadgeSupprimer = co_await BadgeDAO::SupprimerBadgeParIDBadge(db, uidBadge);
    if (NbBadgeSupprimer == 0) {
        ResultatSupprimerBadge.BoolResultat = false;
        ResultatSupprimerBadge.MessageResultat = "Badge introuvable";
        co_return ResultatSupprimerBadge;
    }
    ResultatSupprimerBadge.BoolResultat = true;
    ResultatSupprimerBadge.MessageResultat = "Ok";
    co_return ResultatSupprimerBadge;
}


drogon::Task<ResultatCoro> BadgeLogique::CreationBadge(
    const DbClientPtr &db,
    const std::int32_t &uiduser,
    const std::string &uidBadge) {
    ResultatCoro ResultatMaJBadge;
    auto VerificationExistanceDuBadge = co_await BadgeDAO::ChercherBadgeParIDBadge(db, uidBadge);
    if (VerificationExistanceDuBadge.empty()==false) {
        ResultatMaJBadge.BoolResultat = false;
        ResultatMaJBadge.MessageResultat = "Badge déja present";
        co_return ResultatMaJBadge;
    }
    auto TrouverUtilisateurAssocier = co_await BadgeDAO::ChercherUtilisateurParIDUtilisateur(db, uiduser);
    if (TrouverUtilisateurAssocier.empty()) {
        ResultatMaJBadge.BoolResultat = false;
        ResultatMaJBadge.MessageResultat = "Pas d'utilisateur trouvé";
        co_return ResultatMaJBadge;
    }
    TrouverUtilisateurAssocier[0].setUuidBadge(uidBadge);
    auto MiseAJourUtilisateurAvecBadge = co_await BadgeDAO::MettreAJourUser(db, TrouverUtilisateurAssocier[0]);
    if (MiseAJourUtilisateurAvecBadge == 0) {
        ResultatMaJBadge.BoolResultat = false;
        ResultatMaJBadge.MessageResultat = "Erreur lors de l'association du badge a l'utilisateur";
        co_return ResultatMaJBadge;
    }
    ResultatMaJBadge.BoolResultat = true;
    ResultatMaJBadge.MessageResultat = "Ok";
    co_return ResultatMaJBadge;
}


drogon::Task<ResultatCoro> BadgeLogique::ModifierInfoUtilisateur(
    const DbClientPtr &db,
    const std::string &uidBadge,
    const Json::Value &body) {
    ResultatCoro ResultatModifierUtilisateur;
    auto UtilisateurListe = co_await BadgeDAO::ChercherUtilisateurParIDBadge(db, uidBadge);
    if (UtilisateurListe.empty()) {
        ResultatModifierUtilisateur.BoolResultat = false;
        ResultatModifierUtilisateur.MessageResultat = "Utilisateur introuvable";
        co_return ResultatModifierUtilisateur;
    }
    auto utilisateurModifier = UtilisateurListe[0];
    if (body.isMember("nom"))
        utilisateurModifier.setNomUser(body["nom"].asString());
    if (body.isMember("prenom"))
        utilisateurModifier.setPrenomUser(body["prenom"].asString());
    if (body.isMember("classe"))
        utilisateurModifier.setIdClasse(stoi(body["classe"].asString()));
    if (body.isMember("hash_mdp"))
        utilisateurModifier.setHashMdp(body["hash_mdp"].asString());
    if (body.isMember("login"))
        utilisateurModifier.setLoginUser(body["login"].asString());
    if (body.isMember("role"))
       utilisateurModifier.setRoleUser(body["role"].asString());

    auto MiseAJourUtilisateur = co_await BadgeDAO::MettreAJourUser(db, utilisateurModifier);
    if (MiseAJourUtilisateur == 0) {
        ResultatModifierUtilisateur.BoolResultat = false;
        ResultatModifierUtilisateur.MessageResultat = "Erreur lors de la mise a jour de l'utilisateur";
        co_return ResultatModifierUtilisateur;
    }
    ResultatModifierUtilisateur.BoolResultat = true;
    ResultatModifierUtilisateur.MessageResultat = "Ok";
    co_return ResultatModifierUtilisateur;
}


drogon::Task<ResultatCoro> BadgeLogique::VerifierBadgePEA(
    const DbClientPtr &db,
    const std::string &uidBadge,
    const std::string &mac) {
    ResultatCoro ResultatVerificationPEA;
    auto UtilisateurBadge = co_await BadgeDAO::ChercherUtilisateurParIDBadge(db, uidBadge);
    if (UtilisateurBadge.empty()) {
        ResultatVerificationPEA.BoolResultat = false;
        ResultatVerificationPEA.MessageResultat = "Utilisateur introuvable";
        co_return ResultatVerificationPEA;
    }
    auto utilisateurAVerifier = UtilisateurBadge[0];
    auto SalleListe = co_await BadgeDAO::ChercherSalleAdresseMACpea(db, mac);
    if (SalleListe.empty()) {
        ResultatVerificationPEA.BoolResultat = false;
        ResultatVerificationPEA.MessageResultat = "Salle introuvable";
        co_return ResultatVerificationPEA;
    }
    auto SalleAcceder = SalleListe[0];
    auto CoursListe = co_await BadgeDAO::ChercherCoursParSalle(db, (*SalleAcceder.getNumSalle()));
    if (CoursListe.empty()) {
        ResultatVerificationPEA.BoolResultat = false;
        ResultatVerificationPEA.MessageResultat = "Aucun cours trouve pour cette salle";
        co_return ResultatVerificationPEA;
    }
    bool trouvee = false;
    for (const auto &cours: CoursListe) {
        if (utilisateurAVerifier.getValueOfIdClasse() == cours.getValueOfIdClasse()) {
            trouvee = true;
            break;
        }
    }
    if (!trouvee) {
        ResultatVerificationPEA.BoolResultat = false;
        ResultatVerificationPEA.MessageResultat = "L'eleve n'est pas inscrit dans cette salle";
        co_return ResultatVerificationPEA;
    }
    ResultatVerificationPEA.BoolResultat = true;
    ResultatVerificationPEA.MessageResultat = "Ok";
    co_return ResultatVerificationPEA;
}


drogon::Task<ResultatCoro> BadgeLogique::ScanneBadgeBAE(
const DbClientPtr &db,
const std::string &uidBadge,
const std::string &mac,
const std::uint64_t &heure_badgage) {
    ResultatCoro ResultatScanneBAE;
    auto UtilisateurBadge = co_await BadgeDAO::ChercherUtilisateurParIDBadge(db, uidBadge);
    if (UtilisateurBadge.empty()) {
        ResultatScanneBAE.BoolResultat = false;
        ResultatScanneBAE.MessageResultat = "Utilisateur introuvable";
        co_return ResultatScanneBAE;
    }
    auto utilisateurAVerifier = UtilisateurBadge[0];
    auto SalleListe = co_await BadgeDAO::ChercherSalleAdresseMACpea(db, mac);
    if (SalleListe.empty()) {
        ResultatScanneBAE.BoolResultat = false;
        ResultatScanneBAE.MessageResultat = "Salle introuvable";
        co_return ResultatScanneBAE;
    }
    auto SalleAcceder = SalleListe[0];
    auto CoursListe = co_await BadgeDAO::ChercherCoursParSalle(db, (*SalleAcceder.getNumSalle()));
    if (CoursListe.empty()) {
        ResultatScanneBAE.BoolResultat = false;
        ResultatScanneBAE.MessageResultat = "Aucun cours trouve pour cette salle";
        co_return ResultatScanneBAE;
    }
    drogon_model::ProjetV1::Cours CoursActuelle;


    trantor::Date TimestampBrut = trantor::Date::now();
    int64_t TimestampSecond = TimestampBrut.secondsSinceEpoch();
    bool trouvee = false;
    for (const auto &cours: CoursListe) {
        if (cours.getValueOfHeureDebut().secondsSinceEpoch() - VALEURMARGECOURS < TimestampSecond && cours.getValueOfHeureFin().secondsSinceEpoch() > TimestampSecond) {
            //A l'heure
            CoursActuelle = cours;
            trouvee = true;
            if (heure_badgage > cours.getValueOfHeureDebut().secondsSinceEpoch() + VALEURMARGECOURS) {
                drogon_model::ProjetV1::Retardabsence CreationRetard;
                CreationRetard.setIdCours((*CoursActuelle.getIdCours()));
                CreationRetard.setIdUser((*utilisateurAVerifier.getIdUser()));
                CreationRetard.setTempsRetardMin((heure_badgage - cours.getValueOfHeureDebut().secondsSinceEpoch())/1000);
                CreationRetard.setTimestampHeureCours((*CoursActuelle.getHeureDebut()));
                CreationRetard.setAbsence(false);
                auto AjoutRetardAbsence = co_await BadgeDAO::AjoutRetardAbsence(db, CreationRetard);
                if (AjoutRetardAbsence.getIdUser() != utilisateurAVerifier.getIdUser()) {
                    ResultatScanneBAE.BoolResultat = false;
                    ResultatScanneBAE.MessageResultat = "Erreur lors de l'enregistrement du retard";
                    co_return ResultatScanneBAE;
                }
            }
            break;
        }
    }
    if (!trouvee) {
        ResultatScanneBAE.BoolResultat = false;
        ResultatScanneBAE.MessageResultat = "Aucun cours en cours dans cette salle";
        co_return ResultatScanneBAE;
    }
    auto AbsenceTable = co_await BadgeDAO::ChercherUtilisateurDansAbsencecours(db, (*SalleAcceder.getNumSalle()));
    if (AbsenceTable.empty()) {
        ResultatScanneBAE.BoolResultat = false;
        ResultatScanneBAE.MessageResultat = "Utilisateur non trouve dans la liste d'absence";
        co_return ResultatScanneBAE;
    }
    auto AbsenceUtilisateur = AbsenceTable[0];
    drogon_model::ProjetV1::PresenceCours UtilisateurPresenceChangement;
    UtilisateurPresenceChangement.setIdClasse((*AbsenceUtilisateur.getIdClasse()));
    UtilisateurPresenceChangement.setIdCours((*AbsenceUtilisateur.getIdCours()));
    UtilisateurPresenceChangement.setIdUser((*AbsenceUtilisateur.getIdUser()));

    auto AjoutPresence = co_await BadgeDAO::AjoutUtilisateurPresenceCours(db, UtilisateurPresenceChangement);
    if (AjoutPresence.getIdUser() != utilisateurAVerifier.getIdUser()) {
        ResultatScanneBAE.BoolResultat = false;
        ResultatScanneBAE.MessageResultat = "Erreur lors de l'enregistrement de la presence";
        co_return ResultatScanneBAE;
    }

    auto SuppresionAbsence = co_await BadgeDAO::SupprimerUtilisateurAbsence(db, AbsenceUtilisateur.getValueOfIdAbsence());
    if (SuppresionAbsence == 0) {
        ResultatScanneBAE.BoolResultat = false;
        ResultatScanneBAE.MessageResultat = "Erreur lors de la suppression de l'absence";
        co_return ResultatScanneBAE;
    }

    ResultatScanneBAE.BoolResultat = true;
    ResultatScanneBAE.MessageResultat = "Ok";
    co_return ResultatScanneBAE;
}