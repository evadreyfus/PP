#include "grman/grman.h"
#include <iostream>

#include "graph.h"
///Sous-programme du menu principale

void menu (BITMAP* ecran, BITMAP* bienvenue, BITMAP* bacteries,BITMAP* cycle, BITMAP* savane, BITMAP* quitter, bool fin, Graph *g, bool *ok)
{

    fin = false; // variable de la boucle
    int v = 0 ; //valeur utilisée pour temporisation

    while (!fin && v == 0) //tant qu'on est dans la boucle intérative
    {

        blit(bienvenue,ecran,0,0,0,0,1024,768); //on affiche le menu qui est l'écran principale

        if (mouse_x>=18 && mouse_x<=189 && mouse_y>=268 && mouse_y<=333) //si la souris se trouvent dans cette zone
        {

            blit(bacteries,ecran,0,0,0,0,800,600); //on affiche la bitmap des infos
            if (mouse_b&1)
            {
                g->set_choixGraphe(1);
                g->loadgraphe(g->get_choixGraphe());
                fin = true;

            }
        }

        if (mouse_x>=239 && mouse_x<=566 && mouse_y>=268 && mouse_y<=333)
        {
            blit(cycle,ecran,0,0,0,0,800,600); //on affiche la bitmap des réseaux
            if (mouse_b&1) //si on clique sur le bouton gauche
            {
                g->set_choixGraphe(2);
                g->loadgraphe(g->get_choixGraphe());
                fin = true;

            }
        }

        if (mouse_x>=618 && mouse_x<=758 && mouse_y>=268 && mouse_y<=335)
        {

            blit(savane,ecran,0,0,0,0,800,600); //on affiche la bitmap pour quitter
            if (mouse_b&1)
            {
                g->set_choixGraphe(3);
                g->loadgraphe(g->get_choixGraphe());
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
                v = 2;
            }
        }

        blit(ecran,screen,0,0,0,0,1024,768);

        if(v == 1)
        {
            rest(5000); //temporisation pour pouvoir lire les infos explicatifs des réseaux
            v = 0; //remettre v à zéro nous permet de rester dans le boucle
        }

        if (v == 2)
        {
            rest(2000); //temporisation avant de sotir du programme
            allegro_exit(); //sortie du programme
            exit(EXIT_FAILURE);
        }

        if (mouse_b&2) //On sort de la boucle grace au clique droit
            fin = true ;
    }
    *ok = true;
}


int main()
{

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    BITMAP* ecran = create_bitmap(1600,1200);
    BITMAP* bienvenue = load_bitmap("bienvenue.bmp",NULL);
    BITMAP* bacteries = load_bitmap("bacteries.bmp",NULL);
    BITMAP* cycle = load_bitmap("cycle.bmp",NULL);
    BITMAP* savane = load_bitmap("savane.bmp",NULL);
    BITMAP* quitter = load_bitmap("quitter.bmp",NULL);

    // Initilisation des variables
    bool fin = true;
    bool Run = true;
    bool ok = true;
    Graph g; // Objet Graph

    // boucle générale
    while(Run)
    {
        // Menu
        menu(ecran, bienvenue, bacteries, cycle, savane, quitter, fin, &g,&ok);

        // Boucle de jeu
        while ( (!key[KEY_ESC]) && ok )
        {
            /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
            g.update(&ok,&g);

            /// Mise à jour générale (clavier/souris/buffer etc...)
            grman::mettre_a_jour();
        }

        // "Vider" les vecteurs de l'objet
        g.Erase();
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();
