// generated 2011/7/7 21:51:53 CEST by christof@christof-hp.(none)
// using glademm V2.13.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- --gnome2 tree.glade
// for gtk 2.24.4 and gtkmm 2.24.0
//
// Please modify the corresponding derived classes in .//simplest.cc


#if defined __GNUC__ && __GNUC__ < 3
#error This program will crash if compiled with g++ 2.x
// see the dynamic_cast bug in the gtkmm FAQ
#endif //
#include "config.h"
#include <gtkmmconfig.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <sigc++/bind.h>
#define GMM_GTKMM_22_24(a,b) b
#else //gtkmm 2.2
#define GMM_GTKMM_22_24(a,b) a
#endif //
#include "simplest_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/menu.h>
#include <gtkmm/menubar.h>
#include <gtkmm/box.h>

simplest_glade::simplest_glade(
) : Gtk::Window(Gtk::WINDOW_TOPLEVEL)
{  
   
   Gtk::Window *simplest = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::MenuItem *Beenden = NULL;
   Gtk::Menu *menu1 = Gtk::manage(new class Gtk::Menu());
   Gtk::MenuItem *Datei = NULL;
   Gtk::MenuBar *menubar1 = Gtk::manage(new class Gtk::MenuBar());
   treebase = new class SimpleTree(5);
   
   Gtk::VBox *vbox1 = Gtk::manage(new class Gtk::VBox(false, 0));
   
   menu1->items().push_back(Gtk::Menu_Helpers::MenuElem(""));
   Beenden = (Gtk::MenuItem *)&menu1->items().back();
   
   menubar1->items().push_back(Gtk::Menu_Helpers::MenuElem("", *menu1));
   Datei = (Gtk::MenuItem *)&menubar1->items().back();
   vbox1->pack_start(*menubar1, Gtk::PACK_SHRINK, 0);
   vbox1->pack_start(*treebase);
   simplest->set_title("");
   simplest->set_position(Gtk::WIN_POS_NONE);
   simplest->property_allow_shrink().set_value(true);
   simplest->add(*vbox1);
   Beenden->show();
   menu1->show();
   Datei->show();
   menubar1->show();
   treebase->show();
   vbox1->show();
   simplest->show();
   Beenden->signal_activate().connect(sigc::mem_fun(*this, &simplest_glade::on_Beenden_activate), false);
}

simplest_glade::~simplest_glade()
{  delete treebase;
   delete gmm_data;
}
