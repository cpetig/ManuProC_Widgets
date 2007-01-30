// $Id: int_RadioButtons.hh,v 1.3 2002/12/03 09:10:44 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2007 Christof Petig
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef MANUPROC_WIDGETS_MVC_INT_COMBO_H
#define MANUPROC_WIDGETS_MVC_INT_COMBO_H

#include <gtkmm/combobox.h>
#include <ModelWidgetConnection.h>
#include <gdkmm/pixbuf.h>
#include <map>

class int_ComboBox : public Gtk::ComboBox
{	typedef int T;
public:
	class Connection : public ModelWidgetConnection<T,Gtk::ComboBox>
	{	struct properties
		{  Glib::RefPtr<Gdk::Pixbuf> pixbuf;
		   std::string name;
		};
		struct Columns;
		Columns columns;
		widget_t *combo;
		SigC::Connection toggleconn;

		void model2widget();
		void widget2model() {}
		SigC::Connection connect();
		void disconnect();
		
	public:
		Connection(const Model_ref<T> &m) : this_t(m), combo() { }
		Connection(Widget *w) : combo(dynamic_cast<Gtk::ComboBox*>(w) { }
		void set_widget(widget_t *w, Widget *_combo=0);
		void add_entry(int value, Glib::RefPtr<Gdk::Pixbuf> const& pixbuf, 
				std::string const& name);
	};
	
private:	
	SigC::Connection conn;
	
public:
	int_ComboBox(const Model_ref<T> &model);
	// const std::vector<key_name_t> &names);
	void add_entry(int value, Glib::RefPtr<Gdk::Pixbuf> const& pixbuf, 
				std::string const& name)
	{  conn.add_entry(value,pixbuf,name); }
	void add_entry(int value, std::string const& name)
	{  conn.add_entry(value, Glib::RefPtr<Gdk::Pixbuf>(), name); }
};

#endif
