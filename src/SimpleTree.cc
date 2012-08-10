// $Id: SimpleTree.cc,v 1.94 2006/08/03 11:27:11 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2002-2005 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include "config.h"
#if defined __GNUC__ && __GNUC__ == 4 && __GNUC_MINOR__==0 /* local strangeness? */
# include <libintl.h>
#endif
#include <Misc/i18n.h>
#include <SimpleTree.hh>
#include <Misc/itos.h>
#include <gtkmm/menu.h>
#include <bvector_item_CheckMenuItem.hh>
#include <bool_CheckMenuItem.hh>
#include <CellRendererSimpleTree.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/bind.h>
#endif
#include <iostream>

#define FIRST_COLUMN 1
#define MPC_ST_ADVANCED

#ifdef MPC_ST_EXCEL_EXPORT
# include <WinFileReq.hh>
# include "BasicExcel.hpp"
# include <Misc/EntryValueTimeStamp.h>
# include <Misc/EntryValueFixed.h>
# include <Misc/EntryValueIntString.h>
#endif

void SimpleTree_Basic::detach()
{  unset_model();
}

static void aufklappen(SimpleTree_Basic *tv,Gtk::TreeModel::Path path,
			const Gtk::TreeModel::Children &ch, int depth)
{  path.down();
   for (Gtk::TreeModel::iterator i=ch.begin();i!=ch.end();++i,path.next())
   {  if (!i->children().empty() && (*i)[tv->getStore()->m_columns.childrens_deep]<=depth)
      {  if (!tv->expand_row(path,false) && !tv->row_expanded(path))
            std::cerr << "aufklappen von " << path.to_string() << " schlug fehl\n";
         if ((*i)[tv->getStore()->m_columns.childrens_deep]<depth) aufklappen(tv,path,i->children(),depth);
      }
   }
}

void SimpleTree_Basic::attach()
{  set_model(sts);
   // aufklappen
   if (sts->expandieren_bool && sts->ShowDeep().Value())
   {  aufklappen(this,Gtk::TreeModel::Path(),get_model()->children(),sts->ShowDeep().Value());
   }
//   get_selection()->unselect_all();
}

//void SimpleTree_Basic::on_redisplay()
//{  detach(); attach();
//}

void SimpleTree_Basic::init()
{  on_spalten_geaendert();

   get_selection()->signal_changed().connect(sigc::mem_fun(*this,&SimpleTree_Basic::on_selection_changed));
   // stammt eigentlich aus Prefs_proxy
   getStore()->signal_column_changed().connect(sigc::mem_fun(*this,&SimpleTree_Basic::on_column_changed));
   // Store
   getStore()->signal_please_detach().connect(sigc::mem_fun(*this,&SimpleTree_Basic::detach));
   getStore()->signal_please_attach().connect(sigc::mem_fun(*this,&SimpleTree_Basic::attach));
   getStore()->signal_spalten_geaendert().connect(sigc::mem_fun(*this,&SimpleTree_Basic::on_spalten_geaendert));
   fillMenu();
   signal_button_press_event().connect(sigc::mem_fun(*this,&SimpleTree_Basic::MouseButton),false);
   set_enable_search(false);
}

SimpleTree_Basic::SimpleTree_Basic(unsigned maxcol)
	: SimpleTreeStore_Proxy(maxcol), button_press_vfunc(), menu()
{ init();
}

SimpleTree_Basic::SimpleTree_Basic(SimpleTreeModel_Properties &props)
	: SimpleTreeStore_Proxy(props), button_press_vfunc(), menu()
{ init();
}

SimpleTree_Basic::~SimpleTree_Basic()
{  if (menu) delete menu;
}

void SimpleTree_Basic::on_column_edited(const Glib::ustring &path,const Glib::ustring&new_text,unsigned idx)
{  const Gtk::TreeRow row=*getTreeModel()->get_iter(Gtk::TreeModel::Path(path));
   if (!row) return;
   cH_RowDataBase rdb=row[getStore()->m_columns.leafdata];
   // 2do: think about optimizing if nothing changes (unlikely)
   getModel().about_to_change(rdb);
   bool changed=false;
   getModel().signal_value_changed()(rdb,idx,new_text,changed);
   getModel().has_changed(rdb);
}

void SimpleTree_Basic::on_column_toggled(const Glib::ustring &path,unsigned idx)
{  const Gtk::TreeRow row=*getTreeModel()->get_iter(Gtk::TreeModel::Path(path));
   if (!row) return;
   cH_RowDataBase rdb=row[getStore()->m_columns.leafdata];
   // 2do: think about optimizing if nothing changes (unlikely)
   getModel().about_to_change(rdb);
   bool changed=false;
   getModel().signal_value_changed()(rdb,idx,rdb->Value(idx,getStore()->ValueData())->getIntVal()?"f":"t",changed);
   getModel().has_changed(rdb);
}

void SimpleTree_Basic::on_spalten_geaendert()
{  remove_all_columns();
#if FIRST_COLUMN==1 // hide normal tree expanders
   append_column(*Gtk::manage(new Gtk::TreeViewColumn("tree")));
//   get_column(0)->set_visible(false);
   get_column(0)->set_sizing(Gtk::TREE_VIEW_COLUMN_FIXED);
   get_column(0)->set_fixed_width(1);
#endif
   for (unsigned int i=0;i<VisibleColumns();++i)
   {
#if 1
      Gtk::CellRenderer *crs=NULL;
      Gtk::TreeView::Column* pColumn=NULL;
      unsigned idx(IndexFromColumn(i));
      if (Properties().get_column_type(idx)==SimpleTreeModel_Properties::ct_bool)
      {
        CellRendererSimpleTreeBool *crst = Gtk::manage(new CellRendererSimpleTreeBool(i));
        if (Properties().editable(idx))
        {  crst->property_activatable()=true;
           crst->signal_toggled().connect(sigc::bind(sigc::mem_fun(*this,&SimpleTree_Basic::on_column_toggled),idx));
        }

        pColumn = Gtk::manage(new Gtk::TreeView::Column(getColTitle(i),*crst));
        pColumn->add_attribute(crst->property_active(),sts->m_columns.cols[i]);
        pColumn->add_attribute(crst->property_childrens_deep(),sts->m_columns.childrens_deep);
        if (getStore()->OptionCount().Value())
          pColumn->add_attribute(crst->property_children_count(),sts->m_columns.children_count);
        crs= crst;
      }
      else // text/number
      {
        CellRendererSimpleTreeText *crst = Gtk::manage(new CellRendererSimpleTreeText(i));
        if (Properties().editable(idx))
        {  crst->property_editable()=true;
           crst->signal_edited().connect(sigc::bind(sigc::mem_fun(*this,&SimpleTree_Basic::on_column_edited),idx));
        }

        pColumn = Gtk::manage(new Gtk::TreeView::Column(getColTitle(i),*crst));
        pColumn->add_attribute(crst->property_text(),sts->m_columns.cols[i]);
        pColumn->add_attribute(crst->property_childrens_deep(),sts->m_columns.childrens_deep);
        if (getStore()->OptionCount().Value())
          pColumn->add_attribute(crst->property_children_count(),sts->m_columns.children_count);
        crs= crst;
      }
      crs->property_xalign()=Properties().Alignment(idx);
      pColumn->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&SimpleTree_Basic::on_title_clicked),i));
      if (getStore()->OptionColor().Value())
         pColumn->add_attribute(crs->property_cell_background_gdk(),sts->m_columns.background);

      pColumn->set_alignment(Properties().Alignment(idx));
      pColumn->set_sizing(Properties().get_sizing(idx));
      if (Properties().get_sizing(idx)==Gtk::TREE_VIEW_COLUMN_FIXED)
        pColumn->set_fixed_width(Properties().get_fixed_width(idx));
      pColumn->set_resizable(Properties().resizeable(idx));

      append_column(*pColumn);
#else
      append_column(getColTitle(i),sts->m_columns.cols[i]);
#endif
   }
// (gtk_tree_view_set_headers_clickable): assertion `tree_view->priv->model != NULL'
   set_headers_clickable();
}

void SimpleTree_Basic::on_title_clicked(unsigned nr)
{  unsigned idx=getStore()->currseq[nr];
   sequence_t::iterator i=std::find(clicked_seq.begin(),clicked_seq.end(),idx);
   if (i==clicked_seq.end())
   {  clicked_seq.push_back(idx);
      // if alles voll -> umsortieren
      if (clicked_seq.size()==VisibleColumns()) on_neuordnen_clicked();
      else
	{
	 std::string tmptit=std::string("(")+itos(clicked_seq.size())+")";
	 if(get_column(nr+FIRST_COLUMN)->get_title().size()>=3)
	   tmptit+=get_column(nr+FIRST_COLUMN)->get_title().substr(3);
	 get_column(nr+FIRST_COLUMN)->set_title(tmptit);

	}
   }
   else if (i==--clicked_seq.end())
   {  // umsortieren
      on_neuordnen_clicked();
   }
   else
   {  // abbrechen
      on_abbrechen_clicked();
   }
}

void SimpleTree_Basic::on_abbrechen_clicked()
{
      clicked_seq.clear();
      // Titel wiederherstellen
      for (unsigned i=0;i<VisibleColumns();++i)
         get_column(i+FIRST_COLUMN)->set_title(getColTitle(i));
}

void SimpleTree_Basic::on_zuruecksetzen_clicked()
{  clicked_seq.clear();
   getStore()->fillSequence(clicked_seq,true);
   on_neuordnen_clicked();
}

void SimpleTree_Basic::on_neuordnen_clicked()
{  if (!clicked_seq.empty()) getStore()->ShowDeep()=clicked_seq.size()-1;
   getStore()->fillSequence(clicked_seq);
   std::vector<cH_RowDataBase>
       selection=dynamic_cast<SimpleTree*>(this)->getSelectedRowDataBase_vec();
   if (selection.empty())
       selection=dynamic_cast<SimpleTree*>(this)->getSelectedRowDataBase_vec(true);
   getStore()->setSequence(clicked_seq);
   for (std::vector<cH_RowDataBase>::const_iterator i=selection.begin();
         i!=selection.end();++i)
   { Gtk::TreeModel::Path path=getStore()->getPath(*i);
     if (!path.empty())
     { if (i==selection.begin()) scroll_to_row(path);
       expand_to_path(path);
       get_selection()->select(path);
     }
   }
   clicked_seq.clear();
   getStore()->save_remembered();
}

void SimpleTree_Basic::on_column_changed(guint nr)
{  delete menu;
   menu=0;
   fillMenu();
   on_spalten_geaendert();
}

void SimpleTree_Basic::sel_change_cb(const Gtk::TreeModel::iterator&it,
		std::vector<cH_RowDataBase> *l,std::vector<Handle<const TreeRow> > *n)
{  Gtk::TreeRow row=*it;
   if (!row[getStore()->m_columns.childrens_deep])
      l->push_back(row[getStore()->m_columns.leafdata]);
   else
   {  Handle<TreeRow> htr=row[getStore()->m_columns.row];
      if (htr) n->push_back(Handle<const TreeRow>(htr));
//std::cerr << &*htr << '\n';
   }
}

void SimpleTree_Basic::on_selection_changed()
{
   if (get_selection()->get_selected_rows().empty()) // !sel) // unselect
     _leaf_unselected();
   else
   {  std::vector<cH_RowDataBase> leaves;
      std::vector<Handle<const TreeRow> > nodes;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
      get_selection()->selected_foreach_iter(sigc::bind(sigc::mem_fun(*this,
      		&SimpleTree_Basic::sel_change_cb),&leaves,&nodes));
#else
      get_selection()->selected_foreach(sigc::bind(sigc::mem_fun(*this,
      		&SimpleTree_Basic::sel_change_cb),&leaves,&nodes));
#endif
      for (std::vector<cH_RowDataBase>::const_iterator i=leaves.begin();
      		i!=leaves.end();++i)
      	 _leaf_selected(*i);
      for (std::vector<Handle<const TreeRow> >::const_iterator i=nodes.begin();
      		i!=nodes.end();++i)
      	 _node_selected(*i);
   }
}

cH_RowDataBase SimpleTree::getSelectedRowDataBase() const
	throw(noRowSelected,multipleRowsSelected,notLeafSelected)
{  Gtk::TreeModel::iterator sel=const_cast<SimpleTree*>(this)
		->get_selection()->get_selected();
   if (sel)
   {  const Gtk::TreeRow row=*sel;
      if (!row[getStore()->m_columns.childrens_deep])
         return row[getStore()->m_columns.leafdata];
      else throw notLeafSelected();
   }
   else throw noRowSelected(); // oder multipleRowsSelected()
}

cH_RowDataBase SimpleTree::getFirstSelection() const throw()
{  if (!get_selection()->count_selected_rows()) return cH_RowDataBase();
   Gtk::TreeModel::Path path=*(get_selection()->get_selected_rows().begin());
   Gtk::TreeModel::iterator i=const_cast<SimpleTree*>(this)->getStore()->get_iter(path);
   return (*i)[getStore()->m_columns.leafdata];
}


cH_RowDataBase SimpleTree::getCursorRowDataBase() const
// actually it does not throw multipleRowsSelected
	throw(noRowSelected,multipleRowsSelected,notLeafSelected)
{  Gtk::TreeModel::Path path;
   Gtk::TreeViewColumn *col=0;
   const_cast<SimpleTree*>(this)->get_cursor(path,col);
   if (!path.empty())
   {  const Gtk::TreeRow row=*const_cast<SimpleTree*>(this)->getTreeModel()->get_iter(path);
      if (!row[getStore()->m_columns.childrens_deep])
         return row[getStore()->m_columns.leafdata];
      else throw notLeafSelected();
   }
   else throw noRowSelected(); // oder multipleRowsSelected()
}


Handle<const TreeRow> SimpleTree::getSelectedNode() const
 	throw(noNodeSelected,multipleNodesSelected,notNodeSelected)
{
  Gtk::TreeModel::iterator sel=const_cast<SimpleTree*>(this)
		->get_selection()->get_selected();
   if (sel)
   { const Gtk::TreeRow &row=*sel;
     if (row[getStore()->m_columns.childrens_deep])
       return Handle<const TreeRow>(static_cast<Handle<TreeRow> >(row[getStore()->m_columns.row]));
     else
       throw notNodeSelected();
   }
   else throw noNodeSelected(); // oder multipleRowsSelected()
}

void SimpleTree_Basic::Expand_recursively()
{  expand_all();
}

void SimpleTree_Basic::Collapse()
{  collapse_all();
}

// Pivot element might use expand_to_path

// expand_row(path,false)/collapse_row(path)

void SimpleTree::getSelectedRowDataBase_vec_cb(const Gtk::TreeModel::iterator&it,
		std::vector<cH_RowDataBase> *res, bool include_nodes)
{ Gtk::TreeRow row=*it;
  if (!row[getStore()->m_columns.childrens_deep])
      res->push_back(row[getStore()->m_columns.leafdata]);
  else if (include_nodes)
      res->push_back(row[getStore()->m_columns.leafdata]);
}

std::vector<cH_RowDataBase> SimpleTree::getSelectedRowDataBase_vec(bool include_nodes) const throw()
{  std::vector<cH_RowDataBase> result;
   SimpleTree *non_const_this=const_cast<SimpleTree*>(this);
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   get_selection()->selected_foreach_iter(sigc::bind(
   		sigc::mem_fun(*non_const_this,
      		&SimpleTree::getSelectedRowDataBase_vec_cb),&result,include_nodes));
#else
   non_const_this->get_selection()->selected_foreach(sigc::bind(
   		sigc::mem_fun(*non_const_this,
      		&SimpleTree::getSelectedRowDataBase_vec_cb),&result,include_nodes));
#endif
   return result;
}


static Gtk::MenuItem *add_mitem(Gtk::Menu *m,const std::string &text,const Model_ref<bvector_item> &model)
{  bvector_item_CheckMenuItem *it =
   Gtk::manage(new bvector_item_CheckMenuItem(model,text));
   m->append(*it);
   it->show();
   return it;
}

static Gtk::MenuItem *add_mitem(Gtk::Menu *m,const std::string &text,const Model_ref<bool> &model)
{  bool_CheckMenuItem *it = Gtk::manage(new bool_CheckMenuItem(model,text));
   m->append(*it);
   it->show();
   return it;
}

static Gtk::MenuItem *add_mitem(Gtk::Menu *m,const std::string &text)
{  Gtk::MenuItem *it=Gtk::manage(new class Gtk::MenuItem(text));
   m->append(*it);
   it->show();
   return it;
}

static Gtk::MenuItem *add_mitem(Gtk::Menu *m,const std::string &text,const sigc::slot<void> &callback)
{  Gtk::MenuItem *it=add_mitem(m,text);
   it->signal_activate().connect(callback);
   return it;
}

static Gtk::MenuItem *add_mitem(Gtk::Menu *m,const std::string &text, Gtk::RadioMenuItem::Group &group, const sigc::slot<void> &callback)
{  Gtk::RadioMenuItem *it=manage(new class Gtk::RadioMenuItem(group,text));
   m->append(*it);
   it->show();
   it->signal_activate().connect(callback);
   return it;
}

void SimpleTree_Basic::menu_ranking(int column)
{ getStore()->setSortierspalte(column, true);
}

void SimpleTree_Basic::fillMenu()
{  assert(menu==0);
  menu=new Gtk::Menu();
  // Hauptmenü
  add_mitem(menu,_("Reset order"),sigc::mem_fun(*this,&SimpleTree_Basic::on_zuruecksetzen_clicked));
  add_mitem(menu,_("Cancel"),sigc::mem_fun(*this,&SimpleTree_Basic::on_abbrechen_clicked));
  add_mitem(menu,_("Apply new order"),sigc::mem_fun(*this,&SimpleTree_Basic::on_neuordnen_clicked));

  Gtk::MenuItem *spalten=add_mitem(menu,_("Visble columns"));
  Gtk::Menu *spalten_menu=manage(new Gtk::Menu);
  spalten->set_submenu(*spalten_menu);
  Gtk::TearoffMenuItem *tomi=manage(new Gtk::TearoffMenuItem());
  spalten_menu->append(*tomi);
  tomi->show();
   for (guint i=0;i<getStore()->MaxCol();++i)
      add_mitem(spalten_menu,Properties().Title(i),getStore()->ShowColumn(i));

  Gtk::MenuItem *optionen=add_mitem(menu,_("Options"));
  Gtk::Menu *optionen_menu=manage(new Gtk::Menu);
//  add_mitem(optionen_menu,_("Spaltenüberschriften anzeigen"))->set_sensitive(false); // Model
  add_mitem(optionen_menu,_("Fill with standard order\n"
	  	"(instead of current)"),getStore()->OptionAuffullen());
  add_mitem(optionen_menu,_("Unfold selected columns"),getStore()->OptionExpandieren());
  add_mitem(optionen_menu,_("Colorize by depth"),getStore()->OptionColor());
  add_mitem(optionen_menu,_("Display children count"),getStore()->OptionCount());

  optionen->set_submenu(*optionen_menu);
  add_mitem(menu,_("Unfold all"),sigc::mem_fun(*this,&SimpleTree_Basic::Expand_recursively));
  add_mitem(menu,_("Fold all"),sigc::mem_fun(*this,&SimpleTree_Basic::Collapse));
#ifdef MPC_ST_EXCEL_EXPORT
  add_mitem(menu,_("Export to .XLS"),sigc::mem_fun(*this,&SimpleTree_Basic::write_excel_via_filerequester));
  add_mitem(menu,_("Export to .CSV"),sigc::mem_fun(*this,&SimpleTree_Basic::write_csv_via_filerequester));
#endif
#ifdef MPC_ST_ADVANCED
  Gtk::RadioMenuItem::Group group;
  Gtk::MenuItem *ranking=add_mitem(menu,_("Ranking"));
  Gtk::Menu *ranking_menu=manage(new Gtk::Menu);
  ranking->set_submenu(*ranking_menu);
  add_mitem(ranking_menu,_(" off "),group,sigc::bind(sigc::mem_fun(*this,&SimpleTree_Basic::menu_ranking),-1));
  for (guint i=0;i<getStore()->MaxCol();++i)
    add_mitem(ranking_menu,Properties().Title(i),group,sigc::bind(sigc::mem_fun(*this,&SimpleTree_Basic::menu_ranking),i));
#endif

  // separator
  for (std::list<std::pair<sigc::signal0<void>,Glib::ustring> >::iterator i=user_menuitems.begin();
        i!=user_menuitems.end();++i)
  { add_mitem(menu,i->second,i->first.make_slot());
  }
}

bool SimpleTree_Basic::MouseButton(GdkEventButton *event)
{  Gtk::TreeModel::Path path;
   Gtk::TreeViewColumn *col(0);
   int cell_x(0),cell_y(0);
   bool res=get_path_at_pos(int(event->x),int(event->y),path,col,cell_x,cell_y);

   if (event->type == GDK_BUTTON_PRESS && event->button==1 && button_press_vfunc
       && event->window ==get_bin_window()->gobj())
   {  if (!res || !col) return false;
      Gtk::TreeModel::iterator it=getTreeModel()->get_iter(path);
      if (!it) return false;
      if ((*it)[getStore()->m_columns.childrens_deep]) return false; // node
      int idx=-1;
      for (unsigned colno=0;colno<=VisibleColumns();++colno)
         if (col==get_column(colno))
         {  idx=getStore()->currseq[colno];
            break;
         }
      return (*button_press_vfunc)(this,(*it)[getStore()->m_columns.leafdata],idx);
   }
   if (event->type == GDK_BUTTON_PRESS && event->button==3  && menu)
   {  if (res)
      { Gtk::TreeModel::iterator it=getTreeModel()->get_iter(path);
        if (!!it)
          menuContext=(*it)[getStore()->m_columns.leafdata];
        else menuContext=cH_RowDataBase();
      }
      else menuContext=cH_RowDataBase();
      menu->popup(event->button,event->time);
      return true;
   }
   return false;
}

#ifdef ST_DEPRECATED
void SimpleTree_Basic::setTitles(const std::vector<std::string>& T)
{  SimpleTreeStore_Proxy::setTitles(T);
   delete menu; menu=0;
   fillMenu();
}
#endif

void SimpleTree::ScrollToSelection()
{ Glib::RefPtr<Gtk::TreeSelection> sel=get_selection();
  if (sel->count_selected_rows())
  { Gtk::TreeModel::Path path=*(sel->get_selected_rows().begin());
    expand_to_path(path);
    scroll_to_row(path,0.5);
  }
}

void SimpleTree::scroll_to(const cH_RowDataBase &data, gfloat where)
{ Gtk::TreeModel::Path path=getStore()->getPath(data);
  if (!path.empty())
  { expand_to_path(path);
    if (where<0) scroll_to_row(path);
    else scroll_to_row(path,where);
  }
}

void SimpleTree::select(cH_RowDataBase const& data)
{
	Gtk::TreeModel::Path path=getStore()->getPath(data);
	get_selection()->select(path);
}

void SimpleTree_Basic::setAlignment(const std::vector<gfloat> &A)
{  sts->setAlignment(A);
}

void SimpleTree_Basic::setResizeable(const std::vector<bool> &R)
{ sts->setResizeable(R);
}

void SimpleTree_Basic::setResizeable(const bool b)
{
 SimpleTree_Basic::setResizeable(std::vector<bool>(sts->MaxCol(),b));
}

static void sig0(const char * const x)
{ std::cerr << x << "()\n";
}

static void sig1i(int i, const char * const x)
{ std::cerr << x << "(" << i << ")\n";
}

static void sig1r(cH_RowDataBase r, const char * const x)
{ std::cerr << x << "(" << &r << ")\n";
}

static void sig2pi(const Gtk::TreeModel::Path&p,const Gtk::TreeModel::iterator&i, const char * const x)
{ std::cerr << x << "(" << p.to_string() << "," << i.get_stamp() << "," << i.gobj()->user_data << ")\n";
}

static void sig1p(const Gtk::TreeModel::Path&p, const char * const x)
{ std::cerr << x << "(" << p.to_string() << ")\n";
}

static void sig3piI(const Gtk::TreeModel::Path&p,const Gtk::TreeModel::iterator&i, int *I, const char * const x)
{ std::cerr << x << "(" << p.to_string() << "," << i.get_stamp() << "," << i.gobj()->user_data << "," << I << ")\n";
}

void SimpleTree_Basic::debug()
{ getStore()->signal_please_detach().connect(sigc::bind(&sig0,"please_detach"));
  getStore()->signal_please_attach().connect(sigc::bind(&sig0,"please_attach"));
  getStore()->signal_spalten_geaendert().connect(sigc::bind(&sig0,"spalten_geaendert"));
  getStore()->signal_column_changed().connect(sigc::bind(&sig1i,"column_changed"));
  getModel().signal_line_appended().connect(sigc::bind(&sig1r,"line_appended"));
  getModel().signal_line_to_remove().connect(sigc::bind(&sig1r,"line_to_remove"));
  getModel().signal_please_detach().connect(sigc::bind(&sig0,"Model::please_detach"));
  getModel().signal_please_attach().connect(sigc::bind(&sig0,"Model::please_attach"));
  getStore()->signal_row_changed().connect(sigc::bind(&sig2pi,"row_changed"));
  getStore()->signal_row_inserted().connect(sigc::bind(&sig2pi,"row_inserted"));
  getStore()->signal_row_has_child_toggled().connect(sigc::bind(&sig2pi,"row_has_child_toggled"));
  getStore()->signal_row_deleted().connect(sigc::bind(&sig1p,"row_deleted"));
  getStore()->signal_rows_reordered().connect(sigc::bind(&sig3piI,"rows_reordered"));
}

// deprecated proxies
void SimpleTreeStore_Proxy::set_remember(const std::string &program, const std::string &instance) {  sts->set_remember(program,instance); }
void SimpleTreeStore_Proxy::setTitles(const std::vector<std::string> &T) {  sts->setTitles(T); }
void SimpleTreeStore_Proxy::setTitleAt(unsigned idx, const std::string &s) {  sts->setTitleAt(idx,s); }
void SimpleTreeStore_Proxy::set_NewNode(SimpleTreeStore::NewNode_fp n) { sts->set_NewNode(n); }
void SimpleTreeStore_Proxy::set_value_data(gpointer _p) { sts->set_value_data(_p); }

bool SimpleTree_Basic::clicked_impl(SimpleTree_Basic *_this, const cH_RowDataBase &row, int col_idx)
{  bool handled=false;
   _this->clicked_sig(row,col_idx,handled);
   return handled;
}

sigc::signal<void,const cH_RowDataBase &,int,bool&>
	  &SimpleTree_Basic::signal_clicked()
{ button_press_vfunc=&SimpleTree_Basic::clicked_impl;
  return clicked_sig;
}

sigc::signal0<void> &SimpleTree_Basic::addMenuItem(Glib::ustring const& text)
{ std::list<int>::size_type idx=user_menuitems.size();
  user_menuitems.resize(idx+1);
  user_menuitems.back().second=text;
  if (menu)
  { add_mitem(menu,text,user_menuitems.back().first.make_slot());
  }
  return user_menuitems.back().first;
}

cH_RowDataBase SimpleTree::getMenuContext(bool prefer_selection) const
{ if (prefer_selection && get_selection()->count_selected_rows())
    return getFirstSelection();
  return menuContext;
}

void SimpleTree::select_leaves(Gtk::TreeModel::Path const& p)
{ Gtk::TreeModel::iterator it=get_model()->get_iter(p);
  if (!bool(it)) return;
  Gtk::TreeModel::Children::size_type csz=it->children().size();
  if (csz)
  { expand_row(p,false); // ich muss aufklappen, weil das selektieren sonst nicht geht
    Gtk::TreeModel::Path p2=p;
    p2.down();
    while (csz)
    { get_selection()->select(p2);
      select_leaves(p2);
      p2.next();
      --csz;
    }
  }
}

void SimpleTree::nodes_select_leaves()
{ Gtk::TreeSelection::ListHandle_Path ld=get_selection()->get_selected_rows();
  for (Gtk::TreeSelection::ListHandle_Path::const_iterator i=ld.begin();i!=ld.end();++i)
  { select_leaves(*i);
  }
}

#ifdef MPC_ST_EXCEL_EXPORT
bool is_ascii(std::string const& x)
{ for (std::string::const_iterator i=x.begin();i!=x.end();++i)
    if (*i&0x80) return false;
  return true;
}

// a hack? ... but seems to be the most efficient
std::wstring make_wstring(std::string const& x)
{ static wchar_t wstring[1024];
  mbstowcs(wstring,x.c_str(),sizeof(wstring)/sizeof(wchar_t));
  return wstring;
}

static void write_excel_sub(SimpleTree *tv,YExcel::BasicExcelWorksheet* sheet,unsigned &row,const Gtk::TreeModel::Children &ch)
{ for (Gtk::TreeModel::iterator i=ch.begin();i!=ch.end();++i)
  { Gtk::TreeModel::Path p(tv->get_model()->get_path(*i));
    if (!i->children().empty() && tv->row_expanded(p))
    { // recursion
      write_excel_sub(tv,sheet,row,i->children());
    }
    else
    { // output
      for (unsigned int c=0;c<tv->VisibleColumns();++c)
      { cH_EntryValue val;
        if ((*i)[tv->getStore()->m_columns.childrens_deep]
            && c>=(*i)[tv->getStore()->m_columns.childrens_deep])
        { if (!!static_cast<Handle<TreeRow> >((*i)[tv->getStore()->m_columns.row]))
            val=static_cast<Handle<TreeRow> >((*i)[tv->getStore()->m_columns.row])
              ->Value(tv->getStore()->get_seq()[c],tv->getStore()->ValueData());
          else
            ; // leer lassen
        }
        else
          val=static_cast<cH_RowDataBase>((*i)[tv->getStore()->m_columns.leafdata])
              ->Value(tv->getStore()->get_seq()[c],tv->getStore()->ValueData());

        std::string strval=val->getStrVal();


        if (!!val.cast_dynamic<const EntryValueFixed<2,double,long,false> >())
{	 double v=val.cast_dynamic<const EntryValueFixed<2,double,long,false> >()->Wert().as_float();
         sheet->Cell(row,c)->SetDouble(v);
          std::cout << "fixed 2 double"<< v <<"\n";
          }
        else if (!!val.cast_dynamic<const EntryValueFixed<1> >())
{          sheet->Cell(row,c)->SetDouble(val.cast_dynamic<const EntryValueFixed<1> >()->Wert().as_float());
          std::cout << "fixed 1\n";}
        else if (!!val.cast_dynamic<const EntryValueFixed<2> >())
{          sheet->Cell(row,c)->SetDouble(val.cast_dynamic<const EntryValueFixed<2> >()->Wert().as_float());
          std::cout << "fixed 2\n";}
        else if (!!val.cast_dynamic<const EntryValueFixed<3> >())
{          sheet->Cell(row,c)->SetDouble(val.cast_dynamic<const EntryValueFixed<3> >()->Wert().as_float());
                    std::cout << "fixed 3\n"; }
        else if (!!val.cast_dynamic<const EntryValueIntString>()
            && itos(val->getIntVal())==strval)
        {
          int ival=val.cast_dynamic<const EntryValueIntString>()->getIntVal();
          if (ival>=0x20000000 || ival<-0x20000000)  // will be shifted by 2
            sheet->Cell(row,c)->SetDouble(double(ival));
          else
            sheet->Cell(row,c)->SetInteger(ival);
          std::cout << "int " << ival << "\n";
        }
        else if (!!val.cast_dynamic<const EntryValueTimeStamp>())
        {
          ManuProC::TimeStamp ts=val.cast_dynamic<const EntryValueTimeStamp>()->TimeStamp();
          time_t t=time_t(ts);
          // zone 'correction'
          t+=ts.MinutesFromGmt()*60;
          double dval=t/(24.0*60*60)+25569.0;
          sheet->Cell(row,c)->SetDouble(dval);
          sheet->Cell(row,c)->SetStyle(YExcel::BasicExcelCell::ST_DATETIME);
          std::cout << "time " << dval << "\n";
        }
        else if (strval.empty()) ; // nichts tun
        else if (is_ascii(strval))
          sheet->Cell(row,c)->SetString(strval.c_str());
        else // unicode
          sheet->Cell(row,c)->SetWString(make_wstring(strval).c_str());
      }
      ++row;
    }
  }
}



static void write_csv_sub(SimpleTree *tv,std::ofstream &f,unsigned &row,const Gtk::TreeModel::Children &ch)
{ for (Gtk::TreeModel::iterator i=ch.begin();i!=ch.end();++i)
  { Gtk::TreeModel::Path p(tv->get_model()->get_path(*i));
    if (!i->children().empty() && tv->row_expanded(p))
    { // recursion
      write_csv_sub(tv,f,row,i->children());
    }
    else
    { // output
      for (unsigned int c=0;c<tv->VisibleColumns();++c)
      { cH_EntryValue val;
        if ((*i)[tv->getStore()->m_columns.childrens_deep]
            && c>=(*i)[tv->getStore()->m_columns.childrens_deep])
        { if (!!static_cast<Handle<TreeRow> >((*i)[tv->getStore()->m_columns.row]))
            val=static_cast<Handle<TreeRow> >((*i)[tv->getStore()->m_columns.row])
              ->Value(tv->getStore()->get_seq()[c],tv->getStore()->ValueData());
          else
            ; // leer lassen
        }
        else
          val=static_cast<cH_RowDataBase>((*i)[tv->getStore()->m_columns.leafdata])
              ->Value(tv->getStore()->get_seq()[c],tv->getStore()->ValueData());

        std::string strval=val->getStrVal();

        if(c>0) f << ";";
        f << strval;
      }
      f << "\n";
      ++row;
    }
  }
}


void SimpleTree::write_excel(std::string const& filename) const
{


  YExcel::BasicExcel e;
  e.New(1);
  std::string name=getStore()->Properties().InstanceName();
  if (name.empty()) name=_("Table");
  e.RenameWorksheet(size_t(0),make_wstring(name).c_str());
  YExcel::BasicExcelWorksheet* sheet = e.GetWorksheet(size_t(0));
  assert(sheet);
  for (unsigned int i=0;i<VisibleColumns();++i)
  { std::string title=getColTitle(i);
    if (!is_ascii(title))
      sheet->Cell(0,i)->SetWString(make_wstring(title).c_str());
    else
      sheet->Cell(0,i)->SetString(title.c_str());
    sheet->Cell(0,i)->SetStyle(YExcel::BasicExcelCell::ST_BOLD);
  }
  unsigned row=1;
  SimpleTree* non_const_this=const_cast<SimpleTree*>(this);
  if(non_const_this->get_selection()->get_mode() != Gtk::SELECTION_MULTIPLE)
   {Gtk::TreeModel::iterator iter = non_const_this->get_selection()->get_selected() ;
    write_excel_sub(non_const_this,sheet,row,iter->children());
   }
  else
    write_excel_sub(non_const_this,sheet,row,non_const_this->get_model()->children());
//  non_const_this->get_selection()->selected_foreach_iter(sigc::mem_fun(
//                      *this,&write_excel_sub),non_const_this,&sheet,&row,non_const_this->get_model()->children());

//  konten_tree->get_selection()->selected_foreach_iter(sigc::bind(sigc::mem_fun(*this,
//                   &fibumain::konto_print_one_node),&tf,&os));



  e.SaveAs(filename.c_str());
}

void SimpleTree::write_csv(std::string const& filename) const
{
 std::ofstream f(filename.c_str());
 assert (f.good());

  for (unsigned int i=0;i<VisibleColumns();++i)
  { std::string title=getColTitle(i);
    if(i>0) f << ";";
    f << title;
  }
  f << "\n";


  unsigned row=1;
  SimpleTree* non_const_this=const_cast<SimpleTree*>(this);
  if(non_const_this->get_selection()->get_mode() != Gtk::SELECTION_MULTIPLE)
   {Gtk::TreeModel::iterator iter = non_const_this->get_selection()->get_selected() ;

    write_csv_sub(non_const_this,f,row,iter->children());
   }
  else
    write_csv_sub(non_const_this,f,row,non_const_this->get_model()->children());

 f.close();
}



void SimpleTree::write_excel_via_filerequester() const
{
  if(get_selection()->get_selected_rows().size()!=1)
      return;

  std::string fname=getStore()->Properties().InstanceName();
  if (fname.empty()) fname=_("Table");
  if (getenv("HOME")) fname=std::string(getenv("HOME"))+"/"+fname;
  fname+=".xls";
  Gtk::Window const*toplevel=dynamic_cast<Gtk::Window const*>(get_toplevel());
  std::string patsep(1,char(0));
  WinFileReq::create(sigc::mem_fun(*this,&SimpleTree::write_excel),fname,
      _("Excel tables (*.xls)")+patsep+_("*.xls")+patsep,"xls",_("Export table"),false,
      const_cast<Gtk::Window*>(toplevel));
}


void SimpleTree::write_csv_via_filerequester() const
{
  if(get_selection()->get_selected_rows().size()!=1)
      return;

  std::string fname=getStore()->Properties().InstanceName();
  if (fname.empty()) fname=_("Table");
  if (getenv("HOME")) fname=std::string(getenv("HOME"))+"/"+fname;
  fname+=".csv";
  Gtk::Window const*toplevel=dynamic_cast<Gtk::Window const*>(get_toplevel());
  std::string patsep(1,char(0));
  WinFileReq::create(sigc::mem_fun(*this,&SimpleTree::write_csv),fname,
      _("CSV files (*.csv)")+patsep+_("*.csv")+patsep,"csv",_("Export table"),false,
      const_cast<Gtk::Window*>(toplevel));
}

#endif

#if !defined(GDK_KEY_BackSpace) && defined(GDK_BackSpace)
# define GDK_KEY_BackSpace GDK_BackSpace
# define GDK_KEY_Delete GDK_Delete
# define GDK_KEY_KP_Delete GDK_KP_Delete
#endif

bool SimpleTree::filter_key_handler(GdkEventKey* k)
{
  if (!has_focus()) return false;
  if (k->keyval==GDK_KEY_BackSpace || k->keyval==GDK_KEY_Delete || k->keyval==GDK_KEY_KP_Delete)
  {
    getStore()->set_filter(std::string());
    if (filter_entry)
    {
      entry_connection.block();
      filter_entry->set_text(std::string());
      entry_connection.block(false);
    }
    else if (filter_column!=-1)
    {
      get_column(filter_column+FIRST_COLUMN)->set_title(getColTitle(filter_column));
      filter_column=-1;
    }
    return true;
  }
//  unsigned time=k->time;
  if (k->string && *k->string)
  {
    std::string new_filter;
    if (k->time-filter_time<2000)
      new_filter = getStore()->get_filter();
    filter_time=k->time;
    new_filter+=k->string;
    getStore()->set_filter(new_filter);
    if (filter_column==-1 && !filter_entry)
    {
      sequence_t const& fcol= getStore()->get_filter_match();
      for (sequence_t::const_iterator i=fcol.begin();i!=fcol.end();++i)
	if (getStore()->ColumnFromIndex(*i)!=-1)
	{ filter_column=getStore()->ColumnFromIndex(*i);
	  break;
	}
    }
    if (filter_entry)
    {
      entry_connection.block();
      filter_entry->set_text(new_filter);
      entry_connection.block(false);
    }
    else if (filter_column!=-1)
      get_column(filter_column+FIRST_COLUMN)->set_title("<"+new_filter+">");
  }
  return false;
}

void SimpleTree::set_filter_match(sequence_t const& cols)
{
  key_connection.disconnect();
  getStore()->set_filter_match(cols);
  if (!cols.empty())
    key_connection= signal_key_press_event().connect(sigc::mem_fun(*this,&SimpleTree::filter_key_handler));
}

void SimpleTree::set_filter_entry(Gtk::Entry* e)
{
  entry_connection.disconnect();
  filter_entry=e;
  if (e)
  {
    entry_connection= e->signal_changed().connect(sigc::mem_fun(*this, &SimpleTree::filter_changed));
    getStore()->set_filter(e->get_text());
  }
}

void SimpleTree::filter_changed()
{
  getStore()->set_filter(filter_entry->get_text());
}


void SimpleTree_Basic::EnableTooltips(const bool)
{
  set_has_tooltip(true);
  tooltip_sig=signal_query_tooltip().connect(sigc::mem_fun(*this, &SimpleTree_Basic::on_query_tooltip));
}

bool SimpleTree_Basic::on_query_tooltip(int x, int y, bool keyboard_mode,
    Glib::RefPtr<Gtk::Tooltip> const& tooltip)
{
  //std::cerr << "qt:\n";
  Gtk::TreePath p;
  Gtk::TreeViewColumn* focus = NULL;
  // inspired by jobviewer.py from system-config-printer
  if (keyboard_mode)
  {
    get_cursor(p, focus);
    if (p.empty())
      return false;
  }
  else
  {
    int bin_x, bin_y, cell_x, cell_y;
    convert_widget_to_bin_window_coords(x, y, bin_x, bin_y);
    bool ok = get_path_at_pos(bin_x, bin_y, p, focus, cell_x, cell_y);
    if (!ok)
      return false;
  }
  Gtk::TreeModel::const_iterator i = get_model()->get_iter(p);
  int col = 0;
  Glib::ListHandle<Gtk::TreeViewColumn*> cols = get_columns();
  for (Glib::ListHandle<Gtk::TreeViewColumn*>::const_iterator iloop = cols.begin();
      iloop != cols.end(); ++iloop, ++col)
    if (*iloop == focus)
      break;
  //std::cerr << "qt:" << col << "\n";
#warning ungeklaert
  --col; // ????
  if (col >= Cols())
    return false;
  std::string tip=static_cast<cH_RowDataBase>((*i)[getStore()->m_columns.leafdata])->ToolTip(IndexFromColumn(col),getStore()->ValueData());
  tooltip->set_markup(tip);
  set_tooltip_cell(tooltip, &p, focus,
      focus->get_first_cell_renderer());
  return true;
}

