// $Id: IntStringBox.cc,v 1.5 2006/08/03 11:27:11 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 1998-2006 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include "IntStringBox.hh"
#include <Misc/itos.h>
#include <gtk/gtksignal.h>
#include <cassert>
#include <gtk/gtktable.h> // for changing expand
#include <gtkmm/table.h>
#include <Misc/Query.h>
#include <Misc/i18n.h>

#define ADDING_PATTERN (_("++ add to list ++"))

void IntStringBox::hide_int(bool b)
{
 if(b)
  { label_int->hide();
    sc_int->hide();
  }
 else
  { label_int->show();
    sc_int->show();
  }
}



void IntStringBox::show_string2(bool b)
{
 if(b)
   {
    label_string2->show();
    sc2_string->show();
   }
 else
   {
    label_string2->hide();
    sc2_string->hide();
   }
}

void IntStringBox::string2_info_only(bool b)
{
  sc2_string->set_sensitive(!b);
  string_2_info_only=b;
}

void IntStringBox::setLabel(const std::string &nr,const std::string &name,const std::string& namez)
{
 label_int->set_text(nr);
 label_string1->set_text(name);
 label_string2->set_text(namez);
}

void IntStringBox::int_activate()
{  // search Name and activate
   set_value(strtol(sc_int->get_text().c_str(),0,10));
   activate();
}

void IntStringBox::string1_activate()
{
  if (!string_to_add.empty() && can_add() && sc1_string->get_text()==ADDING_PATTERN)
  {
    sc1_string->set_text(string_to_add);
    id=add(sc1_string->get_text(),sc2_string->get_text());
    if (id!=ManuProcEntity<>::none_id)
    {
      sc_int->set_text(itos(id));
      activate();
      return;
    }
  }
  bool multiple=false;
  bool unique=try_to_get_id(multiple);
  if(!unique && sc2_string->is_visible())
   {  sc2_string->set_always_fill(true);
      sc2_string->reset();
      sc2_string->grab_focus();
   }
}

void IntStringBox::string2_activate()
{
  if (!string_to_add.empty() && can_add() && sc2_string->get_text()==ADDING_PATTERN)
  {
    sc2_string->set_text(string_to_add);
    id=add(sc1_string->get_text(),sc2_string->get_text());
    if (id!=ManuProcEntity<>::none_id)
    {
      sc_int->set_text(itos(id));
      activate();
      return;
    }
  }
  bool multiple=false;
  bool unique=try_to_get_id(multiple);
  if (multiple)
  {
    restricted_int_search=true;
    sc_int->set_always_fill(true);
    sc_int->reset();
    sc_int->grab_focus();
  }
  else if(!unique)
   {  sc1_string->set_always_fill(true);
      sc1_string->reset();
      sc1_string->grab_focus();
   }
}

gint IntStringBox::try_grab_focus(GtkWidget *w,gpointer gp)
{
   IntStringBox *this2((IntStringBox*)gp);
   assert(dynamic_cast<Gtk::EventBox*>(this2));
   this2->sc_int->grab_focus();
   return true;
}

IntStringBox::IntStringBox(ManuProcEntity<>::ID __none_id)
: id(__none_id),tr(std::string(),false), tr2(std::string(),false),
tr3(std::string(),false),
  eingeschraenkt(),
  string_2_info_only(),
  translate(), restricted_int_search(),prefix_only(true),
  _none_id_(__none_id)
{
 show_string2(false);
 // redirect our grab_focus
 gtk_signal_connect(GTK_OBJECT(gobj()), "grab_focus",
                 GTK_SIGNAL_FUNC (&try_grab_focus),(gpointer)this);
}

void IntStringBox::set_value(ManuProcEntity<>::ID i,const std::string &s,
				const std::string& sz)
{
   restricted_int_search=false;
   sc_int->set_text(itos(i));
   sc1_string->set_text(s);
   sc2_string->set_text(sz);
   id=i;
}

void IntStringBox::reset()
{
   restricted_int_search=false;
   prefix_only=true;
   sc_int->reset();
   sc1_string->reset();
   sc1_string->set_always_fill(false);
   sc2_string->reset();
   sc2_string->set_always_fill(false);
   id=_none_id_;
   reset_signal();

}

void IntStringBox::setSize(int int_width,bool int_expand,int width1,int width2)
{ sc_int->get_entry()->set_width_chars(int_width);
  sc1_string->get_entry()->set_width_chars(width1);
  sc2_string->get_entry()->set_width_chars(width2);
  // a bit hackish ...
  for (Gtk::Table::TableList::iterator i=table->children().begin();i!=table->children().end();++i)
  { if (i->get_widget()==sc_int && i->get_xexpand()!=int_expand)
      i->gobj()->xexpand=int_expand;
  }
}

void IntStringBox::Einschraenken(bool an)
{
  eingeschraenkt=true;
}

void IntStringBox::Einschraenkung(const std::string &e, bool an)
{
 restricted_int_search=false;
 eingeschraenkt=an;
 einschraenkung=e;
}

void IntStringBox::PrefixOnly(bool p)
{
 prefix_only=p;
}

void IntStringBox::Join(const std::string j)
{
 joinstring=j;
}


void IntStringBox::setExpandStr2(bool expand)
{
 sc2_string->set_autoexpand(expand);
 sc2_string->set_enable_tab(true);
}

void IntStringBox::setExpandStr1(bool expand)
{
 sc1_string->set_autoexpand(expand);
 sc1_string->set_enable_tab(true);
}

void IntStringBox::FocusToString1()
{
  sc1_string->grab_focus();
}

// Database interfacing
bool IntStringBox::try_to_get_id(bool & multiple)
{
 multiple=false;
 bool check_str2=(!_string2_.empty() && !string_2_info_only);

  std::string squery="select "+
	(_alias_.empty() ? _tabelle_ : _alias_)+"."+_int_;
  if(string_2_info_only) squery +=", "+_string2_ ;
  squery +=" from "+_tabelle_+" "+_alias_+" "+joinstring+" where lower("+_string1_+")=lower(?)";
  if(check_str2)
      squery   +=" and lower("+_string2_+")=lower(?)";

  if(eingeschraenkt)
    squery+=einschraenkung;

  Query q(squery);

  q << sc1_string->get_text();

  if(check_str2)
	q << sc2_string->get_text();

  if (!q.LinesAffected()) return false;
  if (q.LinesAffected()>1) { multiple=true; return false; }

  // This only works for a single line !
  Query::Row is = q >> id;
  sc_int->set_text(itos(id));
  if(string_2_info_only)
    {std::string s;
     is >> s;
     sc2_string->set_text(s);
    }

  activate();
  return true;

}


void IntStringBox::int_search(gboolean *_continue, GtkSCContext newsearch)
{

   try
   {
    static Transaction tr(std::string(),false);

      std::string squery;

      switch(newsearch)
      {
         case GTK_SEARCH_OPEN:
	   {tr.open();
	    Query::Args args;
           squery="declare int_search cursor for "
		  "select "+_tabelle_+"."+_int_+" from "+_tabelle_+
			" "+_alias_+" "+joinstring
                  +" where text("+_tabelle_+"."+_int_+") like ? ";
            args << ((prefix_only ? std::string() : std::string("%")) + sc_int->get_text()+'%');

	   if(eingeschraenkt)
	     squery+=einschraenkung;

	   if (restricted_int_search)
	   { squery += " and lower("+_string1_+")=lower(?) ";
	     squery += " and lower("+_string2_+")=lower(?) ";
	     args << sc1_string->get_text() << sc2_string->get_text();
	   }

	    squery+=" order by "+_tabelle_+"."+_int_;

            Query(squery) << args;
      	    // fall through
	   }
      	 case GTK_SEARCH_FETCH:
	   {
	    int intnr;
 	    Query q("fetch from int_search");
 	    q.FetchOne() >> intnr;
      	    sc_int->add_item(itos(intnr));
      	    *_continue=true;
      	    break;
	   }
      	 case GTK_SEARCH_CLOSE:
	   {
	    Query("close int_search");
      	    SQLerror::test(__FILELINE__);
      	    tr.commit();
      	    break;
	   }
      }
   }
   catch (SQLerror &e)
   {  std::cerr << e << '\n';
   }
}

void IntStringBox::string2_search(gboolean *_continue, GtkSCContext newsearch)
{
   try
   {
    static Transaction tr(std::string(),false);

     std::string squery;
     switch(newsearch)
      {  case GTK_SEARCH_OPEN:
            {
	    tr.open();

            // CP: Was soll das bedeuten?
	    bool equal_op = !sc1_string->get_text().empty();

             squery = "declare str2_search cursor for "
		"select distinct "+_string2_+" from "+_tabelle_+
		" "+_alias_+" "+
		joinstring+" where "
               +_string2_+" ilike ? " ;
             if(!_string1_.empty())
		{if(!equal_op)
		   squery += "and "+_string1_+" ilike ? ";
		 else
		   squery += "and lower("+_string1_+")=lower(?)";
		}

	   if(eingeschraenkt)
	     squery+=einschraenkung;

           squery +=(" order by "+_string2_);

	   Query q(squery);
	   std::string suchtext= sc2_string->get_text();
	   if (suchtext.find('%')!=std::string::npos || suchtext.find('?')!=std::string::npos || suchtext.find('_')!=std::string::npos)
           {
             for (size_t i=suchtext.find('?'); i!=std::string::npos; i=suchtext.find('?'))
               suchtext.replace(i,1,"%");
           }
           else
             suchtext=(prefix_only ? std::string() : std::string("%")) + suchtext +'%';
	   q << suchtext;

           if(!_string1_.empty())
	     {if(equal_op)
		q << sc1_string->get_text();
	      else
		q << (sc1_string->get_text()+'%');
	     }

      	    // fall through
	   }
      	 case GTK_SEARCH_FETCH:
	   {
	    std::string name;
 	    (Query("fetch from str2_search")).FetchOne() >> name;
      	    sc2_string->add_item(name);
      	    *_continue=true;
      	    string_to_add.clear();
      	    break;
	   }
      	 case GTK_SEARCH_CLOSE:
	   {
	    Query("close str2_search");
      	    SQLerror::test(__FILELINE__);
      	    tr.commit();
      	    break;
	   }
      }
   }
   catch (SQLerror &e)
   {
     if (e.Code()==100 && can_add())
     {
	  string_to_add=sc1_string->get_text();
	  sc1_string->add_item(ADDING_PATTERN);
     }
     else
       std::cerr << e << '\n';
   }
}


void IntStringBox::string1_search(gboolean *_continue, GtkSCContext newsearch)
{
   try
   {
    static Transaction tr(std::string(),false);

     std::string squery;
     switch(newsearch)
      {  case GTK_SEARCH_OPEN:
            {
	    tr.open();

	    bool equal_op = !sc2_string->get_text().empty();

             squery = "declare str1_search cursor for "
		"select distinct "+_string1_+" from "+
		_tabelle_+" "+_alias_+" "+
		joinstring+" where "
               +_string1_+" ilike ? " ;
             if(!_string2_.empty())
		{if(!equal_op)
		   squery += "and "+_string2_+" ilike ? ";
		 else
		   squery += "and lower("+_string2_+")=lower(?)";
		}

	   if(eingeschraenkt)
	     squery+=einschraenkung;

           squery +=(" order by "+_string1_);

	   Query q(squery);
	   std::string suchtext= sc1_string->get_text();
	   if (suchtext.find('%')!=std::string::npos || suchtext.find('?')!=std::string::npos || suchtext.find('_')!=std::string::npos)
           {
             for (size_t i=suchtext.find('?'); i!=std::string::npos; i=suchtext.find('?'))
               suchtext.replace(i,1,"%");
           }
           else
             suchtext=((prefix_only ? std::string() : std::string("%")) + suchtext +'%');
	   q << suchtext;

           if(!_string2_.empty())
	     {if(equal_op)
		q << sc2_string->get_text();
	      else
		q << (sc2_string->get_text()+'%');
	     }

      	    // fall through
	   }
      	 case GTK_SEARCH_FETCH:
	   {
	    std::string name;
 	    (Query("fetch from str1_search")).FetchOne() >> name;
      	    sc1_string->add_item(name);
      	    *_continue=true;
      	    string_to_add.clear();
      	    break;
	   }
      	 case GTK_SEARCH_CLOSE:
	   {
	    Query("close str1_search");
      	    SQLerror::test(__FILELINE__);
      	    tr.commit();
      	    break;
	   }
      }
   }
   catch (SQLerror &e)
   {
      if (e.Code()==100 && can_add())
      {
	string_to_add= sc1_string->get_text();
	sc1_string->add_item(ADDING_PATTERN);
      }
      else
       std::cerr << e << '\n';
   }
}


