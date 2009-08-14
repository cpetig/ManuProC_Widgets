/* $Id: treebase_data.h,v 1.9 2003/10/07 06:49:57 christof Exp $ */
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2009 Christof Petig
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

#ifndef KOMPONENTEN_TREEBASE_DATA_H
#define KOMPONENTEN_TREEBASE_DATA_H

// use it like:
//   typedef RowDataBase_easy<int,std::string,std::string> myrow_t;
//   cH_RowDataBase d= new myrow_t(0,"intern","intern");
//    ...
//   int value= d.cast_dynamic<myrow_t const>()->get_0();
//
// or if you don't need to access your data often
//   cH_RowDataBase d= cH_RowDataBase_easy(0,std::string("intern"),std::string("intern"));
//
// or
//   cH_RowDataBase d= cH_RowDataBase_easy<int,std::string,std::string>(0,"intern","intern");
//
//   int value= d.cast_dynamic<RowDataBase_easy<int,std::string,std::string> const>()->get_0();
//

#include <Misc/EntryValue_easy.h>
#include <RowDataBase.h>

template <typename A, typename B=EntryValue_easy::nil, typename C=EntryValue_easy::nil, 
          typename D=EntryValue_easy::nil, typename E=EntryValue_easy::nil, 
          typename F=EntryValue_easy::nil, typename G=EntryValue_easy::nil> 
class RowDataBase_easy : public RowDataBase
{// the data 
 A a;
 B b;
 C c;
 D d;
 E e;
 F f;
 G g;
public:

 RowDataBase_easy(A const &_a, B const &_b=B(), C const &_c=C(), D const &_d=D(), E const&_e=E(), F const &_f=F(), G const &_g=G())
  : a(_a), b(_b), c(_c), d(_d), e(_e), f(_f), g(_g)
 { }

 typedef A type_0;
 typedef B type_1;
 typedef C type_2;
 typedef D type_3;
 typedef E type_4;
 typedef F type_5;
 typedef F type_6;

 A const& get_0() const { return a; }
 B const& get_1() const { return b; }
 C const& get_2() const { return c; }
 D const& get_3() const { return d; }
 E const& get_4() const { return e; }
 F const& get_5() const { return f; }
 G const& get_6() const { return g; }

 virtual cH_EntryValue Value(guint _seqnr, gpointer gp) const
 {
   switch(_seqnr)
   { case 0: return EntryValue_easy::create(a);
     case 1: return EntryValue_easy::create(b);
     case 2: return EntryValue_easy::create(c);
     case 3: return EntryValue_easy::create(d);
     case 4: return EntryValue_easy::create(e);
     case 5: return EntryValue_easy::create(f);
     case 6: return EntryValue_easy::create(g);
     default: return cH_EntryValue();
   }
 }
};

template <typename A>
cH_RowDataBase cH_RowDataBase_easy(A const &a)
{ return new RowDataBase_easy<A>(a);
}

template <typename A, typename B>
cH_RowDataBase cH_RowDataBase_easy(A const &a, B const&b)
{ return new RowDataBase_easy<A,B>(a,b);
}

template <typename A, typename B, typename C>
cH_RowDataBase cH_RowDataBase_easy(A const &a, B const&b, C const&c)
{ return new RowDataBase_easy<A,B,C>(a,b,c);
}

template <typename A, typename B, typename C, typename D> 
cH_RowDataBase cH_RowDataBase_easy(A const &a, B const &b, C const &c, D const &d)
{ return new RowDataBase_easy<A,B,C,D>(a,b,c,d);
}

template <typename A, typename B, typename C, typename D, typename E> 
cH_RowDataBase cH_RowDataBase_easy(A const &a, B const &b, C const &c, D const &d, E const&e)
{ return new RowDataBase_easy<A,B,C,D,E>(a,b,c,d,e);
}

template <typename A, typename B, typename C, typename D, typename E, typename F> 
cH_RowDataBase cH_RowDataBase_easy(A const &a, B const &b, C const &c, D const &d, E const&e, F const &f)
{ return new RowDataBase_easy<A,B,C,D,E,F>(a,b,c,d,e,f);
}

template <typename A, typename B, typename C, typename D, typename E, typename F, typename G> 
cH_RowDataBase cH_RowDataBase_easy(A const &a, B const &b, C const &c, D const &d, E const&e, F const &f, G const &g)
{ return new RowDataBase_easy<A,B,C,D,E,F,G>(a,b,c,d,e,f,g);
}

// helper class to match against first column (use with selectFirstMatchingLine)
template <class T>
 struct RDB_easy_comp_0 { typename T::type_0 key0; RDB_easy_comp_0(typename T::type_0  const& s) : key0(s) {} };

template <class T>
bool operator==(cH_RowDataBase const& r, RDB_easy_comp_0<T> const& key)
{ return r.cast_static<T>()->get_0()==key.key0;
}

#endif
