// app.js - Acces Campus
// Framework : Vue 3
// API       : json-server

const ROUTES = {
  login:        '/psw/login',          // GET  (mock) / POST (prod) - authentification
  reserverSalle:'/psw/reserver_salle', // POST - créer une réservation
  salleCrenaux: '/psw/salle_crenaux',  // GET  - créneaux disponibles
  planning:     '/psw/planning',       // GET  - planning des salles
  etatSalle:    '/psw/etat_salle',     // GET  - état des salles en temps réel
  historique:   '/psw/historique_eleve', // GET - historique de présence
};

const { createApp } = Vue;

createApp({
  data() {
    return {
      currentPage: 'login',

      // Login
      loginId:      '',
      loginPass:    '',
      loginError:   '',
      loginLoading: false,

      // Données statiques
      etudiants: [
        { nom: 'Mathis Blanchard', classe: 'BTS CIEL 2', email: 'mathis.blanchard@campus.fr' },
        { nom: 'Emile Guillerm',   classe: 'BTS CIEL 2', email: 'emile.guillerm@campus.fr' },
        { nom: 'Mael Guillon',     classe: 'BTS CIEL 2', email: 'mael.guillon@campus.fr' },
      ],

      salles: ['Salle B201'],

      heures: ['08:15','09:10','10:20','11:15','13:30','14:25','15:35','16:30'],
      jours:  ['Lundi','Mardi','Mercredi','Jeudi','Vendredi'],

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

      planning: {
        'Salle B201': [
          { jour: 'Lundi',    heure: '08:15', titre: 'Cours Reseaux - Mr Jumel', type: 'cours', duree: 1 },
          { jour: 'Mardi',    heure: '10:20', titre: 'TP Reseaux - Mr Jumel',    type: 'tp',    duree: 1 },
          { jour: 'Jeudi',    heure: '13:30', titre: 'Cours Reseaux - Mr Jumel', type: 'cours', duree: 1 },
          { jour: 'Vendredi', heure: '14:25', titre: 'TP Reseaux - Mr Jumel',    type: 'tp',    duree: 1 },
        ],
      },

      joursFR: { 1:'Lundi', 2:'Mardi', 3:'Mercredi', 4:'Jeudi', 5:'Vendredi' },

      // Annuaire
      searchAnnuaire: '',

      // Disponibilité
      selectedDispo:       '',
      selectedCreneauDispo: '',

      // Planning 
      selectedPlanning: 'Salle B201',

      // Réservation
      creneaux_api:        [],    // tous les créneaux chargés depuis json-server
      creneauxFiltres:     [],    // créneaux filtrés après la recherche
      creneauxCharge:      false,
      rechercheEffectuee:  false, // true après le premier clic sur Rechercher
      reservationEnCours:  null,
      confirmationVisible: false,
      creneauSelectionne:  null,

      // Champs du formulaire de recherche
      reservationDate:     '',
      reservationHeureDeb: '',
      reservationHeureFin: '',
      erreurRecherche:     '',

      // Créneaux horaires proposés dans les menus déroulants
      heuresDispos: [
        '08:00', '08:15', '09:00', '09:10', '10:00', '10:20',
        '11:00', '11:15', '12:00', '13:00', '13:30', '14:00',
        '14:25', '15:00', '15:35', '16:00', '16:30', '17:00',
      ],

      // --- Toast ---
      toastMessage: '',
      toastVisible: false,
      toastType:    'success',
      toastTimer:   null,
    };
  },


  computed: {

    // Filtre l'annuaire selon la saisie de l'utilisateur
    filteredStudents() {
      const q = this.searchAnnuaire.trim().toLowerCase();
      if (!q) return this.etudiants;
      return this.etudiants.filter(s =>
        s.nom.toLowerCase().includes(q) ||
        s.classe.toLowerCase().includes(q) ||
        s.email.toLowerCase().includes(q)
      );
    },

    // Calcule si la salle sélectionnée est libre ou occupée aujourd'hui
    dispoInfo() {
      if (!this.selectedDispo) return null;
      const events  = this.planning[this.selectedDispo] || [];
      const today   = new Date().getDay();
      const jourAuj = this.joursFR[today] || null;
      let eventsDuJour = jourAuj ? events.filter(e => e.jour === jourAuj) : [];
      if (this.selectedCreneauDispo) {
        const heureDebut = this.selectedCreneauDispo.split(' - ')[0];
        eventsDuJour = eventsDuJour.filter(e => e.heure === heureDebut);
      }
      return { libre: eventsDuJour.length === 0, events: eventsDuJour, jour: jourAuj };
    },

    // Construit les lignes du tableau planning (une ligne par heure)
    planningRows() {
      const events = this.planning[this.selectedPlanning] || [];
      return this.heures.map(heure => ({
        heure,
        jours: this.jours.map(jour => events.find(e => e.jour === jour && e.heure === heure) || null),
      }));
    },
  },


  methods: {

    // Change la page affichée
    goTo(page) {
      this.currentPage = page;
      window.scrollTo(0, 0);
      // Charge les créneaux depuis json-server quand on ouvre la réservation
      if (page === 'reservation') this.chargerCreneaux();
    },

   
    // login cherche l'utilisateur par son login
    // puis compare le mot de passe côté client

    async handleLogin() {
      if (!this.loginId.trim() || !this.loginPass.trim()) {
        this.loginError = 'Veuillez remplir tous les champs.';
        return;
      }
      this.loginError   = '';
      this.loginLoading = true;

      try {
        const url         = `${ROUTES.login}?login_user=${encodeURIComponent(this.loginId.trim())}`;
        const reponse     = await fetch(url);
        const utilisateurs = await reponse.json();

        if (utilisateurs.length === 0) {
          this.loginError = 'Identifiant ou mot de passe incorrect.';
          return;
        }

        const user = utilisateurs[0];

        // Vérifie le mot de passe (json-server ne chiffre pas)
        if (user.motdepasse !== this.loginPass) {
          this.loginError = 'Identifiant ou mot de passe incorrect.';
          return;
        }

        // Connexion réussie : sauvegarde en localStorage et redirige
        localStorage.setItem('user_connecte', JSON.stringify(user));
        this.goTo('dashboard');

      } catch (err) {
        this.loginError = 'Impossible de joindre le serveur. Veuillez reessayer.';
        console.error('Erreur login :', err);
      } finally {
        this.loginLoading = false;
      }
    },

    // deconnexion : efface la session et retourne au login
    logout() {
      localStorage.removeItem('user_connecte');
      this.loginId   = '';
      this.loginPass = '';
      this.loginError = '';
      this.goTo('login');
    },


 
    // reservation recherch des créneaux disponibles
    // charge tous les créneaux puis filtre
    // selon la date et les heures choisies par l'utilisateur

    async rechercherCreneaux() {
      // Vérifie que tous les champs sont remplis
      if (!this.reservationDate || !this.reservationHeureDeb || !this.reservationHeureFin) {
        this.erreurRecherche = 'Veuillez remplir la date et les deux heures.';
        return;
      }
      if (this.reservationHeureDeb >= this.reservationHeureFin) {
        this.erreurRecherche = "L'heure de fin doit etre apres l'heure de debut.";
        return;
      }

      this.erreurRecherche    = '';
      this.creneauxCharge     = true;
      this.rechercheEffectuee = false;

      try {
        // Charge tous les créneaux depuis json-server
        const reponse        = await fetch(ROUTES.salleCrenaux);
        this.creneaux_api    = await reponse.json();

        // Filtre côté client selon la date et l'heure choisies
        this.creneauxFiltres = this.creneaux_api.filter(c => {
          const d      = new Date(c.horaire);
          const dateC  = d.toISOString().split('T')[0];         // "2026-03-25"
          const heureC = d.getHours().toString().padStart(2,'0') + ':' + d.getMinutes().toString().padStart(2,'0'); // "08:00"
          return dateC === this.reservationDate && heureC >= this.reservationHeureDeb && heureC < this.reservationHeureFin;
        });

        this.rechercheEffectuee = true;

      } catch (err) {
        this.erreurRecherche = 'Impossible de contacter le serveur.';
        console.error('Erreur créneaux :', err);
      } finally {
        this.creneauxCharge = false;
      }
    },

    // Alias pour le bouton Actualiser (recharge sans réinitialiser le formulaire)
    async chargerCreneaux() {
      await this.rechercherCreneaux();
    },

    // Ouvre la modale de confirmation avec le créneau cliqué
    demanderConfirmation(creneau) {
      this.creneauSelectionne  = creneau;
      this.confirmationVisible = true;
    },

    // Ferme la modale sans rien faire
    annulerConfirmation() {
      this.confirmationVisible = false;
      this.creneauSelectionne  = null;
    },

    // Envoie la réservation (POST)
    async confirmerReservation() {
      if (!this.creneauSelectionne) return;
      const creneau = this.creneauSelectionne;
      this.confirmationVisible = false;
      this.reservationEnCours  = creneau.id;

      try {
        const user = JSON.parse(localStorage.getItem('user_connecte') || '{}');
        const reponse = await fetch(ROUTES.reserverSalle, {
          method:  'POST',
          headers: { 'Content-Type': 'application/json' },
          body:    JSON.stringify({
            horaire:    creneau.horaire,
            numero:     creneau.numero,
            login_user: user.login_user,
          }),
        });

        if (!reponse.ok) throw new Error(`Erreur ${reponse.status}`);

        this.showToast(`Salle ${creneau.numero} reservee pour le ${this.formaterHoraire(creneau.horaire)} !`, 'success');
        // Retire le créneau de la liste
        this.creneaux_api = this.creneaux_api.filter(c => c.id !== creneau.id);

      } catch (err) {
        this.showToast('La reservation a echoue. Reessaie.', 'danger');
        console.error('Erreur reservation :', err);
      } finally {
        this.reservationEnCours = null;
        this.creneauSelectionne = null;
      }
    },

    // Formate une date ISO → "25/03/2026"
    formaterDate(dateISO) {
      const d = new Date(dateISO);
      return `${String(d.getDate()).padStart(2,'0')}/${String(d.getMonth()+1).padStart(2,'0')}/${d.getFullYear()}`;
    },

    // Formate une date → "08h00"
    formaterHeure(dateISO) {
      const d = new Date(dateISO);
      return `${String(d.getHours()).padStart(2,'0')}h${String(d.getMinutes()).padStart(2,'0')}`;
    },

    // Formate une date → "25/03/2026 a 08h00" 
    formaterHoraire(dateISO) {
      return `${this.formaterDate(dateISO)} a ${this.formaterHeure(dateISO)}`;
    },

    // Affiche une notification temporaire (4 secondes)
    showToast(message, type = 'success') {
      this.toastMessage = message;
      this.toastType    = type;
      this.toastVisible = true;
      if (this.toastTimer) clearTimeout(this.toastTimer);
      this.toastTimer = setTimeout(() => { this.toastVisible = false; }, 4000);
    },
  },


  mounted() {
    // Si une session existe déjà, on va directement au dashboard
    const session = localStorage.getItem('user_connecte');
    if (session) this.goTo('dashboard');

    // Touche Entrée pour valider le login
    window.addEventListener('keyup', (e) => {
      if (e.key === 'Enter' && this.currentPage === 'login') this.handleLogin();
    });
  },

}).mount('#app');
