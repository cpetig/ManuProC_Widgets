// $Id: EingabeBox.hh,v 1.9 2004/05/06 13:56:16 jacek Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001-2006 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski & Christof Petig
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

// $Id: EingabeBox.hh,v 1.9 2004/05/06 13:56:16 jacek Exp $

#ifndef _EINGABEBOX_HH
#define _EINGABEBOX_HH
#include <gtkmm/table.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/widget.h>
#include <vector>

class EingabeBox : public Gtk::Table
{	typedef std::vector<Gtk::Label *> t_labels;
	typedef std::vector<Gtk::Entry *> t_entries;
	typedef std::vector<sigc::connection> t_cons;

        t_labels labels;
        t_entries entries;
        // activate->grab_focus connects
        // x -0-> 0 -1-> 1 -last-> y
        t_cons cons;
        sigc::connection last_con;
        unsigned visible_size;
        unsigned boxes_per_row;
        
//        Signal_Converter::signal0_on_focus_in signal_adaptor;
//	friend gint try_grab_focus(GtkWidget *w,gpointer);
        sigc::signal<void> grab_focus_tried;
        
        void grow(int cols);
        void check() const;
        static gint try_grab_focus(GtkWidget *w,gpointer gp);

	sigc::signal<void> _activate;
	sigc::signal<void,int> _activate_entry;
	
	void activate() { _activate(); }
public:
        EingabeBox(unsigned cols=0,unsigned per_row=4);
        ~EingabeBox();
	const std::string get_label(int col) const;
	void set_label(int col,const std::string &s);
	const std::string get_value(int col) const;
	void set_value(int col,const std::string &s, bool editable=true);
	void set_width(int col,int width);
	void clear() { set_size(0); }
	void set_size(int cols);
//   void activate_entry(int nr);
	int size() const
	{  return visible_size; }
	void reset();

	sigc::signal<void> &signal_activate() { return _activate; }
	sigc::signal<void,int> &signal_activate_entry() 
			{ return _activate_entry; }
};
#endif
