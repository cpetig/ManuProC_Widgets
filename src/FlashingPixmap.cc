/*  Midgard Character Generator
 *  Copyright (C) 2001-2002 Malte Thoma
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

#include "FlashingPixmap.hh"
#include <glibmm/main.h> 

FlashingPixmap::FlashingPixmap(Glib::RefPtr<Gdk::Pixbuf> d,
                     Glib::RefPtr<Gdk::Pixbuf> f,unsigned int msec)
:Gtk::Image(d), defaultPix(d),flashPix(f),stop(true), def(true)
{ set(d,f,msec);
}

void FlashingPixmap::setTime(unsigned int msec)
{
  des.disconnect();
  if(msec==0) 
   {
     stop=true;
     if (!def) Gtk::Image::set(defaultPix);
   }
  else 
   {
     stop=false;
     Gtk::Image::set(flashPix); def=false;
     des = Glib::signal_timeout().connect(sigc::mem_fun(*this,&FlashingPixmap::timeout),msec);
   }
}

bool FlashingPixmap::timeout() 
{ 
  switchPix();
  return !stop;
}

void FlashingPixmap::switchPix()
{  def=!def;
   Gtk::Image::set(def?defaultPix:flashPix);
}

void FlashingPixmap::set(Glib::RefPtr<Gdk::Pixbuf> d,Glib::RefPtr<Gdk::Pixbuf> f,unsigned int msec)
{setDefault(d);setFlash(f);setTime(msec); Gtk::Image::set(defaultPix);
}

