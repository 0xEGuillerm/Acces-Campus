#include <drogon/drogon.h>
#include <filesystem>

int main(int argc, char* argv[]) {
    //Load config file
    auto configPath = std::filesystem::canonical(argv[0]).parent_path() / "config.json";
    drogon::app().loadConfigFile(configPath.string());
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}
