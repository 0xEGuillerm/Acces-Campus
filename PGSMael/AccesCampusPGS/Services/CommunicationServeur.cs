using System;
using System.Text.Json;

namespace AccesCampusPGS.Services
{
    public class CommunicationServeur
    {
        // 1. La NOUVELLE fonction : On accepte n'importe quel nom !
        public JsonDocument RechercherEleve(string nom, string prenom)
        {
            // On génère un faux numéro d'étudiant au hasard entre 100 et 999
            int fauxIdEleve = new Random().Next(100, 999);

            // On renvoie un JSON de succès avec le nom tapé à l'écran
            string jsonSucces = $@"{{ ""statut"": ""succes"", ""id_eleve"": {fauxIdEleve}, ""nom"": ""{nom}"", ""prenom"": ""{prenom}"" }}";
            
            return JsonDocument.Parse(jsonSucces);
        }

        // 2. Vérification de l'UUID
        public JsonDocument VerifierExistenceUUID(string uuid)
        {
            if (uuid == "UUID-DEJA-PRIS")
            {
                return JsonDocument.Parse(@"{ ""statut"": ""erreur"", ""message"": ""carte deja utilisee"" }");
            }
            return JsonDocument.Parse(@"{ ""statut"": ""succes"", ""message"": ""carte libre"" }");
        }

        // 3. Création finale
        public JsonDocument CreerBadge(int idEleve, string uuid)
        {
            return JsonDocument.Parse(@"{ ""statut"": ""succes"", ""message"": ""badge cree"" }");
        }
    }
}