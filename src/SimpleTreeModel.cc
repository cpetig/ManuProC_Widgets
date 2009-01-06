// $Id: SimpleTreeModel.cc,v 1.6 2004/12/04 10:53:34 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2002-2005 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *  Copyright (C) 2008-2009 Christof Petig
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

#include <SimpleTreeModel.h>
#include <algorithm>

void SimpleTreeModel::append_line(const cH_RowDataBase &row)
{  datavec.push_back(row);
   line_appended(row);
}

namespace { struct remove_line_compare {
	cH_RowDataBase a;
	bool operator()(const cH_RowDataBase &b) { return &*a == &*b; }
	remove_line_compare(const cH_RowDataBase &_a) : a(_a) {}
};}

void SimpleTreeModel::remove_line(const cH_RowDataBase &row)
{  line_to_remove(row);
   // I'd also have to call erase if I used remove_if()
   datavec_t::iterator i=find_if(datavec.begin(),datavec.end(),remove_line_compare(row));
   if (i!=datavec.end()) datavec.erase(i);
}

void SimpleTreeModel::clear()
{  please_detach();
   datavec.clear();
   please_attach();
}
	
void SimpleTreeModel::setDataVec(const std::vector<cH_RowDataBase> &d)
{  please_detach();
   datavec=d;
   please_attach();
}

// this operates on (content) equivalence, not (address) identity
void SimpleTreeModel::changeDataVec(const std::vector<cH_RowDataBase> &d, bool (*equal)(cH_RowDataBase const& a,cH_RowDataBase const& b))
{
   std::vector<cH_RowDataBase> to_add; // so that we do not have to search the newly added lines, too
   // this would get faster if we remove matching lines once found and add them again later
   // lines in datavec, but not in d: remove
   for (std::vector<cH_RowDataBase>::iterator j=datavec.begin(); j!=datavec.end();)
   {
     std::vector<cH_RowDataBase>::const_iterator i=d.begin();
     for (; i!=d.end() && !(*equal)(*i,*j); ++i);
     if (i==d.end())
     { line_to_remove(*j);
       j=datavec.erase(j);
     }
     else ++j;
   }
   // lines in d, but not in datavec: add
   for (std::vector<cH_RowDataBase>::const_iterator i=d.begin(); i!=d.end(); ++i)
   {
     std::vector<cH_RowDataBase>::const_iterator j=datavec.begin();
     for (; j!=datavec.end() && !(*equal)(*i,*j); ++j);
     if (j==datavec.end()) to_add.push_back(*i);
   }
   for (std::vector<cH_RowDataBase>::const_iterator i=to_add.begin(); i!=to_add.end(); ++i)
      push_back(*i);
}
