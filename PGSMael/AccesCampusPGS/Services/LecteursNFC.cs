using System;
// using PCSC; // On les garde en commentaire pour l'instant
// using PCSC.Iso7816;

namespace AccesCampusPGS.Services // J'ajoute le namespace pour être propre
{
    public class LecteurNFC
    {
        public string LireUUID()
        {
            // ---------------------------------------------------------
            // 1. LA SIMULATION (C'est ce code qui va s'exécuter)
            // ---------------------------------------------------------
            Console.WriteLine("[SIMULATION] Lecture de la puce RFID...");
            return "UUID-9876"; 


            // ---------------------------------------------------------
            // 2. LE VRAI CODE USB (Totalement désactivé et inoffensif)
            // ---------------------------------------------------------
            /* try
            {
                using (var context = ContextFactory.Instance.Establish(SCardScope.System))
                {
                    var lecteurs = context.GetReaders();
                    
                    if (lecteurs == null || lecteurs.Length == 0)
                    {
                        Console.WriteLine("[ERREUR] Aucun lecteur USB détecté.");
                        return "ERREUR";
                    }

                    string nomLecteur = lecteurs[0];
                    Console.WriteLine($"-> Lecteur USB détecté : {nomLecteur}");

                    Console.WriteLine("En attente d'une carte...");
                    using (var isoReader = new IsoReader(context, nomLecteur, SCardShareMode.Shared, SCardProtocol.Any, false))
                    {
                        var commandeApdu = new CommandApdu(IsoCase.Case2Short, isoReader.ActiveProtocol)
                        {
                            CLA = 0xFF, Instruction = InstructionCode.GetData, P1 = 0x00, P2 = 0x00, Le = 0
                        };

                        var reponse = isoReader.Transmit(commandeApdu);
                        
                        if (reponse.HasData)
                        {
                            string uuid = BitConverter.ToString(reponse.GetData());
                            return uuid.Replace("-", ""); 
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[ERREUR CRITIQUE USB] {ex.Message}");
                return "ERREUR";
            }
            */
        }
    }
}