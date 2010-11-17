// $Id: SimpleTreeStore.h,v 1.77 2006/08/09 11:12:16 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
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

#include "getuid.h"
#undef getuid

static int uid_override=-1;

#ifdef WIN32
# include "md5string.h"

# define WINVER 0x0500
# define SECURITY_WIN32
# include <windows.h>
# include <security.h>

// on Windows return the first 4 bytes of the md5 sum of "domain\user"

int getuid_ManuProC()
{
  if (uid_override!=-1) return uid_override;
  char buf[10240];
  DWORD sz=sizeof(buf);

  if (!GetUserNameExA(NameSamCompatible,buf,&sz))
  { if (!GetUserNameA(buf,&sz)) return 0;
  }
  std::string md5sum= md5(std::string(buf,buf+sz));
  uid_override= *(int*)md5sum.data();
  return uid_override;
}

#else // linux
# include <unistd.h>

int getuid_ManuProC()
{
  if (uid_override!=-1) return uid_override;
  uid_override= getuid();
  return uid_override;
}
#endif

void setuid_ManuProC(int id)
{
  uid_override= id;
}
