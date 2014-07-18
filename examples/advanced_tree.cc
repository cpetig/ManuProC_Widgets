
#include <SimpleTree.hh>
#include <Misc/EntryValueIntString.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>

static Pango::AttrList bold,italic,bigred,smallgreen;

struct MyRow: RowDataBase
{
  unsigned i;

  MyRow(unsigned _i=0) : i(_i) {}
  virtual cH_EntryValue Value(guint _seqnr,gpointer gp) const
  {
    if (_seqnr==0) return cH_EntryValueIntString(i);
    return cH_EntryValueIntString(i==2 ? (_seqnr==1 ? "big red" : "small green") : (_seqnr+i)&1 ? "bold" : "italic");
  }
  virtual std::string ToolTip(guint _seqnr,gpointer _g) const 
  { char buf[128]; snprintf(buf,sizeof(buf),"Tooltip for %d,%d", i, _seqnr); return buf; }
  // font rendering attributes
  virtual Pango::AttrList attributes(guint _seqnr,gpointer _g) const 
  { return _seqnr==0 ? Pango::AttrList() : i==2 ? (_seqnr==1 ? bigred : smallgreen) : (_seqnr+i)&1 ? bold : italic; }
};

class MyProperties : public SimpleTreeModel_Properties
{ virtual unsigned Columns() const { return 3; }
  virtual Glib::ustring Title(guint _seqnr) const
  { switch (_seqnr)
    { case 0: return "Integer";
      case 1: return "Second";
      case 2: return "Third";
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
  Pango::Attribute a(Pango::Attribute::create_attr_weight(Pango::WEIGHT_BOLD));
  bold.insert(a);
  
  a= Pango::Attribute::create_attr_style(Pango::STYLE_ITALIC);
  italic.insert(a);
  
  a= Pango::Attribute::create_attr_foreground(0xffff,0,0);
  bigred.insert(a);
  a= Pango::Attribute::create_attr_scale(1.2);
  bigred.insert(a);

  a= Pango::Attribute::create_attr_foreground(0,0xffff,0);
  smallgreen.insert(a);
  a= Pango::Attribute::create_attr_scale(0.8);
  smallgreen.insert(a);
   
  Gtk::Window w;
  MyProperties prop;
  SimpleTree s(3);
  s.setProperties(prop,false);
  std::vector <cH_RowDataBase> datavec;
  datavec.push_back(new MyRow(1));
//  datavec.push_back(new MyRow(1));
  datavec.push_back(new MyRow(2));
  datavec.push_back(new MyRow(3));
  datavec.push_back(new MyRow(4));
  s.setDataVec(datavec);
  s.EnableTooltips(true);
  w.add(s);
  s.show();
  w.show();
  m.run(w);
  return 0;
}
