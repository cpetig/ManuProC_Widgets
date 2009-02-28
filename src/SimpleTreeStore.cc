// $Id: SimpleTreeStore.cc,v 1.119 2006/08/09 11:12:16 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2002-2005 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *  Copyright (C) 2006 Christof Petig
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

#include <SimpleTreeStore.h>
#include <Misc/Global_Settings.h>
#if defined __MINGW32__ || defined WIN32
# define getuid() 0
#else
# include <unistd.h> // getuid
#endif
#include <Misc/itos.h>
#include <Misc/TraceNV.h>
//#include <GType_cH_EntryValue.h>
#include <gtkmm/treepath.h>
#include <Misc/EntryValueSort.h>
#include <Misc/EntryValueInvert.h>

#ifdef __MINGW32__
#define getuid() 0
#endif

#ifdef ST_DEPRECATED
struct SimpleTreeModel_Properties_Proxy::Standard : public SimpleTreeModel_Properties
{	unsigned columns;
	std::vector<std::string> titles;
	std::vector<bool> column_editable;
	std::vector<column_type_t> column_type;
	SimpleTreeModel_Properties_Proxy::NewNode_fp node_creation;
	gpointer gp;
	std::string mem_prog,mem_inst;
	std::vector<gfloat> alignment;
	std::vector<bool> v_resizeable;

  Standard(guint cols) : columns(cols), titles(cols), 
      column_editable(cols), node_creation(), 
      gp(), alignment(cols), v_resizeable(cols,true) {}
  virtual unsigned Columns() const { return columns; }
  virtual gpointer ValueData() const { return gp; }
  virtual Glib::ustring Title(guint _seqnr) const { return titles[_seqnr]; }
  virtual gfloat Alignment(guint _seqnr) const { return alignment[_seqnr]; }
  virtual bool editable(guint _seqnr) const { return column_editable[_seqnr]; }
  virtual Handle<TreeRow> create_node(const Handle<const TreeRow> &suminit) const
  { if (node_creation) return (*node_creation)(suminit);
    else return Handle<TreeRow>(); 
  }
  virtual column_type_t get_column_type(unsigned idx) const
  { return column_type.at(idx); }
  virtual std::string ProgramName() const { return mem_prog; }
  virtual std::string InstanceName() const { return mem_inst; }
  virtual bool resizeable(guint _seqnr) const { return v_resizeable.at(_seqnr); }
  
	__deprecated void setTitles(const std::vector<std::string> &T)
	{ titles=T;
	}
	__deprecated void setTitleAt(unsigned idx,const std::string &s)
	{ titles.at(idx)=s; }
	__deprecated void set_editable(unsigned idx,bool v=true)
	{ column_editable.at(idx)=v; 
	}
	__deprecated void set_column_type(unsigned idx, column_type_t t)
	{ column_type.at(idx)=t; }
	__deprecated void set_value_data(gpointer _p) {gp = _p;}
	__deprecated void set_remember(const std::string &program, const std::string &instance)
	{ mem_prog=program; mem_inst=instance; }
	__deprecated void set_NewNode(NewNode_fp n)
	{  node_creation=n; }
	__deprecated void setAlignment(const std::vector<gfloat> &A)
	{ assert(A.size()==Columns());
	  alignment=A;
	}
	__deprecated void setResizeable(const std::vector<bool> &R)
	{ assert(R.size()==Columns());
	  v_resizeable=R;
	}
};

SimpleTreeModel_Properties_Proxy::Standard &SimpleTreeModel_Properties_Proxy::stdProperties()
{ return dynamic_cast<SimpleTreeModel_Properties_Proxy::Standard&>(*props);
}

void SimpleTreeModel_Properties_Proxy::set_NewNode(NewNode_fp n)
{ stdProperties().set_NewNode(n);
}

void SimpleTreeModel_Properties_Proxy::set_value_data(gpointer p)
{ stdProperties().set_value_data(p);
}

void SimpleTreeModel_Properties_Proxy::setTitles(const std::vector<std::string> &T)
{  stdProperties().setTitles(T);
   column_changed(SimpleTreeStore::invisible_column);
}

void SimpleTreeStore::set_remember(const std::string &program, const std::string &instance)
{ if (Properties().ProgramName()!=program || Properties().InstanceName()!=instance)
  { stdProperties().set_remember(program,instance);
    load_remembered();
  }
}

void SimpleTreeModel_Properties_Proxy::setTitleAt(unsigned idx, const std::string &s)
{ assert(idx<Properties().Columns());
  stdProperties().setTitleAt(idx,s);
  column_changed(idx);
}

void SimpleTreeModel_Properties_Proxy::set_editable(unsigned idx,bool v)
{  assert(idx<Properties().Columns());
   stdProperties().set_editable(idx,v);
   // might not be enough
   column_changed(idx);
}

void SimpleTreeModel_Properties_Proxy::setResizeable(const std::vector<bool> &R)
{ assert(R.size()==Properties().Columns());
  stdProperties().setResizeable(R);
  column_changed(SimpleTreeStore::invisible_column);
}

void SimpleTreeModel_Properties_Proxy::setAlignment(const std::vector<gfloat> &R)
{ assert(R.size()==Properties().Columns());
  stdProperties().setAlignment(R);
  column_changed(SimpleTreeStore::invisible_column);
}
#else // !deprecated
struct SimpleTreeModel_Properties_Proxy::Standard : public SimpleTreeModel_Properties
{ unsigned cols;
  Standard(unsigned c) : cols(c) {}
  virtual unsigned Columns() const
  { return cols; }
  virtual Glib::ustring Title(guint _seqnr) const
  { return "title"; }
};
#endif // deprecated

SimpleTreeModel_Properties_Proxy::SimpleTreeModel_Properties_Proxy(unsigned x)
: props(new SimpleTreeModel_Properties_Proxy::Standard(x)), we_own_props(true)
{}

void SimpleTreeModel_Properties_Proxy::setProperties(SimpleTreeModel_Properties &p, bool we_own)
{ assert(p.Columns()==props->Columns());
  // the number of TreeModelColumns can not easily be changed
  if (we_own_props) 
    delete props;
  props=&p;
  we_own_props=we_own;
  props->column_changed.connect(column_changed.make_slot());
}

void SimpleTreeStore::setProperties(SimpleTreeModel_Properties &p, bool we_own)
{ SimpleTreeModel_Properties_Proxy::setProperties(p, we_own);
  load_remembered();
  column_changed(invisible_column);
}

SimpleTreeModel_Properties_Proxy::~SimpleTreeModel_Properties_Proxy()
{ delete props;
}

void SimpleTreeModel_Proxy::setModel(SimpleTreeModel &_model)
{  if (model_is_ours) { delete model; model_is_ours=false; }
   model=&_model;
}

SimpleTreeModel_Proxy::SimpleTreeModel_Proxy()
	: model(new SimpleTreeModel), model_is_ours(true)
{}

SimpleTreeModel_Proxy::~SimpleTreeModel_Proxy()
{  if (model_is_ours) { delete model; model_is_ours=false; }
}

// =========================================================

std::pair<std::string,std::string> SimpleTreeStore::default_load(const std::string&program, const std::string&instance)
{  std::pair<std::string,std::string> result;
   if (!program.empty())
   {  result.second=Global_Settings(getuid(),program,instance+":visible").get_Wert();
      result.first=Global_Settings(getuid(),program,instance+":order").get_Wert();
   }
   return result;
}

void SimpleTreeStore::default_save(const std::string&program, const std::string&instance, const std::pair<std::string,std::string>&value)
{
   if (!program.empty())
   {  Global_Settings::create(getuid(),program,instance+":visible",value.second);
      Global_Settings::create(getuid(),program,instance+":order",value.first);
   }
}

void SimpleTreeStore::save_remembered() const
{  std::pair<std::string,std::string> value;

   if (block_save || (ProgramName().empty() && InstanceName().empty())) return;
   unsigned int sichtbar=0,bit=1;
   for (std::vector<bool>::const_iterator i=vec_hide_cols.begin();
   		bit && i!=vec_hide_cols.end();++i,bit<<=1)
      if (!*i) sichtbar|=bit;
   std::string flags;
//   if (!titles_bool) flags+='T';
   if (auffuellen_bool) flags+='a';
   if (!expandieren_bool) flags+='E';
   if (!color_bool) flags+='C';
   if (display_count) flags+='o';
   value.second=itos(sichtbar)+','+itos(showdeep)+','+flags;

   guint last=invisible_column;
   sequence_t::const_reverse_iterator i=currseq.rbegin();
   for (;i!=currseq.rend();++i)
   {  if (*i>last) break;
      last=*i;
   }
   for (;i!=currseq.rend();++i) value.first=itos(*i)+','+value.first;
   
   // additional flags from derived widgets?

   default_save(ProgramName(), InstanceName(), value);
}

void SimpleTreeStore::load_remembered()
{  std::pair<std::string,std::string> value;
   if (!ProgramName().empty() || !InstanceName().empty())
     value=default_load(ProgramName(), InstanceName());

   expandieren_bool=value.second.find('E')==std::string::npos;
   color_bool=value.second.find('C')==std::string::npos;
   auffuellen_bool=value.second.find('a')!=std::string::npos;
   display_count=value.second.find('o')!=std::string::npos;
   
   block_save=true;
   // ShowColumn will signal views (and update the menu)
   std::string::size_type k0=value.second.find(','),k1=std::string::npos;
   if (k0!=std::string::npos) 
   {  guint sichtbar=strtoul(value.second.substr(0,k0).c_str(),0,10),bit=1;
      for (guint j=0;j<MaxCol();++j,bit<<=1)
         ShowColumn(j)=!bit ? Properties().visible_default(j) : !(sichtbar&bit);
      k1=value.second.find(',',k0+1);
   }
   else // kein Wert gesetzt
   {  for (guint j=0;j<MaxCol();++j)
      {  ShowColumn(j)=Properties().visible_default(j);
      }
   }
   if (k1!=std::string::npos)
   {  showdeep=strtoul(value.second.substr(k0+1,k1-(k0+1)).c_str(),0,10);
   }

   sequence_t s;
   for (std::string::size_type b=0;;)
   {  std::string::size_type e=value.first.find(',',b);
      if (e==std::string::npos) break;
      s.push_back(strtoul(value.first.substr(b,e-b).c_str(),0,10));
      b=e+1;
   }
   fillSequence(s,true);

   // adapt Menu and display
   setSequence(s);
   signal_save(0);
   block_save=false;
}

static const unsigned col1=0xffff,col0=0xcfff;

void SimpleTreeStore::on_visibly_changed(bvector_iterator it)
{ if (block_save) return;
  if (it!=bvector_iterator())
  {if (!*it) // Spalte versteckt
   {  for (sequence_t::iterator i=currseq.begin();i!=currseq.end();)
      {  if (!ColumnVisible(*i))
         {  i=currseq.erase(i);
         }
         else ++i;
      }
   }
   else // Spalte hinzugekommen
   {
#if 0 // old behaviour
      fillSequence();
#else
    for(guint i=0; i<MaxCol(); ++i) 
      if (ColumnVisible(i) && std::find(currseq.begin(),currseq.end(),i)==currseq.end())
      {  for (sequence_t::iterator j=currseq.begin();j!=currseq.end();++j)
         {  if (*j>i)
            {  currseq.insert(j,i);
               break;
            }
         }
         if (std::find(currseq.begin(),currseq.end(),i)==currseq.end()) //immer noch nicht da
            currseq.push_back(i);
      }
#endif
   }
  }
   bool optimize= it!=bvector_iterator();
   setSequence(currseq, optimize);
}

void SimpleTreeStore::init()
{
  //We need to specify a particular get_type() from one of the virtual base classes, but they should
  //both return the same piece of data.
  {static bool inited=false;
   if (!inited)
   { Gtk::TreeModel::add_interface( Glib::Object::get_type() );
     inited=true;
   }
  }
  vec_hide_cols.resize(MaxCol());
  invert_order.resize(MaxCol());
  for (std::vector<bool>::iterator i=vec_hide_cols.begin();i!=vec_hide_cols.end();++i)
    (*i) = true;
   defaultSequence();
   getModel().signal_please_detach().connect(please_detach.make_slot());
   getModel().signal_please_attach().connect(sigc::mem_fun(*this,&SimpleTreeStore::redisplay));
   getModel().signal_line_appended().connect(sigc::mem_fun(*this,&SimpleTreeStore::on_line_appended));
   getModel().signal_line_to_remove().connect(sigc::mem_fun(*this,&SimpleTreeStore::on_line_removed));
   getModel().signal_value_changed().connect(sigc::mem_fun(*this,&SimpleTreeStore::value_change_impl));
   signal_save.connect(sigc::mem_fun(*this,&SimpleTreeStore::save_remembered1));
   signal_redisplay_save.connect(sigc::mem_fun(*this,&SimpleTreeStore::save_and_redisplay));
   signal_visibly_changed.connect(sigc::mem_fun(*this,&SimpleTreeStore::on_visibly_changed));
  Gdk::Color c;
  c.set_rgb(col1,col1,col1); colors.push_back(c); // white
  c.set_rgb(col1,col0,col0); colors.push_back(c); // red
  c.set_rgb(col1,col1,col0); colors.push_back(c); // yellow
  c.set_rgb(col0,col1,col0); colors.push_back(c); // green
  c.set_rgb(col0,col1,col1); colors.push_back(c); // cyan
  c.set_rgb(col0,col0,col1); colors.push_back(c); // blue
  c.set_rgb(col1,col0,col1); colors.push_back(c); // magenta
  c.set_rgb(col0,col0,col0); colors.push_back(c); // dark grey
  assert(colors.size()==num_colors);
  load_remembered();
}

SimpleTreeStore::SimpleTreeStore(int max_col)
	: 
	  Glib::ObjectBase( typeid(SimpleTreeStore) ), //register a custom GType.
	  Glib::Object(), //The custom GType is actually registered here.
          SimpleTreeModel_Properties_Proxy(max_col),
          columns(max_col), max_column(max_col),
	  showdeep(), auffuellen_bool(), expandieren_bool(true), block_save(),
	  color_bool(true), display_count(),
	  sortierspalte(invisible_column), // invert_sortierspalte(), 
	  stamp(reinterpret_cast<long>(this)),
	  m_columns(max_col)
{ init();
}

SimpleTreeStore::SimpleTreeStore(SimpleTreeModel_Properties &props)
	: 
	  Glib::ObjectBase( typeid(SimpleTreeStore) ), //register a custom GType.
	  Glib::Object(), //The custom GType is actually registered here.
          SimpleTreeModel_Properties_Proxy(&props),
          columns(props.Columns()), max_column(props.Columns()),
	  showdeep(), auffuellen_bool(), expandieren_bool(true), block_save(),
	  color_bool(true), display_count(),
	  sortierspalte(invisible_column), // invert_sortierspalte(), 
	  stamp(reinterpret_cast<long>(this)),
	  m_columns(props.Columns())
{ init();
}

// eigentlich müssen versteckte Spalten gar nicht hinzugenommen werden
// aber wie dann das Model neu aufbauen?

#define MC_ADD(name) add(name); assert(name.index()==int(s_##name));

SimpleTreeStore::ModelColumns::ModelColumns(int _cols)
{  // GType t=cH_entry_value_get_type();
   
   MC_ADD(row);
   MC_ADD(deep);
   MC_ADD(childrens_deep);
//   MC_ADD(value);
   MC_ADD(leafdata);
   MC_ADD(background);
   MC_ADD(children_count);
   for (int i=0; i<_cols; ++i)
   {  cols.push_back(Gtk::TreeModelColumn<Glib::ustring>());
      add(cols.back());
      assert(cols.back().index()==int(s_text_start)+i);
   }
}

#if 0
void SimpleTreeStore::on_column_changed(guint idx)
{ if (idx==invisible_column) column_changed(idx);
  { unsigned col=ColumnFromIndex(idx);
    ManuProC::Trace(trace_channel,__FUNCTION__,idx,col);
    if (col!=invisible_column) column_changed(col);
  }
}
#endif

const std::string SimpleTreeStore::getColTitle(guint nr) const
{  return SimpleTreeModel_Properties_Proxy::getColTitle(currseq[nr])+SpaltenMarkierung(currseq[nr]);
}

void SimpleTreeStore::defaultSequence()
{  currseq.clear();
   for(guint i=0; i<MaxCol(); ++i) 
      if (ColumnVisible(i)) currseq.push_back(i);
}

void SimpleTreeStore::fillSequence(sequence_t &seq,bool standard) const
{  if (auffuellen_bool || standard)
   {for(guint i=0; i<MaxCol(); ++i) 
      if (ColumnVisible(i) && std::find(seq.begin(),seq.end(),i)==seq.end())
         seq.push_back(i);
   }
   else
   {  for (sequence_t::const_iterator i=currseq.begin();i!=currseq.end();++i)
         if (ColumnVisible(*i) && std::find(seq.begin(),seq.end(),*i)==seq.end())
            seq.push_back(*i);
   }
}

#ifdef MPC_STSN_MAGIC
# define STSN_CHECK_MAGIC(x) assert(!(x) || (x)->magic==MPC_STSN_MAGIC)
#else
# define STSN_CHECK_MAGIC(x)
#endif

void SimpleTreeStore::resort(Node &parent, unsigned stop_at)
{  STSN_CHECK_MAGIC(&parent);
   ManuProC::Trace(trace_channel,__FUNCTION__,&parent,stop_at);
   SimpleTreeStoreNode::map_t swapmap;
   std::swap(swapmap,parent.children);
   for (SimpleTreeStoreNode::map_t::iterator i=swapmap.begin();i!=swapmap.end();++i)
   {  STSN_CHECK_MAGIC(&i->second);
      STSN_CHECK_MAGIC(i->second.parent);
      cH_EntryValue x=i->first;
      cH_EntryValue y,z;
      if (i->second.row) y=i->second.row->Value(sortierspalte,ValueData());
      else y=i->second.leafdata->Value(sortierspalte,ValueData());
      if (invert_order.at(sortierspalte)) z=cH_EntryValueReverseSort(y,x);
      else z=cH_EntryValueSort(y,x);
      Node &nd=parent.children.insert(parent.children.upper_bound(z),std::make_pair(z,Node()))->second;
      STSN_CHECK_MAGIC(&nd);
      std::swap(i->second,nd);
      nd.parent=&parent;
      if (nd.childrens_deep<stop_at) resort(nd,stop_at);
   }
}

void SimpleTreeStore::redisplay()
{
 please_detach();
 ++stamp;
// liste loeschen
 root.children.clear();

 SimpleTreeModel::const_iterator i=getModel().begin();
 SimpleTreeModel::const_iterator j=getModel().end();

// neu einordnen, Summen berechnen
 for(; i!=j; ++i)
    insertLine(root,*i,currseq.begin(),0,false);
 if (sortierspalte!=invisible_column)
 {  sequence_t::const_iterator si=std::find(currseq.begin(),currseq.end(),sortierspalte); 
    resort(root,si-currseq.begin());
    root.fix_pointer();
//    test();
 }
// needs_redisplay();
 please_attach();
}

void SimpleTreeStore::on_line_appended(cH_RowDataBase row)
{  ManuProC::Trace(trace_channel,__FUNCTION__,row->Value(0,ValueData())->getStrVal(),
		row->Value(1,ValueData())->getStrVal());
   ++stamp;
   assert(sortierspalte==invisible_column);
   insertLine(root,row,currseq.begin(),0,true);
}

// these macros are a lot faster than x.size() because that needs a division
#define KeinKind(x) ((x).begin()==(x).end())
#define NurEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) == (x).end())
#define MehrAlsEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) != (x).end())

void SimpleTreeStore::insertLine(Node &parent,
            const cH_RowDataBase &v,  sequence_t::const_iterator seqit,
            guint deep, bool live)
{STSN_CHECK_MAGIC(&parent);
 STSN_CHECK_MAGIC(parent.parent);
 if (seqit==currseq.end()) return;

 iterator current_iter=parent.children.begin();
 iterator apiend = parent.children.end();
 iterator upper_b=apiend;
 sequence_t::const_iterator seqlast=--currseq.end();
 guint seqnr=*seqit;
 cH_EntryValue ev=v->Value(seqnr,ValueData());
 if (invert_order[seqnr]) ev=cH_EntryValueInvert(ev);
 
// node/leaf mit Wert<=ev suchen
// optimization: we expect to need upper_bound if this is the last attribute
 if (seqit==seqlast) // !MehrAlsEinKind(selseq))
 {  std::pair<iterator,iterator> range 
 		= parent.children.equal_range(ev);
    current_iter=range.first;	// lower_bound
    upper_b=range.second;	// upper_bound
 }
 else
    current_iter=parent.children.lower_bound(ev);
 				
 if(current_iter!=apiend) // dann einfuegen
   {STSN_CHECK_MAGIC(&current_iter->second);
    STSN_CHECK_MAGIC(current_iter->second.parent);
    //----------------- gleicher Wert ------------------
    if ((ev) == current_iter->first)
     { 
      if (seqit!=seqlast)
      // MehrAlsEinKind(selseq)) // ???? wenn Blatt noch nicht erreicht ???
      				// SimpleTree2: Summen stehen mit drin ...
      // eine neue Node erzeugen(?)
      {  cH_RowDataBase v2=current_iter->second.leafdata;
         guint child_s_deep=deep;

	do 
	{seqit++;
	 ++child_s_deep;
	 
	 // darum muss sich eine andere Node kümmern
         if (child_s_deep==current_iter->second.childrens_deep)
         {weiter_unten_einhaengen:
            Handle<TreeRow> htr=current_iter->second.row;
            if (htr) 
            {  htr->cumulate(v);
               if (live) row_changed(getPath(current_iter),getIter(current_iter));
            }
            // goto ist schneller als (end?)rekursion ?
            insertLine(current_iter->second,v,seqit,child_s_deep,live);
            return;
         }
         
        } while (seqit!=seqlast // MehrAlsEinKind(selseq) 
			&& v->Value(*seqit,ValueData())
				==v2->Value(*seqit,ValueData()));
         
	 // mitten in current_iter einfügen 
	 // (current_iter wandert nach unten rechts)
         // (man könnte dies auch aufbrechen nennen)
 	 current_iter= MoveTree (current_iter,deep,child_s_deep,*seqit,live);
 	 STSN_CHECK_MAGIC(&current_iter->second);
 	 STSN_CHECK_MAGIC(current_iter->second.parent);
         goto weiter_unten_einhaengen;
      }
      else // Blatt erreicht
      {  // als letztes der Gleichen an parent anhängen
         // upper_b steht schon richtig (s.o.)
         current_iter=upper_b;
      }
     }
   }
   // --------------- kleinerer Wert (davor Einfügen) ----------   
   //----------------- oder am Ende der Liste: anhängen ---------------------
   iterator newnode=parent.children.insert(current_iter,std::make_pair(ev,Node(deep,&parent,v)));
   STSN_CHECK_MAGIC(&newnode->second);
   STSN_CHECK_MAGIC(newnode->second.parent);
   if (live) row_inserted(getPath(newnode),getIter(newnode));
}                                

// deep    childs_deep   childrens_deep
//  (vorher)
// -+ (oldnode=current_iter)=A
//                       + old children=B
//  (hinterher)
// -+ (oldnode=current_iter)
//         +  newchild=A                      leafdata
//                       + old children=B

SimpleTreeStore::iterator SimpleTreeStore::MoveTree(iterator current_iter,
	guint deep,guint child_s_deep,guint value_index,bool live)
{//  ManuProC::Trace(trace_channel,__FUNCTION__,deep,child_s_deep,value_index);
   Node &oldnode=current_iter->second;
   STSN_CHECK_MAGIC(&oldnode);
   STSN_CHECK_MAGIC(oldnode.parent);
   Node newnode(deep,oldnode.parent,oldnode.leafdata,child_s_deep);
   STSN_CHECK_MAGIC(&newnode);
   STSN_CHECK_MAGIC(newnode.parent);

//   if (node_creation) 
   {  Handle<const TreeRow> arg=oldnode.row; // to convert between Handle and Handle<const>
	  Handle<TreeRow> htr= create_node(arg);
      newnode.row=htr;
      // leaves have no row (so initial sum is always 0), 
      // so we need to cumulate their data
	  // VC7 needs the explicit cast
      if (!oldnode.childrens_deep && bool(htr)) // leaf
         htr->cumulate(oldnode.leafdata);
   }

   // ein Kind an newnode anfügen (newchild)
   cH_EntryValue val=oldnode.leafdata->Value(value_index,ValueData());
   newnode.children.insert(std::make_pair(val,Node(deep,oldnode.parent)));
   // dieser Knoten wird als oldnode im Baum hängen
   Node &newchild=newnode.children.begin()->second;
   STSN_CHECK_MAGIC(&newchild);
   STSN_CHECK_MAGIC(newchild.parent);

   // newchild und oldnode vertauschen (Unterbaum ist dann außerhalb des Baumes
   //		unterhalb von newnode, newchild ist dann im Baum)
   // oldnode ist jetzt die unnötige Node, newchild die alte Node
   std::swap(newchild,oldnode);

   // newnode und oldnode vertauschen (newnode+Kinder sind dann wieder im Baum)
   //      nun muss newnode wieder leer sein und den Inhalt von newchild enthalten
   // oldnode ist jetzt die neue Eltern-Node, newnode ist jetzt die unnötige
   std::swap(newnode,oldnode);
   newchild.parent=&oldnode;
   newchild.deep=child_s_deep;
   // oldnode's address has changed to newchild ...
   for (iterator i=newchild.children.begin();i!=newchild.children.end();++i)
   {  i->second.parent=&newchild;
      STSN_CHECK_MAGIC(&newchild);
      STSN_CHECK_MAGIC(newchild.parent);
   }
   // row_has_child_toggled(newnode), row_changed(newnode)
   
//   ManuProC::Trace(trace_channel,std::string(),&oldnode,&newnode,&newchild);
//   oldnode.fix_pointer();
   if (live) 
   { row_has_child_toggled(getPath(current_iter),getIter(current_iter));
//     row_changed(getPath(current_iter),getIter(current_iter));
     iterator child=current_iter->second.children.begin();
     assert(child!=current_iter->second.children.end());
     row_inserted(getPath(child),getIter(child));
   }
   STSN_CHECK_MAGIC(&current_iter->second);
   STSN_CHECK_MAGIC(current_iter->second.parent);
   return current_iter;
}

// optimize ist nun unnötig (da Ausnahme)
void SimpleTreeStore::setSequence(const sequence_t &neu, bool optimize)
{  please_detach();
   ++stamp;
   currseq=neu; // Spaltenzahl anpassen?
   columns=currseq.size();
   save_remembered();
   spalten_geaendert();
   redisplay();
}

unsigned SimpleTreeStore::ColumnFromIndex(unsigned idx) const
{  unsigned currsize=currseq.size();
   for (guint i=0; i<currsize; ++i)
   {  if (currseq[i]==idx) return i;
   }
   return invisible_column;
}

const UniqueValue::value_t SimpleTreeStore::trace_channel
      = ManuProC::Tracer::channels.get();
static ManuProC::Tracer::Environment trace_channele("STS_TRACE",SimpleTreeStore::trace_channel);

void SimpleTreeStore::on_line_removed(cH_RowDataBase r)
{  ManuProC::Trace _t(trace_channel,__FUNCTION__,&*r);
   ++stamp;
   std::list<iterator> l=find_row(r,true);
   assert(sortierspalte==invisible_column);
   if (l.begin()==l.end()) 
   { std::cerr << "SimpleTreeStore::on_line_removed: trying again unoptimized\n";
     l=find_row(r,false);
   }
   if (l.begin()!=l.end())
   {  ManuProC::Trace(trace_channel,__FUNCTION__,"depth=",l.size());
      Path p=getPath(l.front());
      std::list<iterator>::const_iterator parent_iter=++l.begin();
      if (parent_iter==l.end()) 
      // nur 1 iterator, das zu löschende Element ist dann Kind von root
      { root.children.erase(l.front());
        row_deleted(p);
      }
      else
      { (*parent_iter)->second.children.erase(l.front());
        row_deleted(p);
      // we might be able to eliminate the nodes if they no longer
      //   have children
        for (std::list<iterator>::const_iterator i=parent_iter;i!=l.end();++i)
        { if (&*(*i)->second.leafdata==&*r)
          { if ((*i)->second.children.empty())
            { std::list<iterator>::const_iterator parent_iter2=i;
              ++parent_iter2;
              Path p2=getPath(*i);
              if (parent_iter2==l.end())
                root.children.erase(*i);
              else
                (*parent_iter2)->second.children.erase(*i);
              row_deleted(p2);
              goto continue_outer;
            }
            else
            { (*i)->second.leafdata=(*i)->second.children.begin()->second.leafdata;
            }
          }
          { Handle<TreeRow> htr=(*i)->second.row;
            if (htr)
            { htr->deduct(r);
              row_changed(getPath(*i),getIter(*i));
            }
          }
         continue_outer: ;
        }
      }
   }
   else std::cerr << "line to remove not found\n";
}

// optimize indicates we could binary search by value (possible optimization)
// this is impossible, if a value has already changed ...

bool SimpleTreeStore::find_row(Node &parent, const cH_RowDataBase &r,bool optimize,std::list<iterator> &result)
{
  STSN_CHECK_MAGIC(&parent);
  STSN_CHECK_MAGIC(parent.parent);
  ManuProC::Trace _t(trace_channel,__FUNCTION__,&parent,&*r);
   if (optimize && parent.childrens_deep && sortierspalte==invisible_column)
   {  cH_EntryValue val=r->Value(currseq[parent.childrens_deep],ValueData());
      ManuProC::Trace(trace_channel,std::string(),NV("depth",parent.childrens_deep),
                NV("val",val->getStrVal()));
      std::pair<iterator,iterator> p=parent.children.equal_range(val);
      if (p.first==p.second) 
      { ManuProC::Trace(trace_channel,std::string(),"empty equal_range");
        return false;
      }
      if (!p.first->second.childrens_deep) // nodes
      {  ManuProC::Trace(trace_channel,std::string(),"leaves");
         for (iterator i=p.first;i!=p.second;++i) 
         {  STSN_CHECK_MAGIC(&i->second);
            STSN_CHECK_MAGIC(i->second.parent);
            if (&*i->second.leafdata==&*r)
            {  result.push_back(i);
               return true;
            }
         }
      }
      else 
      {  ManuProC::Trace(trace_channel,std::string(),"nodes");
         if (find_row(p.first->second,r,optimize,result))
         {  result.push_back(p.first);
            return true;
         }
      }
      ManuProC::Trace(trace_channel,std::string(),"not found");
      return false;
   }
   
   for (iterator i= parent.children.begin(); i!=parent.children.end(); ++i)
   {  STSN_CHECK_MAGIC(&i->second);
      STSN_CHECK_MAGIC(i->second.parent);
      if (i->second.children.empty())
      {  ManuProC::Trace(trace_channel,std::string(),NV("found",&*i->second.leafdata));
         if (&*i->second.leafdata==&*r)
         {  result.push_back(i);
            return true;
         }
      }
      else 
      {  ManuProC::Trace(trace_channel,std::string(),"recurse");
         if (find_row(i->second,r,optimize,result))
         {  result.push_back(i);
            return true;
         }
      }
   }
   return false;
}

std::list<SimpleTreeStore::iterator> SimpleTreeStore::find_row(const cH_RowDataBase &r,bool optimize)
{ ManuProC::Trace _t(trace_channel,__FUNCTION__,&*r,optimize);
  std::list<iterator> result;
  find_row(root,r,optimize,result);
  return result;
}

void SimpleTreeStore::redisplay_old(Gtk::TreeModel::iterator it, unsigned index)
{  unsigned col=ColumnFromIndex(index);
   if (col==invisible_column) return;
   row_changed(get_path(it),it);
}

#if 1 // wird das noch gebraucht?
void SimpleTreeStore::redisplay_old(cH_RowDataBase data, unsigned index)
{  unsigned col=ColumnFromIndex(index);
   if (col==invisible_column) return;
   std::list<iterator> rows=find_row(data);
   if (rows.begin()!=rows.end())
     row_changed(getPath(rows.back()),getIter(rows.back()));
}
#endif

void SimpleTreeStore::set_tree_column_visibility(unsigned index,bool visible)
{  ShowColumn(index)=visible;
//   on_visibly_changed(vec_hide_cols.begin()+index);
}

Gtk::TreeModelFlags SimpleTreeStore::get_flags_vfunc() const
{  return Gtk::TreeModelFlags(0);
  //Gtk::TREE_MODEL_ITERS_PERSIST; // Gtk::TreeModelFlags(0); 
}
int SimpleTreeStore::get_n_columns_vfunc() const
{  return unsigned(s_text_start)+max_column; }

// speed this up by an array?
GType SimpleTreeStore::get_column_type_vfunc(int index) const
{  switch(e_spalten(index))
   {  case s_row: return m_columns.row.type();
      case s_deep: return m_columns.deep.type();
      case s_childrens_deep: return m_columns.childrens_deep.type();
      case s_children_count: return m_columns.children_count.type();
      case s_leafdata: return m_columns.leafdata.type();
      case s_background: return m_columns.background.type();
      default: return G_TYPE_STRING;
   }
}

unsigned SimpleTreeStore::IterStamp() const
{  return stamp;
}

bool SimpleTreeStore::iter_valid(vfunc_constiter_t iter) const
{  return iter.get_stamp()==IterStamp();
}

SimpleTreeStore::iterator &SimpleTreeStore::iterconv(vfunc_iter_t iter)
{  assert(iter_valid(iter));
   STSN_CHECK_MAGIC(&reinterpret_cast<SimpleTreeStore::iterator&>(iter.gobj()->user_data)->second);
   STSN_CHECK_MAGIC(reinterpret_cast<SimpleTreeStore::iterator&>(iter.gobj()->user_data)->second.parent);
   return reinterpret_cast<SimpleTreeStore::iterator&>(iter.gobj()->user_data);
}

const SimpleTreeStore::iterator &SimpleTreeStore::iterconv(vfunc_constiter_t iter) const
{  if (!iter_valid(iter))
   {  std::cerr << "iterconv: iterator mismatch " << iter .get_stamp() << "!=" << IterStamp()
   	<< " user_data=" << iter.gobj()->user_data << '\n';
      abort();
   }
   STSN_CHECK_MAGIC(&reinterpret_cast<const SimpleTreeStore::iterator&>(iter.gobj()->user_data)->second);
   STSN_CHECK_MAGIC(reinterpret_cast<const SimpleTreeStore::iterator&>(iter.gobj()->user_data)->second.parent);
   return reinterpret_cast<const SimpleTreeStore::iterator&>(iter.gobj()->user_data);
}

void SimpleTreeStore::iterinit(vfunc_iter_t iter,const const_iterator &schema) const
{  assert(3*sizeof(iter.gobj()->user_data)>=sizeof(SimpleTreeStore::const_iterator));
   STSN_CHECK_MAGIC(&schema->second);
   STSN_CHECK_MAGIC(schema->second.parent);
   iter.set_stamp(stamp);
   reinterpret_cast<SimpleTreeStore::const_iterator&>(iter.gobj()->user_data)=schema;
   iter.set_model_gobject(const_cast<GtkTreeModel*>(Gtk::TreeModel::gobj()));
   ManuProC::Trace(trace_channel,__FUNCTION__,iter .get_stamp(),
   		iter.gobj()->user_data,
   		iter.gobj()->user_data2,
   		iter.gobj()->user_data3);
}

void SimpleTreeStore::iterclear(vfunc_iter_t iter) const
{  ManuProC::Trace(trace_channel,__FUNCTION__);
   iter=TreeModel::iterator();
}

void SimpleTreeStore::iterinit(vfunc_iter_t iter,const iterator &schema) const
{  iterinit(iter,static_cast<const_iterator>(schema));
}

#define VALUE_INIT0(type) \
	g_value_init(value.gobj(),(type))
#define VALUE_SET(type,val) \
	g_value_set_##type(value.gobj(),(val))

#define VALUE_INIT3(type2,name,val) \
	VALUE_INIT0(m_columns.name.type()); \
	VALUE_SET(type2,(val))
#define VALUE_INIT(name) VALUE_INIT3(boxed,name,&nd.name)
#define VALUE_INIT_U(name) VALUE_INIT3(uint,name,nd.name)
#define VALUE_STRING(val) \
	VALUE_SET(string,(val).c_str())

void SimpleTreeStore::get_value_vfunc(const TreeModel::iterator& iter, 
		int column, Glib::ValueBase& value) const
{  if (!iter_valid(iter )) return;
   Node &nd=iterconv(iter )->second;
   STSN_CHECK_MAGIC(&nd);
   STSN_CHECK_MAGIC(nd.parent);
   switch(e_spalten(column))
   {  case s_row: VALUE_INIT(row);
         return;
      case s_deep: VALUE_INIT_U(deep);
         return;
      case s_childrens_deep: VALUE_INIT_U(childrens_deep);
         return;
      case s_children_count: VALUE_INIT3(uint,childrens_deep,nd.children.size());
         return;
      case s_leafdata: VALUE_INIT(leafdata);
         return;
      case s_background: VALUE_INIT3(boxed,background,colors[nd.deep%num_colors].gobj());
         return;
      default:
         if (int(s_text_start)<=column && column<int(s_text_start)+int(max_column))
         {  VALUE_INIT0(G_TYPE_STRING);
            int colno=column-int(s_text_start);
            if (colno<0 || colno>=int(Cols())) return;
            unsigned idx=currseq[colno];
            if (nd.row)
            {  if (colno<int(nd.deep)) return;
               if (colno<int(nd.childrens_deep))
               {  VALUE_STRING(nd.leafdata->Value(idx,ValueData())->getStrVal());
                  return;
               }
               const Glib::ustring s=nd.row->Value(idx,ValueData())->getStrVal();
                  VALUE_STRING(s);
            }
            else if (nd.childrens_deep) // node
            {  if (unsigned(colno)>nd.childrens_deep || unsigned(colno)<nd.deep) 
                  return;
               if (colno!=int(nd.childrens_deep))
                  VALUE_STRING(nd.leafdata->Value(idx,ValueData())->getStrVal());
            }
            else // leaf
            {  if (unsigned(colno)<nd.deep) return;
               VALUE_STRING(nd.leafdata->Value(idx,ValueData())->getStrVal());
            }
         }
         return;
   }
}

bool SimpleTreeStore::iter_next_vfunc(vfunc_constiter_t iter, vfunc_iter_t iter_next) const
{
   ManuProC::Trace _t(trace_channel, __FUNCTION__,iter.gobj()->user_data);
   if (!iter_valid(iter)) { iterclear(iter_next); return false; }

   iterator old=iterconv(iter),newit=old;
   STSN_CHECK_MAGIC(&old->second);
   STSN_CHECK_MAGIC(old->second.parent);
   iterclear(iter_next);
   if (!old->second.parent) return false;
   newit++;
   if (newit==old->second.parent->children.end()) return false;
   iterinit(iter_next,newit);
   ManuProC::Trace(trace_channel,"new iter",iter_next.gobj()->user_data);
   return true;
}

bool SimpleTreeStore::iter_children_vfunc(vfunc_constiter_t parent, vfunc_iter_t iter) const
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,parent.gobj()->user_data);
   iterclear(iter);
   if (!iter_valid(parent)) return false;
   iterator p=iterconv(parent);
   STSN_CHECK_MAGIC(&p->second);
   STSN_CHECK_MAGIC(p->second.parent);
   if (p->second.children.empty()) return false;
   iterinit(iter,p->second.children.begin());
   return true;
}

bool SimpleTreeStore::iter_has_child_vfunc(vfunc_constiter_t iter) const
{  if (!iter_valid(iter)) return false;
   STSN_CHECK_MAGIC(&iterconv(iter)->second);
   STSN_CHECK_MAGIC(iterconv(iter)->second.parent);
   return !(iterconv(iter)->second.children.empty());
}

int SimpleTreeStore::iter_n_children_vfunc(vfunc_constiter_t iter) const
{  if (!iter_valid(iter)) return 0;
   STSN_CHECK_MAGIC(&iterconv(iter)->second);
   STSN_CHECK_MAGIC(iterconv(iter)->second.parent);
   return iterconv(iter)->second.children.size();
}

int SimpleTreeStore::iter_n_root_children_vfunc() const
{  STSN_CHECK_MAGIC(&root);
   return root.children.size();
}

bool SimpleTreeStore::iter_nth_child_vfunc(vfunc_constiter_t parent, int n, vfunc_iter_t iter) const
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,parent?parent.gobj()->user_data:0,n);
   iterator res,end;
   iterclear(iter);
   {  if (!iter_valid(parent)) return false;
      iterator p=iterconv(parent);
      STSN_CHECK_MAGIC(&p->second);
      STSN_CHECK_MAGIC(p->second.parent);
      res=p->second.children.begin();
      end=p->second.children.end();
   }
   
   if (res==end) return false;
   for (;n>0;--n) 
   {  ++res;
      if (res==end) return false;
   }
   iterinit(iter,res); 
   return true;
}

bool SimpleTreeStore::iter_nth_root_child_vfunc(int n, vfunc_iter_t iter) const
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,n);
   iterclear(iter);
   
   const_iterator res=root.children.begin(),
		   end=root.children.end();
   if (res==end) return false;
   for (;n>0;--n)
   {  STSN_CHECK_MAGIC(&res->second);
      STSN_CHECK_MAGIC(res->second.parent);
      ++res;
      if (res==end) return false;
   }
   STSN_CHECK_MAGIC(&res->second);
   STSN_CHECK_MAGIC(res->second.parent);
   iterinit(iter,res); 
   return true;
}

bool SimpleTreeStore::iter_parent_vfunc(vfunc_constiter_t child, vfunc_iter_t iter) const
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,child.gobj()->user_data);
   iterclear(iter);
   if (!iter_valid(child)) return false;
   iterator c=iterconv(child);
   STSN_CHECK_MAGIC(&c->second);
   STSN_CHECK_MAGIC(c->second.parent);
   if (!c->second.parent || c->second.parent==&root) return false;  
   iterator p=iterbyNode(*c->second.parent);
   if (p==c->second.parent->parent->children.end()) return false;
   iterinit(iter,p);
   return true;
}

Gtk::TreeModel::Path SimpleTreeStore::get_path_vfunc(const Gtk::TreeModel::iterator& iter) const
{ ManuProC::Trace _t(trace_channel, __FUNCTION__,iter->gobj()->user_data);
  if (!iter_valid(iter )) 
     return Path();
  return getPath(iterconv(iter ));
}

bool SimpleTreeStore::get_iter_vfunc(const Path& path, vfunc_iter_t iter) const
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,path.to_string());
   
   const_iterator 
   		res=root.children.begin(),  end=root.children.end();
   
   iterclear(iter);
   for (Path::const_iterator piter=path.begin();piter!=path.end();)
   {  if (res==end) return false;
      for (unsigned i=*piter;i>0;--i) 
      {  STSN_CHECK_MAGIC(&res->second);
         STSN_CHECK_MAGIC(res->second.parent);
         ++res;
         if (res==end) return false;
      }
      STSN_CHECK_MAGIC(&res->second);
      STSN_CHECK_MAGIC(res->second.parent);
      ++piter;
      if (piter==path.end()) {  iterinit(iter,res); return true; }
      end=res->second.children.end();
      res=res->second.children.begin();
   }
   return false;
}

SimpleTreeStore::iterator SimpleTreeStore::iterbyValue(Node &parent,const cH_EntryValue &val) const
{ STSN_CHECK_MAGIC(&parent);
  STSN_CHECK_MAGIC(parent.parent);
  return parent.children.find(val);
}

std::ostream &operator<<(std::ostream &o,const SimpleTreeStore::iterator &i)
{  return o << reinterpret_cast<const gconstpointer&>(i);
//(&*i);
}

SimpleTreeStore::iterator SimpleTreeStore::iterbyNode(Node &nd) const
{  STSN_CHECK_MAGIC(&nd);
   STSN_CHECK_MAGIC(nd.parent);
   ManuProC::Trace _t(trace_channel, __FUNCTION__,&nd);
   cH_EntryValue val=nd.leafdata->Value(currseq[nd.deep],ValueData());
   std::pair<iterator,iterator> p;
   if (sortierspalte==invisible_column) p=nd.parent->children.equal_range(val);
   else p=make_pair(nd.parent->children.begin(),nd.parent->children.end());
   for (iterator i=p.first;i!=p.second;++i) 
   {  STSN_CHECK_MAGIC(&i->second);
      STSN_CHECK_MAGIC(i->second.parent);
      ManuProC::Trace(trace_channel,"i",&i->second);
      if (&i->second==&nd) return i;
   }
   return nd.parent->children.end();
}

void SimpleTreeStoreNode::swap(SimpleTreeStoreNode &b)
{  STSN_CHECK_MAGIC(this);
   STSN_CHECK_MAGIC(this->parent);
   STSN_CHECK_MAGIC(&b);
   STSN_CHECK_MAGIC(b.parent);
   std::swap(children,b.children);
   std::swap(row,b.row);
   std::swap(leafdata,b.leafdata);
   std::swap(parent,b.parent);
   std::swap(deep,b.deep);
   std::swap(childrens_deep,b.childrens_deep);
}

void std::swap(SimpleTreeStoreNode &a,SimpleTreeStoreNode &b)
{  a.swap(b);
}

void SimpleTreeStoreNode::fix_pointer()
{  //ManuProC::Trace _t(SimpleTreeStore::trace_channel,__FUNCTION__);
   STSN_CHECK_MAGIC(this);
   STSN_CHECK_MAGIC(this->parent);
   for (iterator i=children.begin();i!=children.end();++i)
   {  STSN_CHECK_MAGIC(&i->second);
      if (i->second.parent!=this)
      {  ManuProC::Trace(SimpleTreeStore::trace_channel,__FUNCTION__,i->second.parent,this);
         i->second.parent=this;
      }
      i->second.fix_pointer();
   }
}

Gtk::TreeModel::iterator SimpleTreeStore::getIter(iterator it) const
{
   TreeModel::iterator res;
   iterinit(res,it);
   return res;
}

Gtk::TreeModel::const_iterator SimpleTreeStore::getIter(const_iterator it) const
{  
   TreeModel::iterator res;
   iterinit(res,it);
   return res;
}

Gtk::TreeModel::Path SimpleTreeStore::getPath(const cH_RowDataBase &data) const
{ std::list<iterator> l=const_cast<SimpleTreeStore*>(this)->find_row(data,true);
  // optimized search
  if (l.empty()) return Path();
  return getPath(l.front());
}

Gtk::TreeModel::Path SimpleTreeStore::getPath(const_iterator it) const
{  std::vector<unsigned> store;
   store.reserve(max_column);
   while(true)
   {  store.push_back(Node2nth_child(it->second));
      if (it->second.parent==&root) break;
      it=iterbyNode(*it->second.parent);
   }
   Path res;
   for (std::vector<unsigned>::reverse_iterator i=store.rbegin();i!=store.rend();++i)
   {  res.push_back(*i);
   }
   ManuProC::Trace _t(trace_channel,__FUNCTION__,res.to_string());
   return res;
}

Gtk::TreeModel::Path SimpleTreeStore::getPath(iterator it) const
{  return getPath(static_cast<const_iterator>(it));
}

unsigned SimpleTreeStore::Node2nth_child(const Node &nd) const
{  unsigned res=0;
   STSN_CHECK_MAGIC(&nd);
   STSN_CHECK_MAGIC(nd.parent);
   for (const_iterator i=nd.parent->children.begin();
   			i!=nd.parent->children.end();++i,++res)
   {  STSN_CHECK_MAGIC(&i->second);
      STSN_CHECK_MAGIC(i->second.parent);
      if (&i->second==&nd) return res;
   }
   abort();
}

void SimpleTreeStore::setSortierspalte(unsigned s,bool i)
{  if (sortierspalte!=s 
      || (sortierspalte!=invisible_column && i!=invert_order.at(sortierspalte)))
   {  please_detach();
      ++stamp;
      sortierspalte=s;
      if (sortierspalte!=invisible_column) invert_order.at(sortierspalte)=i;
      redisplay();
   }
}

Glib::RefPtr<SimpleTreeStore> SimpleTreeStore::create(int cols)
{
  return Glib::RefPtr<SimpleTreeStore>( new SimpleTreeStore(cols));
}

Glib::RefPtr<SimpleTreeStore> SimpleTreeStore::create(SimpleTreeModel_Properties &props)
{
  return Glib::RefPtr<SimpleTreeStore>( new SimpleTreeStore(props));
}

bool SimpleTreeStore::row_draggable_vfunc(const Gtk::TreeModel::Path& path) const
{ return true;
}
bool SimpleTreeStore::drag_data_get_vfunc(const Gtk::TreeModel::Path& path, Gtk::SelectionData& selection_data) const
{ return false;
}
bool SimpleTreeStore::drag_data_delete_vfunc(const Gtk::TreeModel::Path& path)
{ return false;
}
bool SimpleTreeStore::drag_data_received_vfunc(const Gtk::TreeModel::Path& dest, const Gtk::SelectionData& selection_data)
{ return false;
}
bool SimpleTreeStore::row_drop_possible_vfunc(const Gtk::TreeModel::Path& dest, const Gtk::SelectionData& selection_data) const
{ return false;
}

void SimpleTreeStore::value_change_impl(cH_RowDataBase row,unsigned idx,std::string const& newval, bool &has_changed)
{ has_changed|=row.cast_const<RowDataBase>()->changeValue(idx,ValueData(),newval);
}

const unsigned SimpleTreeStore::invisible_column
#ifdef _MSC_VER
												=unsigned(-1)
#endif
												;

void SimpleTreeStore::save_and_redisplay(gpointer g)
{ signal_save(g);
  column_changed(invisible_column);
}

std::string SimpleTreeStore::SpaltenMarkierung(unsigned idx) const
{ assert(idx!=invisible_column);
  if (idx==sortierspalte) return invert_order.at(idx) ? "⇑" : "⇓"; // double arrow
  if (invert_order.at(idx)) return "↑"; // up arrow
  return std::string();
}
