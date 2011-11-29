// $Id: logwin.cc,v 1.10 2004/02/06 09:46:38 christof Exp $
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

#include "logwin.h"
#include <gtkmm/adjustment.h>
#include <gtk/gtkadjustment.h>

void logwin::scroll() throw()
{
   emu.scroll_to_end();
}

logwin::logwin(guint minimum_size) : color()
{  add(gtklist);
   gtklist.show();
   set_size_request(-1,minimum_size);
   set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   emu.set_title(Glib::ustring());
   emu.attach_to(gtklist);
}

void logwin::set_color()
{  if (color) return;
   emu.add(colorcol);
   Gtk::CellRendererText *crt = dynamic_cast<Gtk::CellRendererText *>(gtklist.get_column(0)->get_first_cell_renderer());
   gtklist.get_column(0)->add_attribute(crt->property_foreground_gdk(),colorcol);
   color=true;
}

void logwin::append(const Glib::ustring &text, const Gdk::Color &col)
{  if (!color) set_color();
   std::vector<Glib::ustring> cols;
   cols.push_back(text);
   Gtk::TreeModel::iterator iter=emu.append(cols);
   (*iter)[colorcol]=col;
}
