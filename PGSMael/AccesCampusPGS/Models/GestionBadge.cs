using System;
using System.Text.Json;
using AccesCampusPGS.Services; 

namespace AccesCampusPGS.Models
{
    public class GestionBadge
    {
        private CommunicationServeur api = new CommunicationServeur();
        private LecteurNFC lecteur = new LecteurNFC();

        //Fonction de création de badge
        public string ModuleCreationBadge(string nom, string prenom)
        {
            JsonDocument reponseRecherche = api.RechercherEleve(nom, prenom);
            JsonElement racineRecherche = reponseRecherche.RootElement;

            if (racineRecherche.GetProperty("statut").GetString() == "erreur")
            {
                return $"[ERREUR] {racineRecherche.GetProperty("message").GetString()}";
            }

            int idEleveTrouve = racineRecherche.GetProperty("id_eleve").GetInt32();

            string uuid = lecteur.LireUUID();

            if (string.IsNullOrEmpty(uuid) || uuid == "ERREUR")
            {
                return "[ERREUR] Le scan a échoué, veuillez réessayer.";
            }

            JsonDocument reponseVerif = api.VerifierExistenceUUID(uuid);

            if (reponseVerif.RootElement.GetProperty("statut").GetString() == "erreur")
            {
                return $"[ERREUR] {reponseVerif.RootElement.GetProperty("message").GetString()}";
            }

            JsonDocument reponseCreation = api.CreerBadge(idEleveTrouve, uuid);

            if (reponseCreation.RootElement.GetProperty("statut").GetString() == "succes")
            {
                return $"[CONFIRMATION] Le badge (UUID: {uuid}) a été associé avec succès à {nom} {prenom} !";
            }

            return "Une erreur inconnue s'est produite.";
        }

        
        //Suppression de badge
        
        public string ModuleSupprimerBadge(string uuid)
        {
            // Simulation : Dans la vraie vie, tu demanderais à l'API de supprimer
            // api.SupprimerBadge(uuid); 
            
            return $"[INFO] Le badge {uuid} a été désactivé avec succès.";
        }
    }
}