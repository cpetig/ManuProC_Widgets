// $Id: IntStringBox_gui.hh,v 1.6 2003/03/17 16:53:36 christof Exp $
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

#ifndef _INTSTRINGBOX_GUI_HH
#  define _INTSTRINGBOX_GUI_HH

#include <IntStringBox_glade.hh>
#include "IntStringPopup_glade.hh"

struct IntStringPopup : public IntStringPopup_glade
{  IntStringPopup();
   Glib::SignalProxy0<void> signal_zurueck()
   {  return zurueck->signal_activate();
   }
//   ~IntStringPopup() {}
};

class IntStringBox_gui : public IntStringBox_glade
{   
        void fuelleMenu();
        bool MouseButton(GdkEventButton *event);
        virtual void reset() = 0;
protected:

        IntStringPopup *menu;

        IntStringBox_gui();
        ~IntStringBox_gui();
        
};
#endif
