// generated 2010/11/15 10:41:30 CET by christof@christof-hp.(none)
// using glademm V2.13.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- --noaccelerators widgets.glade
// for gtk 2.22.0 and gtkmm 2.20.3
//
// Please modify the corresponding derived classes in ./src/datewin_popup.cc


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
#endif
#include <gtkmmconfig.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
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
#include <gtkmm/frame.h>
#ifndef ENABLE_NLS
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif


datewin_popup_glade::datewin_popup_glade(
) : Gtk::Window(Gtk::WINDOW_POPUP)
{  
   
   Gtk::Window *datewin_popup = this;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   Gtk::ToolButton *toolbutton1 = Gtk::manage(new class Gtk::ToolButton(_("Date")));
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   Gtk::ToolButton *toolbutton2 = Gtk::manage(new class Gtk::ToolButton(_("Week")));
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   Gtk::ToolButton *toolbutton3 = Gtk::manage(new class Gtk::ToolButton(_("Empty")));
#endif //
   Gtk::Toolbar *toolbar1 = Gtk::manage(new class Gtk::Toolbar());
   calendar1 = Gtk::manage(new class Gtk::Calendar());
   
   Gtk::VBox *vbox2 = Gtk::manage(new class Gtk::VBox(false, 0));
   Gtk::Frame *frame1 = Gtk::manage(new class Gtk::Frame());
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbar1->append(*toolbutton1);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem(_("Date"), Gtk::Toolbar_Helpers::Callback0()));
   Gtk::Button *toolbutton1 = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbar1->append(*toolbutton2);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem(_("Week"), Gtk::Toolbar_Helpers::Callback0()));
   Gtk::Button *toolbutton2 = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbar1->append(*toolbutton3);
#else //
   
   
   toolbar1->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem(_("Empty"), Gtk::Toolbar_Helpers::Callback0()));
   Gtk::Button *toolbutton3 = static_cast<Gtk::Button *>(toolbar1->tools().back().get_widget());
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbutton1->set_flags(Gtk::CAN_DEFAULT);
   toolbutton1->set_homogeneous(true);
   toolbutton1->set_expand(true);
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbutton2->set_flags(Gtk::CAN_DEFAULT);
   toolbutton2->set_homogeneous(true);
   toolbutton2->set_expand(true);
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbutton3->set_flags(Gtk::CAN_DEFAULT);
   toolbutton3->set_homogeneous(true);
   toolbutton3->set_expand(true);
#endif //
   toolbar1->set_toolbar_style(Gtk::TOOLBAR_TEXT);
   toolbar1->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>=3
   toolbar1->set_show_arrow(false);
#endif //
   calendar1->set_flags(Gtk::CAN_FOCUS);
   calendar1->display_options(Gtk::CALENDAR_SHOW_HEADING|Gtk::CALENDAR_SHOW_DAY_NAMES);
   vbox2->pack_start(*toolbar1, Gtk::PACK_SHRINK, 0);
   vbox2->pack_start(*calendar1);
   frame1->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
   frame1->add(*vbox2);
   datewin_popup->set_title(_("Date properties"));
   datewin_popup->add(*frame1);
   toolbutton1->show();
   toolbutton2->show();
   toolbutton3->show();
   toolbar1->show();
   calendar1->show();
   vbox2->show();
   frame1->show();
   toolbutton1->signal_clicked().connect(sigc::mem_fun(*this, &datewin_popup_glade::datum_clicked), false);
   toolbutton2->signal_clicked().connect(sigc::mem_fun(*this, &datewin_popup_glade::woche_clicked), false);
   toolbutton3->signal_clicked().connect(sigc::mem_fun(*this, &datewin_popup_glade::empty_clicked), false);
   calendar1->signal_day_selected().connect(sigc::mem_fun(*this, &datewin_popup_glade::on_day_selected), false);
   toolbutton1->grab_default();
}

datewin_popup_glade::~datewin_popup_glade()
{  
}
