-- ** Database generated with pgModeler (PostgreSQL Database Modeler).
-- ** pgModeler version: 2.0.0-alpha1
-- ** PostgreSQL version: 17.0
-- ** Project Site: pgmodeler.io
-- ** Model Author: ---

-- ** Database creation must be performed outside a multi lined SQL file. 
-- ** These commands were put in this file only as a convenience.



SET search_path TO pg_catalog,public;
-- ddl-end --

-- object: public.utilisateur | type: TABLE --
-- DROP TABLE IF EXISTS public.utilisateur CASCADE;
CREATE TABLE public.utilisateur (
	id_user serial NOT NULL,
	nom_user varchar(50) NOT NULL,
	prenom_user varchar(50) NOT NULL,
	login_user varchar(60) NOT NULL,
	hash_mdp varchar(255) NOT NULL,
	id_classe integer,
	role_user varchar(16) NOT NULL,
	uuid_badge varchar(8),
	CONSTRAINT pk_utilisateur PRIMARY KEY (id_user),
	CONSTRAINT uq_utilisateur_login UNIQUE (login_user)
);
-- ddl-end --
COMMENT ON COLUMN public.utilisateur.role_user IS E'admin\neleve\nprofesseur';
-- ddl-end --
ALTER TABLE public.utilisateur OWNER TO postgres;
-- ddl-end --

-- object: public.badge | type: TABLE --
-- DROP TABLE IF EXISTS public.badge CASCADE;
CREATE TABLE public.badge (
	uuid_badge varchar NOT NULL,
	date_creation timestamp NOT NULL DEFAULT NOW(),
	CONSTRAINT pk_badge PRIMARY KEY (uuid_badge)
);
-- ddl-end --
ALTER TABLE public.badge OWNER TO postgres;
-- ddl-end --

-- object: public.salle | type: TABLE --
-- DROP TABLE IF EXISTS public.salle CASCADE;
CREATE TABLE public.salle (
	num_salle varchar(4) NOT NULL,
	place_maximum integer NOT NULL,
	mac_bae varchar(17),
	mac_pea varchar(17),
	CONSTRAINT pk_salle PRIMARY KEY (num_salle)
);
-- ddl-end --
ALTER TABLE public.salle OWNER TO postgres;
-- ddl-end --

-- object: public.cours | type: TABLE --
-- DROP TABLE IF EXISTS public.cours CASCADE;
CREATE TABLE public.cours (
	id_cours serial NOT NULL,
	num_salle varchar(4) NOT NULL,
	heure_debut timestamp NOT NULL,
	heure_fin timestamp NOT NULL,
	id_classe integer,
	reserve_par integer,
	professeur integer,
	CONSTRAINT pk_cours PRIMARY KEY (id_cours),
	CONSTRAINT ck_cours_horaires CHECK (heure_fin > heure_debut)
);
-- ddl-end --
ALTER TABLE public.cours OWNER TO postgres;
-- ddl-end --

-- object: public.presence_cours | type: TABLE --
-- DROP TABLE IF EXISTS public.presence_cours CASCADE;
CREATE TABLE public.presence_cours (
	id_presence serial NOT NULL,
	id_user integer,
	id_cours integer,
	id_classe integer,
	CONSTRAINT pk_presence PRIMARY KEY (id_presence)
);
-- ddl-end --
ALTER TABLE public.presence_cours OWNER TO postgres;
-- ddl-end --

-- object: public.retardabsence | type: TABLE --
-- DROP TABLE IF EXISTS public.retardabsence CASCADE;
CREATE TABLE public.retardabsence (
	id_retardabsence serial NOT NULL,
	id_user integer NOT NULL,
	id_cours integer NOT NULL,
	temps_retard_min integer DEFAULT 0,
	absence boolean NOT NULL,
	timestamp_heure_cours timestamp,
	CONSTRAINT pk_retardabsence PRIMARY KEY (id_retardabsence),
	CONSTRAINT uq_presence_user_cours UNIQUE (id_user,id_cours)
);
-- ddl-end --
ALTER TABLE public.retardabsence OWNER TO postgres;
-- ddl-end --

-- object: public.historique | type: TABLE --
-- DROP TABLE IF EXISTS public.historique CASCADE;
CREATE TABLE public.historique (
	id_historique serial NOT NULL,
	id_user integer NOT NULL,
	date_action timestamp NOT NULL DEFAULT NOW(),
	type_action varchar(50) NOT NULL,
	CONSTRAINT pk_historique PRIMARY KEY (id_historique)
);
-- ddl-end --
ALTER TABLE public.historique OWNER TO postgres;
-- ddl-end --

-- object: public.absence_cours | type: TABLE --
-- DROP TABLE IF EXISTS public.absence_cours CASCADE;
CREATE TABLE public.absence_cours (
	id_absence serial NOT NULL,
	id_user integer,
	id_cours integer,
	id_classe integer,
	CONSTRAINT pk_absence PRIMARY KEY (id_absence)
);
-- ddl-end --
ALTER TABLE public.absence_cours OWNER TO postgres;
-- ddl-end --

-- object: public.classe | type: TABLE --
-- DROP TABLE IF EXISTS public.classe CASCADE;
CREATE TABLE public.classe (
	id_classe integer NOT NULL,
	nom_classe varchar NOT NULL,
	CONSTRAINT pk_classe PRIMARY KEY (id_classe)
);
-- ddl-end --
ALTER TABLE public.classe OWNER TO postgres;
-- ddl-end --

-- object: fk_id_badge | type: CONSTRAINT --
-- ALTER TABLE public.utilisateur DROP CONSTRAINT IF EXISTS fk_id_badge CASCADE;
ALTER TABLE public.utilisateur ADD CONSTRAINT fk_id_badge FOREIGN KEY (uuid_badge)
REFERENCES public.badge (uuid_badge) MATCH SIMPLE
ON DELETE SET NULL ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_id_classe | type: CONSTRAINT --
-- ALTER TABLE public.utilisateur DROP CONSTRAINT IF EXISTS fk_id_classe CASCADE;
ALTER TABLE public.utilisateur ADD CONSTRAINT fk_id_classe FOREIGN KEY (id_classe)
REFERENCES public.classe (id_classe) MATCH SIMPLE
ON DELETE SET NULL ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_cours_salle | type: CONSTRAINT --
-- ALTER TABLE public.cours DROP CONSTRAINT IF EXISTS fk_cours_salle CASCADE;
ALTER TABLE public.cours ADD CONSTRAINT fk_cours_salle FOREIGN KEY (num_salle)
REFERENCES public.salle (num_salle) MATCH SIMPLE
ON DELETE RESTRICT ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_id_classe | type: CONSTRAINT --
-- ALTER TABLE public.cours DROP CONSTRAINT IF EXISTS fk_id_classe CASCADE;
ALTER TABLE public.cours ADD CONSTRAINT fk_id_classe FOREIGN KEY (id_classe)
REFERENCES public.classe (id_classe) MATCH SIMPLE
ON DELETE NO ACTION ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_reserve_par | type: CONSTRAINT --
-- ALTER TABLE public.cours DROP CONSTRAINT IF EXISTS fk_reserve_par CASCADE;
ALTER TABLE public.cours ADD CONSTRAINT fk_reserve_par FOREIGN KEY (reserve_par)
REFERENCES public.utilisateur (id_user) MATCH SIMPLE
ON DELETE NO ACTION ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_professeur_utilisateur | type: CONSTRAINT --
-- ALTER TABLE public.cours DROP CONSTRAINT IF EXISTS fk_professeur_utilisateur CASCADE;
ALTER TABLE public.cours ADD CONSTRAINT fk_professeur_utilisateur FOREIGN KEY (professeur)
REFERENCES public.utilisateur (id_user) MATCH SIMPLE
ON DELETE NO ACTION ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_utilisateur | type: CONSTRAINT --
-- ALTER TABLE public.presence_cours DROP CONSTRAINT IF EXISTS fk_utilisateur CASCADE;
ALTER TABLE public.presence_cours ADD CONSTRAINT fk_utilisateur FOREIGN KEY (id_user)
REFERENCES public.utilisateur (id_user) MATCH SIMPLE
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_id_cour | type: CONSTRAINT --
-- ALTER TABLE public.presence_cours DROP CONSTRAINT IF EXISTS fk_id_cour CASCADE;
ALTER TABLE public.presence_cours ADD CONSTRAINT fk_id_cour FOREIGN KEY (id_cours)
REFERENCES public.cours (id_cours) MATCH SIMPLE
ON DELETE NO ACTION ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_id_classe | type: CONSTRAINT --
-- ALTER TABLE public.presence_cours DROP CONSTRAINT IF EXISTS fk_id_classe CASCADE;
ALTER TABLE public.presence_cours ADD CONSTRAINT fk_id_classe FOREIGN KEY (id_classe)
REFERENCES public.classe (id_classe) MATCH SIMPLE
ON DELETE NO ACTION ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_presence_utilisateur | type: CONSTRAINT --
-- ALTER TABLE public.retardabsence DROP CONSTRAINT IF EXISTS fk_presence_utilisateur CASCADE;
ALTER TABLE public.retardabsence ADD CONSTRAINT fk_presence_utilisateur FOREIGN KEY (id_user)
REFERENCES public.utilisateur (id_user) MATCH SIMPLE
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_presence_cours | type: CONSTRAINT --
-- ALTER TABLE public.retardabsence DROP CONSTRAINT IF EXISTS fk_presence_cours CASCADE;
ALTER TABLE public.retardabsence ADD CONSTRAINT fk_presence_cours FOREIGN KEY (id_cours)
REFERENCES public.cours (id_cours) MATCH SIMPLE
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_historique_utilisateur | type: CONSTRAINT --
-- ALTER TABLE public.historique DROP CONSTRAINT IF EXISTS fk_historique_utilisateur CASCADE;
ALTER TABLE public.historique ADD CONSTRAINT fk_historique_utilisateur FOREIGN KEY (id_user)
REFERENCES public.utilisateur (id_user) MATCH SIMPLE
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_cours | type: CONSTRAINT --
-- ALTER TABLE public.absence_cours DROP CONSTRAINT IF EXISTS fk_cours CASCADE;
ALTER TABLE public.absence_cours ADD CONSTRAINT fk_cours FOREIGN KEY (id_cours)
REFERENCES public.cours (id_cours) MATCH SIMPLE
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_utilisateur | type: CONSTRAINT --
-- ALTER TABLE public.absence_cours DROP CONSTRAINT IF EXISTS fk_utilisateur CASCADE;
ALTER TABLE public.absence_cours ADD CONSTRAINT fk_utilisateur FOREIGN KEY (id_user)
REFERENCES public.utilisateur (id_user) MATCH SIMPLE
ON DELETE CASCADE ON UPDATE NO ACTION;
-- ddl-end --

-- object: fk_id_classe | type: CONSTRAINT --
-- ALTER TABLE public.absence_cours DROP CONSTRAINT IF EXISTS fk_id_classe CASCADE;
ALTER TABLE public.absence_cours ADD CONSTRAINT fk_id_classe FOREIGN KEY (id_classe)
REFERENCES public.classe (id_classe) MATCH SIMPLE
ON DELETE NO ACTION ON UPDATE NO ACTION;
-- ddl-end --


