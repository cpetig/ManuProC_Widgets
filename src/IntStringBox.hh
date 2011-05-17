// $Id: IntStringBox.hh,v 1.5 2006/08/03 11:27:11 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Malte Thoma
 *  Copyright (C) 2009-2010 Christof Petig
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

#ifndef _INTSTRINGBOX_HH
#  include "IntStringBox_gui.hh"
#  define _INTSTRINGBOX_HH

#include <Misc/Transaction.h>
#include <BaseObjects/ManuProcEntity.h>

class IntStringBox : public IntStringBox_gui
{
  friend class IntStringBox_gui;
        void int_activate();
        void int_search(gboolean *_continue, GtkSCContext newsearch);
        void string1_activate();
        void string1_search(gboolean *_continue, GtkSCContext newsearch);
        void string2_activate();
        void string2_search(gboolean *_continue, GtkSCContext newsearch);
        static gint try_grab_focus(GtkWidget *w,gpointer gp);

	ManuProcEntity<>::ID id;
	Transaction tr,tr2,tr3;

   std::string einschraenkung;
   std::string joinstring;

   bool eingeschraenkt:1;
   bool string_2_info_only:1;
   bool translate:1;
   bool restricted_int_search:1;


   bool try_to_get_id(bool &multiple);

protected:
   std::string _tabelle_,_alias_,_string1_,_string2_,_int_;
   int _none_id_;
   bool prefix_only:1;
   std::string string_to_add; //!< unfortunately selecting add voids the content, so we save it here
   //! overload and return true if adding is possible
   virtual bool can_add() const { return false; }
   //! add a new entry to the data, return new id
   virtual ManuProcEntity<>::ID add(std::string const&, std::string const&) { return ManuProcEntity<>::none_id; }

public:
	IntStringBox(ManuProcEntity<>::ID __none_id=ManuProcEntity<>::none_id);

   void setLabel(const std::string &nr,const std::string &name,const std::string& namez=std::string());

	ManuProcEntity<>::ID get_value() const { return id; }
	void set_value(ManuProcEntity<>::ID i,const std::string &s,const std::string &sz=std::string()) ;
	virtual void set_value(int i)=0 ;
   void hide_int(bool b);
   void show_string2(bool b);
//   void string2_set_sensitive(bool b);
   void string2_info_only(bool b);
   void clear(){reset();}
   void reset();
   void setExpandStr2(bool expand);
   void setExpandStr1(bool expand);
   void setSize(int int_width=3,bool int_expand=false,int width1=8,int width2=8);

   void Einschraenken(bool an);
   void Einschraenkung(const std::string &e, bool an=true);
   void Join(const std::string j,bool=false);
   void PrefixOnly(bool p);

   void FocusToString1();

private:
	sigc::signal<void> activate;
	sigc::signal<void> reset_signal;
public:
	sigc::signal<void> &signal_activate() { return activate; }
	sigc::signal<void> &signal_reset_signal() { return reset_signal; }
};
#endif
