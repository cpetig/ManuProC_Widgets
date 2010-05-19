// $Id: CellRendererSimpleTree.h,v 1.1 2004/01/06 09:07:09 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#ifndef KOMPONENTEN_CELLREND_SIMPLE_TREE
#define KOMPONENTEN_CELLREND_SIMPLE_TREE

#include <gtkmm/cellrenderertext.h>
#include <gtkmm/cellrenderertoggle.h>

class CellRendererSimpleTreeText : public Gtk::CellRendererText
{protected:
	typedef Gtk::CellRendererText Parent;
	
	const guint column; // which column this is in
	Glib::Property<guint> childrens_deep; // the changing properties
	Glib::Property<guint> children_count;
	
	virtual void get_size_vfunc(Gtk::Widget& widget, const Gdk::Rectangle* cell_area, 
			int* x_offset, int* y_offset, int* width, int* height) const;
	virtual void render_vfunc(const Glib::RefPtr<Gdk::Drawable>& window,
			Gtk::Widget& widget, const Gdk::Rectangle& background_area, 
			const Gdk::Rectangle& cell_area, const Gdk::Rectangle& expose_area,
			Gtk::CellRendererState flags);
	virtual bool activate_vfunc(GdkEvent* event, Gtk::Widget& widget, 
			const Glib::ustring& path, const Gdk::Rectangle& background_area,
			const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags);
public:
	CellRendererSimpleTreeText(guint col);
	Glib::PropertyProxy<guint> property_childrens_deep();
	Glib::PropertyProxy<guint> property_children_count();
};
  
class CellRendererSimpleTreeBool : public Gtk::CellRendererToggle
{protected:
	typedef Gtk::CellRendererToggle Parent;
	
	const guint column; // which column this is in
	Glib::Property<guint> childrens_deep; // the changing properties
	Glib::Property<guint> children_count;
	
	virtual void get_size_vfunc(Gtk::Widget& widget, const Gdk::Rectangle* cell_area, 
			int* x_offset, int* y_offset, int* width, int* height) const;
	virtual void render_vfunc(const Glib::RefPtr<Gdk::Drawable>& window,
			Gtk::Widget& widget, const Gdk::Rectangle& background_area, 
			const Gdk::Rectangle& cell_area, const Gdk::Rectangle& expose_area,
			Gtk::CellRendererState flags);
	virtual bool activate_vfunc(GdkEvent* event, Gtk::Widget& widget, 
			const Glib::ustring& path, const Gdk::Rectangle& background_area,
			const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags);
public:
	CellRendererSimpleTreeBool(guint col);
	Glib::PropertyProxy<guint> property_childrens_deep();
	Glib::PropertyProxy<guint> property_children_count();
};
  
#endif
