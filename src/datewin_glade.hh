// generated 2005/12/19 18:18:36 CET by christof@vesta.petig-baender.de
// using glademm V2.10.0.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- --noaccelerators Komponenten.glade
// for gtk 2.8.9 and gtkmm 2.8.2
//
// Please modify the corresponding derived classes in ././datewin.hh and././datewin.cc

#ifndef _DATEWIN_GLADE_HH
#  define _DATEWIN_GLADE_HH

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/notebook.h>
#include <gtkmm/togglebutton.h>

class datewin_glade : public Gtk::HBox
{  
public:
        
        class Gtk::HBox * datewin;
protected:
        class Gtk::Label * datum_label;
        class Gtk::SpinButton * tag;
        class Gtk::SpinButton * monat;
        class Gtk::SpinButton * jahr;
        class Gtk::SpinButton * kw_spinbutton;
        class Gtk::SpinButton * jahr_spinbutton;
        class Gtk::Notebook * notebook;
        class Gtk::ToggleButton * togglebutton_menu;
        
        datewin_glade();
        
        ~datewin_glade();
private:
        virtual void on_tag_activate() = 0;
        virtual void on_monat_activate() = 0;
        virtual void datum_activate() = 0;
        virtual void kw_activate() = 0;
        virtual void on_togglebutton_menu_toggled() = 0;
};
#endif
