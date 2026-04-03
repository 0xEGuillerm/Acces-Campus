//
// Created by Emile Guillerm on 30.03.2026.
//

#include "BadgeDAO.h"
#include "models/Badge.h"
#include "models/Utilisateur.h"
#include "models/Salle.h"
#include "models/Cours.h"
#include "AbsenceCours.h"
#include "PresenceCours.h"
#include <chrono>

const int VALEURMARGECOURS = 5;

void InformationBadge(
    const DbClientPtr &db,
    const std::string &uid,
    //Appel la fonction BadgeObtenu(callback)
    std::function<void(const Json::Value &body)> &&InformationObtenu,
    std::function<void(const DrogonDbException &)> &&Erreur) {
    Mapper<drogon_model::ProjetV1::Utilisateur> mapperuser(db);

    mapperuser.findBy(

        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Utilisateur::Cols::_uuid_badge, CompareOperator::EQ, uid),

        //Prend le callback a appeler pour envoyer les info
        [InformationObtenu]

        //Calback,finby prend un vecteur
(const std::vector<drogon_model::ProjetV1::Utilisateur> &userrr) {
            //Verifie que le vecteur est rempli
            if (userrr.empty()) {
                //Renvoie un null si vide
                InformationObtenu(Json::Value::null);
                return;
            }
            //Renvoie les info user en format json
            InformationObtenu(userrr[0].toJson());
        },

        //Erreur lors de find
        [Erreur](const DrogonDbException &e) {
            Erreur(e);
        });
};

void SupprimerBadge(
    const DbClientPtr &db,
    const std::string &uid,
    std::function<void(const size_t)> &&BadgeSupprimer,
    std::function<void(const DrogonDbException &)> &&Erreur) {
    Mapper<drogon_model::ProjetV1::Badge> mapper(db);
    //Prend l'uid à supprimer et renvoie le nombre de lignes supprimer ou une erreur si ca na pas réussit
    mapper.deleteByPrimaryKey(
        uid,
        [BadgeSupprimer = std::move(BadgeSupprimer)](const size_t count) {
            BadgeSupprimer(count);
        },
        [Erreur = std::move(Erreur)](const DrogonDbException &e) {
            Erreur(e);
        });
}

void ModifierBadge(
    //Paramètres:

    //Connexion
    const DbClientPtr &db,

    //uid du badge dont il faut modifier
    const std::string &uid,

    //info a modifier
    const Json::Value &body,

    //Callback à appeler en cas de réussite
    std::function<void(bool, const std::string &)> &&ModificationReussit,

    //Callback en cas d'echec
    std::function<void(const DrogonDbException &)> &&ErreurVide) {
    //Stocker la connexion pour instancier utilisateur
    Mapper<drogon_model::ProjetV1::Utilisateur> mapperuser(db);

    //transformation en shared pointeur pour pouvoir le passer au lambda imbrique
    auto erreurVidePtr = std::make_shared<std::function<void(const DrogonDbException &)> >(std::move(ErreurVide));
    auto reussitePtr = std::make_shared<std::function<void
        (bool, const std::string &)> >(std::move(ModificationReussit));

    //fonction de recherche par critere
    mapperuser.findBy(

        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Utilisateur::Cols::_uuid_badge, CompareOperator::EQ, uid),

        //Prend le badge à modifier le body (avec les informations a changer) contexte
        [InfoAChanger = body, mapperuser, reussitePtr, erreurVidePtr]


        //Calback avec mutable car prend uniquement du constant mais on a besoin de le modifier car update et la fonction finby prend un vecteur
(const std::vector<drogon_model::ProjetV1::Utilisateur> &userrr) mutable {
            //verifie que le vecteur n'est pas vide
            if (userrr.empty()) {
                (*reussitePtr)(false, "Pas de uuid trouvée");
                return;
            }
            //fais une copie car c'est un const
            auto updatedUser = userrr[0];

            //Changement des info si prtésente
            if (InfoAChanger.isMember("nom"))
                updatedUser.setNomUser(InfoAChanger["prenom"].asString());
            if (InfoAChanger.isMember("prenom"))
                updatedUser.setPrenomUser(InfoAChanger["prenom"].asString());
            if (InfoAChanger.isMember("classe"))
                updatedUser.setIdClasse(stoi(InfoAChanger["classe"].asString()));
            if (InfoAChanger.isMember("hash_mdp"))
                updatedUser.setHashMdp(InfoAChanger["hash_mdp"].asString());
            if (InfoAChanger.isMember("login"))
                updatedUser.setLoginUser(InfoAChanger["login"].asString());
            if (InfoAChanger.isMember("role"))
                updatedUser.setRoleUser(InfoAChanger["role"].asString());

            //Fais le changmeent dans la db
            mapperuser.update(
                updatedUser,
                [reussitePtr](const size_t count) {
                    (*reussitePtr)(true, "Utilisateur mis à jour");
                },
                //Erreur lors de update
                [erreurVidePtr](const DrogonDbException &e) {
                    (*erreurVidePtr)(e);
                });
        },
        //Erreur lors de find
        [erreurVidePtr](const DrogonDbException &e) {
            (*erreurVidePtr)(e);
        });
}

void CreeBadge(
    const DbClientPtr &db,
    const std::int32_t &uiduser,
    const std::string &uidbadge,
    std::function<void(bool, const std::string &)> &&BadgeCree,
    std::function<void(const DrogonDbException &)> &&Erreur) {
    //Stocker la connexion pour instancier utilisateur
    Mapper<drogon_model::ProjetV1::Utilisateur> mapperuser(db);

    //transformation en shared pointeur pour pouvoir le passer au lambda imbrique
    auto erreurVidePtr = std::make_shared<std::function<void(const DrogonDbException &)> >(std::move(Erreur));
    auto reussitePtr = std::make_shared<std::function<void(bool, const std::string &)> >(std::move(BadgeCree));

    //fonction de recherche par critere
    mapperuser.findBy(

        //Cherche à partir de uid dans la colonne _uuid_badge de la table utilisateur
        Criteria(drogon_model::ProjetV1::Utilisateur::Cols::_id_user, CompareOperator::EQ, uiduser),

        //Prend le badge à modifier le body (avec les informations a changer) contexte
        [uidbadge, mapperuser, reussitePtr, erreurVidePtr]


        //Calback avec mutable car prend uniquement du constant mais on a besoin de le modifier car update / la fonction finby prend un vecteur
(const std::vector<drogon_model::ProjetV1::Utilisateur> &userrr) mutable {
            //verifie que le vecteur n'est pas vide
            if (userrr.empty() or userrr[0].getValueOfUuidBadge() != "") {
                (*reussitePtr)(false, "Pas de uuid trouvée ou badge déjà present");
                return;
            }
            //fais une copie car c'est un const
            auto updatedUser = userrr[0];

            //modifie le champs uuid badge uniquement
            updatedUser.setUuidBadge(uidbadge);

            //Fais le changmeent dans la db
            mapperuser.update(
                updatedUser,
                [reussitePtr](const size_t count) {
                    if (count >= 1)
                        (*reussitePtr)(true, "Utilisateur mis à jour");
                    else
                        (*reussitePtr)(false, "Quelqu'un a modifier");
                },
                //Erreur lors de update
                [erreurVidePtr](const DrogonDbException &e) {
                    (*erreurVidePtr)(e);
                });
        },
        //Erreur lors de find
        [erreurVidePtr](const DrogonDbException &e) {
            (*erreurVidePtr)(e);
        });
}


void VerifierBadge(
    const DbClientPtr &db,
    const std::int32_t &uid,
    const std::string &MAC,
    std::function<void(bool, const std::string &)> &&BadgeVerifierResultat,
    std::function<void(const DrogonDbException &)> &&Erreur) {
    Mapper<drogon_model::ProjetV1::Utilisateur> mapperuser(db);
    Mapper<drogon_model::ProjetV1::Salle> mappersalle(db);
    Mapper<drogon_model::ProjetV1::Cours> mappercours(db);
    auto ResultatVerificationPtr = std::make_shared<std::function<void(bool, const std::string &)> >(
        std::move(BadgeVerifierResultat));
    auto ErreurTechnique = std::make_shared<std::function<void(const DrogonDbException &)> >(std::move(Erreur));
    mapperuser.findBy(
        Criteria(drogon_model::ProjetV1::Utilisateur::Cols::_uuid_badge, CompareOperator::EQ, uid),
        [mappersalle, mapperuser, uid, MAC, ResultatVerificationPtr, ErreurTechnique, mappercours]
(const std::vector<drogon_model::ProjetV1::Utilisateur> &userrr) mutable {
            //verifie que le vecteur n'est pas vide
            if (userrr.empty()) {
                (*ResultatVerificationPtr)(false, "Pas de uuid trouvée");
                return;
            }
            auto UtilisateurBadge = userrr[0];
            mappersalle.findBy(
                Criteria(drogon_model::ProjetV1::Salle::Cols::_mac_pea, CompareOperator::EQ, MAC),
                [mappersalle, UtilisateurBadge, uid, MAC, ResultatVerificationPtr, ErreurTechnique, mappercours]
        (const std::vector<drogon_model::ProjetV1::Salle> &salleMAC) mutable {
                    if (salleMAC.empty()) {
                        (*ResultatVerificationPtr)(false, "Pas de uuid trouvée");
                        return;
                    }
                    auto SallePEA = salleMAC[0];
                    mappercours.findBy(
                        Criteria(drogon_model::ProjetV1::Cours::Cols::_num_salle, CompareOperator::EQ,
                                 (*SallePEA.getNumSalle())),
                        [mappersalle, UtilisateurBadge, uid, MAC, ResultatVerificationPtr, mappercours]
                (const std::vector<drogon_model::ProjetV1::Cours> &salleMAC) mutable {
                            if (salleMAC.empty()) {
                                (*ResultatVerificationPtr)(false, "Pas de uuid trouvée");
                                return;
                            }
                            bool trouvee = false;
                            for (const auto &cours: salleMAC) {
                                if (UtilisateurBadge.getValueOfIdClasse() == cours.getValueOfIdClasse()) {
                                    (*ResultatVerificationPtr)(true, "Eleve autorisée");
                                    trouvee = true;
                                    break;
                                }
                            }
                            if (!trouvee) {
                                (*ResultatVerificationPtr)(false, "Pas de uuid trouvée");
                            }
                        }, [ErreurTechnique](const DrogonDbException &e) {
                            (*ErreurTechnique)(e);
                        }
                    );
                },
                [ErreurTechnique](const DrogonDbException &e) {
                    (*ErreurTechnique)(e);
                }
            );
        },
        [ErreurTechnique](const DrogonDbException &e) {
            (*ErreurTechnique)(e);
        }
    );
}


//Code Faux

//Verifier badge scanne BAE
void BadgeScanner(
    const DbClientPtr &db,
    const std::string &uid,
    const std::string &MAC,
    const std::uint64_t &heure_badgage,
    std::function<void(bool, const std::string &)> &&BadgeScannerResultat,
    std::function<void(const DrogonDbException &)> &&Erreur) {
    Mapper<drogon_model::ProjetV1::Utilisateur> mapperuser(db);
    Mapper<drogon_model::ProjetV1::Salle> mappersalle(db);
    Mapper<drogon_model::ProjetV1::Cours> mappercours(db);
    Mapper<drogon_model::ProjetV1::AbsenceCours> mapperabsence(db);
    Mapper<drogon_model::ProjetV1::PresenceCours> mapperPrecours(db);
    auto ResultatScannePtr = std::make_shared<std::function<void(bool, const std::string &)> >(
        std::move(BadgeScannerResultat));
    auto ErreurTechnique = std::make_shared<std::function<void(const DrogonDbException &)> >(std::move(Erreur));
    mapperuser.findBy(
        Criteria(drogon_model::ProjetV1::Utilisateur::Cols::_uuid_badge, CompareOperator::EQ, uid),
        [mappersalle, mapperuser, uid, MAC, ResultatScannePtr, ErreurTechnique, mappercours, mapperabsence, mapperPrecours, heure_badgage]
(const std::vector<drogon_model::ProjetV1::Utilisateur> &userrr) mutable {
            //verifie que le vecteur n'est pas vide
            if (userrr.empty()) {
                (*ResultatScannePtr)(false, "Pas de uuid trouvée");
                return;
            }
            auto UtilisateurBadge = userrr[0];
            mappersalle.findBy(
                Criteria(drogon_model::ProjetV1::Salle::Cols::_mac_pea, CompareOperator::EQ, MAC),
                [mappersalle, UtilisateurBadge, uid, MAC, ResultatScannePtr, ErreurTechnique, mappercours, mapperabsence, mapperPrecours, heure_badgage]
        (const std::vector<drogon_model::ProjetV1::Salle> &salleMAC) mutable {
                    if (salleMAC.empty()) {
                        (*ResultatScannePtr)(false, "Pas de uuid trouvée");
                        return;
                    }
                    auto SallePEA = salleMAC[0];
                    mappercours.findBy(
                        Criteria(drogon_model::ProjetV1::Cours::Cols::_num_salle, CompareOperator::EQ,
                                 (*SallePEA.getNumSalle())),
                        [mappersalle, UtilisateurBadge, uid, MAC, ResultatScannePtr, mappercours, mapperabsence, mapperPrecours, heure_badgage, ErreurTechnique]
                (const std::vector<drogon_model::ProjetV1::Cours> &salleMAC) mutable {
                            if (salleMAC.empty()) {
                                (*ResultatScannePtr)(false, "Pas de uuid trouvée");
                                return;
                            }
                            trantor::Date TimestampBrut = trantor::Date::now();
                            int64_t TimestampSecond = TimestampBrut.secondsSinceEpoch();
                            bool trouvee = false;
                            for (const auto &cours: salleMAC) {
                                if (cours.getValueOfHeureDebut().secondsSinceEpoch() - VALEURMARGECOURS < TimestampSecond && cours.getValueOfHeureDebut().secondsSinceEpoch() + VALEURMARGECOURS > TimestampSecond) {
                                    mapperabsence.findBy(
                                        Criteria(drogon_model::ProjetV1::AbsenceCours::Cols::_id_user, CompareOperator::EQ,(*UtilisateurBadge.getIdUser())),
                                        [mappersalle, UtilisateurBadge, uid, MAC, ResultatScannePtr, mappercours, mapperabsence, mapperPrecours, heure_badgage, ErreurTechnique]
                                        (const std::vector<drogon_model::ProjetV1::AbsenceCours> &listeabsent) mutable {
                                            if (listeabsent.empty()) {
                                                (*ResultatScannePtr)(false, "Pas de uuid trouvée");
                                                return;
                                            }
                                            auto AbsenceCoursUser = listeabsent[0];

                                            drogon_model::ProjetV1::PresenceCours UtilisateurPresenceChangement = drogon_model::ProjetV1::PresenceCours();
                                            UtilisateurPresenceChangement.setIdClasse((*AbsenceCoursUser.getIdClasse()));
                                            UtilisateurPresenceChangement.setIdCours((*AbsenceCoursUser.getIdCours()));
                                            UtilisateurPresenceChangement.setIdUser((*AbsenceCoursUser.getIdUser()));
                                            mapperPrecours.insert(
                                                UtilisateurPresenceChangement,
                                                [mapperabsence, UtilisateurBadge, uid, MAC, ResultatScannePtr,AbsenceCoursUser, ErreurTechnique]
                                                (const std::function<void(drogon_model::ProjetV1::PresenceCours &presenceID)>{
                                                    mapperabsence.deleteByPrimaryKey(
                                                        AbsenceCoursUser.getPrimaryKey(),
                                                        [ResultatScannePtr](const size_t count) {
                                                            (*ResultatScannePtr)(count);
                                                        },
                                                        [ErreurTechnique](const DrogonDbException &e) {
                                                            (*ErreurTechnique)(e);
                                                        });
                                                },[ErreurTechnique](const DrogonDbException &e) {
                                                        (*ErreurTechnique)(e);
                                            });
                                        },[ErreurTechnique](const DrogonDbException &e) {
                                            (*ErreurTechnique)(e);
                                    });
                                }
                                    trouvee = true;
                                    break;
                                }
                            if (!trouvee) {
                                (*ResultatScannePtr)(false, "Pas de uuid trouvée");
                            }
                        }, [ErreurTechnique](const DrogonDbException &e) {
                            (*ErreurTechnique)(e);
                        });
                },
                [ErreurTechnique](const DrogonDbException &e) {
                    (*ErreurTechnique)(e);
                }
            );
        },
        [ErreurTechnique](const DrogonDbException &e) {
            (*ErreurTechnique)(e);
        }
    );
}