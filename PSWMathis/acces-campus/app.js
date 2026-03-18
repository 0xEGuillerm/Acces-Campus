const { createApp } = Vue;

createApp({
  data() {
    return {
      currentPage: 'login',

      // Login
      loginId: '',
      loginPass: '',
      loginError: '',

      // Donnees
      etudiants: [
        { nom: 'Mathis Blanchard', classe: 'BTS CIEL 2', email: 'mathis.blanchard@campus.fr' },
        { nom: 'Emile Guillerm',   classe: 'BTS CIEL 2', email: 'emile.guillerm@campus.fr' },
        { nom: 'Mael Guillon',     classe: 'BTS CIEL 2', email: 'mael.guillon@campus.fr' },
      ],

      salles: ['Salle B201'],

      heures: ['08:15','09:10','10:20','11:15','13:30','14:25','15:35','16:30'],
      jours: ['Lundi','Mardi','Mercredi','Jeudi','Vendredi'],

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
          { jour: 'Lundi',    heure: '08:15', titre: 'Cours Reseaux - Mr Jumel',    type: 'cours',  duree: 1 },
          { jour: 'Mardi',    heure: '10:20', titre: 'TP Reseaux - Mr Jumel',       type: 'tp',     duree: 1 },
          { jour: 'Jeudi',    heure: '13:30', titre: 'Cours Reseaux - Mr Jumel',    type: 'cours',  duree: 1 },
          { jour: 'Vendredi', heure: '14:25', titre: 'TP Reseaux - Mr Jumel',       type: 'tp',     duree: 1 },
        ],
      },

      reservations: [
        { date: '2026-03-12', creneau: '10:20 - 11:15', salle: 'Salle B201', titre: 'TP Reseaux - Mr Jumel',    statut: 'confirme' },
        { date: '2026-03-13', creneau: '13:30 - 14:25', salle: 'Salle B201', titre: 'Cours Reseaux - Mr Jumel', statut: 'confirme' },
        { date: '2026-03-14', creneau: '08:15 - 09:10', salle: 'Salle B201', titre: 'Cours Reseaux - Mr Jumel', statut: 'en attente' },
      ],

      joursFR: { 1: 'Lundi', 2: 'Mardi', 3: 'Mercredi', 4: 'Jeudi', 5: 'Vendredi' },

      // Annuaire
      searchAnnuaire: '',

      // Disponibilite : salle selectionnee et creneau optionnel
      selectedDispo: '',
      selectedCreneauDispo: '',

      // Agenda
      agendaCreneauDeb: '',
      agendaCreneauFin: '',
      agendaDateDeb: '',
      agendaDateFin: '',
      agendaResultats: null,

      // Planning
      selectedPlanning: 'Salle B201',

      // Toast
      toastMessage: '',
      toastVisible: false,
      toastTimer: null,
    };
  },

  computed: {
    filteredStudents() {
      const q = this.searchAnnuaire.trim().toLowerCase();
      if (!q) return this.etudiants;
      return this.etudiants.filter(s =>
        s.nom.toLowerCase().includes(q) ||
        s.classe.toLowerCase().includes(q) ||
        s.email.toLowerCase().includes(q)
      );
    },

    dispoInfo() {
      // Aucune salle selectionnee : on ne retourne rien
      if (!this.selectedDispo) return null;

      // Recupere les evenements de la salle choisie
      const events = this.planning[this.selectedDispo] || [];

      // Determine le jour actuel en francais (1=Lundi ... 5=Vendredi)
      const today = new Date().getDay();
      const jourAuj = this.joursFR[today] || null;

      // Filtre les cours qui ont lieu aujourd'hui
      let eventsDuJour = jourAuj ? events.filter(e => e.jour === jourAuj) : [];

      // Si un creneau est selectionne, on filtre en plus par l'heure de debut
      if (this.selectedCreneauDispo) {
        const heureDebut = this.selectedCreneauDispo.split(' - ')[0];
        eventsDuJour = eventsDuJour.filter(e => e.heure === heureDebut);
      }

      return { libre: eventsDuJour.length === 0, events: eventsDuJour, jour: jourAuj };
    },

    planningRows() {
      const events = this.planning[this.selectedPlanning] || [];
      return this.heures.map(heure => ({
        heure,
        jours: this.jours.map(jour => events.find(e => e.jour === jour && e.heure === heure) || null),
      }));
    },
  },

  methods: {
    goTo(page) {
      this.currentPage = page;
      window.scrollTo(0, 0);
    },

    handleLogin() {
      if (!this.loginId.trim() || !this.loginPass.trim()) {
        this.loginError = 'Veuillez remplir tous les champs.';
        return;
      }
      this.loginError = '';
      this.goTo('dashboard');
    },

    logout() {
      this.loginId = '';
      this.loginPass = '';
      this.loginError = '';
      this.goTo('login');
    },

    showToast(message) {
      this.toastMessage = message;
      this.toastVisible = true;
      if (this.toastTimer) clearTimeout(this.toastTimer);
      this.toastTimer = setTimeout(() => { this.toastVisible = false; }, 4000);
    },

    simulerErreur() {
      this.showToast('Erreur 503 - Le service de reservation est temporairement indisponible.');
    },

    searchAgenda() {
      const { agendaCreneauDeb, agendaCreneauFin, agendaDateDeb, agendaDateFin } = this;
      this.agendaResultats = this.reservations.filter(r => {
        if (agendaDateDeb && r.date < agendaDateDeb) return false;
        if (agendaDateFin && r.date > agendaDateFin) return false;
        if (agendaCreneauDeb && r.creneau < agendaCreneauDeb) return false;
        if (agendaCreneauFin && r.creneau > agendaCreneauFin) return false;
        return true;
      });
    },

    formatDate(dateStr) {
      const [y, m, d] = dateStr.split('-');
      return `${d}/${m}/${y}`;
    },

    badgeClass(statut) {
      return statut === 'confirme' ? 'bg-success' : 'bg-warning text-dark';
    },
  },

  mounted() {
    window.addEventListener('keyup', (e) => {
      if (e.key === 'Enter' && this.currentPage === 'login') {
        this.handleLogin();
      }
    });
  },
}).mount('#app');
