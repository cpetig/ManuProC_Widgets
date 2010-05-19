// $Id: IntStringBox_sql.pgcc,v 1.1 2005/11/21 18:21:53 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++ Copyright (C)
 *  1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include "IntStringBox.hh"
#include <Misc/itos.h>
#include <Misc/Query.h>

bool IntStringBox::try_to_get_id()
{
 bool check_str2=(!_string2_.empty() && !string_2_info_only);

  std::string squery="select "+_tabelle_+"."+_int_;
  if(string_2_info_only) squery +=", "+_string2_ ;
  squery +=" from "+_tabelle_+joinstring+" where lower("+_string1_+")=lower(?)";
  if(check_str2) 
      squery   +=" and lower("+_string2_+")=lower(?)";

  if(eingeschraenkt)
    squery+=einschraenkung;

  Query q(squery);

  q << sc1_string->get_text();

  if(check_str2) 
	q << sc2_string->get_text();

  SQLerror::test(__FILELINE__,100);
  if (Query::Code()) return false;

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
    static Transaction tr("",false);

      std::string squery;

      switch(newsearch)
      {  
         case GTK_SEARCH_OPEN:
	   {tr.open();
           squery="declare int_search cursor for "
		  "select "+_tabelle_+"."+_int_+" from "+_tabelle_+joinstring
                  +" where text("+_tabelle_+"."+_int_+") like ? ";

	   if(eingeschraenkt)
	     squery+=einschraenkung;

	    squery+=" order by "+_tabelle_+"."+_int_;

            Query(squery) << (sc_int->get_text()+'%');
      	    // fall through
	   }
      	 case GTK_SEARCH_FETCH:
	   {
	    int intnr;
 	    (Query("fetch from int_search")).FetchOne() >> intnr;
      	    SQLerror::test(__FILELINE__,100);
      	    if (Query::Code()) return;
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
    static Transaction tr("",false);

     std::string squery;
     switch(newsearch)
      {  case GTK_SEARCH_OPEN:
            {
	    tr.open();

	    bool equal_op = !sc1_string->get_text().empty();

             squery = "declare str2_search cursor for "
		"select distinct "+_string2_+" from "+_tabelle_+
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
	   q << (sc2_string->get_text()+'%');
	     
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
      	    SQLerror::test(__FILELINE__,100);
      	    if (Query::Code()) return;
      	    sc2_string->add_item(name);
      	    *_continue=true;
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
   {  std::cerr << e << '\n';
   }
}


void IntStringBox::string1_search(gboolean *_continue, GtkSCContext newsearch)
{
   try
   {  
    static Transaction tr("",false);

     std::string squery;
     switch(newsearch)
      {  case GTK_SEARCH_OPEN:
            {
	    tr.open();

	    bool equal_op = !sc2_string->get_text().empty();

             squery = "declare str1_search cursor for "
		"select distinct "+_string1_+" from "+_tabelle_+
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
	   q << (sc1_string->get_text()+'%');
	     
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
      	    SQLerror::test(__FILELINE__,100);
      	    if (Query::Code()) return;
      	    sc1_string->add_item(name);
      	    *_continue=true;
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
   {  std::cerr << e << '\n';
   }
}


/* not Query::Row
void IntStringBox::string1_search(gboolean *_continue, GtkSCContext newsearch)
{
   exec sql begin declare section;
    char name[80];
    char query[1024];
   exec sql end declare section;
   
   try
   {  
     std::string squery;
     switch(newsearch)
      {  case GTK_SEARCH_OPEN:
            {
	     tr.open();
             squery = "declare str1_search "
		"select distinct "+_string1_+" from "+_tabelle_+
		  joinstring+" where "
                  +_string1_+" ilike '"+sc1_string->get_text() +"%' ";
             if(!_string2_.empty()) 
		{if(sc2_string->get_text().empty())
		  squery += "and "+_string2_+" ilike '"
	                  +sc2_string->get_text()+"%' ";
		 else
		  squery += "and lower("+_string2_+")=lower('"
        	          +sc2_string->get_text()+"') ";
		}

	   if(eingeschraenkt)
	     squery+=einschraenkung;

             squery +=(" order by "+_string1_);
             strncpy(query,squery.c_str(),sizeof(query));
             exec sql prepare string1_cursor_ from :query;
      	    exec sql declare string1_cursor cursor for string1_cursor_;
      	    exec sql open string1_cursor;
      	    SQLerror::test(__FILELINE__);
      	    // fall through
	   }
      	 case GTK_SEARCH_FETCH:
	   {
       	    exec sql fetch string1_cursor into :name;
      	    SQLerror::test(__FILELINE__,100);
      	    if (sqlca.sqlcode) return;
      	    sc1_string->add_item(name);
      	    *_continue=true;
      	    break;
	   }
      	 case GTK_SEARCH_CLOSE:
	   {
      	    exec sql close string1_cursor;
      	    SQLerror::test(__FILELINE__);
      	    tr3.commit();
      	    break;
	   }
      }
   }
   catch (SQLerror &e)
   {  std::cerr << e << '\n';
   }
}
*/


