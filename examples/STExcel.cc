// $Id: test_sts.cc,v 1.2 2004/05/05 13:31:53 christof Exp $
/*  ManuProcWidgets: ManuProC's GUI element library
 *  Copyright (C) 2012 Christof Petig
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

#include <Misc/TimeStamp.h>
#include <Misc/EntryValueTimeStamp.h>
#include <Misc/EntryValue_easy.h>
namespace EntryValue_easy
{
  cH_EntryValue create(ManuProC::TimeStamp const& a) { return cH_EntryValueTimeStamp(a); }
}

#include <SimpleTree.hh>
#include <RowDataBase_easy.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <Misc/Date.h>

typedef RowDataBase_easy<int,std::string,ManuProC::TimeStamp,bool> row_t;

namespace ManuProC {
template <>
TimeStamp parse<TimeStamp>(std::string const& x)
{
  return TimeStamp(x.c_str());
}
}

int main(int argc, char**argv)
{
  Gtk::Main m(argc,argv);
  Gtk::Window w;
  SimpleTree tree(4);
  w.add(tree);

  std::vector<std::string> headers;
  headers.push_back("Integer");
  headers.push_back("String");
  headers.push_back("Time");
  headers.push_back("Boolean");
  tree.setTitles(headers);
  tree.getStore()->set_column_type(3, SimpleTreeModel_Properties::ct_bool);
  std::vector<gfloat> align(4);
  align[0]=1.0;
  tree.getStore()->setAlignment(align);
  tree.getStore()->set_editable(0, true);
  tree.getStore()->set_editable(3, true);

  std::vector<cH_RowDataBase> vec;
  vec.push_back(new row_t(987654321,"eins",ManuProC::TimeStamp(ManuProC::Date(1,2,2012),12,0),true));
  vec.push_back(new row_t(1,"zwei",ManuProC::TimeStamp("2012-06-01 12:00:00+2"),false));
  tree.setDataVec(vec);

  tree.show();
  w.show();
//  m.run(w);

  tree.write_excel("STExcel.xls");
  return 0;
}
