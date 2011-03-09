/* $Id: TreeRow.h,v 1.6 2002/11/28 17:09:42 christof Exp $ */
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

// $Id: TreeRow.h,v 1.6 2002/11/28 17:09:42 christof Exp $

#ifndef MPC_TREEROW_H
#define MPC_TREEROW_H

#include<string>
#include<vector>
#include "RowDataBase.h"

class SimpleTreeStoreNode;

class TreeRow : public HandleContent
{
protected:
	// old API, still overloadable but gives less information
	virtual void cumulate(const cH_RowDataBase &rd) {}
public:
	virtual ~TreeRow() {}

	virtual cH_EntryValue Value(guint _seqnr,gpointer _g) const = 0;

	// actually deep and childrens_deep is the most interesting information of the second arg
	virtual void cumulate(const cH_RowDataBase &rd, SimpleTreeStoreNode const& nd)
	// fallback to old API by default
	{ cumulate(rd); }
	virtual void deduct(const cH_RowDataBase &rd) = 0;
};

#endif
