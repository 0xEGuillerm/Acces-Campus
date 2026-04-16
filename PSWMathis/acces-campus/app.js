// app.js - Acces Campus
// Framework : Vue 3
// API       : json-server

//  ROUTES API endpoints du serveur
const ROUTES = {
  login:        '/psw/login',
  reserverSalle:'/psw/reserver_salle',
  salleCrenaux: '/psw/salle_crenaux',
  planning:     '/psw/planning',
  etatSalle:    '/psw/etat_salle',
  historique:   '/psw/historique_eleve',
  classes:      '/psw/classes',
  badges:       '/psw/badges',
};

const { createApp } = Vue;

createApp({

  //  DATA
  data() {
    return {

      // module Navigation
      currentPage: 'login',
      toastMessage: '',
      toastVisible: false,
      toastType:    'success',
      toastTimer:   null,

      // Module Login
      loginId:      '',
      loginPass:    '',
      loginError:   '',
      loginLoading: false,

      // module Annuaire
      searchAnnuaire: '',
      filtreRole: '',
      filtreClasse: '',
      etudiants: [],
      classes: [],
      annuaireCharge: false,
      selectedUser: null,

      // module Disponibilité
      selectedDispo:        '',
      selectedCreneauDispo: '',
      salles:   ['Salle B201', 'Salle B134'],
      creneaux: [
        '08:00 - 09:00', '09:00 - 10:00',
        '10:00 - 11:00', '11:00 - 12:00',
        '13:00 - 14:00', '14:00 - 15:00',
        '15:00 - 16:00', '16:00 - 17:00',
      ],

      // Module Planning
      selectedPlanning: 'Salle B201',
      planningSemaineOffset: 0,
      heures: ['08:15','09:10','10:20','13:30','13:55','15:35'],
      jours:  ['Lundi','Mardi','Mercredi','Jeudi','Vendredi'],
      joursFR: { 1:'Lundi', 2:'Mardi', 3:'Mercredi', 4:'Jeudi', 5:'Vendredi' },
      planning: {
        'Salle B201': [
          { jour: 'Lundi',    heure: '08:15', titre: 'Cours Reseaux - Mr Jumel', type: 'cours', duree: 1 },
          { jour: 'Mardi',    heure: '10:20', titre: 'TP Reseaux - Mr Jumel',    type: 'tp',    duree: 1 },
          { jour: 'Jeudi',    heure: '13:30', titre: 'Cours Reseaux - Mr Jumel', type: 'cours', duree: 1 },
          { jour: 'Vendredi', heure: '14:25', titre: 'TP Reseaux - Mr Jumel',    type: 'tp',    duree: 1 },
        ],
        'Salle B134': [
          { jour: 'Lundi',    heure: '08:15', titre: 'Informatique & Reseaux ST2IR - Mr Jumel', type: 'cours', duree: 2 },
          { jour: 'Lundi',    heure: '10:20', titre: 'Informatique & Reseaux ST2IR - Mr Jumel', type: 'tp',    duree: 2 },
          { jour: 'Lundi',    heure: '13:30', titre: 'Sc. Techniques Ind. ST1IR - Mr Salmon',   type: 'cours', duree: 2 },
          { jour: 'Lundi',    heure: '15:35', titre: 'Sc. Techniques Ind. ST1IR - Mr Jumel',    type: 'tp',    duree: 2 },
          { jour: 'Mardi',    heure: '09:10', titre: 'Sc. Techniques Ind. ST1IR - Mr Salmon',   type: 'cours', duree: 1 },
          { jour: 'Mardi',    heure: '13:55', titre: 'Informatique & Reseaux ST2IR - Mr Jumel', type: 'tp',    duree: 3 },
          { jour: 'Mercredi', heure: '08:15', titre: 'Informatique & Reseaux ST2IR - Mr Jumel', type: 'cours', duree: 2 },
          { jour: 'Mercredi', heure: '10:20', titre: 'Informatique & Reseaux ST2IR - Mr Jumel', type: 'tp',    duree: 2 },
          { jour: 'Jeudi',    heure: '08:15', titre: 'Informatique & Reseaux ST2IR - Mr Jumel', type: 'cours', duree: 2 },
          { jour: 'Jeudi',    heure: '13:30', titre: 'Sc. Techniques Ind. ST1IR - Mr Salmon',   type: 'cours', duree: 2 },
          { jour: 'Jeudi',    heure: '15:35', titre: 'Sc. Techniques Ind. ST1IR - Mr Salmon',   type: 'tp',    duree: 2 },
          { jour: 'Vendredi', heure: '08:15', titre: 'Informatique & Reseaux ST2IR - Mr Jumel', type: 'cours', duree: 2 },
          { jour: 'Vendredi', heure: '10:20', titre: 'Informatique & Reseaux ST2IR - Mr Jumel', type: 'tp',    duree: 2 },
          { jour: 'Vendredi', heure: '13:30', titre: 'Sc. Techniques Ind. ST1IR - Mr Salmon',   type: 'cours', duree: 2 },
        ],
      },

      // ---------- Module Réservation ----------
      creneaux_api:        [],
      creneauxFiltres:     [],
      creneauxCharge:      false,
      rechercheEffectuee:  false,
      reservationEnCours:  null,
      confirmationVisible: false,
      creneauSelectionne:  null,
      reservationDate:     '',
      reservationHeureDeb: '',
      reservationHeureFin: '',
      erreurRecherche:     '',
      mesReservations:     [],
      maxReservations:     3,
      heuresDispos: [
        '08:00', '08:15', '09:00', '09:10', '10:00', '10:20',
        '11:00', '11:15', '12:00', '13:00', '13:30', '14:00',
        '14:25', '15:00', '15:35', '16:00', '16:30', '17:00',
      ],

      // Module Historique Eleve
      historiqueEleve:      [],
      historiqueCharge:     false,

      // Module Etat Salle
      etatSalleData:        null,
      etatAbsentNoms:       [],
      toutesReservations:   [],
      filtreResaNom:        '',
      filtreResaSalle:      '',
      triReservations:      'recent',
      selectedEtatSalle:    '',
    };
  },



  //  computed calculs automatiques par module

  computed: {

    // module Réservation

    // Filtre les réservations futures de l'utilisateur
    mesReservationsAVenir() {
      const nowTs = Math.floor(Date.now() / 1000);
      return this.mesReservations.filter(r => r.heure_debut >= nowTs);
    },

    // Filtre les réservations passées (historique)
    mesReservationsPassees() {
      const nowTs = Math.floor(Date.now() / 1000);
      return this.mesReservations
        .filter(r => r.heure_debut < nowTs)
        .sort((a, b) => b.heure_debut - a.heure_debut);
    },

    // Module Reservations (toutes)

    // Liste des salles ayant au moins une réservation
    sallesReservees() {
      return [...new Set(this.toutesReservations.map(r => r.salle))].sort();
    },

    // Filtre + tri des réservations
    reservationsTriees() {
      let liste = [...this.toutesReservations];
      const q = this.filtreResaNom.trim().toLowerCase();
      if (q) {
        liste = liste.filter(r => r.nom_complet.toLowerCase().includes(q));
      }
      if (this.filtreResaSalle) {
        liste = liste.filter(r => String(r.salle) === String(this.filtreResaSalle));
      }
      switch (this.triReservations) {
        case 'recent':
          liste.sort((a, b) => b.heure_debut - a.heure_debut);
          break;
        case 'ancien':
          liste.sort((a, b) => a.heure_debut - b.heure_debut);
          break;
        case 'nom_asc':
          liste.sort((a, b) => a.nom_complet.localeCompare(b.nom_complet));
          break;
        case 'nom_desc':
          liste.sort((a, b) => b.nom_complet.localeCompare(a.nom_complet));
          break;
        case 'salle_asc':
          liste.sort((a, b) => String(a.salle).localeCompare(String(b.salle)));
          break;
      }
      return liste;
    },

    // Module Annuaire

    // Filtre la liste des utilisateurs selon recherche, rôle et classe
    filteredStudents() {
      let liste = this.etudiants;
      if (this.filtreRole) {
        liste = liste.filter(s => s.role_user === this.filtreRole);
      }
      if (this.filtreClasse) {
        liste = liste.filter(s => s.id_classe === Number(this.filtreClasse));
      }
      const q = this.searchAnnuaire.trim().toLowerCase();
      if (q) {
        liste = liste.filter(s =>
          s.nom_user.toLowerCase().includes(q) ||
          s.prenom_user.toLowerCase().includes(q) ||
          s.nom_classe.toLowerCase().includes(q) ||
          s.login_user.toLowerCase().includes(q)
        );
      }
      return liste;
    },

    // Module Disponibilité

    // Vérifie si la salle sélectionnée est libre ou occupée
    dispoInfo() {
      if (!this.selectedDispo) return null;
      const events  = this.planning[this.selectedDispo] || [];
      const today   = new Date().getDay();
      const jourAuj = this.joursFR[today] || null;
      let eventsDuJour = jourAuj ? events.filter(e => e.jour === jourAuj) : [];
      if (this.selectedCreneauDispo) {
        const [debCreneau, finCreneau] = this.selectedCreneauDispo.split(' - ');
        eventsDuJour = eventsDuJour.filter(e => e.heure >= debCreneau && e.heure < finCreneau);
      }
      return { libre: eventsDuJour.length === 0, events: eventsDuJour, jour: jourAuj };
    },

    // module Planning

    // Calcule les dates Lundi-Vendredi de la semaine affichée
    semaineDates() {
      const now = new Date();
      const jour = now.getDay(); // 0=dim, 1=lun...
      const diffLundi = (jour === 0 ? -6 : 1 - jour);
      const lundi = new Date(now);
      lundi.setDate(now.getDate() + diffLundi + this.planningSemaineOffset * 7);
      const dates = [];
      for (let i = 0; i < 5; i++) {
        const d = new Date(lundi);
        d.setDate(lundi.getDate() + i);
        dates.push(d);
      }
      return dates;
    },

    // Retourne pour les en-têtes
    joursAvecDates() {
      const noms = ['Lundi', 'Mardi', 'Mercredi', 'Jeudi', 'Vendredi'];
      return this.semaineDates.map((d, i) => {
        const dd = String(d.getDate()).padStart(2, '0');
        const mm = String(d.getMonth() + 1).padStart(2, '0');
        return `${noms[i]} ${dd}/${mm}`;
      });
    },

    // Label de la semaine affichée (ex: "Semaine du 06/04/2026")
    labelSemaine() {
      const d = this.semaineDates[0];
      if (!d) return '';
      const dd = String(d.getDate()).padStart(2, '0');
      const mm = String(d.getMonth() + 1).padStart(2, '0');
      const yy = d.getFullYear();
      return `Semaine du ${dd}/${mm}/${yy}`;
    },

    // Construit les lignes du tableau à partir des cours de la salle
    planningRows() {
      const events = this.planning[this.selectedPlanning] || [];
      const heuresTrie = [...new Set(events.map(e => e.heure))].sort();
      return heuresTrie.map(heure => ({
        heure,
        jours: this.jours.map(jour =>
          events.find(e => e.jour === jour && e.heure === heure) || null
        ),
      }));
    },
  },


  //  methods fonctions organisées par module

  methods: {

  
    //  Module Navigation
  

    // Change la page affichée
    goTo(page) {
      this.currentPage = page;
      window.scrollTo(0, 0);
      const user = JSON.parse(localStorage.getItem('user_connecte') || '{}');
      if (page === 'reservation') {
        this.rechercherCreneaux(this.reservationDate, this.reservationHeureDeb, this.reservationHeureFin);
        this.chargerMesReservations(user.id);
        this.chargerToutesReservations();
      }
      if (page === 'annuaire' && !this.annuaireCharge) {
        this.chargerAnnuaire();
        this.selectedUser = null;
      }
    },

    // Affiche une notification temporaire (4 secondes)
    showToast(message, type = 'success') {
      this.toastMessage = message;
      this.toastType    = type;
      this.toastVisible = true;
      if (this.toastTimer) clearTimeout(this.toastTimer);
      this.toastTimer = setTimeout(() => { this.toastVisible = false; }, 4000);
    },

    // Formate "2026-03-31" → "31/03/2026"
    formaterDate(dateStr) {
      if (!dateStr) return '';
      const [y, m, d] = dateStr.split('T')[0].split('-');
      return `${d}/${m}/${y}`;
    },

    // Formate "09:00" → "09h00"  (ou ISO → heure locale)
    formaterHeure(heureStr) {
      if (!heureStr) return '';
      if (heureStr.includes('T')) {
        const d = new Date(heureStr);
        return `${String(d.getHours()).padStart(2,'0')}h${String(d.getMinutes()).padStart(2,'0')}`;
      }
      return heureStr.replace(':', 'h');
    },

    // Formate "31/03/2026 a 09h00"
    formaterHoraire(dateStr, heureStr) {
      return `${this.formaterDate(dateStr)} a ${this.formaterHeure(heureStr)}`;
    },

  
    //  module Login
  
    // Authentification : GET /psw/login → filtre côté client
    async handleLogin(login_user, hash_mdp) {
      if (!login_user.trim() || !hash_mdp.trim()) {
        this.loginError = 'Veuillez remplir tous les champs.';
        return;
      }
      this.loginError   = '';
      this.loginLoading = true;

      try {
        const reponse      = await fetch(ROUTES.login);
        const utilisateurs = await reponse.json();
        const user = utilisateurs.find(
          u => u.login_user === login_user.trim() && u.hash_mdp === hash_mdp
        );

        if (!user) {
          this.loginError = 'Identifiant ou mot de passe incorrect.';
          return;
        }

        localStorage.setItem('user_connecte', JSON.stringify(user));
        this.goTo('dashboard');

      } catch (err) {
        this.loginError = 'Impossible de joindre le serveur. Veuillez reessayer.';
        console.error('Erreur login :', err);
      } finally {
        this.loginLoading = false;
      }
    },

    // Déconnexion : efface la session et retourne au login
    logout() {
      localStorage.removeItem('user_connecte');
      this.loginId    = '';
      this.loginPass  = '';
      this.loginError = '';
      this.goTo('login');
    },

    //  Module Réservation

    // Recherche les créneaux disponibles : GET /psw/salle_crenaux → filtre côté client
    async rechercherCreneaux(date, heure_debut, heure_fin) {
      if (!date || !heure_debut || !heure_fin) {
        this.erreurRecherche = 'Veuillez remplir la date et les deux heures.';
        return;
      }
      const aujourd_hui = new Date().toISOString().split('T')[0];
      if (date < aujourd_hui) {
        this.erreurRecherche = 'Impossible de reserver une date passee.';
        return;
      }
      if (heure_debut >= heure_fin) {
        this.erreurRecherche = "L'heure de fin doit etre apres l'heure de debut.";
        return;
      }

      this.erreurRecherche    = '';
      this.creneauxCharge     = true;
      this.rechercheEffectuee = false;

      try {
        const [repCreneaux, repReservations] = await Promise.all([
          fetch(ROUTES.salleCrenaux),
          fetch(ROUTES.reserverSalle),
        ]);
        this.creneaux_api    = await repCreneaux.json();
        const reservations   = await repReservations.json();

        // Construire les timestamps de début et fin de recherche
        const tsDebRecherche = Math.floor(new Date(`${date}T${heure_debut}:00`).getTime() / 1000);
        const tsFinRecherche = Math.floor(new Date(`${date}T${heure_fin}:00`).getTime() / 1000);

        this.creneauxFiltres = this.creneaux_api.filter(c => {
          // Le créneau doit être dans la plage recherchée
          if (c.heure_debut < tsDebRecherche || c.heure_debut >= tsFinRecherche) return false;

          // Exclure les créneaux déjà réservés (chevauchement)
          const dejaReserve = reservations.some(r =>
            String(r.salle) === String(c.salle) &&
            r.heure_debut < c.heure_fin &&
            r.heure_fin > c.heure_debut
          );
          return !dejaReserve;
        });

        this.rechercheEffectuee = true;

      } catch (err) {
        this.erreurRecherche = 'Impossible de contacter le serveur.';
        console.error('Erreur créneaux :', err);
      } finally {
        this.creneauxCharge = false;
      }
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

    // Raccourci appelé par le bouton Confirmer de la modale (max 3 reservations)
    validerReservation() {
      if (this.mesReservationsAVenir.length >= this.maxReservations) {
        this.confirmationVisible = false;
        this.showToast(`Impossible : vous avez deja ${this.maxReservations} reservations en cours.`, 'danger');
        return;
      }
      const user = JSON.parse(localStorage.getItem('user_connecte') || '{}');
      const c = this.creneauSelectionne;
      // Construire timestamps à partir de la date + heures sélectionnées
      const tsDebut = Math.floor(new Date(`${this.reservationDate}T${this.reservationHeureDeb}:00`).getTime() / 1000);
      const tsFin   = Math.floor(new Date(`${this.reservationDate}T${this.reservationHeureFin}:00`).getTime() / 1000);
      this.confirmerReservation(user.id, c ? c.salle : '', tsDebut, tsFin);
    },

    // Confirme la réservation : POST /psw/reserver_salle (format Swagger : timestamps)
    async confirmerReservation(utilisateur, salle, heure_debut, heure_fin) {
      this.confirmationVisible = false;
      this.reservationEnCours  = this.creneauSelectionne ? this.creneauSelectionne.id : null;

      try {
        const reponse = await fetch(ROUTES.reserverSalle, {
          method:  'POST',
          headers: { 'Content-Type': 'application/json' },
          body:    JSON.stringify({
            utilisateur: utilisateur,
            salle:       salle,
            heure_debut: heure_debut,
            heure_fin:   heure_fin,
          }),
        });

        if (!reponse.ok) throw new Error(`Erreur ${reponse.status}`);

        this.showToast(
          `Salle ${salle} reservee le ${this.formaterTimestamp(heure_debut)} !`,
          'success'
        );
        if (this.creneauSelectionne) {
          this.creneaux_api    = this.creneaux_api.filter(c => c.id !== this.creneauSelectionne.id);
          this.creneauxFiltres = this.creneauxFiltres.filter(c => c.id !== this.creneauSelectionne.id);
        }
        await this.chargerMesReservations(utilisateur);
        await this.chargerToutesReservations();

      } catch (err) {
        this.showToast('La reservation a echoue. Reessaie.', 'danger');
        console.error('Erreur reservation :', err);
      } finally {
        this.reservationEnCours = null;
        this.creneauSelectionne = null;
      }
    },

    // Charge les réservations de l'utilisateur : GET /psw/reserver_salle → filtre par id
    async chargerMesReservations(userId) {
      try {
        const reponse = await fetch(ROUTES.reserverSalle);
        const toutes  = await reponse.json();
        this.mesReservations = toutes.filter(r => r.utilisateur === userId);
      } catch (err) {
        console.error('Erreur chargement reservations :', err);
      }
    },

    // Charge toutes les réservations avec les noms : GET /psw/reserver_salle + /psw/login
    async chargerToutesReservations() {
      try {
        const [repResa, repUsers] = await Promise.all([
          fetch(ROUTES.reserverSalle),
          fetch(ROUTES.login),
        ]);
        const reservations = await repResa.json();
        const utilisateurs = await repUsers.json();
        this.toutesReservations = reservations.map(r => {
          const u = utilisateurs.find(u => u.id === r.utilisateur);
          return { ...r, nom_complet: u ? `${u.prenom_user} ${u.nom_user}` : 'Inconnu' };
        });
      } catch (err) {
        console.error('Erreur chargement reservations :', err);
      }
    },

    // Annule une réservation : DELETE /psw/reserver_salle/:id (si > 24h avant)
    async annulerReservation(id, heure_debut_ts) {
      const nowTs = Math.floor(Date.now() / 1000);
      if ((heure_debut_ts - nowTs) < 24 * 3600) {
        this.showToast("Impossible d'annuler moins de 24h avant.", 'danger');
        return;
      }
      try {
        const reponse = await fetch(`${ROUTES.reserverSalle}/${id}`, { method: 'DELETE' });
        if (!reponse.ok) throw new Error(`Erreur ${reponse.status}`);
        this.mesReservations = this.mesReservations.filter(r => r.id !== id);
        this.toutesReservations = this.toutesReservations.filter(r => r.id !== id);
        this.showToast('Reservation annulee.', 'success');
      } catch (err) {
        this.showToast("Erreur lors de l'annulation.", 'danger');
        console.error('Erreur annulation :', err);
      }
    },

    //  Module Annuaire

    // Ouvre le profil d'un utilisateur et charge son historique
    async voirProfil(user) {
      this.selectedUser = user;
      this.historiqueEleve = [];
      this.historiqueCharge = false;
      await this.chargerHistorique(user.id);
    },

    // Ferme le profil
    fermerProfil() {
      this.selectedUser = null;
    },

    // Charge utilisateurs + classes depuis l'API et les associe
    async chargerAnnuaire() {
      try {
        const [repUsers, repClasses] = await Promise.all([
          fetch(ROUTES.login),
          fetch(ROUTES.classes),
        ]);
        const utilisateurs = await repUsers.json();
        this.classes       = await repClasses.json();

        // Associer le nom_classe à chaque utilisateur
        this.etudiants = utilisateurs.map(u => ({
          ...u,
          nom_classe: (this.classes.find(c => c.id === u.id_classe) || {}).nom_classe || '—',
        }));
        this.annuaireCharge = true;
      } catch (err) {
        this.showToast('Impossible de charger l\'annuaire.', 'danger');
        console.error('Erreur annuaire :', err);
      }
    },


    //  module Historique Eleve

    // Charge l'historique absences/retards : GET /psw/historique_eleve
    async chargerHistorique(userId) {
      try {
        const reponse = await fetch(ROUTES.historique);
        const tous    = await reponse.json();
        this.historiqueEleve = tous
          .filter(h => h.utilisateur === userId)
          .sort((a, b) => b.timestamp_debut_cours - a.timestamp_debut_cours);
        this.historiqueCharge = true;
      } catch (err) {
        console.error('Erreur historique :', err);
      }
    },

    // Formate un timestamp unix en date lisible complète
    formaterTimestamp(ts) {
      if (!ts) return '';
      const d = new Date(ts * 1000);
      const dd = String(d.getDate()).padStart(2, '0');
      const mm = String(d.getMonth() + 1).padStart(2, '0');
      const yy = d.getFullYear();
      const hh = String(d.getHours()).padStart(2, '0');
      const mi = String(d.getMinutes()).padStart(2, '0');
      return `${dd}/${mm}/${yy} a ${hh}h${mi}`;
    },

    // Formate un timestamp unix → "dd/mm/yyyy" (date seule)
    tsDate(ts) {
      if (!ts) return '';
      const d = new Date(ts * 1000);
      const dd = String(d.getDate()).padStart(2, '0');
      const mm = String(d.getMonth() + 1).padStart(2, '0');
      return `${dd}/${mm}/${d.getFullYear()}`;
    },

    // Formate un timestamp unix → "HHhMM" (heure seule)
    tsHeure(ts) {
      if (!ts) return '';
      const d = new Date(ts * 1000);
      return `${String(d.getHours()).padStart(2,'0')}h${String(d.getMinutes()).padStart(2,'0')}`;
    },


   //  module Etat Salle
    
    // Charge l'état d'une salle + noms des absents : GET /psw/etat_salle + /psw/login
    async chargerEtatSalle(num_salle) {
      this.etatSalleData = null;
      this.etatAbsentNoms = [];
      if (!num_salle) return;
      try {
        const [repEtat, repUsers] = await Promise.all([
          fetch(ROUTES.etatSalle),
          fetch(ROUTES.login),
        ]);
        const tous         = await repEtat.json();
        const utilisateurs = await repUsers.json();
        this.etatSalleData = tous.find(e => String(e.num_salle) === String(num_salle)) || null;
        if (this.etatSalleData && this.etatSalleData.listeAbsent) {
          this.etatAbsentNoms = this.etatSalleData.listeAbsent.map(id => {
            const u = utilisateurs.find(u => u.id === id);
            return u ? `${u.prenom_user} ${u.nom_user}` : `ID ${id}`;
          });
        }
      } catch (err) {
        console.error('Erreur etat salle :', err);
      }
    },

        //  module Planning  navigation semaine

    // Passe à la semaine précédente
    semainePrecedente() {
      this.planningSemaineOffset--;
    },

    // Passe à la semaine suivante
    semaineSuivante() {
      this.planningSemaineOffset++;
    },

    // Retourne à la semaine courante
    semaineActuelle() {
      this.planningSemaineOffset = 0;
    },
  },


  //   réactions aux changements de données

  watch: {
    selectedDispo(val) {
      const num = val ? val.replace('Salle ', '') : '';
      this.chargerEtatSalle(num);
    },
  },

  // Exécuté une seule fois au chargement
  mounted() {
    const session = localStorage.getItem('user_connecte');
    if (session) this.goTo('dashboard');

    window.addEventListener('keyup', (e) => {
      if (e.key === 'Enter' && this.currentPage === 'login') this.handleLogin(this.loginId, this.loginPass);
    });
  },

}).mount('#app');
