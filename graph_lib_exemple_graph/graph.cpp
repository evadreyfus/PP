#include "graph.h"
#include <fstream>
#include <string>
using namespace std;
/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 130);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }
    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);
    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );


}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}




/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}

/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    //declaration de la box contenant le bouton delete
    m_tool_box.add_child(m_delete_box);
    m_delete_box.set_pos(-10,100);
    m_delete_box.set_dim(80,80);
    m_delete.set_dim(40,15);
    m_delete_image.set_pic_name("delete.jpg");
    m_delete.add_child(m_delete_image);
    m_delete_box.add_child(m_delete);

    //Déclaration de la box contenant le boutin sauvegarde
    m_tool_box.add_child(m_sauv_box);
    m_sauv_box.set_pos(-10,10);
    m_sauv_box.set_dim(80,80);
    m_sauv.set_dim(40,15);
    m_sauv_image.set_pic_name("save.jpg");
    m_sauv.add_child(m_sauv_image);
    m_sauv_box.add_child(m_sauv);

    //Déclaration de la box contenant le boutin add
    m_tool_box.add_child(m_add_box);
    m_add_box.set_pos(-10,200);
    m_add_box.set_dim(80,80);
    m_add.set_dim(40,15);
    m_add_image.set_pic_name("add.jpg");
    m_add.add_child(m_add_image);
    m_add_box.add_child(m_add);

    //Déclaration de la box contenant le boutin return
    m_tool_box.add_child(m_return_box);
    m_return_box.set_pos(-10,600);
    m_return_box.set_dim(100,100);
    m_return.set_dim(40,15);
    m_return_image.set_pic_name("return.jpg");
    m_return.add_child(m_return_image);
    m_return_box.add_child(m_return);

    //Déclaration de la box contenant le boutin CONNEXE
    m_tool_box.add_child(m_connexe_box);
    m_connexe_box.set_pos(-10,300);
    m_connexe_box.set_dim(80,80);
    m_connexe.set_dim(40,15);
    m_connexe_image.set_pic_name("connexe.jpg");
    m_connexe.add_child(m_connexe_image);
    m_connexe_box.add_child(m_connexe);

    //Déclaration de la box contenant le boutin K-CONNEXE
    m_tool_box.add_child(m_kconnex_box);
    m_kconnex_box.set_pos(-10,400);
    m_kconnex_box.set_dim(80,80);
    m_kconnex.set_dim(40,15);
    m_kconnex_image.set_pic_name("kconnex.jpg");
    m_kconnex.add_child(m_kconnex_image);
    m_kconnex_box.add_child(m_kconnex);

    //Déclaration de la box contenant le bouton Simulation
    m_tool_box.add_child(m_simulation_box);
    m_simulation_box.set_pos(-10,500);
    m_simulation_box.set_dim(80,80);
    m_simulation.set_dim(40,15);
    m_simulation_image.set_pic_name("simulation.jpg");
    m_simulation.add_child(m_simulation_image);
    m_simulation_box.add_child(m_simulation);
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.

///________________________________________________________
///        CREATION DU GRAPHE ET ARCS ENTRE SOMMETS
///________________________________________________________

void Graph::loadgraphe(int m_choixgraphe)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    int m_sommet; ///nombre de sommet
    int m_poids; ///poids de l'arete
    int m_pos1;
    int m_pos2;
    string m_nom;
    string image;
    string fichier1, fichier2;
    int m_edge;
    int m_s1;
    int m_s2;
    int m_pe;
    int nbSommets, nbEdges;
    int signe;

    switch(m_choixgraphe)
    {
    case 1:
    {

        fichier1 = "sommet1.txt"; ///fichier sommet 1
        fichier2 = "edge1.txt"; ///fichier edge 1
        ifstream newfichier(fichier1, ios::in); ///lecture du fichier
        if(newfichier)
        {
            //on cherche ici a recuperer les donnees ordre et nbre d'aretes du fichier
            newfichier >> nbSommets; /// le fichier lit la premiere valeur
            std::cout << nbSommets << std::endl; ///affichage du nombre de sommets

            for(int i=0; i<nbSommets; i++) ///parcourt de chaque sommet du graphe
            {
                newfichier >>  m_sommet >> m_poids >> m_pos1 >> m_pos2 >> m_nom;
                std::cout << " " << m_sommet << " " << m_poids << " " << m_pos1 << " " << m_pos2 << " " << m_nom;
                image = m_nom+".jpg";
///creation du sommet dans l'interface allegro
                add_interfaced_vertex(m_sommet, m_poids, m_pos1, m_pos2, image);
            }
            newfichier.close();
        }
        else
            std::cout << "Erreur lors du chargement du fichier..." << std::endl;
///meme chose pour les aretes
        ifstream nouveaufichier(fichier2, ios :: in);
        if(nouveaufichier)
        {
            nouveaufichier >> nbEdges;

            for (int i=0; i<nbEdges; i++)
            {
                nouveaufichier >> m_edge >> m_s1 >> m_s2 >> m_pe >> signe;
                add_interfaced_edge(m_edge, m_s1, m_s2, signe, m_pe);
            }

            nouveaufichier.close();
        }
        else
            std::cout << "Erreur lors du chargement du fichier..." << std::endl;

        break;
    }
    case 2:
    {

        fichier1 = "sommet2.txt";
        fichier2 = "edge2.txt";
        ifstream newfichier(fichier1, ios::in);
        if(newfichier)
        {
            newfichier >> nbSommets;
            std::cout << nbSommets << std::endl;

            for(int i=0; i<nbSommets; i++)
            {
                newfichier >>  m_sommet >> m_poids >> m_pos1 >> m_pos2 >> m_nom;
                std::cout << " " << m_sommet << " " << m_poids << " " << m_pos1 << " " << m_pos2 << " " << m_nom;
                image = m_nom+".jpg";
///creation du sommet dans l'interface allegro
                add_interfaced_vertex(m_sommet, m_poids, m_pos1,m_pos2, image);
            }
            newfichier.close();
        }
        else
            std::cout << "Erreur lors du chargement du fichier..." << std::endl;

        ifstream nouveaufichier(fichier2, ios :: in);
        if(nouveaufichier)
        {
            nouveaufichier >> nbEdges;

            for (int i=0; i<nbEdges; i++)
            {
                nouveaufichier >> m_edge >> m_s1 >> m_s2 >> m_pe >> signe;
                add_interfaced_edge(m_edge, m_s1, m_s2, signe, m_pe);
            }
            nouveaufichier.close();
        }
        else
            std::cout << "Erreur lors du chargement du fichier..." << std::endl;


        break;
    }

    case 3:
    {
        fichier1 = "sommet3.txt";
        fichier2 = "edge3.txt";

        ifstream newfichier(fichier1, ios::in);

        if(newfichier)
        {
            newfichier >> nbSommets;
            std::cout << nbSommets << std::endl;

            for(int i=0; i<nbSommets; i++)
            {
                newfichier >>  m_sommet >> m_poids >> m_pos1 >> m_pos2 >> m_nom;
                std::cout << " " << m_sommet << " " << m_poids << " " << m_pos1 << " " << m_pos2 << " " << m_nom;
                image = m_nom+".jpg";
///creation du sommet dans l'interface allegro

                add_interfaced_vertex(m_sommet, m_poids, m_pos1,m_pos2, image);
            }
            newfichier.close();
        }
        else
            std::cout << "Erreur lors du chargement du fichier..." << std::endl;

        ifstream nouveaufichier(fichier2, ios :: in);
        if(nouveaufichier)
        {
            nouveaufichier >> nbEdges;

            for (int i=0; i<nbEdges; i++)
            {
                nouveaufichier >> m_edge >> m_s1 >> m_s2 >> m_pe >> signe;
                add_interfaced_edge(m_edge, m_s1, m_s2, signe, m_pe);
            }
            nouveaufichier.close();
        }
        else
            std::cout << "Erreur lors du chargement du fichier..." << std::endl;

        break;
    }
    default:
    {
        break;
    }

    }

}
/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update(bool *ok, Graph *g)
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();
///Si le bouton "sauvegarder" est cliqué
    if(m_interface->m_sauv.clicked())
    {
        Sauvegarde();

        for (auto &elt : m_vertices)
            elt.second.pre_update();

        for (auto &elt : m_edges)
            elt.second.pre_update();

    }
///Si le bouton "supprimer" est cliqué

    if(m_interface->m_delete.clicked())
    {
        Supprimer();

        for (auto &elt : m_vertices)
            elt.second.pre_update();

        for (auto &elt : m_edges)
            elt.second.pre_update();
    }
///Si le bouton "ajouter" est cliqué

    if(m_interface->m_add.clicked())
    {
        Ajouter();
        for (auto &elt : m_vertices)
            elt.second.pre_update();

        for (auto &elt : m_edges)
            elt.second.pre_update();
    }
///Si le bouton "retour au menu" est cliqué

    if(m_interface->m_return.clicked())
    {
        *ok = false;
        for (auto &elt : m_vertices)
            elt.second.pre_update();

        for (auto &elt : m_edges)
            elt.second.pre_update();
    }
///Si le bouton "connexe" est cliqué

    if(m_interface->m_connexe.clicked())
    {

        Fortementconnexe(g);
        for (auto &elt : m_vertices)
            elt.second.pre_update();

        for (auto &elt : m_edges)
            elt.second.pre_update();
    }
///Si le bouton "k-connexe" est cliqué

    if(m_interface->m_kconnex.clicked())
    {
        Is_Connexe();
        for (auto &elt : m_vertices)
            elt.second.pre_update();

        for (auto &elt : m_edges)
            elt.second.pre_update();
    }
///Si le bouton "simulation" est cliqué

    if(m_interface->m_simulation.clicked())
    {

        testpopulation();
        for (auto &elt : m_vertices)
            elt.second.pre_update();

        for (auto &elt : m_edges)
            elt.second.pre_update();

    }

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();


}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx) != m_vertices.end())
    {
        std::cerr << "Error adding vertex at idx = " << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    /// Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    /// Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    /// On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, int signe, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;
    m_vertices[id_vert1].m_out.push_back(id_vert2);
    m_vertices[id_vert2].m_in.push_back(id_vert1);
///COLORATION DES ARETES
    ///si 0 dans le fichier alors couleur de l'arete rouge
    if (signe == 0)
    {
        ei->m_top_edge.m_color = ROUGECLAIR;
        m_edges[idx].m_signe = 0;
    }
    ///si 1 dans le fichier alors couleur de l'arete verte

    else if(signe == 1)
    {
        ei->m_top_edge.m_color = VERTCLAIR;
        m_edges[idx].m_signe = 1;
    }
    ///si 2 dans le fichier alors couleur de l'arete grise

    else if(signe == 2)
    {
        ei->m_top_edge.m_color = GRISSOMBRE;
        m_edges[idx].m_signe = 2;
    }
}


/// Enregistrer les sommets dans le fichier
void Graph::save_vertex()
{
    std::string fichier1;

    if(m_choixGraphe == 1) ///Si le graphe correspond au graphe 1 ouverture du fichier du meme nom
        fichier1 = "sommet1.txt";

    if(m_choixGraphe == 2) ///meme chose
        fichier1 = "sommet2.txt";

    if(m_choixGraphe == 3)
        fichier1 = "sommet3.txt";

    std::ofstream newfichier(fichier1, std::ios::out | std::ios::trunc); ///ecriture dans le fichier (remplacement des donnees)

    if(newfichier)
    {
        newfichier << m_vertices.size() << std::endl; ///lecture du nombre de sommets total dans le fichier

        for(unsigned int i=0 ; i < m_vertices.size() ; i++)
        {

            std::string name = m_vertices[i].m_interface->m_img.get_pic_name();  ///nom a sauvegarder correspond au nom attribue au sommet
            name.erase(name.end()-4, name.end()); ///suppression du .jpg
            std::cout << m_vertices[i].m_value << " " << m_vertices[i].m_interface->m_top_box.get_posx() << " " <<  m_vertices[i].m_interface->m_top_box.get_posy() << " " << name << std::endl; ///enregistrement des caracteristiques de chaque sommet

            newfichier << i <<" " << m_vertices[i].m_value << " " << m_vertices[i].m_interface->m_top_box.get_posx() << " " <<  m_vertices[i].m_interface->m_top_box.get_posy() << " " << name << std::endl;
        }
        newfichier.close();
    }
    else
        std::cout << "erreur lors de l'enregistrement" << std::endl;
}

/// Enregistrer les aretes dans le fichier
void Graph::save_edge() ///MEME FONCTION QUE SAVE VERTEX
{
    std::string fichier2;

    if(m_choixGraphe == 1)
        fichier2 = "edge1.txt";

    if(m_choixGraphe == 2)
        fichier2 = "edge2.txt";

    if(m_choixGraphe == 3)
        fichier2 = "edge3.txt";

    std::ofstream nouveaufichier(fichier2, std::ios::out | std::ios::trunc);

    if(nouveaufichier)
    {
        nouveaufichier << m_edges.size() << std::endl;

        for(unsigned int i=0 ; i < m_edges.size() ; i++)
        {
            nouveaufichier << i << " " << m_edges[i].m_from << " " << m_edges[i].m_to << " " << m_edges[i].m_weight << " " << m_edges[i].m_signe << std::endl;
        }
        nouveaufichier.close();
    }
    else
        std::cout << "erreur lors de l'enregistrement" << std::endl;
}


/// Méthode pour supprimer un sommet
void Graph::Supprimer ()
{
    int num;
    std::string choix;

    std::cout << "Que voulez-vous supprimer ? (sommet ou arc) :" << std::endl;
    cin >> choix; ///possibilite de choix

    do
    {
        if(choix == "sommet")
        {
            cout << "Quel sommet voulez-vous supprimer ?" << endl;
            cin >> num;
            test_remove_vertex(num); ///fonction suppression du sommet est appelee
        }

        if(choix == "arc")
        {
            cout << "Quelle arete voulez-vous supprimer ?" << endl;
            cin >> num;
            test_remove_edge(num); ///fonction suppression de l'arete est appelee
        }

    }
    while((choix != "sommet") && (choix != "arc")); ///blindage
}



/// Méthode pour sauvegarder un graphe
void Graph::Sauvegarde()
{
    save_edge();
    save_vertex();

}

/// Méthode pour ajouter une arete ou un sommet
void Graph::Ajouter()
{
    string x;


    std::cout<<"Que voulez vous ajouter ? (sommet ou arc)" << endl;
    cin >> x; ///choix de l'utilisateur
    if (x=="sommet")
    {
        Add_Vertices(); ///fonction ajout de sommet
    }
    if(x=="arc")
    {
        Add_Edge(); ///fonction ajout de l'arc
    }
}


/// Méthode pour ajouter un sommet
void Graph::Add_Vertices()
{
    string choix;
    string image;

    std::cout << "Quel sommet voulez vous charger ?" << endl;
    std::cin >> choix;

    image = choix+".jpg"; ///nom de l'animal

    add_interfaced_vertex(m_vertices.size(), 0, 0, 0, image, m_vertices.size()); ///creation du sommet avec par défaut poids et coordonnees
}

/// Méthode pour ajouter une arete
void Graph::Add_Edge()
{
    // Variables
    int n = 0, som1, som2, signe;
    int poids;
    bool x = false;

    do
    {
        if (m_edges.count(n)==1) ///verification si l'ajout d'aretes est possible (arete existante ou non)
            n++;

        else
            x=true;

    }
    while(!x);

    do ///boucle de creation de l'arete entre sommets choisis par l'utilisateur
    {
        std::cout << "Choisir votre sommet 1 : " << std::endl;
        std::cin >> som1;
        std::cout << "Choisir votre sommet 2 : " << std::endl;
        std::cin >> som2;

    }
    while((som1 == som2)); ///blindage

    do
    {
        std::cout << "Quel poids voulez vous pour cet arc ?" << std::endl;
        std::cin >> poids;
    }
    while((poids < 0)|| (poids > 100));

    do
    {
        std::cout<< "Influence positive, negative ou neutre ? "<< std::endl; ///pour les reseaux non trophique
        std::cout << "1 - Positive" << std::endl;
        std::cout << "0 - Negative" << std::endl;
        std::cout << "2 - Neutre" << std::endl;
        std::cin >> signe;

    }
    while((signe!=0) && (signe!=1) && (signe!=2));

    add_interfaced_edge(n, som1, som2, signe, poids); /// Ajout d'une nouvelle arete
}

/// Méthode pour supprimer une arete
void Graph::test_remove_edge(int eidx)
{
    std::map<int, Edge>::iterator it; ///creation d'un iterateur

    /// test : on a bien des éléments interfacés
    if (m_interface && m_edges[eidx].m_interface)
        m_interface->m_main_box.remove_child( m_edges[eidx].m_interface->m_top_edge );

    it = m_edges.find(eidx); ///l'iterateur retrouve l'indice de l'arete

}

/// Méthode pour supprimer un sommet
void Graph::test_remove_vertex(int vidx)
{
    std::map<int, Edge>::iterator it; ///creation de l'iterateur map (int et aretes)
    std::map<int, Vertex>::iterator it2; ///de meme avec les sommets

    std::cout << "On enleve le sommet " << vidx << std::endl;

    if (m_interface && m_vertices[vidx].m_interface)
    {
        m_interface->m_main_box.remove_child (m_vertices[vidx].m_interface->m_top_box);
        it2 = m_vertices.find(vidx); ///l'iterateur retrouve le sommet choisit par l'utilisateur

        for (unsigned int i=0 ; i<m_edges.size() ; i++)
        {
            if ((m_edges[i].m_to == vidx)||(m_edges[i].m_from == vidx))
            {
                it = m_edges.find(i); ///l'iterateur prends la valeur de l'indice de l'arete a enlever

                /// test : on a bien des éléments interfacés
                if (m_interface && m_edges[i].m_interface)
                    m_interface->m_main_box.remove_child( m_edges[i].m_interface->m_top_edge );

                cout << "on a supprime arete " << i << endl;
            }
        }
    }
}
///fonction pour passer d'un graphe à un autre
void Graph::Erase()
{
    int taille1 = m_edges.size();
    int taille2 = m_vertices.size();

    for(int i = 0 ; i < taille1 ; i++)
    {
        m_edges.erase(m_edges.find(i)); ///efface la map contenant les aretes
    }

    for(int i = 0 ; i < taille2 ; i++)
    {
        m_vertices.erase(m_vertices.find(i)); ///efface la map contenant les sommets
    }
}

void Graph::Fortementconnexe(Graph *g) ///fonction qui retrouve la forte connexite
{
    if (m_choixGraphe==1)
    {

        int SommetTOT = 5;
        Graphe ge(SommetTOT); ///initialisation du graphe G à 5 sommets
///AJOUT DES ARETES MANUELS (NOUS N AVONS PAS REUSSI A FAIRE AUTREMENT)
        ge.addEdge(0, 1);     ///ajout des aretes
        ge.addEdge(1, 2);
        ge.addEdge(2, 3);
        ge.addEdge(3, 0);
        ge.addEdge(2, 4);

        cout << "Les composantes fortement connexes sont :" << std::endl;

        ge.SCC(g);
    }
    if (m_choixGraphe == 2)
    {
        int SommetTOT=12;
        Graphe ge(SommetTOT);
        ge.addEdge(4, 0);
        ge.addEdge(4, 1);
        ge.addEdge(3, 0);
        ge.addEdge(11, 3);
        ge.addEdge(11, 4);
        ge.addEdge(2, 1);
        ge.addEdge(9, 10);
        ge.addEdge(9, 4);
        ge.addEdge(8, 7);
        ge.addEdge(7, 6);
        ge.addEdge(6, 5);
        ge.addEdge(5, 2);
        ge.addEdge(9, 8);
        ge.addEdge(10, 2);
        cout << "Les composantes fortement connexes sont :" << std::endl;

        ge.SCC(g);
    }
    if (m_choixGraphe == 3)
    {
        int SommetTOT=14;
        Graphe ge(SommetTOT);
        ge.addEdge(4, 0);
        ge.addEdge(5, 0);
        ge.addEdge(3, 1);
        ge.addEdge(6, 0);
        ge.addEdge(6, 1);
        ge.addEdge(6, 2);
        ge.addEdge(6, 3);
        ge.addEdge(7, 0);
        ge.addEdge(8, 0);
        ge.addEdge(8, 1);
        ge.addEdge(8, 2);
        ge.addEdge(8, 3);
        ge.addEdge(11, 9);
        ge.addEdge(11, 10);
        ge.addEdge(11, 4);
        ge.addEdge(11, 5);
        ge.addEdge(11, 6);
        ge.addEdge(11, 7);
        ge.addEdge(11, 8);
        ge.addEdge(11, 12);
        ge.addEdge(11, 13);

        cout << "Les composantes fortement connexes sont :" << std::endl;
        ge.SCC(g);
    }
}

///fonction pour créer le graphe réduit lié aux composantes fortement connexes
void Graph::reduit(int ind)
{
    for (unsigned int i=0 ; i < m_edges.size() ; i++) ///parcourt la map aretes
    {
        if ((m_edges[i].m_to == ind)||(m_edges[i].m_from == ind)) ///ind correspond au sommet fortement connexe
        {

            /// test : on a bien des éléments interfacés
            if (m_interface && m_edges[i].m_interface)
                m_interface->m_main_box.remove_child(m_edges[i].m_interface->m_top_edge );

            cout << "on a supprime arete " << i << endl; ///suppression de l'arete
        }
    }
}

void Graph::Is_Connexe() ///Fonction qui teste la k-connexite
{
    if (m_choixGraphe==1) ///Pour le graphe Bacteries
    {

        int SommetTOT=4;
        Graphe g(SommetTOT); ///initialisation du graphe G à 5 sommets
        g.addEdge(3, 0);     ///aretes qui ne compte pas le sommet k-connexe
        g.addEdge(0, 1);

        int nb_sommet = check_connex(g.adj); ///nb_sommet prend la valeur de la taille de la liste contenant les aretes
        if (nb_sommet == 1) ///si la liste contient un nombre different de 4 alors on peut déconnecter ce graphe pour le rendre non connexe
        {
          std::cout << "Le sommet 2 peut etre retire pour rendre le graphe non connexe" << std::endl;
          test_remove_vertex(2); ///On supprime le sommet qu'on peut enlever pour montrer que le graphe est devenu non connexe
        }
        else
        {
            std::cout << "Le sommet 2 ne peut etre retire car le graphe est toujours connexe." << std::endl;
        }
    }
    if (m_choixGraphe==2)
    {
        int SommetTOT=12;
        Graphe g(SommetTOT);
        g.addEdge(3, 0);
        g.addEdge(11, 3);
        g.addEdge(2, 1);
        g.addEdge(9, 10);
        g.addEdge(8, 7);
        g.addEdge(7, 6);
        g.addEdge(6, 5);
        g.addEdge(5, 2);
        g.addEdge(9, 8);
        g.addEdge(10, 2);

        int nb_sommet = check_connex(g.adj);
        if (nb_sommet == 0)
        {
          std::cout << "Le sommet 4 peut etre retire pour rendre le graphe non connexe" << std::endl;
          test_remove_vertex(4);
        }
        else
        {
            std::cout << "Le sommet 4 ne peut etre retire car le graphe est toujours connexe." << std::endl;
        }
    }
    if (m_choixGraphe==3)
    {int SommetTOT=14;
        Graphe g(SommetTOT);
        g.addEdge(4, 0);
        g.addEdge(5, 0);
        g.addEdge(3, 1);
        g.addEdge(6, 0);
        g.addEdge(6, 1);
        g.addEdge(6, 2);
        g.addEdge(6, 3);
        g.addEdge(7, 0);
        g.addEdge(8, 0);
        g.addEdge(8, 1);
        g.addEdge(8, 2);
        g.addEdge(8, 3);

        int nb_sommet = check_connex(g.adj);
        if (nb_sommet == 0)
        {
          std::cout << "Le sommet 11 peut etre retire pour rendre le graphe non connexe" << std::endl;
          test_remove_vertex(11);
        }
        else
        {
            std::cout << "Le sommet 11 ne peut etre retire car le graphe est toujours connexe." << std::endl;
        }
    }

}

int Graph::check_connex(list<int> *adj)
{
    return adj->size(); ///retourne la valeur que prend la liste d'aretes
}

void Graph::testpopulation() ///fonction de simulation
{
    double coef_K; ///coefficient de portage
    double coef_R=0.1; ///coefficient __ rythme de croissance

    cout<<"Vous avez active la simulation, changez les valeurs de vos populations "<<endl;
    cout<<" et des arcs et recliquez pour voir leur evolution."<<endl;

    for (auto& elem : m_vertices) ///parcourt la map sommet
    {
        for (const auto& elem2: elem.second.m_in) ///parcourt les predecesseurs du sommet
        {
            for (const auto& elem3: m_edges) ///parcourt les aretes
            {
                coef_K+= 0.0002*elem3.second.m_weight*m_vertices[elem2].m_value/7; ///calcul du portage
            }
        }
        if (elem.second.m_value > 0) ///tant que c'est superieur a 0
        {
            double inter= elem.second.m_value/coef_K;
            double inter1= 1 - inter;
            double inter2= elem.second.m_value*inter1;
            double inter3= coef_R*inter2;
            elem.second.m_value = elem.second.m_value+inter3; ///La population evolue
        }
    }
}

///------------------------------------------------------------------------------------------------------------------------
/// CLASS GRAPHE
///------------------------------------------------------------------------------------------------------------------------

///constructeur
Graphe::Graphe(int V)
{
    this->V = V;
    adj = new list<int>[V];
}

///Fonction qui ajoute une arête (création)
void Graphe::addEdge(int v, int w)
{
    adj[v].push_back(w);
}


///affichage du resultat du DFS à partir d'un sommet V
int Graphe::DFS(int v, bool visited[])
{
    ///Marquage du sommet comme visité et affichage
    visited[v] = true;
    cout << v << " ";
    m_compo++;

    ///Marquage des sommets adjacents au sommet marqué
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            DFS(*i, visited);

    if(m_compo==1)
    {
        m_compo=0;
        return v;
    }
    else if(m_compo != 1)
    {
        m_compo=0;
        return -1;
    }
}

///Fonction de marquage lors du passage du DFS
void Graphe::marquageDFS(int v, bool visited[], stack<int> &Stack)
{
    /// Le sommet est marqué
    visited[v] = true;

    ///Marquage des sommets adjacents au sommet qui vient d'être marqué
    list<int>::iterator i;
    for(i = adj[v].begin(); i != adj[v].end(); ++i)
        if(!visited[*i])
            marquageDFS(*i, visited, Stack);

    ///Le sommet est placé dans une pile
    Stack.push(v);
}

///Fonction principale qui trouve les compo fortement connexes
void Graphe::SCC(Graph *g)
{
    stack<int> Stack;

    ///Marque tous les sommets comme non visités
    bool *visited = new bool[V];
    for(int i = 0; i < V; i++)
        visited[i] = false;

    ///Rempli la pile en fonction de l'ordre de visite
    for(int i = 0; i < V; i++)
        if(visited[i] == false)
            marquageDFS(i, visited, Stack);

    ///creation du graphe transposer
    Graphe gr = Renverser();

    ///Seconde DFS sur le nouveau graphe et marquage des sommets comme non visite
    for(int i = 0; i < V; i++)
        visited[i] = false;

    ///Les sommets sont étudiés en fonction de l'ordre dans la pile FIFO
    while (Stack.empty() == false)
    {
        ///Un sommet est retiré de la pile
        int v = Stack.top();
        Stack.pop();

        ///Affichage des compo connexes
        if (visited[v] == false)
        {
            int sommet = gr.DFS(v, visited);
            if(sommet != -1)
            {
                std::cout << "sommet : " << sommet << std::endl;

                g->reduit(sommet);
            }

            cout << endl;
        }
    }
}

Graphe Graphe::Renverser()
{
    ///creation d'un graphe bis que l'on va transposer
    Graphe g(V);
    for (int v = 0; v < V; v++)
    {
        ///Transforme l'arete u -> v en v -> u
        list<int>::iterator i;
        for(i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            g.adj[*i].push_back(v);
        }
    }
    return g; ///retourne le graphe transposer
}


