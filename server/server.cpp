#include <cairo.h>
#include <gtk/gtk.h>
#include <librsvg/rsvg.h>
#include <gio/gfile.h>
#include <tinyxml2.h>
#include <iostream>
#include <map>
#include <vector>
#include <iterator>
#include <string.h>
#include <tuple>    
using namespace tinyxml2;
using namespace std;
static void do_drawing(cairo_t *);
static void do_drawing_svg(cairo_t *);

RsvgHandle *svg_handle;
RsvgRectangle viewport;

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{      
  do_drawing_svg(cr);

  return FALSE;
}
void editValue(XMLElement *elem,char* nom,char* valeur)
{
elem->SetAttribute(nom,valeur);

}
int howManyKids(XMLElement *elem){
    int x=0;
    if(elem->NoChildren()){return x;}
    XMLNode *currentchild=elem->FirstChild();
    while(currentchild!=elem->LastChild()){
        currentchild=currentchild->NextSibling();
        x+=1;
    }
       return x+1;
}
static void do_drawing_svg(cairo_t * cr)
{
  rsvg_handle_render_document (svg_handle, cr, &viewport, NULL);
}

map<char*,tuple<XMLElement*,char*>> drivenMap(XMLElement *elem){
    map<char*,tuple<XMLElement*,char*>> driveMap;
    if(howManyKids(elem)!=0){
        XMLElement *current=elem->FirstChildElement();
        for(int i=0;i<=howManyKids(elem);i++)
        {
            if(strcmp((char*)current->Name(),(char*)"driven")==0){
                char* key=(char*)current->Attribute("by");
                tuple<XMLElement*,char*> value=tuple<XMLElement*,char*>{elem,(char*)current->Attribute("target")};
                driveMap[key]=value;
                
               
            }
         else{
             map<char*,tuple<XMLElement*,char*>> driveMapcurrent=drivenMap(current);
             driveMap.insert(driveMapcurrent.begin(), driveMapcurrent.end());}
            XMLElement *next=current->NextSiblingElement();
                
            if (i!=howManyKids(elem)-1){
            current=next;}
            
        }
        

    }
    return driveMap;
}
int main(int argc, char *argv[])
{
////INIT chaussette
//	 int socket_desc;
//    struct sockaddr_in server_addr, client_addr;
//	socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//    if(socket_desc < 0){
//        printf("erreur creation\n");
//        return -1;
//    }
//    printf("Socket creer\n");
//	server_addr.sin_family = AF_INET;
//    server_addr.sin_port = htons(2000);
//    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  GtkWidget *window;
  GtkWidget *darea;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);
    
  XMLDocument svg_data;
  svg_data.LoadFile("toto.svg");
  XMLElement *rectangle1 = svg_data.FirstChildElement("svg")->FirstChildElement("g")->FirstChildElement("rect");
  rectangle1->SetAttribute("style", "fill:#000000");
XMLElement *soleil =svg_data.FirstChildElement("svg")->FirstChildElement("g")->NextSiblingElement("g")->FirstChildElement("g")->FirstChildElement("circle");
    
      map<char*,tuple<XMLElement*,char*>> pouff=drivenMap(svg_data.FirstChildElement("svg"));
    map<char*,tuple<XMLElement*,char*>>::iterator itr;
   for(itr=pouff.begin();itr!=pouff.end();itr++)
        
  {
       cout<<itr->first<<get<0>(itr->second)<<" "<<get<1>(itr->second)<<endl;;
    }
  XMLPrinter printer;
  svg_data.Print(&printer);
  svg_handle = rsvg_handle_new_from_data ((const unsigned char*) printer.CStr(), printer.CStrSize(), NULL);
  rsvg_handle_get_geometry_for_element  (svg_handle, NULL, &viewport, NULL, NULL);
  std::cout<<howManyKids(svg_data.FirstChildElement("svg"));

  g_signal_connect(G_OBJECT(darea), "draw", 
      G_CALLBACK(on_draw_event), NULL); 
  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), viewport.width, viewport.height); 
  gtk_window_set_title(GTK_WINDOW(window), "GTK window");

  gtk_widget_show_all(window);

  gtk_main();


  return 0;
}