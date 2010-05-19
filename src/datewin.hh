// $Id: datewin.h,v 1.15 2004/09/27 15:47:17 jacek Exp $
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

#ifndef _DATEWIN_HH
#  include "datewin_glade.hh"
#  define _DATEWIN_HH
#include <Misc/Datum.h>

class datewin_popup;

class datewin : public datewin_glade
{  	friend class datewin_popup;
	static gint try_grab_focus(GtkWidget *w,gpointer gp) throw();
        enum FELD {TAG=1,MONAT};
        enum PAGE { p_Datum, p_Woche, p_leer };
//        bool block;
        bool expandyear;
        std::string instance;
        bool kw_bevorzugen;
        datewin_popup *popup;
        sigc::connection switch_page_connection;

//        static PAGE defaultpage;

	void on_tag_activate();
	void on_monat_activate();	
        void datum_activate();
        void kw_activate();
        void on_togglebutton_menu_toggled();
        void on_datewin_change_current_page(GtkNotebookPage *p0, guint p1);

   public:
	datewin();
	~datewin();
	ManuProC::Datum get_value() const throw(); 
	void set_value(const ManuProC::Datum &d) throw();
	void setLabel(const std::string &s);
	void setExpandYear(bool exp) { expandyear=exp; }
	void preferWeek(bool b=true);
	
private:	
	sigc::signal<void> activate;
	sigc::signal<void> changed;
public:
        sigc::signal<void> &signal_activate() { return activate; }
        sigc::signal<void> &signal_changed() { return changed; }        

};

// compatibility
//class Datewin : public datewin {};
#endif
