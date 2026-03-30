using System;
using System.Drawing;
using System.Windows.Forms;
using AccesCampus.Services;
using AccesCampus.Models; 

namespace AccesCampusPGS;

public partial class Form1 : Form

{

    private GestionBadgesService _badgesService; //Service qui gèrent le JSON des badges

    private DataGridView _gridBadges;
    private Button _btnActualiserBadges;
    private Button _btnAjouterBadge;
    private Button _btnSupprimerBadge;
    private Label _lblTitre;

    public Form1()
    {
        _badgesService = new GestionBadgesService();// Initialisation du service de gestion des badges
        ConfigurerFenetre();// Configuration de la fenêtre
        InitialiserComposants();

        ActualiserGrille(); //Chargement des badges au lancement de l'application
    }
    private void ConfigurerFenetre()
    {
        this.Text="PGS-Accès Campus SALS";
        this.Size=new Size(800, 600);
        this.StartPosition=FormStartPosition.CenterScreen;
        this.BlackColor=Color.White;

    }
    private void InitialiserComposants()
    {
        _lblTitre = new Label
        {
         Text="Gestion des Badges";
         Font= new Font("Segoe UI", 16, FontStyle.Bold);
         Location=new Point(20, 20);
         Autosize=true;
       }
       this.Controls.Add(_lblTitre);
        {
            
      
        }
}
