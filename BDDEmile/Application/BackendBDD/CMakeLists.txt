#version de cmake minimum
cmake_minimum_required(VERSION 3.20)
#nom de l'éxecutable --> BackendBDD
project(BackendBDD CXX)

#standard c++ à 20
set(CMAKE_CXX_STANDARD 20)

#verification des fonctionnalité du compilateur (pour les coroutines)
include(CheckIncludeFileCXX)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)
if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
    # Do nothing
elseif (HAS_ANY AND HAS_STRING_VIEW AND HAS_COROUTINE)
    set(CMAKE_CXX_STANDARD 20)
elseif (HAS_ANY AND HAS_STRING_VIEW)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

#obligation d'utiliser le standard 20 en c++
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#fichier a compiler
add_executable(${PROJECT_NAME} main.cc
        models/Badge.cc
        dao/BadgeDAO.cpp
        dao/BadgeDAO.h
        models/AbsenceCours.h
        Logique/BadgeLogique.cpp
        Logique/BadgeLogique.h
        resultat/StructResultat.h
        dao/SalleDAO.cpp
        dao/SalleDAO.h
        Logique/SalleLogique.cpp
        Logique/SalleLogique.h
        Logique/RetardAbsenceLogique.cpp
        Logique/RetardAbsenceLogique.h
        dao/RetardAbsenceDAO.cpp
        dao/RetardAbsenceDAO.h
        dao/CoursDAO.cpp
        dao/CoursDAO.h
        Logique/CoursLogique.cpp
        Logique/CoursLogique.h
        Logique/UtilisateurLogique.cpp
        Logique/UtilisateurLogique.h
        dao/UtilisateurDAO.cpp
        dao/UtilisateurDAO.h
        controllers/PEAController.cpp
        controllers/PEAController.h
        controllers/BAEController.cpp
        controllers/BAEController.h
        controllers/PSWController.cpp
        controllers/PSWController.h
        controllers/PGSController.cpp
        controllers/PGSController.h
        dao/ClasseDAO.cpp
        dao/ClasseDAO.h
        dao/PresenceAbsenceCoursDAO.cpp
        dao/PresenceAbsenceCoursDAO.h
)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# add_subdirectory(drogon) 
# target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
#
# and comment out the following lines
#recherche de drogon sur ma machine
find_package(Drogon CONFIG REQUIRED)
#ajout de drogon en lien
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

#copie de la config drogon vers mon dossier
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.json ${CMAKE_CURRENT_BINARY_DIR}/config.json COPYONLY)

# ##############################################################################

#reverification de version
if (CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "c++17 or higher is required")
elseif (CMAKE_CXX_STANDARD LESS 20)
    message(STATUS "use c++17")
else ()
    message(STATUS "use c++20")
endif ()

aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(models MODEL_SRC)

drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
                    ${CMAKE_CURRENT_BINARY_DIR})
# use the following line to create views with namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE)
# use the following line to create views with namespace CHANGE_ME prefixed
# and path namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE CHANGE_ME)

#préprocesseur
target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                   ${CMAKE_CURRENT_SOURCE_DIR}/models)
target_sources(${PROJECT_NAME}
               PRIVATE
               ${SRC_DIR}
               ${CTL_SRC}
               ${FILTER_SRC}
               ${PLUGIN_SRC}
               ${MODEL_SRC})
# ##############################################################################
# uncomment the following line for dynamically loading views 
# set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)

# ##############################################################################

#verification openssl (pour jwt)
find_package(OpenSSL REQUIRED)

#Dit au compilateur de prendre en compte le dossier avec la librairie jwt
target_include_directories(${PROJECT_NAME}  PRIVATE ${CMAKE_SOURCE_DIR}/librairie_divers/jwt-cpp/include)

#linker openssl
target_link_libraries(${PROJECT_NAME}  PRIVATE OpenSSL::SSL OpenSSL::Crypto)

#ajout librairie libsodium (chiffrement)
#recherche
find_package(PkgConfig REQUIRED)
#ajout du pkg
pkg_check_modules(SODIUM REQUIRED libsodium)
target_include_directories(${PROJECT_NAME} PRIVATE ${SODIUM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SODIUM_LIBRARIES})