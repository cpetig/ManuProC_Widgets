// $Id: SimpleTreeModel.h,v 1.17 2006/08/03 11:27:12 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *  Copyright (C) 2008-2010 Christof Petig
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

#ifndef SIMPLE_TREE_MODEL_H
#define SIMPLE_TREE_MODEL_H

#include <string>
#include <vector>
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <glib/gtypes.h>
#include <RowDataBase.h>

class SimpleTreeStore;
class SimpleTree_Basic;

// this is only a container, properties of the data belong into STM_Properties

class SimpleTreeModel : sigc::trackable
{public:
	typedef std::vector<cH_RowDataBase> datavec_t;
	
private:
	friend class SimpleTreeStore; // für signal_value_changed
	friend class SimpleTree_Basic;
	
	datavec_t datavec;

	sigc::signal<void,cH_RowDataBase> line_appended;
	sigc::signal<void,cH_RowDataBase> line_to_remove;
//	sigc::signal<void> redraw_needed;
	sigc::signal<void> please_detach;
	sigc::signal<void> please_attach;
	// a column was changed, change data, redraw?
	sigc::signal<void,cH_RowDataBase,unsigned,const std::string &,bool &> value_changed;

protected:
	// sollte besser über RowDataBase::changeValue gehen, ist sauberer
	sigc::signal<void,cH_RowDataBase,unsigned,std::string const&,bool &> 
		&signal_value_changed()
	{  return value_changed; }
public:
	// more STLish names
	void push_back(const cH_RowDataBase &row)
	{ append_line(row); }
	void erase(const cH_RowDataBase &row)
	{ remove_line(row); }
	void clear();
	void operator=(const datavec_t &d)
	{ setDataVec(d); }
	typedef datavec_t::const_iterator const_iterator;
	typedef datavec_t::iterator iterator;
	typedef datavec_t::size_type size_type;
	const_iterator begin() const { return datavec.begin(); }
	const_iterator end() const { return datavec.end(); }
	iterator begin() { return datavec.begin(); }
	iterator end() { return datavec.end(); }
	bool empty() { return datavec.empty(); }
	size_type size() { return datavec.size(); }

	// please prefer the more STLish variants
	void append_line(const cH_RowDataBase &row);
	void remove_line(const cH_RowDataBase &row);
	
	void setDataVec(const datavec_t &d);
	// slower variant which incrementally changes contents
	void changeDataVec(const datavec_t &d, bool (*equal)(cH_RowDataBase const& a,cH_RowDataBase const& b));
	const datavec_t &getDataVec() const
	{  return datavec; }

	sigc::signal<void,cH_RowDataBase> &signal_line_appended()
	{  return line_appended; }
	sigc::signal<void,cH_RowDataBase> &signal_line_to_remove()
	{  return line_to_remove; }
	// a big reorganization is about to occur
	sigc::signal<void> &signal_please_detach()
	{  return please_detach; }
	// reorganization finished, you can redisplay
	sigc::signal<void> &signal_please_attach()
	{  return please_attach; }

	void about_to_change(const cH_RowDataBase &row)
	{  signal_line_to_remove()(row); }
	void has_changed(const cH_RowDataBase &row)
	{  signal_line_appended()(row); }
};

#endif
