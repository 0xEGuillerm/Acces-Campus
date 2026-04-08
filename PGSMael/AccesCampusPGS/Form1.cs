using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic; // Pour utiliser List<>
using System.ComponentModel;    // Pour BindingList<>
using AccesCampusPGS.Models;

namespace AccesCampusPGS
{
    public partial class Form1 : Form
    {
        private GestionBadge moduleBadge;
        
        // Éléments de saisie
        private TextBox txtNom;
        private TextBox txtPrenom;
        private Button btnCreerBadge;
        
        // Éléments du tableau et suppression
        private DataGridView dgvBadges;
        private Button btnSupprimerBadge;
        
        // La liste magique qui met à jour le tableau automatiquement
        private BindingList<BadgeInfo> listeBadgesUI;

        public Form1()
        {
            InitializeComponent();
            moduleBadge = new GestionBadge();
            listeBadgesUI = new BindingList<BadgeInfo>();
            DessinerInterface();
        }

        private void DessinerInterface()
        {
            this.Text = "Gestionnaire de Badges PGS";
            this.Size = new Size(800, 500); // On agrandit la fenêtre
            this.StartPosition = FormStartPosition.CenterScreen;

            // --- Zone de saisie (à gauche) ---
            Label lblNom = new Label() { Text = "Nom :", Location = new Point(20, 20), Width = 150 };
            txtNom = new TextBox() { Location = new Point(20, 40), Width = 200 };

            Label lblPrenom = new Label() { Text = "Prénom :", Location = new Point(20, 80), Width = 150 };
            txtPrenom = new TextBox() { Location = new Point(20, 100), Width = 200 };

            btnCreerBadge = new Button() { 
                Text = "Ajouter le Badge", 
                Location = new Point(20, 150), 
                Width = 200, Height = 40, 
                BackColor = Color.LightBlue 
            };
            btnCreerBadge.Click += btnCreerBadge_Click;

            // --- Zone du Tableau (à droite) ---
            dgvBadges = new DataGridView();
            dgvBadges.Location = new Point(250, 20);
            dgvBadges.Size = new Size(510, 350);
            dgvBadges.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            dgvBadges.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            dgvBadges.MultiSelect = false;
            dgvBadges.ReadOnly = true;
            
            // On lie la liste au tableau
            dgvBadges.DataSource = listeBadgesUI;

            // --- Bouton Supprimer (sous le tableau) ---
            btnSupprimerBadge = new Button() { 
                Text = "Supprimer le badge sélectionné", 
                Location = new Point(250, 380), 
                Width = 510, Height = 40, 
                BackColor = Color.LightCoral 
            };
            btnSupprimerBadge.Click += btnSupprimerBadge_Click;

            // Ajout des contrôles
            this.Controls.Add(lblNom);
            this.Controls.Add(txtNom);
            this.Controls.Add(lblPrenom);
            this.Controls.Add(txtPrenom);
            this.Controls.Add(btnCreerBadge);
            this.Controls.Add(dgvBadges);
            this.Controls.Add(btnSupprimerBadge);
        }

        private void btnCreerBadge_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrWhiteSpace(txtNom.Text) || string.IsNullOrWhiteSpace(txtPrenom.Text)) return;

            // On appelle ton module
            string resultat = moduleBadge.ModuleCreationBadge(txtNom.Text, txtPrenom.Text);
            
            if (resultat.Contains("[CONFIRMATION]"))
            {
                // On ajoute visuellement dans le tableau
                // Note : En réalité on récupérerait l'UUID du module
                listeBadgesUI.Add(new BadgeInfo { 
                    Nom = txtNom.Text, 
                    Prenom = txtPrenom.Text, 
                    UUID = "UUID-9876", 
                    DateCreation = DateTime.Now.ToShortDateString() 
                });
                
                txtNom.Clear();
                txtPrenom.Clear();
            }
            MessageBox.Show(resultat);
        }

        private void btnSupprimerBadge_Click(object sender, EventArgs e)
        {
            if (dgvBadges.SelectedRows.Count > 0)
            {
                var badge = (BadgeInfo)dgvBadges.SelectedRows[0].DataBoundItem;
                
                DialogResult dr = MessageBox.Show($"Supprimer le badge de {badge.Nom} ?", "Confirmation", MessageBoxButtons.YesNo);
                if (dr == DialogResult.Yes)
                {
                    // Logique de suppression
                    string message = moduleBadge.ModuleSupprimerBadge(badge.UUID);
                    listeBadgesUI.Remove(badge);
                    MessageBox.Show(message);
                }
            }
        }
    }

    // Petite classe pour définir les colonnes du tableau
    public class BadgeInfo
    {
        public string Nom { get; set; }
        public string Prenom { get; set; }
        public string UUID { get; set; }
        public string DateCreation { get; set; }
    }
}