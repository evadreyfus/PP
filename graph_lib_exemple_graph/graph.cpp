#include "graph.h"
#include <fstream>
#include <string>
using namespace std;
/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 130);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
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


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}




/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
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

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);
    //  m_box_edge.add_child(m_delete_image);
    // m_delete_image.set_pic_name("delete.jpg");
    //m_delete.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);
    //m_delete.set_bg_color(ROUGE);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}

/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
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
    // m_delete_box.set_pos(-10,4);
    m_delete_box.set_dim(100,100);
    m_delete.set_dim(50,15);
    m_delete_image.set_pic_name("delete.jpg");
    m_delete.add_child(m_delete_image);
    m_delete_box.add_child(m_delete);

    m_tool_box.add_child(m_sauv_box);
    m_sauv_box.set_pos(-10,50);
    m_sauv_box.set_dim(100,100);
    m_sauv.set_dim(40,15);
    m_sauv_image.set_pic_name("save.jpg");
    m_sauv.add_child(m_sauv_image);
    m_sauv_box.add_child(m_sauv);


}


/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.

///________________________________________________________
///        CREATION DU GRAPHE ET ARCS ENTRE SOMMETS
///________________________________________________________

void Graph::loadgraphe(int m_choixgraphe)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
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
   // int m_choixgraphe;

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

        for(unsigned int i=0; i<14; i++)
        {
            newfichier >>  m_sommet >> m_poids >> m_pos1 >> m_pos2 >> m_nom;
            std::cout << " " << m_sommet << " " << m_poids << " " << m_pos1 << " " << m_pos2 << " " << m_nom;
            image = m_nom+".jpg";
            // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...

            add_interfaced_vertex(m_sommet, m_poids, m_pos1,m_pos2, image);
        }
        newfichier.close();
    }
    else std::cout << "Erreur lors du chargement du fichier..." << std::endl;

    ifstream nouveaufichier(fichier2, ios :: in);
    if(nouveaufichier)
    {
        nouveaufichier >> nbEdges;

        for (int i=0; i<nbEdges; i++)
        {
            nouveaufichier >> m_edge >> m_s1 >> m_s2 >> m_pe;
            add_interfaced_edge(m_edge, m_s1, m_s2,m_pe);
        }
        nouveaufichier.close();
    }
    else std::cout << "Erreur lors du chargement du fichier..." << std::endl;

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

        for(unsigned int i=0; i<14; i++)
        {
            newfichier >>  m_sommet >> m_poids >> m_pos1 >> m_pos2 >> m_nom;
            std::cout << " " << m_sommet << " " << m_poids << " " << m_pos1 << " " << m_pos2 << " " << m_nom;
            image = m_nom+".jpg";
            // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...

            add_interfaced_vertex(m_sommet, m_poids, m_pos1,m_pos2, image);
        }
        newfichier.close();
    }
    else std::cout << "Erreur lors du chargement du fichier..." << std::endl;

    ifstream nouveaufichier(fichier2, ios :: in);
    if(nouveaufichier)
    {
        nouveaufichier >> nbEdges;

        for (int i=0; i<nbEdges; i++)
        {
            nouveaufichier >> m_edge >> m_s1 >> m_s2 >> m_pe;
            add_interfaced_edge(m_edge, m_s1, m_s2,m_pe);
        }
        nouveaufichier.close();
    }
    else std::cout << "Erreur lors du chargement du fichier..." << std::endl;


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

        for(unsigned int i=0; i<14; i++)
        {
            newfichier >>  m_sommet >> m_poids >> m_pos1 >> m_pos2 >> m_nom;
            std::cout << " " << m_sommet << " " << m_poids << " " << m_pos1 << " " << m_pos2 << " " << m_nom;
            image = m_nom+".jpg";
            // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...

            add_interfaced_vertex(m_sommet, m_poids, m_pos1,m_pos2, image);
        }
        newfichier.close();
    }
    else std::cout << "Erreur lors du chargement du fichier..." << std::endl;

    ifstream nouveaufichier(fichier2, ios :: in);
    if(nouveaufichier)
    {
        nouveaufichier >> nbEdges;

        for (int i=0; i<nbEdges; i++)
        {
            nouveaufichier >> m_edge >> m_s1 >> m_s2 >> m_pe;
            add_interfaced_edge(m_edge, m_s1, m_s2,m_pe);
        }
        nouveaufichier.close();
    }
    else std::cout << "Erreur lors du chargement du fichier..." << std::endl;

break;
        }
    default:
        {
        break;
        }

}

}
/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

    Sauvegarde();
    Supprimer();

}

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide � l'ajout d'arcs interfac�s
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
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
            nouveaufichier << i << " " << m_edges[i].m_from << " " << m_edges[i].m_to << " " << m_edges[i].m_weight << std::endl;
        }
        nouveaufichier.close();
    }
    else
        std::cout << "erreur lors de l'enregistrement" << std::endl;
}
void Graph::Supprimer ()
{
    int num;
    m_interface->m_top_box.update();

    if(m_interface->m_delete.clicked())
    {
        cout<<"Quel sommet voulez-vous supprimer ?"<<endl;
        cin>>num;
    }
    // test_remove_vertex(num);

    for (auto& elem : m_edges)
    {
        if ((elem.second.m_to==num)||(elem.second.m_from==num))
        {
            test_remove_edge(elem.first);

        }
    }
}

void Graph::Sauvegarde()
{
    m_interface->m_top_box.update();

    if(m_interface->m_sauv.clicked())
    {
        save_edge();
        save_vertex();
    }
}

/// eidx index of edge to remove
void Graph::test_remove_edge(int eidx)
{
/// r�f�rence vers le Edge � enlever
    Edge &remed=m_edges.at(eidx);
    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;
/// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;
/// test : on a bien des �l�ments interfac�s
    if (m_interface && remed.m_interface)
    {
/// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge); */
        /* m_edges[idx] = Edge(weight, ei); */
/// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr
/// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
/// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }
/// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !
/// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );
/// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
/// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface
/// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );
/// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;
}

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
/*void Graph::menu()
{
    do
    {
        std::cout << "Quel graphe voulez-vous charger ?" << std::endl;
        std::cin >> m_choixGraphe;

    } while((m_choixGraphe != 1) && (m_choixGraphe != 2) && (m_choixGraphe != 3));

    loadgraphe();
}
*/

