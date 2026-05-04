#include <drogon/drogon.h>
#include <filesystem>
#include <sodium.h>
#include "controllers/BAEController.h"


int main(int argc, char* argv[]) {
    //Chargement du fichier de configuration
    auto configPath = std::filesystem::canonical(argv[0]).parent_path() / "config.json";
    drogon::app().loadConfigFile(configPath.string());

    //Execute et stock le résultat de l'inisalisation de libsodium
    //(chiffrment) notamment le générateur de nombres aléatoires
    int resultat_initisalisation_sodium = sodium_init();
    if (resultat_initisalisation_sodium < 0) {
        //Envoie un message et une erreur dans le cas ou pas initialiser
        std::cerr << "Erreur inisalisalisation sodium" << "\n";
        return EXIT_FAILURE;
    }

//Gestion CORS
    drogon::app().registerPostHandlingAdvice(
    [](const drogon::HttpRequestPtr& req, const drogon::HttpResponsePtr& resp) {
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, DELETE");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    });

    drogon::app().registerPreRoutingAdvice(
    [](const drogon::HttpRequestPtr& req,
    drogon::AdviceCallback&& acb,
    drogon::AdviceChainCallback&& accb) {
        if (req->method() == drogon::Options) {
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setStatusCode(drogon::k204NoContent);
            resp->addHeader("Access-Control-Allow-Origin", "*");
            resp->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, DELETE");
            resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
            resp->addHeader("Access-Control-Max-Age", "86400");
            acb(resp);
            return;
        }
        accb();
    });

    //Lancement du backendbdd
    drogon::app().run();
    return 0;
}




