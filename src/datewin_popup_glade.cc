// generated 2005/12/19 18:18:37 CET by christof@vesta.petig-baender.de
// using glademm V2.10.0.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- --noaccelerators Komponenten.glade
// for gtk 2.8.9 and gtkmm 2.8.2
//
// Please modify the corresponding derived classes in ././datewin_popup.cc


#if defined __GNUC__ && __GNUC__ < 3
#error This program will crash if compiled with g++ 2.x
// see the dynamic_cast bug in the gtkmm FAQ
#endif //
#include "config.h"
/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (GETTEXT_PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif
#include <gtkmmconfig.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <sigc++/compatibility.h>
#include <sigc++/bind.h>
#define GMM_GTKMM_22_24(a,b) b
#else //gtkmm 2.2
#define GMM_GTKMM_22_24(a,b) a
#endif //
#include "datewin_popup_glade.hh"
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
#include <gtkmm/toolbutton.h>
#else //
#include <gtkmm/button.h>
#endif //
#include <gtkmm/toolbar.h>
#include <gtkmm/box.h>

datewin_popup_glade::datewin_popup_glade(
) : Gtk::Window(Gtk::WINDOW_POPUP)
{  
   
   Gtk::Window *datewin_popup = this;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   Gtk::ToolButton *toolbutton1 = Gtk::manage(new class Gtk::ToolButton(_("Datum")));
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   Gtk::ToolButton *toolbutton2 = Gtk::manage(new class Gtk::ToolButton(_("Woche")));
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   Gtk::ToolButton *toolbutton3 = Gtk::manage(new class Gtk::ToolButton(_("leer")));
#endif //
   Gtk::Toolbar *toolbar1 = Gtk::manage(new class Gtk::Toolbar());
   calendar1 = Gtk::manage(new class Gtk::Calendar());
   
   Gtk::VBox *vbox2 = Gtk::manage(new class Gtk::VBox(false, 0));
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbar1->append(*toolbutton1);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem(_("Datum"), Gtk::Toolbar_Helpers::Callback0()));
   Gtk::Button *toolbutton1 = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbar1->append(*toolbutton2);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem(_("Woche"), Gtk::Toolbar_Helpers::Callback0()));
   Gtk::Button *toolbutton2 = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbar1->append(*toolbutton3);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem(_("leer"), Gtk::Toolbar_Helpers::Callback0()));
   Gtk::Button *toolbutton3 = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbutton1->set_flags(Gtk::CAN_DEFAULT);
   toolbutton1->set_visible_horizontal(true);
   toolbutton1->set_visible_vertical(true);
   toolbutton1->set_is_important(false);
   toolbutton1->set_homogeneous(true);
   toolbutton1->set_expand(true);
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbutton2->set_flags(Gtk::CAN_DEFAULT);
   toolbutton2->set_visible_horizontal(true);
   toolbutton2->set_visible_vertical(true);
   toolbutton2->set_is_important(false);
   toolbutton2->set_homogeneous(true);
   toolbutton2->set_expand(true);
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbutton3->set_flags(Gtk::CAN_DEFAULT);
   toolbutton3->set_visible_horizontal(true);
   toolbutton3->set_visible_vertical(true);
   toolbutton3->set_is_important(false);
   toolbutton3->set_homogeneous(true);
   toolbutton3->set_expand(true);
#endif //
   toolbar1->set_tooltips(true);
   toolbar1->set_toolbar_style(Gtk::TOOLBAR_TEXT);
   toolbar1->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbar1->set_show_arrow(true);
#endif //
   calendar1->set_flags(Gtk::CAN_FOCUS);
   calendar1->display_options(Gtk::CALENDAR_SHOW_HEADING|Gtk::CALENDAR_SHOW_DAY_NAMES);
   vbox2->pack_start(*toolbar1, Gtk::PACK_SHRINK, 0);
   vbox2->pack_start(*calendar1);
   datewin_popup->set_title(_("Datumseigenschaften"));
   datewin_popup->set_modal(false);
   datewin_popup->property_window_position().set_value(Gtk::WIN_POS_NONE);
   datewin_popup->set_resizable(true);
   datewin_popup->property_destroy_with_parent().set_value(false);
   datewin_popup->add(*vbox2);
   toolbutton1->show();
   toolbutton2->show();
   toolbutton3->show();
   toolbar1->show();
   vbox2->show();
   datewin_popup->show();
   toolbutton1->signal_clicked().connect(SigC::slot(*this, &datewin_popup_glade::datum_clicked), false);
   toolbutton2->signal_clicked().connect(SigC::slot(*this, &datewin_popup_glade::woche_clicked), false);
   toolbutton3->signal_clicked().connect(SigC::slot(*this, &datewin_popup_glade::empty_clicked), false);
   calendar1->signal_day_selected().connect(SigC::slot(*this, &datewin_popup_glade::on_day_selected), false);
   toolbutton1->grab_default();
}

datewin_popup_glade::~datewin_popup_glade()
{  
}
