SET search_path TO pg_catalog,public;

---------------------------------------------------------
-- 1. CLASSE (10 enregistrements)
---------------------------------------------------------
INSERT INTO public.classe (id_classe, nom_classe) VALUES 
(1, 'BTS SIO 1 - SLAM'),
(2, 'BTS SIO 1 - SISR'),
(3, 'BTS SIO 2 - SLAM'),
(4, 'BTS SIO 2 - SISR'),
(5, 'Licence Pro CDA'),
(6, 'Licence Pro ASRALL'),
(7, 'Bachelor Cybersécurité'),
(8, 'Master 1 Ingénierie Web'),
(9, 'Master 2 Architecte Cloud'),
(10, 'Prépa Numérique');

---------------------------------------------------------
-- 2. BADGE (12 enregistrements) - Max 8 caractères
---------------------------------------------------------
INSERT INTO public.badge (uuid_badge, date_creation) VALUES 
('BDG00001', NOW() - INTERVAL '100 days'),
('BDG00002', NOW() - INTERVAL '90 days'),
('BDG00003', NOW() - INTERVAL '80 days'),
('BDG00004', NOW() - INTERVAL '70 days'),
('BDG00005', NOW() - INTERVAL '60 days'),
('BDG00006', NOW() - INTERVAL '50 days'),
('BDG00007', NOW() - INTERVAL '40 days'),
('BDG00008', NOW() - INTERVAL '30 days'),
('BDG00009', NOW() - INTERVAL '20 days'),
('BDG00010', NOW() - INTERVAL '10 days'),
('BDG00011', NOW() - INTERVAL '5 days'),
('BDG00012', NOW() - INTERVAL '1 days');

---------------------------------------------------------
-- 3. SALLE (10 enregistrements) - Max 4 caractères
---------------------------------------------------------
INSERT INTO public.salle (num_salle, place_maximum, mac_bae, mac_pea) VALUES 
('A101', 30, '00:1A:2B:3C:4D:01', '00:1A:2B:3C:4D:A1'),
('A102', 30, '00:1A:2B:3C:4D:02', '00:1A:2B:3C:4D:A2'),
('A201', 20, '00:1A:2B:3C:4D:03', '00:1A:2B:3C:4D:A3'),
('A202', 20, '00:1A:2B:3C:4D:04', '00:1A:2B:3C:4D:A4'),
('B101', 40, '00:1A:2B:3C:4D:05', '00:1A:2B:3C:4D:A5'),
('B102', 40, '00:1A:2B:3C:4D:06', '00:1A:2B:3C:4D:A6'),
('C101', 15, '00:1A:2B:3C:4D:07', '00:1A:2B:3C:4D:A7'),
('C102', 15, '00:1A:2B:3C:4D:08', '00:1A:2B:3C:4D:A8'),
('AMP1', 150, NULL, NULL),
('AMP2', 200, NULL, NULL);

---------------------------------------------------------
-- 4. UTILISATEUR (12 enregistrements)
-- 2 admins, 3 profs, 7 élèves
---------------------------------------------------------
INSERT INTO public.utilisateur (id_user, nom_user, prenom_user, login_user, hash_mdp, id_classe, role_user, uuid_badge) VALUES 
-- Admins (pas de classe)
(1, 'Super', 'Admin', 'admin1', 'hash123', NULL, 'admin', 'BDG00001'),
(2, 'Tech', 'Support', 'admin2', 'hash123', NULL, 'admin', 'BDG00002'),
-- Professeurs (pas de classe attitrée)
(3, 'Lovelace', 'Ada', 'alovelace', 'hash123', NULL, 'professeur', 'BDG00003'),
(4, 'Turing', 'Alan', 'aturing', 'hash123', NULL, 'professeur', 'BDG00004'),
(5, 'Hopper', 'Grace', 'ghopper', 'hash123', NULL, 'professeur', 'BDG00005'),
-- Élèves
(6, 'Dupont', 'Jean', 'jdupont', 'hash123', 1, 'eleve', 'BDG00006'),
(7, 'Martin', 'Alice', 'amartin', 'hash123', 1, 'eleve', 'BDG00007'),
(8, 'Durand', 'Paul', 'pdurand', 'hash123', 2, 'eleve', 'BDG00008'),
(9, 'Petit', 'Sophie', 'spetit', 'hash123', 3, 'eleve', 'BDG00009'),
(10, 'Leroy', 'Marc', 'mleroy', 'hash123', 4, 'eleve', 'BDG00010'),
(11, 'Roux', 'Emma', 'eroux', 'hash123', 5, 'eleve', 'BDG00011'),
(12, 'Moreau', 'Luc', 'lmoreau', 'hash123', 6, 'eleve', 'BDG00012');

SELECT setval('utilisateur_id_user_seq', 12);

---------------------------------------------------------
-- 5. COURS (10 enregistrements)
-- Constraint : heure_fin > heure_debut
---------------------------------------------------------
INSERT INTO public.cours (id_cours, num_salle, heure_debut, heure_fin, id_classe, reserve_par, professeur) VALUES 
(1, 'A101', '2026-05-10 08:00:00', '2026-05-10 10:00:00', 1, 1, 3), -- Algo (Ada)
(2, 'A102', '2026-05-10 10:15:00', '2026-05-10 12:15:00', 2, 1, 4), -- Réseau (Alan)
(3, 'B101', '2026-05-10 14:00:00', '2026-05-10 17:00:00', 3, 2, 5), -- BDD (Grace)
(4, 'B102', '2026-05-11 08:00:00', '2026-05-11 12:00:00', 4, 3, 3), 
(5, 'AMP1', '2026-05-11 13:30:00', '2026-05-11 15:30:00', 5, 4, 4), 
(6, 'AMP2', '2026-05-12 09:00:00', '2026-05-12 11:00:00', 6, 5, 5),
(7, 'C101', '2026-05-12 14:00:00', '2026-05-12 16:00:00', 1, 1, 3),
(8, 'C102', '2026-05-13 10:00:00', '2026-05-13 12:00:00', 2, 2, 4),
(9, 'A201', '2026-05-13 15:00:00', '2026-05-13 17:00:00', 3, 3, 5),
(10, 'A202', '2026-05-14 08:30:00', '2026-05-14 10:30:00', 4, 1, 3);

SELECT setval('cours_id_cours_seq', 10);

---------------------------------------------------------
-- 6. PRESENCE_COURS (12 enregistrements)
-- Des élèves qui ont bien badgé et sont présents
---------------------------------------------------------
INSERT INTO public.presence_cours (id_presence, id_user, id_cours, id_classe) VALUES 
(1, 6, 1, 1),
(2, 7, 1, 1),
(3, 8, 2, 2),
(4, 9, 3, 3),
(5, 10, 4, 4),
(6, 11, 5, 5),
(7, 12, 6, 6),
(8, 6, 7, 1),
(9, 7, 7, 1),
(10, 8, 8, 2),
(11, 9, 9, 3),
(12, 10, 10, 4);

SELECT setval('presence_cours_id_presence_seq', 12);

---------------------------------------------------------
-- 7. RETARDABSENCE (10 enregistrements)
-- Constraint : (id_user, id_cours) UNIQUE
-- Mixte de retards (absence = false) et absences pures
---------------------------------------------------------
INSERT INTO public.retardabsence (id_retardabsence, id_user, id_cours, temps_retard_min, absence, timestamp_heure_cours) VALUES 
(1, 6, 2, 0, true, '2026-05-10 10:15:00'),  -- Jean est absent (Cours 2)
(2, 7, 3, 15, false, '2026-05-10 14:00:00'), -- Alice a 15 min de retard (Cours 3)
(3, 8, 4, 0, true, '2026-05-11 08:00:00'),   
(4, 9, 5, 5, false, '2026-05-11 13:30:00'),  
(5, 10, 6, 0, true, '2026-05-12 09:00:00'),  
(6, 11, 7, 30, false, '2026-05-12 14:00:00'),
(7, 12, 8, 0, true, '2026-05-13 10:00:00'),  
(8, 6, 9, 10, false, '2026-05-13 15:00:00'), 
(9, 7, 10, 0, true, '2026-05-14 08:30:00'),  
(10, 8, 1, 45, false, '2026-05-10 08:00:00');

SELECT setval('retardabsence_id_retardabsence_seq', 10);

---------------------------------------------------------
-- 8. ABSENCE_COURS (10 enregistrements)
-- Historise spécifiquement les absences totales
---------------------------------------------------------
INSERT INTO public.absence_cours (id_absence, id_user, id_cours, id_classe) VALUES 
(1, 6, 2, 1),
(2, 8, 4, 2),
(3, 10, 6, 4),
(4, 12, 8, 6),
(5, 7, 10, 1),
(6, 9, 1, 3),
(7, 11, 2, 5),
(8, 6, 4, 1),
(9, 8, 6, 2),
(10, 10, 8, 4);

SELECT setval('absence_cours_id_absence_seq', 10);

---------------------------------------------------------
-- 9. HISTORIQUE (10 enregistrements)
---------------------------------------------------------
INSERT INTO public.historique (id_historique, id_user, date_action, type_action) VALUES 
(1, 1, '2026-05-01 08:00:00', 'Connexion admin réussie'),
(2, 1, '2026-05-01 08:05:00', 'Création du cours ID 1'),
(3, 3, '2026-05-01 09:15:00', 'Modification du cours ID 4'),
(4, 6, '2026-05-10 07:55:00', 'Scan badge ENTREE bâtiment'),
(5, 7, '2026-05-10 07:58:00', 'Scan badge ENTREE bâtiment'),
(6, 3, '2026-05-10 08:05:00', 'Validation de l''appel manuel'),
(7, 6, '2026-05-10 12:05:00', 'Scan badge SORTIE bâtiment'),
(8, 2, '2026-05-11 14:00:00', 'Ajout d''une nouvelle salle (C101)'),
(9, 8, '2026-05-12 08:00:00', 'Déclaration absence justifiée via portail'),
(10, 1, '2026-05-15 18:00:00', 'Exportation des statistiques d''assiduité');

SELECT setval('historique_id_historique_seq', 10);