#include "GestionBadge.h"
#include <iostream>

void GestionBadge::ModuleCreationBadge()
{
    std::string nom, prenom;

    // Etape 1 : saisie informations élève
    std::cout << "Entrez le nom : ";
    std::cin >> nom;
    std::cout << "Entrez le prenom : ";
    std::cin >> prenom;

    // Etape 2 : communication avec la base de données
    // (implémentation attendue dans votre projet)

    // Etape 3 : lecture du badge physique
    std::cout << "Veuillez présenter votre badge physique sur le lecteur NFC..." << std::endl;

    // Remarque : 'lecteur' doit être un membre de la classe GestionBadge
    std::string uuid = lecteur.LireUUID(); // Appel de la bibliothèque NFC

    // Alternative : badge non reconnu
    if (uuid.empty())
    {
        std::cout << "Badge non reconnu. Veuillez réessayer." << std::endl;
        return;
    }

    // Poursuivre le traitement avec 'uuid'...
}



	
	
