
#include <SimpleTree.hh>
#include <Misc/EntryValueIntString.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>

static Pango::AttrList bold,italic,bigred;

struct MyRow: RowDataBase
{
  unsigned i;

  MyRow(unsigned _i=0) : i(_i) {}
  virtual cH_EntryValue Value(guint _seqnr,gpointer gp) const
  {
    return cH_EntryValueIntString(i);
  }
  virtual std::string ToolTip(guint _seqnr,gpointer _g) const 
  { char buf[128]; snprintf(buf,sizeof(buf),"Tooltip for %d,%d", i, _seqnr); return buf; }
  // font rendering attributes
  virtual Pango::AttrList attributes(guint _seqnr,gpointer _g) const { return i==2 ? bigred : (_seqnr+i)&1 ? bold : italic; }
};

class MyProperties : public SimpleTreeModel_Properties
{ virtual unsigned Columns() const { return 2; }
  virtual Glib::ustring Title(guint _seqnr) const
  { switch (_seqnr)
    { case 0: return "Integer";
      case 1: return "Second";
      default: return Glib::ustring();
    }
  }
  virtual std::string ProgramName() const { return "(example)"; }
  virtual std::string InstanceName() const { return "advanced"; }
  virtual bool attributes(guint idx) const { return true; }
};

int main(int argc, char **argv)
{
  Gtk::Main m(argc,argv);
//  Pango::Attribute a(Pango::Attribute::create_attr_size (30));
//  bold.insert(a);
  bold= Pango::AttrList("<b>1</b>");
  italic= Pango::AttrList("<i>1</i>");
  bigred= Pango::AttrList("<big><span foreground='red'>1</span></big>");
  Gtk::Window w;
  MyProperties prop;
  SimpleTree s(2);
  s.setProperties(prop,false);
  std::vector <cH_RowDataBase> datavec;
  datavec.push_back(new MyRow(1));
  datavec.push_back(new MyRow(1));
  datavec.push_back(new MyRow(2));
  datavec.push_back(new MyRow(3));
  s.setDataVec(datavec);
  s.EnableTooltips(true);
  w.add(s);
  s.show();
  w.show();
  m.run(w);
  return 0;
}
