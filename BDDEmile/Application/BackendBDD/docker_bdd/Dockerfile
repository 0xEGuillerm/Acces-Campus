#image postgres
FROM postgres:17

#variable nécessaire a postgres
ENV POSTGRES_USER=postgres
ENV POSTGRES_DB=acces_campus_bdd
ENV POSTGRES_PASSWORD=K5QGdgygCs

#port d'écoute de la bdd 
EXPOSE 5432

#prend les fichier sql pour crée les schéma et peupler la bdd
COPY ./aSchemaBDDv12.sql /docker-entrypoint-initdb.d/
COPY ./bcommandesql.sql /docker-entrypoint-initdb.d/
COPY ./ccommandeuser.sql /docker-entrypoint-initdb.d/

#le volume qui stockeras les donnée permannent 
VOLUME /var/lib/postgresql/data
