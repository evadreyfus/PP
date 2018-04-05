#include "grman/grman.h"
#include <iostream>

#include "graph.h"
///Sous-programme du menu principale

void menu (BITMAP* ecran, BITMAP* bienvenue, BITMAP* bacteries,BITMAP* cycle, BITMAP* savane, BITMAP* quitter, int fin, Graph *g)
{
    int m_choixgraphe;
    fin = 0; // variable de la boucle
    int v = 0 ; //valeur utilisée pour temporisation
    while (!fin && v==0) //tant qu'on est dans la boucle intérative
    {

        blit(bienvenue,ecran,0,0,0,0,800,600); //on affiche le menu qui est l'écran principale

        if (mouse_x>=18 && mouse_x<=189 && mouse_y>=268 && mouse_y<=333) //si la souris se trouvent dans cette zone
        {

            blit(bacteries,ecran,0,0,0,0,800,600); //on affiche la bitmap des infos
            if (mouse_b&1)
            {
                m_choixgraphe=1;
                g->loadgraphe(m_choixgraphe);
                fin = true;

            }
        }

        if (mouse_x>=239 && mouse_x<=566 && mouse_y>=268 && mouse_y<=333)
        {
            blit(cycle,ecran,0,0,0,0,800,600); //on affiche la bitmap des réseaux
            if (mouse_b&1) //si on clique sur le bouton gauche
            {
                m_choixgraphe=2;
                g->loadgraphe(m_choixgraphe);
                fin = true;

            }
        }

        if (mouse_x>=618 && mouse_x<=758 && mouse_y>=268 && mouse_y<=335)
        {

        blit(savane,ecran,0,0,0,0,800,600); //on affiche la bitmap pour quitter
           if (mouse_b&1)
           {m_choixgraphe=1;
                g->loadgraphe(m_choixgraphe);
                fin = true;

        }
        }
        if (mouse_x>=315 && mouse_x<=458 && mouse_y>=473 && mouse_y<=534)
        {
            blit(quitter,ecran,0,0,0,0,800,600); //on affiche la bitmap pour quitter
            if(mouse_b&1)
            {
                BITMAP* bye=load_bitmap("bye.bmp",NULL); //Chargement bitmap pour affichage des informations
                blit(bye,ecran,0,0,0,0,800,600); //Affichage du bitmap explicatif de nos réseaux
                v=2;
            }
        }

         blit(ecran,screen,0,0,0,0,800,600);
        //clear(ecran);

        if(v==1)
        {
            rest(5000); //temporisation pour pouvoir lire les infos explicatifs des réseaux
            v=0; //remettre v à zéro nous permet de rester dans le boucle
        }

        if (v==2)
        {
            rest(2000); //temporisation avant de sotir du programme
            allegro_exit(); //sortie du programme
            exit(EXIT_FAILURE);
        }

        if (mouse_b&2) //On sort de la boucle grace au clique droit
            fin = 1 ;
    }
}

/*///Sous-programme relatifs au fichier
void writeFile(const std::string& path,std::vector<std::string>::iterator it)
{
    // Ouverture fichier en mode "w"
    // pour le mode append, utiliser std::ios::app
    std::ofstream ofs(path.c_str(), std::ios::app);
    if (ofs)
    {
        // Ecriture dans le fichier
        ofs << *it << std::endl;
        ofs.close(); // fermeture du flux
    }
}

void Lirefichier(const std::string& path)
{
    std::ifstream ifs(path.c_str());
    if (ifs) // test si le fichier est bien ouvert
    {
        std::string line;
        while (std::getline(ifs, line)) // lecture ligne par ligne
        {

            std::cout << line << std::endl;
        }
        std::cout << "Reading " << path << " => OK" << std::endl;
        ifs.close(); // fermeture du flux
    }
    else // en cas d'erreur...
    {
        std::cout << "Cannot read " << path << std::endl;
    }
}
*/
int main()
{

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

     /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    BITMAP* ecran = create_bitmap(1600,1200);
    BITMAP* bienvenue = load_bitmap("bienvenue.bmp",NULL);
    BITMAP* bacteries=load_bitmap("bacteries.bmp",NULL);
    BITMAP* cycle=load_bitmap("cycle.bmp",NULL);
    BITMAP* savane=load_bitmap("savane.bmp",NULL);
    BITMAP* quitter=load_bitmap("quitter.bmp",NULL);
int fin;
//menu(ecran,bienvenue,bacteries,cycle,savane,quitter,fin);






    /// Un exemple de graphe
    Graph g;

    menu(ecran, bienvenue, bacteries, cycle, savane, quitter, fin, &g);

    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();

