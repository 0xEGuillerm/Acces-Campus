SET search_path TO pg_catalog,public;

---------------------------------------------------------
-- 1. CLASSE (10 enregistrements)
---------------------------------------------------------
INSERT INTO public.classe (id_classe, nom_classe) VALUES 
(1, 'BTS SIO 1 - SLAM'),
(2, 'BTS SIO 1 - SISR'),
(3, 'BTS CIEL 2 - SLAM'),
(4, 'BTS CIEL 2 - SISR'),
(5, 'Licence Pro CDA'),
(6, 'Licence Pro ASRALL'),
(7, 'Bachelor Cybersécurité'),
(8, 'Master 1 Ingénierie Web'),
(9, 'Master 2 Architecte Cloud'),
(10, 'Prépa Numérique');

---------------------------------------------------------
-- 2. BADGE (32 enregistrements) - Max 8 caractères
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
('BDG00012', NOW() - INTERVAL '1 days'),
('BDG00013', NOW() - INTERVAL '15 days'),
('BDG00014', NOW() - INTERVAL '15 days'),
('BDG00015', NOW() - INTERVAL '14 days'),
('BDG00016', NOW() - INTERVAL '14 days'),
('BDG00017', NOW() - INTERVAL '13 days'),
('BDG00018', NOW() - INTERVAL '13 days'),
('BDG00019', NOW() - INTERVAL '12 days'),
('BDG00020', NOW() - INTERVAL '12 days'),
('BDG00021', NOW() - INTERVAL '11 days'),
('BDG00022', NOW() - INTERVAL '11 days'),
('BDG00023', NOW() - INTERVAL '10 days'),
('BDG00024', NOW() - INTERVAL '10 days'),
('BDG00025', NOW() - INTERVAL '9 days'),
('BDG00026', NOW() - INTERVAL '9 days'),
('BDG00027', NOW() - INTERVAL '8 days'),
('BDG00028', NOW() - INTERVAL '8 days'),
('BDG00029', NOW() - INTERVAL '7 days'),
('BDG00030', NOW() - INTERVAL '7 days'),
('BDG00031', NOW() - INTERVAL '6 days'),
('BDG00032', NOW() - INTERVAL '6 days');

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
-- 4. UTILISATEUR (32 enregistrements)
-- 2 admins, 3 profs, 27 élèves
---------------------------------------------------------
INSERT INTO public.utilisateur (id_user, nom_user, prenom_user, login_user, hash_mdp, id_classe, role_user, uuid_badge) VALUES 
-- Admins (pas de classe)
(1, 'Super', 'Admin', 'admin1', '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', NULL, 'admin', 'BDG00001'),
(2, 'Tech', 'Support', 'admin2', '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', NULL, 'admin', 'BDG00002'),
-- Professeurs (pas de classe attitrée)
(3, 'Lovelace', 'Ada', 'alovelace', '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', NULL, 'professeur', 'BDG00003'),
(4, 'Turing', 'Alan', 'aturing', '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', NULL, 'professeur', 'BDG00004'),
(5, 'Hopper', 'Grace', 'ghopper', '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', NULL, 'professeur', 'BDG00005'),
-- Élèves d'origine
(6,  'Dupont', 'Jean',   'jdupont', '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 1, 'eleve', 'BDG00006'),
(7,  'Martin', 'Alice',  'amartin', '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 1, 'eleve', 'BDG00007'),
(8,  'Durand', 'Paul',   'pdurand', '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 2, 'eleve', 'BDG00008'),
(9,  'Petit',  'Sophie', 'spetit',  '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 3, 'eleve', 'BDG00009'),
(10, 'Leroy',  'Marc',   'mleroy',  '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 4, 'eleve', 'BDG00010'),
(11, 'Roux',   'Emma',   'eroux',   '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 5, 'eleve', 'BDG00011'),
(12, 'Moreau', 'Luc',    'lmoreau', '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 6, 'eleve', 'BDG00012'),
-- Nouveaux élèves
(13, 'Bernard',  'Lucas',   'lbernard',  '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 1,  'eleve', 'BDG00013'),
(14, 'Thomas',   'Chloé',   'cthomas',   '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 1,  'eleve', 'BDG00014'),
(15, 'Robert',   'Hugo',    'hrobert',   '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 2,  'eleve', 'BDG00015'),
(16, 'Richard',  'Léa',     'lrichard',  '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 2,  'eleve', 'BDG00016'),
(17, 'Garcia',   'Nathan',  'ngarcia',   '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 3,  'eleve', 'BDG00017'),
(18, 'Lefevre',  'Jade',    'jlefevre',  '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 3,  'eleve', 'BDG00018'),
(19, 'Roy',      'Tom',     'troy',      '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 4,  'eleve', 'BDG00019'),
(20, 'Vincent',  'Manon',   'mvincent',  '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 4,  'eleve', 'BDG00020'),
(21, 'Fournier', 'Louis',   'lfournier', '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 5,  'eleve', 'BDG00021'),
(22, 'Girard',   'Camille', 'cgirard',   '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 6,  'eleve', 'BDG00022'),
(23, 'Bonnet',   'Raphaël', 'rbonnet',   '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 7,  'eleve', 'BDG00023'),
(24, 'Dupuis',   'Sarah',   'sdupuis',   '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 7,  'eleve', 'BDG00024'),
(25, 'Lambert',  'Arthur',  'alambert',  '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 7,  'eleve', 'BDG00025'),
(26, 'Fontaine', 'Inès',    'ifontaine', '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 8,  'eleve', 'BDG00026'),
(27, 'Rousseau', 'Mathéo',  'mrousseau', '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 8,  'eleve', 'BDG00027'),
(28, 'Mercier',  'Eva',     'emercier',  '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 8,  'eleve', 'BDG00028'),
(29, 'Blanc',    'Léo',     'lblanc',    '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 9,  'eleve', 'BDG00029'),
(30, 'Guerin',   'Anaïs',   'aguerin',   '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 9,  'eleve', 'BDG00030'),
(31, 'Boyer',    'Noah',    'nboyer',    '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 10, 'eleve', 'BDG00031'),
(32, 'Brun',     'Mia',     'mbrun',     '$argon2id$v=19$m=65536,t=2,p=1$cPWG3v66wOCGnUYtxsLohg$KdGEBx91IFQMUjvZg/KePJoN2Xu9vKy7HzJYP6qQoWg', 10, 'eleve', 'BDG00032');

SELECT setval('utilisateur_id_user_seq', 32);

---------------------------------------------------------
-- 5. COURS (10 enregistrements d'origine)
-- Constraint : heure_fin > heure_debut
---------------------------------------------------------
INSERT INTO public.cours (id_cours, num_salle, heure_debut, heure_fin, id_classe, reserve_par, professeur) VALUES 
(1,  'A101', '2026-05-10 08:00:00', '2026-05-10 10:00:00', 1, 1, 3), -- Algo (Ada)
(2,  'A102', '2026-05-10 10:15:00', '2026-05-10 12:15:00', 2, 1, 4), -- Réseau (Alan)
(3,  'B101', '2026-05-10 14:00:00', '2026-05-10 17:00:00', 3, 2, 5), -- BDD (Grace)
(4,  'B102', '2026-05-11 08:00:00', '2026-05-11 12:00:00', 4, 3, 3), 
(5,  'AMP1', '2026-05-11 13:30:00', '2026-05-11 15:30:00', 5, 4, 4), 
(6,  'AMP2', '2026-05-12 09:00:00', '2026-05-12 11:00:00', 6, 5, 5),
(7,  'C101', '2026-05-12 14:00:00', '2026-05-12 16:00:00', 1, 1, 3),
(8,  'C102', '2026-05-13 10:00:00', '2026-05-13 12:00:00', 2, 2, 4),
(9,  'A201', '2026-05-13 15:00:00', '2026-05-13 17:00:00', 3, 3, 5),
(10, 'A202', '2026-05-14 08:30:00', '2026-05-14 10:30:00', 4, 1, 3);

---------------------------------------------------------
-- 5bis. NOUVEAUX COURS sur 3 semaines (IDs 11 → 55)
-- Semaines : 11/05, 18/05, 25/05 (mai 2026)
-- Créneaux : 08:00-10:00 / 10:15-12:15 / 13:30-15:30 / 15:45-17:45
-- Profs disponibles : 3 (Ada), 4 (Alan), 5 (Grace)
---------------------------------------------------------
INSERT INTO public.cours (id_cours, num_salle, heure_debut, heure_fin, id_classe, reserve_par, professeur) VALUES

-- ============ SEMAINE 1 : 11 → 15 mai 2026 ============
-- Lundi 11/05 (à noter : cours 4 et 5 d'origine sont déjà sur cette journée)
-- (cours 4 d'origine : B102 08h-12h prof 3 / cours 5 : AMP1 13h30-15h30 prof 4)
(11, 'A101', '2026-05-11 15:45:00', '2026-05-11 17:45:00', 1,  3, 5),
(12, 'A102', '2026-05-11 08:00:00', '2026-05-11 10:00:00', 2,  4, 4),
(13, 'B101', '2026-05-11 10:15:00', '2026-05-11 12:15:00', 7,  5, 5),
(14, 'A102', '2026-05-11 13:30:00', '2026-05-11 15:30:00', 3,  3, 3),

-- Mardi 12/05 (cours 6 : AMP2 09h-11h prof 5 / cours 7 : C101 14h-16h prof 3)
(15, 'B102', '2026-05-12 08:00:00', '2026-05-12 10:00:00', 4,  4, 4),
(16, 'A201', '2026-05-12 10:15:00', '2026-05-12 12:15:00', 8,  5, 4),
(17, 'AMP1', '2026-05-12 13:30:00', '2026-05-12 15:30:00', 5,  1, 5),
(18, 'C102', '2026-05-12 15:45:00', '2026-05-12 17:45:00', 9,  2, 4),

-- Mercredi 13/05
(19, 'A102', '2026-05-13 08:00:00', '2026-05-13 10:00:00', 6,  3, 5),
(20, 'A202', '2026-05-13 10:15:00', '2026-05-13 12:15:00', 10, 4, 3),
(21, 'B101', '2026-05-13 13:30:00', '2026-05-13 15:30:00', 7,  5, 4),

-- Jeudi 14/05 (cours 10 : A202 08h30-10h30 prof 3)
(22, 'A101', '2026-05-14 10:45:00', '2026-05-14 12:45:00', 1,  1, 4),
(23, 'C102', '2026-05-14 13:30:00', '2026-05-14 15:30:00', 8,  2, 5),
(24, 'AMP2', '2026-05-14 15:45:00', '2026-05-14 17:45:00', 9,  3, 4),

-- Vendredi 15/05
(25, 'A102', '2026-05-15 08:00:00', '2026-05-15 10:00:00', 2,  4, 4),
(26, 'B102', '2026-05-15 10:15:00', '2026-05-15 12:15:00', 4,  5, 3),

-- ============ SEMAINE 2 : 18 → 22 mai 2026 ============
-- Lundi 18/05
(27, 'A101', '2026-05-18 08:00:00', '2026-05-18 10:00:00', 1,  3, 3),
(28, 'B101', '2026-05-18 08:00:00', '2026-05-18 10:00:00', 7,  5, 5),
(29, 'A102', '2026-05-18 10:15:00', '2026-05-18 12:15:00', 2,  4, 4),
(30, 'A201', '2026-05-18 13:30:00', '2026-05-18 15:30:00', 3,  1, 3),
(31, 'C101', '2026-05-18 15:45:00', '2026-05-18 17:45:00', 8,  2, 5),

-- Mardi 19/05
(32, 'B102', '2026-05-19 08:00:00', '2026-05-19 10:00:00', 4,  4, 4),
(33, 'AMP1', '2026-05-19 10:15:00', '2026-05-19 12:15:00', 5,  3, 3),
(34, 'A202', '2026-05-19 13:30:00', '2026-05-19 15:30:00', 9,  5, 5),
(35, 'A101', '2026-05-19 15:45:00', '2026-05-19 17:45:00', 1,  1, 4),

-- Mercredi 20/05
(36, 'B101', '2026-05-20 08:00:00', '2026-05-20 10:00:00', 6,  3, 3),
(37, 'C102', '2026-05-20 10:15:00', '2026-05-20 12:15:00', 10, 2, 4),
(38, 'A102', '2026-05-20 13:30:00', '2026-05-20 15:30:00', 7,  4, 5),

-- Jeudi 21/05
(39, 'AMP2', '2026-05-21 08:00:00', '2026-05-21 10:00:00', 8,  5, 4),
(40, 'A201', '2026-05-21 10:15:00', '2026-05-21 12:15:00', 3,  3, 3),
(41, 'B102', '2026-05-21 13:30:00', '2026-05-21 15:30:00', 4,  1, 5),

-- Vendredi 22/05
(42, 'A101', '2026-05-22 08:00:00', '2026-05-22 10:00:00', 1,  4, 3),
(43, 'C101', '2026-05-22 10:15:00', '2026-05-22 12:15:00', 9,  2, 4),

-- ============ SEMAINE 3 : 25 → 29 mai 2026 ============
-- Lundi 25/05
(44, 'A102', '2026-05-25 08:00:00', '2026-05-25 10:00:00', 2,  3, 4),
(45, 'B101', '2026-05-25 10:15:00', '2026-05-25 12:15:00', 5,  4, 3),
(46, 'A101', '2026-05-25 13:30:00', '2026-05-25 15:30:00', 1,  5, 5),

-- Mardi 26/05
(47, 'B102', '2026-05-26 08:00:00', '2026-05-26 10:00:00', 4,  1, 4),
(48, 'AMP1', '2026-05-26 10:15:00', '2026-05-26 12:15:00', 6,  2, 3),
(49, 'A202', '2026-05-26 13:30:00', '2026-05-26 15:30:00', 7,  3, 5),
(50, 'C102', '2026-05-26 15:45:00', '2026-05-26 17:45:00', 10, 4, 4),

-- Mercredi 27/05
(51, 'A201', '2026-05-27 08:00:00', '2026-05-27 10:00:00', 3,  5, 3),
(52, 'AMP2', '2026-05-27 10:15:00', '2026-05-27 12:15:00', 8,  3, 5),
(53, 'B101', '2026-05-27 13:30:00', '2026-05-27 15:30:00', 9,  1, 4),

-- Jeudi 28/05
(54, 'A102', '2026-05-28 08:00:00', '2026-05-28 10:00:00', 2,  2, 3),
(55, 'C101', '2026-05-28 10:15:00', '2026-05-28 12:15:00', 7,  4, 5);

SELECT setval('cours_id_cours_seq', 55);

---------------------------------------------------------
-- 6. PRESENCE_COURS (12 enregistrements)
-- Des élèves qui ont bien badgé et sont présents
---------------------------------------------------------
INSERT INTO public.presence_cours (id_presence, id_user, id_cours, id_classe) VALUES 
(1,  6,  1,  1),
(2,  7,  1,  1),
(3,  8,  2,  2),
(4,  9,  3,  3),
(5,  10, 4,  4),
(6,  11, 5,  5),
(7,  12, 6,  6),
(8,  6,  7,  1),
(9,  7,  7,  1),
(10, 8,  8,  2),
(11, 9,  9,  3),
(12, 10, 10, 4);

SELECT setval('presence_cours_id_presence_seq', 12);

---------------------------------------------------------
-- 7. RETARDABSENCE (10 enregistrements)
-- Constraint : (id_user, id_cours) UNIQUE
-- Mixte de retards (absence = false) et absences pures
---------------------------------------------------------
INSERT INTO public.retardabsence (id_retardabsence, id_user, id_cours, temps_retard_min, absence, timestamp_heure_cours) VALUES 
(1,  6,  2,  0,  true,  '2026-05-10 10:15:00'),  -- Jean est absent (Cours 2)
(2,  7,  3,  15, false, '2026-05-10 14:00:00'),  -- Alice a 15 min de retard (Cours 3)
(3,  8,  4,  0,  true,  '2026-05-11 08:00:00'),
(4,  9,  5,  5,  false, '2026-05-11 13:30:00'),
(5,  10, 6,  0,  true,  '2026-05-12 09:00:00'),
(6,  11, 7,  30, false, '2026-05-12 14:00:00'),
(7,  12, 8,  0,  true,  '2026-05-13 10:00:00'),
(8,  6,  9,  10, false, '2026-05-13 15:00:00'),
(9,  7,  10, 0,  true,  '2026-05-14 08:30:00'),
(10, 8,  1,  45, false, '2026-05-10 08:00:00');

SELECT setval('retardabsence_id_retardabsence_seq', 10);

---------------------------------------------------------
-- 8. ABSENCE_COURS (10 enregistrements)
-- Historise spécifiquement les absences totales
---------------------------------------------------------
INSERT INTO public.absence_cours (id_absence, id_user, id_cours, id_classe) VALUES 
(1,  6,  2, 1),
(2,  8,  4, 2),
(3,  10, 6, 4),
(4,  12, 8, 6),
(5,  7,  10, 1),
(6,  9,  1, 3),
(7,  11, 2, 5),
(8,  6,  4, 1),
(9,  8,  6, 2),
(10, 10, 8, 4);

SELECT setval('absence_cours_id_absence_seq', 10);

---------------------------------------------------------
-- 9. HISTORIQUE (10 enregistrements)
---------------------------------------------------------
INSERT INTO public.historique (id_historique, id_user, date_action, type_action) VALUES 
(1,  1, '2026-05-01 08:00:00', 'Connexion admin réussie'),
(2,  1, '2026-05-01 08:05:00', 'Création du cours ID 1'),
(3,  3, '2026-05-01 09:15:00', 'Modification du cours ID 4'),
(4,  6, '2026-05-10 07:55:00', 'Scan badge ENTREE bâtiment'),
(5,  7, '2026-05-10 07:58:00', 'Scan badge ENTREE bâtiment'),
(6,  3, '2026-05-10 08:05:00', 'Validation de l''appel manuel'),
(7,  6, '2026-05-10 12:05:00', 'Scan badge SORTIE bâtiment'),
(8,  2, '2026-05-11 14:00:00', 'Ajout d''une nouvelle salle (C101)'),
(9,  8, '2026-05-12 08:00:00', 'Déclaration absence justifiée via portail'),
(10, 1, '2026-05-15 18:00:00', 'Exportation des statistiques d''assiduité');

SELECT setval('historique_id_historique_seq', 10);
