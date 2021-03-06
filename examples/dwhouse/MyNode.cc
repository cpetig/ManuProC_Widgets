// $Id: MyNode.cc,v 1.5 2002/07/05 12:36:56 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include<Misc/Ausgabe_neu.h>
#include"MyNode.h"
#include <Misc/EntryValueIntString.h>

void MyNode::cumulate(const cH_RowDataBase &rd)
{
 sum1+= (dynamic_cast<const MyRowData &>(*rd)).Data1();
 sum2+= (dynamic_cast<const MyRowData &>(*rd)).Data2();
}

void MyNode::deduct(const cH_RowDataBase &rd)
{
 sum1-= (dynamic_cast<const MyRowData &>(*rd)).Data1();
 sum2-= (dynamic_cast<const MyRowData &>(*rd)).Data2();
}

cH_EntryValue MyNode::Value(guint index,gpointer gp) const
{
 switch(index)
   { case 3 :
	return cH_EntryValueIntString(sum1);
     case 4 :
	return cH_EntryValueIntString(sum2);
     default : 
     	return cH_EntryValueIntString("-");
   }
}
