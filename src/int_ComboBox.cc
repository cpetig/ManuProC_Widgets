// $Id: int_RadioButtons.cc,v 1.4 2003/03/07 08:10:25 christof Exp $
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

#include "int_ComboBox.hh"

struct int_ComboBox::Connection::Columns : Gtk::TreeModelColumnRecord
{ Gtk::TreeModelColumn<int> index;
  Gtk::TreeModelColumn<Glib::ustring> name;
  Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > image;
  
  Columns() { add(index); add(name); add(image); }
};

#if 0
bool int_ComboBox::Connection::toggle(GdkEventButton *ev)
{  if (props.empty()) return false;
   std::map<int,properties>::const_iterator i=props.find(model.Value());
   if (i==props.end()) i=props.begin();
   else 
   {  ++i;
      if (i==props.end()) i=props.begin();
   }
   model=i->first;
   return false;
}
#endif

void int_ComboBox::Connection::model2widget()
{ @@
  widget->set(props[model.Value()].pixbuf);
}

SigC::Connection int_ComboBox::Connection::connect()
{ if (eventbox) toggleconn=eventbox->signal_button_press_event().connect(SigC::slot(*this,&int_ComboBox::Connection::toggle));
  return SigC::Connection();
}

void int_ComboBox::Connection::disconnect()
{ toggleconn.disconnect();
}

int_ComboBox::int_ComboBox(const Model_ref<T> &m)
	: conn(m), imag()
{
};

void int_ComboBox::Connection::set_widget(widget_t *w,Widget *ev)
{  combo=dynamic_cast<Gtk::ComboBox*>(ev);
   this_t::set_widget(w);
}

void int_ComboBox::Connection::add_entry(int value, Glib::RefPtr<Gdk::Pixbuf> pixbuf, 
				const std::string &tooltip)
{  
@@@
props[value].pixbuf=pixbuf;
   props[value].tooltip=tooltip;
   if (!!model && model.Value()==value) model2widget();
}
