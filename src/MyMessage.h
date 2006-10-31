
#include<gtkmm/messagedialog.h>
#include<Misc/Datum.h>
#include <strstream>
#include<Misc/SQLerror.h>
#include <Misc/compiler_ports.h>

struct MyMessage : Gtk::MessageDialog
{
  std::string stringify(const SQLerror &e)
  { char tmp[100]; 
    std::string _msg;
    snprintf(tmp,sizeof tmp,"DB-Error Code:%d %s\n",e.Code(),e.State().c_str());
    _msg=tmp;
    snprintf(tmp,sizeof tmp,"DB-Error Message:%s\n",e.Message().c_str());  
    _msg+=tmp;
    snprintf(tmp,sizeof tmp,"Context:%s\n",e.Context().c_str());  
    _msg+=tmp;
    return _msg;
  }
  std::string stringify(const ManuProC::Datumsfehler &e)
  { std::strstream ostr;
    ostr << e;  
    return ostr.str();
  }

  MyMessage() : Gtk::MessageDialog("") {}

  MyMessage(const SQLerror &e,Gtk::MessageType mt=Gtk::MESSAGE_ERROR) 
      : Gtk::MessageDialog(stringify(e),false,mt)
  { }
 
  MyMessage(const ManuProC::Datumsfehler &e,Gtk::MessageType mt=Gtk::MESSAGE_ERROR)
      : Gtk::MessageDialog(stringify(e),false,mt)
  { }
 
  MyMessage(Glib::ustring const& s,Gtk::MessageType mt=Gtk::MESSAGE_INFO)
      : Gtk::MessageDialog(s,false,mt)
  { }

  template <class T>
   static int show_and_wait(T const& s, Gtk::Window *parent, Gtk::MessageType mt)
  { MyMessage m(s,mt);
    if (parent) m.set_transient_for(*parent);
    m.show();
    return m.run();
  }
  template <class T>
   static int show_and_wait(T const& s, Gtk::Window *parent=0)
  { MyMessage m(s);
    if (parent) m.set_transient_for(*parent);
    m.show();
    return m.run();
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

