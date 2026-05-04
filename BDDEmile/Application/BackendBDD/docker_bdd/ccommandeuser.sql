--Commande création user BDD 
	CREATE ROLE drogon WITH LOGIN PASSWORD 'pF4Z1gA5H2' NOSUPERUSER NOCREATEDB NOCREATEROLE;
--Ajout des droits d’accès
	GRANT CONNECT ON DATABASE acces_campus_bdd TO drogon;
--Ajout des accès aux tables
	GRANT USAGE ON SCHEMA public TO drogon;
--Ajout des commandes autorisée
	GRANT SELECT, INSERT, UPDATE, DELETE ON ALL TABLES IN SCHEMA public TO drogon;
--Ajout d’une commande pour les séries (utilisateur qui se suivent id)
	GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO drogon;
