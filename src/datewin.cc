// $Id: datewin.cc,v 1.21 2005/11/25 12:49:02 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *  Copyright (C) 2010 Christof Petig
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "datewin.h"
#include <Misc/itos.h>
#include <Misc/Global_Settings.h>
//#include <unistd.h>
#include <iostream>
#include <cassert>
#include <gtk/gtksignal.h>
#include "datewin_popup.hh"


void datewin::preferWeek(bool b)
{ if(kw_bevorzugen==b) return;
  kw_bevorzugen=b;
  set_value(get_value().KW());
}

//#include <glib-object.h>
//#include <gtk/gtknotebook.h>

datewin::~datewin()
{
#if 0 // should be possible as well
 guint switch_page_num=g_signal_lookup("switch_page",GTK_TYPE_NOTEBOOK);
std::cerr << "num " << switch_page_num << '\n';
 int num=g_signal_handlers_disconnect_matched(G_OBJECT(notebook->gobj()),
                     G_SIGNAL_MATCH_ID,switch_page_num,0,0,0,0);
std::cerr << "- " << num << '\n';
#endif

 switch_page_connection.disconnect();
 if (popup) delete popup;
 popup=0;
}

datewin::datewin() : // const std::string &inst) : block(false),
	expandyear(true),kw_bevorzugen(), popup()
{  set_value(ManuProC::Datum::today());
//   jahr->signal_activate().connect(activate.make_slot());
   gtk_signal_connect_after(GTK_OBJECT(gobj()), "grab_focus",
    		GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
   tag->signal_changed().connect(changed.make_slot());
   monat->signal_changed().connect(changed.make_slot());
   jahr->signal_changed().connect(changed.make_slot());
   kw_spinbutton->signal_changed().connect(changed.make_slot());
   jahr_spinbutton->signal_changed().connect(changed.make_slot());
   switch_page_connection=notebook->signal_switch_page().connect(sigc::mem_fun(*this, &datewin::on_datewin_change_current_page), true);
}

ManuProC::Datum datewin::get_value() const throw()
{  ManuProC::Datum d;

   switch(notebook->get_current_page())
   {  case p_Datum:
         tag->update();
         monat->update();
         jahr->update();
         d=ManuProC::Datum(tag->get_value_as_int(),monat->get_value_as_int()
		,jahr->get_value_as_int(),expandyear);
         break;
      case p_Woche:
         kw_spinbutton->update();
         jahr_spinbutton->update();
         d=ManuProC::Datum(Kalenderwoche(kw_spinbutton->get_value_as_int(),
         	jahr_spinbutton->get_value_as_int()));
         break;
      case p_leer: d=ManuProC::Datum(); break;
   }
   return d;
}

void datewin::set_value (const ManuProC::Datum &d) throw()
{
   if (d.valid())
   {  tag->set_value (d.Tag());
      monat->set_value (d.Monat());
      jahr->set_value (d.Jahr());
      try
      {
        Kalenderwoche kw=d.KW();
	kw_spinbutton->set_value(kw.Woche());
	jahr_spinbutton->set_value(kw.Jahr());
	int pg=kw_bevorzugen?p_Woche:p_Datum;
	// Muß weg, da sonst bei jedem set_value auf "Datum" umgesprungen wird
	//      if (pg==p_Woche && d.Tag()!=ManuProC::Datum(d.KW()).Tag()) pg=p_Datum;
	notebook->set_current_page(pg);
      }
      catch (ManuProC::Datumsfehler &e) // manche Jahre sind ungültig für die KW Berechnung
      {
	jahr_spinbutton->set_value(d.Jahr());
	kw_spinbutton->set_value(42);
	notebook->set_current_page(p_Datum);
      }
   }
   else
   {  notebook->set_current_page(p_leer);
   }
   changed();
}

gint datewin::try_grab_focus(GtkWidget *w,gpointer gp) throw()
{  datewin *_this=dynamic_cast<datewin*>((Glib::Object*)gp);
   assert(_this);
   switch(_this->notebook->get_current_page())
   {  case p_Datum:
          _this->correct_day_of_month();
   	  _this->jahr->select_region(0,_this->jahr->get_text_length());
   	  _this->monat->select_region(0,_this->monat->get_text_length());
   	  _this->tag->select_region(0,_this->tag->get_text_length());
	  _this->tag->grab_focus();
   	  break;
      case p_Woche:
   	  _this->jahr_spinbutton->select_region(0,_this->jahr_spinbutton->get_text_length());
   	  _this->kw_spinbutton->select_region(0,_this->kw_spinbutton->get_text_length());
          _this->kw_spinbutton->grab_focus();
   	  break;
      case p_leer:
	  break;
   }
   return true;
}


void datewin::correct_day_of_month()
{
 int itag=tag->get_value();
 int imonat=monat->get_value();
 int ijahr=jahr->get_value();
 int tim=ManuProC::Datum::Tage_in_Monat(imonat,ijahr);
 
 if(itag>tim)
   tag->set_value(tim);
}

void datewin::on_tag_activate()
{
 correct_day_of_month();  
 this->monat->grab_focus(); 
 this->monat->select_region(0,this->monat->get_text_length());
}

void datewin::on_monat_activate()
{
 correct_day_of_month();  
 this->jahr->grab_focus();
 this->jahr->select_region(0,this->jahr->get_text_length());
}


void datewin::setLabel(const std::string &s)
{  datum_label->set_text(s);
}

void datewin::datum_activate()
{  kw_bevorzugen=false; // Datumsicht behalten
   correct_day_of_month();  
   set_value(get_value());
   activate();
}
void datewin::kw_activate()
{  kw_bevorzugen=true;  // Wochensicht behalten
   set_value(get_value());
   activate();
}

void datewin::on_datewin_change_current_page(GtkNotebookPage *p0, guint p1)
{
 switch(p1)
   {  case p_Datum: kw_bevorzugen=false;
   	  break;
      case p_Woche: kw_bevorzugen=true;
   	  break;
      case p_leer: break;
   }
}


// display datewin_popup
void datewin::on_togglebutton_menu_toggled()
{ if (popup || !togglebutton_menu->get_active())
  { if (popup) delete popup;
    popup=0;
  }
  else popup=new datewin_popup(this);
}

void datewin::closePopup()
{
 if (popup) delete popup;
    popup=0;
}

