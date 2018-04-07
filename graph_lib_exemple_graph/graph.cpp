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


///________________________________________________
///AJOUT DU BOUTON DELETE DANS CHAQUE SOMMET
///____________________________________________________


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
    //  m_box_edge.add_child(m_delete_image);
    // m_delete_image.set_pic_name("delete.jpg");
    //m_delete.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);
    //m_delete.set_bg_color(ROUGE);

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
    m_connexe_box.set_pos(-10,400);
    m_connexe_box.set_dim(80,80);
    m_connexe.set_dim(40,15);
    m_connexe_image.set_pic_name("connexe.jpg");
    m_connexe.add_child(m_connexe_image);
    m_connexe_box.add_child(m_connexe);

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
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    int m_sommet;
    int m_poids;
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

        fichier1 = "sommet1.txt";
        fichier2 = "edge1.txt";
        ifstream newfichier(fichier1, ios::in);
        if(newfichier)
        {
            //on cherche ici a recuperer les donnees ordre et nbre d'aretes du fichier
            newfichier >> nbSommets;
            std::cout << nbSommets << std::endl;

            for(int i=0; i<nbSommets; i++)
            {
                newfichier >>  m_sommet >> m_poids >> m_pos1 >> m_pos2 >> m_nom;
                std::cout << " " << m_sommet << " " << m_poids << " " << m_pos1 << " " << m_pos2 << " " << m_nom;
                image = m_nom+".jpg";
                // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...

                add_interfaced_vertex(m_sommet, m_poids, m_pos1, m_pos2, image);
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
    case 2:
    {

        fichier1 = "sommet2.txt";
        fichier2 = "edge2.txt";
        ifstream newfichier(fichier1, ios::in);
        if(newfichier)
        {
            //on cherche ici a recuperer les donnees ordre et nbre d'aretes du fichier
            newfichier >> nbSommets;
            std::cout << nbSommets << std::endl;

            for(int i=0; i<nbSommets; i++)
            {
                newfichier >>  m_sommet >> m_poids >> m_pos1 >> m_pos2 >> m_nom;
                std::cout << " " << m_sommet << " " << m_poids << " " << m_pos1 << " " << m_pos2 << " " << m_nom;
                image = m_nom+".jpg";
                // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...

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
            //on cherche ici a recuperer les donnees ordre et nbre d'aretes du fichier
            newfichier >> nbSommets;
            std::cout << nbSommets << std::endl;

            for(int i=0; i<nbSommets; i++)
            {
                newfichier >>  m_sommet >> m_poids >> m_pos1 >> m_pos2 >> m_nom;
                std::cout << " " << m_sommet << " " << m_poids << " " << m_pos1 << " " << m_pos2 << " " << m_nom;
                image = m_nom+".jpg";
                // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...

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
void Graph::update(bool *ok)
{
      if (!m_interface)
        return;

    for (auto &elt : m_vertices)
    elt.second.pre_update();

    for (auto &elt : m_edges)
    elt.second.pre_update();

        m_interface->m_top_box.update();

  if(m_interface->m_sauv.clicked())
    {
    Sauvegarde();

    for (auto &elt : m_vertices)
    elt.second.pre_update();

    for (auto &elt : m_edges)
    elt.second.pre_update();

    }

    if(m_interface->m_delete.clicked()){
        Supprimer();

    for (auto &elt : m_vertices)
    elt.second.pre_update();

    for (auto &elt : m_edges)
    elt.second.pre_update();
    }
        if(m_interface->m_add.clicked()){
    Ajouter();
    for (auto &elt : m_vertices)
    elt.second.pre_update();

    for (auto &elt : m_edges)
    elt.second.pre_update();
    }

if(m_interface->m_return.clicked())
{
        *ok = false;
        for (auto &elt : m_vertices)
    elt.second.pre_update();

    for (auto &elt : m_edges)
    elt.second.pre_update();
}

if(m_interface->m_connexe.clicked())
    {

        Fortementconnexe();
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
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
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

    if (signe == 0)
    {
        ei->m_top_edge.m_color = ROUGECLAIR;
        m_edges[idx].m_signe = 0;
    }

    else if(signe == 1)
    {
        ei->m_top_edge.m_color = VERTCLAIR;
        m_edges[idx].m_signe = 1;
    }

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

    if(m_choixGraphe == 1)
        fichier1 = "sommet1.txt";

    if(m_choixGraphe == 2)
        fichier1 = "sommet2.txt";

    if(m_choixGraphe == 3)
        fichier1 = "sommet3.txt";

    std::ofstream newfichier(fichier1, std::ios::out | std::ios::trunc);

    if(newfichier)
    {
        newfichier << m_vertices.size() << std::endl;

        for(unsigned int i=0 ; i < m_vertices.size() ; i++)
        {

            std::string name = m_vertices[i].m_interface->m_img.get_pic_name();
            name.erase(name.end()-4, name.end());
            std::cout << m_vertices[i].m_value << " " << m_vertices[i].m_interface->m_top_box.get_posx() << " " <<  m_vertices[i].m_interface->m_top_box.get_posy() << " " << name << std::endl;

            newfichier << i <<" " << m_vertices[i].m_value << " " << m_vertices[i].m_interface->m_top_box.get_posx() << " " <<  m_vertices[i].m_interface->m_top_box.get_posy() << " " << name << std::endl;
        }
        newfichier.close();
    }
    else
        std::cout << "erreur lors de l'enregistrement" << std::endl;
}

/// Enregistrer les aretes dans le fichier
void Graph::save_edge()
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

        cout<<"Quel sommet voulez-vous supprimer ?"<<endl;
        cin>>num;


    // test_remove_vertex(num);

    for (auto& aretes : m_edges)
    {
        if ((aretes.second.m_to==num)||(aretes.second.m_from==num))
        {
            test_remove_edge(aretes.first);

        }
    }
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
        cin >> x;
        if (x=="sommet")
        {
            Add_Vertices();
        }
        if(x=="arc")
        {
           Add_Edge();
        }
}


/// Méthode pour ajouter un sommet
void Graph::Add_Vertices()
{
    string choix;
    string image;

    std::cout << "Quel sommet voulez vous charger ?" << endl;
    std::cin >> choix;

    image = choix+".jpg";

    add_interfaced_vertex(m_vertices.size(), 0, 0, 0, image, m_vertices.size()); // Ajouter le sommet
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
        if (m_edges.count(n)==1)
            n++;

        else x=true;

    } while(!x);

    do
    {
        std::cout << "Choisir votre sommet 1 : " << std::endl;
        std::cin >> som1;
        std::cout << "Choisir votre sommet 2 : " << std::endl;
        std::cin >> som2;

    }while((som1 == som2));

    do
    {
        std::cout << "Quel poids voulez vous pour cet arc ?" << std::endl;
        std::cin >> poids;
    }
    while((poids < 0)|| (poids > 100));

    do
    {
        std::cout<< "Influence positive, negative ou neutre ? "<< std::endl;
        std::cout << "1 - Positive" << std::endl;
        std::cout << "0 - Negative" << std::endl;
        std::cout << "2 - Neutre" << std::endl;
        std::cin >> signe;

    }while((signe!=0) && (signe!=1) && (signe!=2));

    add_interfaced_edge(n, som1, som2, signe, poids); // Ajouter une arete
}

/// Méthode pour supprimer une arete
void Graph::test_remove_edge(int eidx)
{
/// référence vers le Edge à enlever
    Edge &remed=m_edges.at(eidx);
    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;
/// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;
/// test : on a bien des éléments interfacés
    if (m_interface && remed.m_interface)
    {
/// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge); */
        /* m_edges[idx] = Edge(weight, ei); */
/// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr
/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }
/// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !
/// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );
/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface
/// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );
/// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;
}

/// Méthode pour supprimer un sommet
void Graph::test_remove_vertex(int vidx)
{
    Vertex &remver = m_vertices.at(vidx);
    std::cout << "On enleve le sommet " << vidx << std::endl;

    for (unsigned int i = 0; i < remver.m_in.size(); i++)
        test_remove_edge(remver.m_in[i]);

    for (unsigned int i = 0; i < remver.m_out.size(); i++)
        test_remove_edge(remver.m_out[i]);

    if (m_interface && remver.m_interface)
        m_interface->m_main_box.remove_child (remver.m_interface->m_top_box);
}

void Graph::Erase()
{
    int taille1 = m_edges.size();
    int taille2 = m_vertices.size();

    for(int i = 0 ; i < taille1 ; i++)
    {
        m_edges.erase(m_edges.find(i));
        //test_remove_edge(i);
    }

    for(int i = 0 ; i < taille2 ; i++)
    {
        m_vertices.erase(m_vertices.find(i));
        //test_remove_vertex(i);
    }

}

void Graph::DFS(int v, bool marquage[])
{
///marque le sommet comme marqué et l'affiche
    marquage[v] = true;
    cout << v << " ";

///parcourt les sommets adjacents au sommet qui vient d'être marqué
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!marquage[*i])
            DFS(*i, marquage);
}

void Graph::Transposer()
{
    //Graph g1;//(SommetTOT);
    for (int v = 0; v <SommetTOT; v++)
    {
///inverse le graphe si on  un arc a vers b il devient b vers a
        list<int>::iterator i;
        for(i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            adj[*i].push_back(v);
        }
    }
//    return g1;
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); ///creation d'adjacences
}

void Graph::marquageDFS(int v, bool marquage[], stack<int> &Stack)
{
///marquage du sommet comme marqué
    marquage[v] = true;

///marque les sommets adjacents
    list<int>::iterator i;

    for(i = adj[v].begin(); i != adj[v].end(); ++i)
    {
        if(!marquage[*i])
        {
            marquageDFS(*i, marquage, Stack);
        }
    }
///remplissage de la pile en fonction du temps (plutot position attribué lors du marquage.. peu clair comme explcation)
    Stack.push(v);
cout<<v<<endl;
}

///Fonction qui trouve les composantes fortement connexes
void Graph::SCC()
{
    stack<int> Stack;

///Premiere DFS marque tous les sommets comme non marqué / non visité
    bool *marquage = new bool[SommetTOT];
    for(int i = 0; i < SommetTOT ; i++)
        marquage[i] = false;

///marquage des sommets par DFS (donc dernier temps premier dans la pile )
    for(int i = 0; i < SommetTOT; i++)
        if(marquage[i] == false)
            marquageDFS(i, marquage, Stack);

///Retourne l'orientation du graphe
     Transposer();

///Deuxieme Dfs réalisé marque tous les sommets comme  non marqués
    for(int i = 0; i < SommetTOT; i++)
{
            marquage[i] = false;
}
///On suit l'ordre de la pile stack et on fait un dfs en fonction de chaque sommet.
    while (Stack.empty() == false)
    {
        int v = Stack.top();
        Stack.pop();

///affichage du SCC
        if (marquage[v] == false)
        {
            DFS(v, marquage);
            cout << endl;
        }
    }
}

void Graph::Fortementconnexe()

{
    SommetTOT=m_vertices.size();
    cout << "Les composantes fortement connexes sont " << endl;
    SCC();

}

