# $Id: Makefile.am,v 1.35 2004/07/15 08:10:00 christof Exp $
AM_CXXFLAGS= @COMMONGTK2_INCLUDES@ @COMMONXX_INCLUDES@ -DlibManuProC_Widgets_COMPILATION

if WINDOWS
WIN_LDFLAGS=-no-undefined -export-symbols libManuProC_Widgets.def
DLL_LDADD=libManuProC_Widgets_res.lo @COMMONGTK2_LIBS@ @SIGC_LIBS@ @ECPG_LIBS@
DLL_DEF=libManuProC_Widgets.def
else
WIN_LDFLAGS=
DLL_LDADD=@SIGC_LIBS@ @ECPG_LIBS@
DLL_DEF=
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
		$(DLL_DEF)

BUILT_SOURCES=datewin.hh logwin.hh

# alias links
datewin.hh: datewin.h
	ln -sf datewin.h datewin.hh
logwin.hh: logwin.h
	ln -sf logwin.h logwin.hh

CLEANFILES= *~ $(BUILT_SOURCES)
