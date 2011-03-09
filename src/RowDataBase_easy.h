/* $Id: treebase_data.h,v 1.9 2003/10/07 06:49:57 christof Exp $ */
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2009-2011 Christof Petig
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
#include <Misc/create_parse.h>
#include <RowDataBase.h>
#include <SimpleTree.hh>

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
 typedef RowDataBase_easy<A,B,C,D,E,F,G> this_type;

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

 void set_0(A const& x) { a=x; }
 void set_1(B const& x) { b=x; }
 void set_2(C const& x) { c=x; }
 void set_3(D const& x) { d=x; }
 void set_4(E const& x) { e=x; }
 void set_5(F const& x) { f=x; }
 void set_6(G const& x) { g=x; }

 template <typename X>
  void update_and_select(SimpleTree *tree, void (this_type::*set)(X const&), X const& val)
	{
		// quite a complicated way to cause an update to the list
		cH_RowDataBase h(this);
		tree->getModel().about_to_change(h); // causes unselection
		(this->*set)(val);
		tree->getModel().has_changed(h);
		tree->select(h);
	}

 void update_and_select_0(SimpleTree *tree, A const& x) { update_and_select(tree, &this_type::set_0, x); }
 void update_and_select_1(SimpleTree *tree, B const& x) { update_and_select(tree, &this_type::set_1, x); }
 void update_and_select_2(SimpleTree *tree, C const& x) { update_and_select(tree, &this_type::set_2, x); }
 void update_and_select_3(SimpleTree *tree, D const& x) { update_and_select(tree, &this_type::set_3, x); }
 void update_and_select_4(SimpleTree *tree, E const& x) { update_and_select(tree, &this_type::set_4, x); }
 void update_and_select_5(SimpleTree *tree, F const& x) { update_and_select(tree, &this_type::set_5, x); }
 void update_and_select_6(SimpleTree *tree, G const& x) { update_and_select(tree, &this_type::set_6, x); }

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
 virtual bool changeValue(guint _seqnr,gpointer _g, const Glib::ustring &newvalue)
 {
   try
   {
     switch(_seqnr)
     { case 0: a = ManuProC::parse<A>(newvalue); return true;
       case 1: b = ManuProC::parse<B>(newvalue); return true;
       case 2: c = ManuProC::parse<C>(newvalue); return true;
       case 3: d = ManuProC::parse<D>(newvalue); return true;
       case 4: e = ManuProC::parse<E>(newvalue); return true;
       case 5: f = ManuProC::parse<F>(newvalue); return true;
       case 6: g = ManuProC::parse<G>(newvalue); return true;
     }
   }
   catch (...) 
   {}
   return false;
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
