# $Id: Makefile.am,v 1.35 2004/07/15 08:10:00 christof Exp $
AM_CXXFLAGS= @COMMONGTK2_INCLUDES@ @COMMONXX_INCLUDES@ -DlibManuProC_Widgets_COMPILATION
AM_CFLAGS= @GTKMM2_INCLUDES@ -DlibManuProC_Widgets_COMPILATION -I../../GtkmmAddons/searchcombo/src

if WINDOWS
WIN_LDFLAGS=-no-undefined -export-symbols libManuProC_Widgets.def @COMMONXX_LDFLAGS@
DLL_LDADD=libManuProC_Widgets_res.lo @GTKMM2_LIBS@ @COMMONXX_LIBS@ @SIGC_LIBS@ @ECPG_LIBS@ -lcomdlg32
DLL_DEF=libManuProC_Widgets.def
ADDONS_FILES=Gtk_OStream_label.cc \
		 Gtk_OStream_optionmenu.cc Gtk_OStream_stream.cc \
		 SearchCombo.cc TeX.cc Gtk_OStream.cc \
		 gtksearchcombo.c Gtk_OStream_TreeView.cc \
		 DoubleButton.cc doublebutton.c WinFileReq.cc \
		 ChoiceButton.cc gtkhacks.cc Gtk2TeX.cc \
		 expander_hook.c 
else
WIN_LDFLAGS=
DLL_LDADD=@SIGC_LIBS@ @ECPG_LIBS@
DLL_DEF=
ADDONS_FILES=
endif

%.cc: %.pgcc ; @ECPG@ -o $*.cc $<

libManuProC_Widgets_res.lo: libManuProC_Widgets.rc
	WINDRES=i586-mingw32msvc-windres $(top_srcdir)/../ManuProC_Base/build/win32/lt-compile-resource $< $@ 

include_HEADERS = logwin.h EingabeBox.hh \
	RowDataBase.h TreeRow.h treebase_data.h \
	SimpleTreeModel.h SimpleTreeStore.h \
	bool_CheckMenuItem.hh char_Pixmap.hh \
	FlashingPixmap.hh LabelSpin.hh MyWindow.hh MyScale.hh \
	TreeViewUtility.h bool_properties.hh bool_ImageButton.hh \
	ModelWidgetConnection.h string_Label.hh RefPtr_Pixmap.hh \
	Association.h TreeModelDebug.h \
	SearchComboContent.hh int_ImageButton.hh \
	IntStringBox_gui.hh
	
noinst_HEADERS=SimpleTree.hh IntStringBox.hh

lib_LTLIBRARIES= libManuProC_Widgets.la
libManuProC_Widgets_la_LIBADD=$(DLL_LDADD)

EXTRA_PROGRAMS=dummy

libManuProC_Widgets_la_SOURCES=EingabeBox.cc \
		bool_CheckButton.cc \
		int_SpinButton.cc string_Entry.cc \
		SimpleTreeModel.cc SimpleTreeStore.cc \
		GType_cH_EntryValue.cc SimpleTree.cc \
		bool_CheckMenuItem.cc char_Pixmap.cc \
		FlashingPixmap.cc LabelSpin.cc RefPtr_Pixmap.cc \
		MyWindow.cc MyScale.cc logwin.cc TreeViewUtility.cc \
		bool_properties.cc bool_ImageButton.cc string_Label.cc \
		Association.cc Event.cc TooltipView.cc \
		SimpleTreeStore_test.cc CellRendererSimpleTree.cc \
		TreeModelDebug.cc int_ImageButton.cc \
		IntStringBox_gui.cc IntStringBox.cc IntStringBox_glade.cc \
		IntStringPopup_glade.cc datewin.cc datewin_glade.cc \
		datewin_popup.cc datewin_popup_glade.cc BasicExcel.cpp \
		md5/md5.c md5/md5string.cc getuid.cc \
		$(DLL_DEF) $(ADDONS_FILES)

BUILT_SOURCES=datewin.hh logwin.hh

AM_LDFLAGS=@LDFLAGS@ @ECPG_LDFLAGS@ $(WIN_LDFLAGS)

# alias links
datewin.hh: datewin.h
	ln -sf datewin.h datewin.hh
logwin.hh: logwin.h
	ln -sf logwin.h logwin.hh

CLEANFILES= *~ $(BUILT_SOURCES)

$(ADDONS_FILES):
	ln -sf ../../GtkmmAddons/src/$@ .
	