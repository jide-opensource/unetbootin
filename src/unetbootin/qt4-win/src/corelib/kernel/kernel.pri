# Qt core object module

HEADERS += \
	kernel/qabstracteventdispatcher.h \
        kernel/qabstractitemmodel.h \
        kernel/qabstractitemmodel_p.h \
	kernel/qbasictimer.h \
	kernel/qeventloop.h\
	kernel/qpointer.h \
        kernel/qcorecmdlineargs_p.h \
	kernel/qcoreapplication.h \
	kernel/qcoreevent.h \
	kernel/qmetaobject.h \
	kernel/qmetatype.h \
        kernel/qmimedata.h \
	kernel/qobject.h \
	kernel/qobjectdefs.h \
	kernel/qsignalmapper.h \
	kernel/qsocketnotifier.h \
	kernel/qtimer.h \
	kernel/qtranslator.h \
        kernel/qtranslator_p.h \
        kernel/qvariant.h \
	kernel/qabstracteventdispatcher_p.h \
	kernel/qcoreapplication_p.h \
	kernel/qobjectcleanuphandler.h \
        kernel/qvariant_p.h \
        kernel/qmetaobject_p.h \
        kernel/qobject_p.h \
        kernel/qcoreglobaldata_p.h \
        kernel/qsharedmemory.h \
        kernel/qsharedmemory_p.h \
        kernel/qsystemsemaphore.h \
        kernel/qsystemsemaphore_p.h

SOURCES += \
	kernel/qabstracteventdispatcher.cpp \
        kernel/qabstractitemmodel.cpp \
	kernel/qbasictimer.cpp \
	kernel/qeventloop.cpp \
	kernel/qcoreapplication.cpp \
	kernel/qcoreevent.cpp \
	kernel/qmetaobject.cpp \
	kernel/qmetatype.cpp \
        kernel/qmimedata.cpp \
	kernel/qobject.cpp \
	kernel/qobjectcleanuphandler.cpp \
	kernel/qsignalmapper.cpp \
	kernel/qsocketnotifier.cpp \
	kernel/qtimer.cpp \
	kernel/qtranslator.cpp \
	kernel/qvariant.cpp \
        kernel/qcoreglobaldata.cpp \
        kernel/qsharedmemory.cpp \
        kernel/qsystemsemaphore.cpp

win32 {
	SOURCES += \
		kernel/qeventdispatcher_win.cpp \
		kernel/qcoreapplication_win.cpp \
		kernel/qwineventnotifier_p.cpp \
                kernel/qsharedmemory_win.cpp \
                kernel/qsystemsemaphore_win.cpp
	HEADERS += \
		kernel/qeventdispatcher_win_p.h \
		kernel/qwineventnotifier_p.h
}


wince*: {
	SOURCES += \
		kernel/qfunctions_wince.cpp
	HEADERS += \
		kernel/qfunctions_wince.h
}

mac:!embedded {
    SOURCES += \
        kernel/qcoreapplication_mac.cpp
}

mac {
       SOURCES += \
		kernel/qcore_mac.cpp
}

unix {
	SOURCES += \
                kernel/qcrashhandler.cpp \
                kernel/qsharedmemory_unix.cpp \
                kernel/qsystemsemaphore_unix.cpp
	HEADERS += \
		kernel/qcrashhandler_p.h

        contains(QT_CONFIG, glib) {
            SOURCES += \
                kernel/qeventdispatcher_glib.cpp
            HEADERS += \
                kernel/qeventdispatcher_glib_p.h
            QMAKE_CXXFLAGS += $$QT_CFLAGS_GLIB
            LIBS +=$$QT_LIBS_GLIB
        }
            SOURCES += \
                kernel/qeventdispatcher_unix.cpp
            HEADERS += \
                kernel/qeventdispatcher_unix_p.h

   contains(QT_CONFIG, clock-gettime):include($$QT_SOURCE_TREE/config.tests/unix/clock-gettime/clock-gettime.pri)
}

