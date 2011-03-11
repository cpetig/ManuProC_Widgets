
#include<gtkmm/messagedialog.h>
#include<Misc/Datum.h>
#include <sstream>
#include<Misc/SQLerror.h>
#include <Misc/compiler_ports.h>
#include <Misc/itos.h>
//#include <Lager/FertigWarenLager.h>
//#include <ArtikelBox.hh>

#if defined(MABELLA_EXTENSIONS)
#include <Lager/FertigWarenLager.h>
#include "ArtikelBox.hh"
#endif


struct MyMessage : Gtk::MessageDialog
{
  static Glib::ustring stringify(const SQLerror &e)
  { char tmp[100];
    std::string _msg;
#ifdef MPC_SQLITE
    snprintf(tmp,sizeof tmp,"DB-Error Code:%d\n",e.Code());
#else
    snprintf(tmp,sizeof tmp,"DB-Error Code:%d %s\n",e.Code(),e.State().c_str());
#endif
    _msg=tmp;
    snprintf(tmp,sizeof tmp,"DB-Error Message:%s\n",e.Message().c_str());
    _msg+=tmp;
    snprintf(tmp,sizeof tmp,"Context:%s\n",e.Context().c_str());
    _msg+=tmp;
    return _msg;
  }
  static Glib::ustring stringify(const ManuProC::Datumsfehler &e)
  { std::stringstream ostr;
    ostr << e;
    return ostr.str();
  }
/*  static Glib::ustring stringify(std::string const&s)
  { return s; }*/
  static Glib::ustring stringify(Glib::ustring const&s)
  { return s; }

  MyMessage() : Gtk::MessageDialog("") {}

  MyMessage(const SQLerror &e,Gtk::MessageType mt=Gtk::MESSAGE_ERROR)
      : Gtk::MessageDialog(stringify(e),false,mt)
  { }

  MyMessage(const SQLerror &e, Gtk::Window& parent, Gtk::MessageType mt=Gtk::MESSAGE_ERROR)
      : Gtk::MessageDialog(parent,Glib::ustring(stringify(e)),false,mt,Gtk::BUTTONS_OK,true)
  { }

  MyMessage(const ManuProC::Datumsfehler &e,Gtk::MessageType mt=Gtk::MESSAGE_ERROR)
      : Gtk::MessageDialog(stringify(e),false,mt)
  { }

  MyMessage(Glib::ustring const& s,Gtk::MessageType mt=Gtk::MESSAGE_INFO)
      : Gtk::MessageDialog(s,false,mt)
  {
   property_window_position().set_value(Gtk::WIN_POS_CENTER);
  }

  MyMessage(Gtk::Window& parent, Glib::ustring const& s,Gtk::MessageType mt=Gtk::MESSAGE_INFO, bool markup=false, Gtk::ButtonsType buttons = Gtk::BUTTONS_OK, bool modal=true)
      : Gtk::MessageDialog(parent,s,markup,mt,buttons,modal)
  {
//   property_window_position().set_value(Gtk::WIN_POS_CENTER);
  }

#ifdef MABELLA_EXTENSIONS
  MyMessage(const LagerError &e,Gtk::MessageType mt=Gtk::MESSAGE_ERROR)
      : Gtk::MessageDialog(e.Text()+" ArtID:"+itos(e.ArtID()),false,mt)
  { }

  MyMessage(const ArtikelBoxErr &e,Gtk::MessageType mt=Gtk::MESSAGE_ERROR)
      : Gtk::MessageDialog(e.ErrMsg(),false,mt)
  { }
#endif

  template <class T>
   static int show_and_wait(T const& s, Gtk::Window &parent, Gtk::MessageType mt=Gtk::MESSAGE_INFO, bool markup=false, Gtk::ButtonsType buttons = Gtk::BUTTONS_OK, bool modal=true)
  { MyMessage m(parent, stringify(s),mt,markup,buttons,modal);
//    if (parent) m.set_transient_for(*parent);
    m.show();
    return m.run();
  }
  template <class T>
   static int show_and_wait(T const& s)
  { MyMessage m(stringify(s));
    m.show();
    return m.run();
  }
  //
  template <class T>
   static int show_and_wait(T const& s, Gtk::Container *toplevel)
  { if (toplevel) return show_and_wait(stringify(s),dynamic_cast<Gtk::Window*>(toplevel));
    else return show_and_wait(stringify(s));
  }

 __deprecated void set_Message(const std::string msg)
   { set_message(msg,true);
   }
 void set(const std::string msg)
   { set_message(msg,true);
   }
 void set(const ManuProC::Datumsfehler &e)
   { set_message(stringify(e),true);
   }
 void set(const SQLerror &e)
   { set_message(stringify(e),true);
   }
};

