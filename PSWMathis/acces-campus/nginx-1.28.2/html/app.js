// ============================================================
// app.js  -  Logique principale de l'interface Acces Campus
// Framework : Vue 3 (chargé via CDN dans index.html)
// API       : json-server sur http://localhost:3001
// ============================================================

// -------------------------------------------------------
// Routes disponibles - à compléter avec les routes
// définitives fournies par le binôme (voir routes.json)
//
// Avec nginx  : les URLs commencent par /psw/...
//   nginx reçoit la requête sur le port 80 et la
//   redirige automatiquement vers json-server (port 3001)
//
// Sans nginx  : remplace '' par 'http://localhost:3001'
//   pour appeler json-server directement depuis le navigateur
// -------------------------------------------------------
const API = '';   // laisser vide si nginx est utilisé

const ROUTES = {
  utilisateurs: `${API}/psw/utilisateurs`,
  salles:       `${API}/psw/salles`,
  etudiants:    `${API}/psw/etudiants`,
  reservations: `${API}/psw/reservations`,
  planning:     `${API}/psw/planning`,
};


const { createApp } = Vue;

createApp({

  // ==========================================================
  // DATA  -  Toutes les variables réactives de l'application
  // Vue.js surveille ces variables : dès qu'elles changent,
  // l'interface se met à jour automatiquement.
  // ==========================================================
  data() {
    return {
      // Page actuellement affichée ('login', 'dashboard', 'dispo', 'agenda', 'annuaire', 'planning')
      pageCourante: 'login',

      // --- Connexion ---
      loginId:      '',      // Identifiant tapé dans le formulaire
      loginMdp:     '',      // Mot de passe tapé dans le formulaire
      loginErreur:  '',      // Message d'erreur affiché sous le formulaire
      loginCharge:  false,   // true pendant l'appel API (affiche un spinner)
      userConnecte: null,    // Objet utilisateur récupéré depuis json-server

      // --- Données chargées depuis json-server ---
      salles:       [],      // Liste des noms de salles  ex: ['Salle B201']
      etudiants:    [],      // Liste des étudiants pour l'annuaire
      reservations: [],      // Réservations de l'utilisateur connecté
      planning:     {},      // Planning indexé par salle  ex: { 'Salle B201': [...] }

      // --- Créneaux horaires (fixes, définis dans le contrat API) ---
      heures: ['08:15','09:10','10:20','11:15','13:30','14:25','15:35','16:30'],
      jours:  ['Lundi','Mardi','Mercredi','Jeudi','Vendredi'],

      // Correspondance numéro JS → nom du jour en français
      // new Date().getDay() retourne 0=dim, 1=lun ... 5=ven
      joursFR: { 1: 'Lundi', 2: 'Mardi', 3: 'Mercredi', 4: 'Jeudi', 5: 'Vendredi' },

      creneaux: [
        '08:15 - 09:10',
        '09:10 - 10:05',
        '10:20 - 11:15',
        '11:15 - 12:10',
        '13:30 - 14:25',
        '14:25 - 15:20',
        '15:35 - 16:30',
        '16:30 - 17:25',
      ],

      // --- Module Disponibilité ---
      salleChoisie:   '',    // Salle sélectionnée dans le menu déroulant
      creneauChoisi:  '',    // Créneau optionnel pour affiner la recherche

      // --- Module Agenda ---
      agendaCreneauDeb: '',
      agendaCreneauFin: '',
      agendaDateDeb:    '',
      agendaDateFin:    '',
      agendaResultats:  null,  // null = pas encore recherché, [] = aucun résultat

      // --- Module Annuaire ---
      rechercheAnnuaire: '',   // Texte tapé dans la barre de recherche

      // --- Module Planning ---
      sallesPlanningChoisie: '',

      // --- Toast (notification en haut à droite) ---
      toastMessage: '',
      toastVisible: false,
      toastTimer:   null,
    };
  },


  // ==========================================================
  // COMPUTED  -  Propriétés calculées automatiquement
  // Se recalculent dès que les variables dont elles dépendent changent.
  // ==========================================================
  computed: {

    // Filtre la liste des étudiants selon le texte tapé dans la recherche
    // Insensible à la casse grâce à toLowerCase()
    etudiantsFiltres() {
      const recherche = this.rechercheAnnuaire.trim().toLowerCase();
      // Si la barre est vide, on affiche tout le monde
      if (!recherche) return this.etudiants;
      return this.etudiants.filter(e =>
        e.nom.toLowerCase().includes(recherche) ||
        e.classe.toLowerCase().includes(recherche) ||
        e.email.toLowerCase().includes(recherche)
      );
    },

    // Calcule si la salle sélectionnée est libre ou occupée
    // en croisant le planning avec le jour d'aujourd'hui (et le créneau si choisi)
    infoDispo() {
      // Rien à afficher si aucune salle n'est sélectionnée
      if (!this.salleChoisie) return null;

      // Récupère les événements de la salle (ou tableau vide si aucun)
      const evenements = this.planning[this.salleChoisie] || [];

      // Récupère le jour d'aujourd'hui en français
      const jourdeSemaine = new Date().getDay();
      const jourAujourdhui = this.joursFR[jourdeSemaine] || null;

      // Filtre les événements qui ont lieu aujourd'hui
      let evenementsDuJour = jourAujourdhui
        ? evenements.filter(e => e.jour === jourAujourdhui)
        : [];

      // Si un créneau est sélectionné, on filtre encore plus précisément
      if (this.creneauChoisi) {
        // On extrait l'heure de début du créneau  ex: '08:15' depuis '08:15 - 09:10'
        const heureDebut = this.creneauChoisi.split(' - ')[0];
        evenementsDuJour = evenementsDuJour.filter(e => e.heure === heureDebut);
      }

      return {
        libre:  evenementsDuJour.length === 0,   // true si aucun cours trouvé
        events: evenementsDuJour,
        jour:   jourAujourdhui,
      };
    },

    // Construit le tableau de lignes pour la grille du planning
    // Chaque ligne = une heure, chaque colonne = un jour de la semaine
    lignesPlanning() {
      const evenements = this.planning[this.sallesPlanningChoisie] || [];
      return this.heures.map(heure => ({
        heure,
        // Pour chaque jour, on cherche si un événement existe à cette heure
        jours: this.jours.map(jour =>
          evenements.find(e => e.jour === jour && e.heure === heure) || null
        ),
      }));
    },
  },


  // ==========================================================
  // METHODS  -  Toutes les fonctions de l'application
  // ==========================================================
  methods: {

    // Change la page affichée et remonte automatiquement en haut
    allerA(page) {
      this.pageCourante = page;
      window.scrollTo(0, 0);
    },

    // ----------------------------------------------------------
    // Chargement des données depuis json-server au démarrage
    // On récupère tout en parallèle pour aller plus vite
    // ----------------------------------------------------------
    async chargerDonnees() {
      try {
        // Promise.all lance les 4 requêtes en même temps
        const [salles, etudiants, reservations, planningFlat] = await Promise.all([
          fetch(ROUTES.salles).then(r => r.json()),
          fetch(ROUTES.etudiants).then(r => r.json()),
          fetch(ROUTES.reservations).then(r => r.json()),
          fetch(ROUTES.planning).then(r => r.json()),
        ]);

        // json-server renvoie les salles sous forme d'objets { id, nom }
        // On extrait juste le nom pour les menus déroulants
        this.salles = salles.map(s => s.nom);

        this.etudiants    = etudiants;
        this.reservations = reservations;

        // Le planning est stocké à plat dans db.json (tableau d'événements)
        // On le réorganise en objet indexé par salle pour faciliter les recherches
        // Résultat : { 'Salle B201': [ev1, ev2, ...], 'Salle B102': [...] }
        this.planning = {};
        planningFlat.forEach(ev => {
          if (!this.planning[ev.salle]) this.planning[ev.salle] = [];
          this.planning[ev.salle].push(ev);
        });

        // Sélectionne la première salle par défaut dans le planning
        if (this.salles.length > 0) {
          this.sallesPlanningChoisie = this.salles[0];
        }

      } catch (err) {
        // Si json-server n'est pas lancé, on prévient l'utilisateur
        this.afficherToast('Impossible de charger les données. Lance json-server avec : npm run api');
        console.error('Erreur chargement données :', err);
      }
    },

    // ----------------------------------------------------------
    // Login - vérifie les identifiants contre json-server
    // On interroge /psw/utilisateurs?login_user=xxx
    // ----------------------------------------------------------
    async gererLogin() {
      // Validation rapide côté client avant d'envoyer quoi que ce soit
      if (!this.loginId.trim() || !this.loginMdp.trim()) {
        this.loginErreur = 'Veuillez remplir tous les champs.';
        return;
      }

      this.loginErreur  = '';
      this.loginCharge  = true;

      try {
        // Cherche l'utilisateur dans json-server par son identifiant
        const url = `${ROUTES.utilisateurs}?login_user=${encodeURIComponent(this.loginId.trim())}`;
        const reponse = await fetch(url);
        const utilisateurs = await reponse.json();

        // Utilisateur introuvable dans la base
        if (utilisateurs.length === 0) {
          this.loginErreur = 'Identifiant ou mot de passe incorrect.';
          return;
        }

        const user = utilisateurs[0];

        // Vérification du mot de passe (comparaison directe avec json-server)
        if (user.motdepasse !== this.loginMdp) {
          this.loginErreur = 'Identifiant ou mot de passe incorrect.';
          return;
        }

        // Connexion réussie : on stocke l'utilisateur en mémoire et en localStorage
        this.userConnecte = user;
        localStorage.setItem('user_connecte', JSON.stringify(user));
        this.allerA('dashboard');

      } catch (err) {
        // json-server probablement pas lancé
        console.error('Erreur login :', err);
        this.loginErreur = 'Impossible de joindre le serveur. Lance json-server avec : npm run api';

      } finally {
        // Le spinner s'arrête dans tous les cas (succès ou erreur)
        this.loginCharge = false;
      }
    },

    // Déconnexion : efface la session et retourne au login
    deconnecter() {
      this.userConnecte = null;
      localStorage.removeItem('user_connecte');
      // Réinitialise le formulaire pour la prochaine connexion
      this.loginId     = '';
      this.loginMdp    = '';
      this.loginErreur = '';
      this.allerA('login');
    },

    // ----------------------------------------------------------
    // Agenda - filtre les réservations selon les critères saisis
    // ----------------------------------------------------------
    rechercherAgenda() {
      const { agendaCreneauDeb, agendaCreneauFin, agendaDateDeb, agendaDateFin } = this;

      this.agendaResultats = this.reservations.filter(r => {
        // Les dates au format YYYY-MM-DD se comparent alphabétiquement (= chronologiquement)
        if (agendaDateDeb    && r.date    < agendaDateDeb)    return false;
        if (agendaDateFin    && r.date    > agendaDateFin)    return false;
        if (agendaCreneauDeb && r.creneau < agendaCreneauDeb) return false;
        if (agendaCreneauFin && r.creneau > agendaCreneauFin) return false;
        return true;
      });
    },

    // Convertit '2026-03-12' en '12/03/2026' pour l'affichage
    formaterDate(dateStr) {
      const [annee, mois, jour] = dateStr.split('-');
      return `${jour}/${mois}/${annee}`;
    },

    // Retourne la classe Bootstrap du badge selon le statut de la réservation
    classeBadge(statut) {
      return statut === 'confirme' ? 'bg-success' : 'bg-warning text-dark';
    },

    // ----------------------------------------------------------
    // Toast - notification temporaire en haut à droite
    // Disparaît automatiquement après 4 secondes
    // ----------------------------------------------------------
    afficherToast(message) {
      this.toastMessage = message;
      this.toastVisible = true;
      // Annule un éventuel timer précédent si un nouveau toast est déclenché
      if (this.toastTimer) clearTimeout(this.toastTimer);
      this.toastTimer = setTimeout(() => { this.toastVisible = false; }, 4000);
    },

    // Bouton de test pour vérifier que le toast fonctionne
    simulerErreur() {
      this.afficherToast('Erreur 503 - Le service de réservation est temporairement indisponible.');
    },
  },


  // ==========================================================
  // MOUNTED  -  Exécuté une fois quand la page est prête
  // ==========================================================
  mounted() {
    // Charge toutes les données depuis json-server dès le démarrage
    this.chargerDonnees();

    // Vérifie si une session existe déjà dans le navigateur
    // Si oui, l'utilisateur n'a pas besoin de se reconnecter
    const sessionSauvegardee = localStorage.getItem('user_connecte');
    if (sessionSauvegardee) {
      this.userConnecte = JSON.parse(sessionSauvegardee);
      this.allerA('dashboard');
    }

    // Permet de valider le formulaire de login avec la touche Entrée
    window.addEventListener('keyup', (e) => {
      if (e.key === 'Enter' && this.pageCourante === 'login') {
        this.gererLogin();
      }
    });
  },

}).mount('#app');
