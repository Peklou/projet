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