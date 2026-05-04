#on veut compiler pour Debian version bookworm (os du containaire)
FROM debian:bookworm-slim AS builder


# installer les libraire / dépendance
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    ca-certificates \
    libjsoncpp-dev \
    uuid-dev \
    zlib1g-dev \
    libssl-dev \
    libpq-dev \
    pkg-config \
    libargon2-dev \
    libsodium-dev


# Répertoire de travail dans le conteneur. Toutes les commandes suivantes
# s'exécutent depuis là.
WORKDIR /src


#Installation et compilation de drogon car pas dans le dépôt de Debian 12 

RUN git clone https://github.com/drogonframework/drogon.git \
&& cd drogon \
&& git submodule update --init \
&& mkdir build && cd build \
&& cmake .. -DCMAKE_BUILD_TYPE=Release \
&& make -j$(nproc) \
&& make install 


# Maintenant copie tout le reste du code source.
COPY . .

# Build en mode Release.
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build -j$(nproc)


FROM debian:bookworm-slim AS runtime


# Librairie pour le fonctionnement
RUN apt-get update && apt-get install -y --no-install-recommends \
    libjsoncpp25 \
    libssl3 \
    libpq5 \
    zlib1g \
    libuuid1 \
    libc-ares2 \
    libargon2-1 \
    libsodium23

# creation d'un utilisateur (non root)
RUN useradd --system --create-home --shell /bin/bash utilisateur

COPY --from=builder /usr/local/lib/libdrogon.so* /usr/local/lib/
COPY --from=builder /usr/local/lib/libtrantor.so* /usr/local/lib/
RUN ldconfig

WORKDIR /app

# recuperation du fichier compilee
COPY --from=builder /src/build/BackendBDD ./BackendBDD

# recuperation de la config
COPY --from=builder /src/build/config.json ./config.json


# permission à l'utilisateur de l'écexuté
RUN chown -R utilisateur:utilisateur /app

#on se met en mode utilisateur
USER utilisateur

# on expose le port 4444 du docker (pour se connecter)
EXPOSE 4444

# lancement du programme
CMD ["./BackendBDD"]