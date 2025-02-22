/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qplatformdefs.h"
#include "qstring.h"
#include "qvector.h"
#include "qlist.h"
#include "qthreadstorage.h"

#ifndef QT_NO_QOBJECT
#include <private/qthread_p.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>

#if !defined(Q_OS_WINCE)
#  include <errno.h>
#  if defined(Q_CC_MSVC)
#    include <crtdbg.h>
#  endif
#endif

#ifdef Q_CC_MWERKS
#include <CoreServices/CoreServices.h>
#endif

QT_BEGIN_NAMESPACE

/* ### Qt 5: We should get QFlags more typesafe by not allowing implicit
 * construction from int/uint or bool. See task 181764, or talk to Matthias,
 * Jasmin or Frans. */

/*!
    \class QFlag
    \brief The QFlag class is a helper data type for QFlags.

    It is equivalent to a plain \c int, except with respect to
    function overloading and type conversions. You should never need
    to use this class in your applications.

    \sa QFlags
*/

/*!
    \fn QFlag::QFlag(int value)

    Constructs a QFlag object that stores the given \a value.
*/

/*!
    \fn QFlag::operator int() const

    Returns the value stored by the QFlag object.
*/

/*!
    \class QFlags
    \brief The QFlags class provides a type-safe way of storing
    OR-combinations of enum values.

    \mainclass
    \ingroup tools

    The QFlags<Enum> class is a template class, where Enum is an enum
    type. QFlags is used throughout Qt for storing combinations of
    enum values.

    The traditional C++ approach for storing OR-combinations of enum
    values is to use an \c int or \c uint variable. The inconvenience
    with this approach is that there's no type checking at all; any
    enum value can be OR'd with any other enum value and passed on to
    a function that takes an \c int or \c uint.

    Qt uses QFlags to provide type safety. For example, the
    Qt::Alignment type is simply a typedef for
    QFlags<Qt::AlignmentFlag>. QLabel::setAlignment() takes a
    Qt::Alignment parameter, which means that any combination of
    Qt::AlignmentFlag values is legal:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 0

    If you try to pass a value from another enum, the compiler will
    report an error.

    If you want to use QFlags for your own enum types, use
    the Q_DECLARE_FLAGS() and Q_DECLARE_OPERATORS_FOR_FLAGS().
    For example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 1

    You can then use the \c MyClass::Options type to store
    combinations of \c MyClass::Option values.

    A sensible naming convension for enum types and associated QFlags
    types is to give a singular name to the enum type (e.g., \c
    Option) and a plural name to the QFlags type (e.g., \c Options).
    When a singular name is desired for the QFlags type (e.g., \c
    Alignment), you can use \c Flag as the suffix for the enum type
    (e.g., \c AlignmentFlag).

    \sa QFlag
*/

/*!
    \typedef QFlags::enum_type

    Typedef for the Enum template type.
*/

/*!
    \fn QFlags::QFlags(const QFlags &other)

    Constructs a copy of \a other.
*/

/*!
    \fn QFlags::QFlags(Enum flag)

    Constructs a QFlags object storing the given \a flag.
*/

/*!
    \fn QFlags::QFlags(Zero zero)

    Constructs a QFlags object with no flags set. \a zero must be a
    literal 0 value.
*/

/*!
    \fn QFlags::QFlags(QFlag value)

    Constructs a QFlags object initialized with the given integer \a
    value.

    The QFlag type is a helper type. By using it here instead of \c
    int, we effectively ensure that arbitrary enum values cannot be
    cast to a QFlags, whereas untyped enum values (i.e., \c int
    values) can.
*/

/*!
    \fn QFlags &QFlags::operator=(const QFlags &other)

    Assigns \a other to this object and returns a reference to this
    object.
*/

/*!
    \fn QFlags &QFlags::operator&=(int mask)

    Performs a bitwise AND operation with \a mask and stores the
    result in this QFlags object. Returns a reference to this object.

    \sa operator&(), operator|=(), operator^=()
*/

/*!
    \fn QFlags &QFlags::operator&=(uint mask)

    \overload
*/

/*!
    \fn QFlags &QFlags::operator|=(QFlags other)

    Performs a bitwise OR operation with \a other and stores the
    result in this QFlags object. Returns a reference to this object.

    \sa operator|(), operator&=(), operator^=()
*/

/*!
    \fn QFlags &QFlags::operator|=(Enum other)

    \overload
*/

/*!
    \fn QFlags &QFlags::operator^=(QFlags other)

    Performs a bitwise XOR operation with \a other and stores the
    result in this QFlags object. Returns a reference to this object.

    \sa operator^(), operator&=(), operator|=()
*/

/*!
    \fn QFlags &QFlags::operator^=(Enum other)

    \overload
*/

/*!
    \fn QFlags::operator int() const

    Returns the value stored in the QFlags object as an integer.
*/

/*!
    \fn QFlags QFlags::operator|(QFlags other) const

    Returns a QFlags object containing the result of the bitwise OR
    operation on this object and \a other.

    \sa operator|=(), operator^(), operator&(), operator~()
*/

/*!
    \fn QFlags QFlags::operator|(Enum other) const

    \overload
*/

/*!
    \fn QFlags QFlags::operator^(QFlags other) const

    Returns a QFlags object containing the result of the bitwise XOR
    operation on this object and \a other.

    \sa operator^=(), operator&(), operator|(), operator~()
*/

/*!
    \fn QFlags QFlags::operator^(Enum other) const

    \overload
*/

/*!
    \fn QFlags QFlags::operator&(int mask) const

    Returns a QFlags object containing the result of the bitwise AND
    operation on this object and \a mask.

    \sa operator&=(), operator|(), operator^(), operator~()
*/

/*!
    \fn QFlags QFlags::operator&(uint mask) const

    \overload
*/

/*!
    \fn QFlags QFlags::operator&(Enum mask) const

    \overload
*/

/*!
    \fn QFlags QFlags::operator~() const

    Returns a QFlags object that contains the bitwise negation of
    this object.

    \sa operator&(), operator|(), operator^()
*/

/*!
    \fn bool QFlags::operator!() const

    Returns true if no flag is set (i.e., if the value stored by the
    QFlags object is 0); otherwise returns false.
*/

/*!
    \fn bool QFlags::testFlag(Enum flag) const
    \since 4.2

    Returns true if the \a flag is set, otherwise false.
*/

/*!
    \macro Q_DECLARE_FLAGS(Flags, Enum)
    \relates QFlags

    The Q_DECLARE_FLAGS() macro expands to

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 2

    \a Enum is the name of an existing enum type, whereas \a Flags is
    the name of the QFlags<\e{Enum}> typedef.

    See the QFlags documentation for details.

    \sa Q_DECLARE_OPERATORS_FOR_FLAGS()
*/

/*!
    \macro Q_DECLARE_OPERATORS_FOR_FLAGS(Flags)
    \relates QFlags

    The Q_DECLARE_OPERATORS_FOR_FLAGS() macro declares global \c
    operator|() functions for \a Flags, which is of type QFlags<T>.

    See the QFlags documentation for details.

    \sa Q_DECLARE_FLAGS()
*/

/*!
    \headerfile <QtGlobal>
    \title Global Qt Declarations
    \ingroup architecture

    \brief The <QtGlobal> header provides basic declarations and
    is included by all other Qt headers.

    The declarations include \l {types}, \l functions and
    \l macros.

    The type definitions are partly convenience definitions for basic
    types (some of which guarantee certain bit-sizes on all platforms
    supported by Qt), partly types related to Qt message handling. The
    functions are related to generating messages, Qt version handling
    and comparing and adjusting object values. And finally, some of
    the declared macros enable programmers to add compiler or platform
    specific code to their applications, while others are convenience
    macros for larger operations.

    \section1 Types

    The header file declares several type definitions that guarantee a
    specified bit-size on all platforms supported by Qt for various
    basic types, for example \l qint8 which is a signed char
    guaranteed to be 8-bit on all platforms supported by Qt. The
    header file also declares the \l qlonglong type definition for \c
    {long long int } (\c __int64 on Windows).

    Several convenience type definitions are declared: \l qreal for \c
    double, \l uchar for \c unsigned char, \l uint for \c unsigned
    int, \l ulong for \c unsigned long and \l ushort for \c unsigned
    short.

    Finally, the QtMsgType definition identifies the various messages
    that can be generated and sent to a Qt message handler;
    QtMsgHandler is a type definition for a pointer to a function with
    the signature \c {void myMsgHandler(QtMsgType, const char *)}.

    \section1 Functions

    The <QtGlobal> header file contains several functions comparing
    and adjusting an object's value. These functions take a template
    type as argument: You can retrieve the absolute value of an object
    using the qAbs() function, and you can bound a given object's
    value by given minimum and maximum values using the qBound()
    function. You can retrieve the minimum and maximum of two given
    objects using qMin() and qMax() respectively. All these functions
    return a corresponding template type; the template types can be
    replaced by any other type. For example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 3

    <QtGlobal> also contains functions that generate messages from the
    given string argument: qCritical(), qDebug(), qFatal() and
    qWarning(). These functions call the message handler with the
    given message. For example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 4

    The remaining functions are qRound() and qRound64(), which both
    accept a \l qreal value as their argument returning the value
    rounded up to the nearest integer and 64-bit integer respectively,
    the qInstallMsgHandler() function which installs the given
    QtMsgHandler, and the qVersion() function which returns the
    version number of Qt at run-time as a string.

    \section1 Macros

    The <QtGlobal> header file provides a range of macros (Q_CC_*)
    that are defined if the application is compiled using the
    specified platforms. For example, the Q_CC_SUN macro is defined if
    the application is compiled using Forte Developer, or Sun Studio
    C++.  The header file also declares a range of macros (Q_OS_*)
    that are defined for the specified platforms. For example,
    Q_OS_X11 which is defined for the X Window System.

    The purpose of these macros is to enable programmers to add
    compiler or platform specific code to their application.

    The remaining macros are convenience macros for larger operations:
    The QT_TRANSLATE_NOOP() and QT_TR_NOOP() macros provide the
    possibility of marking text for dynamic translation,
    i.e. translation without changing the stored source text. The
    Q_ASSERT() and Q_ASSERT_X() enables warning messages of various
    level of refinement. The Q_FOREACH() and foreach() macros
    implement Qt's foreach loop.

    The Q_INT64_C() and Q_UINT64_C() macros wrap signed and unsigned
    64-bit integer literals in a platform-independent way. The
    Q_CHECK_PTR() macro prints a warning containing the source code's
    file name and line number, saying that the program ran out of
    memory, if the pointer is 0. The qPrintable() macro represent an
    easy way of printing text.

    Finally, the QT_POINTER_SIZE macro expands to the size of a
    pointer in bytes, and the QT_VERSION and QT_VERSION_STR macros
    expand to a numeric value or a string, respectively, specifying
    Qt's version number, i.e the version the application is compiled
    against.

    \sa <QtAlgorithms>, QSysInfo
*/

/*!
    \typedef qreal
    \relates <QtGlobal>

    Typedef for \c double on all platforms except for those using CPUs with
    ARM architectures.
    On ARM-based platforms, \c qreal is a typedef for \c float for performance
    reasons.
*/

/*! \typedef uchar
    \relates <QtGlobal>

    Convenience typedef for \c{unsigned char}.
*/

/*!
    \fn qt_set_sequence_auto_mnemonic(bool on)
    \relates <QtGlobal>

    Enables automatic mnemonics on Mac if \a on is true; otherwise
    this feature is disabled.

    Note that this function is only available on Mac where mnemonics
    are disabled by default.

    \sa {QShortcut#mnemonic}{QShortcut}
*/

/*! \typedef ushort
    \relates <QtGlobal>

    Convenience typedef for \c{unsigned short}.
*/

/*! \typedef uint
    \relates <QtGlobal>

    Convenience typedef for \c{unsigned int}.
*/

/*! \typedef ulong
    \relates <QtGlobal>

    Convenience typedef for \c{unsigned long}.
*/

/*! \typedef qint8
    \relates <QtGlobal>

    Typedef for \c{signed char}. This type is guaranteed to be 8-bit
    on all platforms supported by Qt.
*/

/*!
    \typedef quint8
    \relates <QtGlobal>

    Typedef for \c{unsigned char}. This type is guaranteed to
    be 8-bit on all platforms supported by Qt.
*/

/*! \typedef qint16
    \relates <QtGlobal>

    Typedef for \c{signed short}. This type is guaranteed to be
    16-bit on all platforms supported by Qt.
*/

/*!
    \typedef quint16
    \relates <QtGlobal>

    Typedef for \c{unsigned short}. This type is guaranteed to
    be 16-bit on all platforms supported by Qt.
*/

/*! \typedef qint32
    \relates <QtGlobal>

    Typedef for \c{signed int}. This type is guaranteed to be 32-bit
    on all platforms supported by Qt.
*/

/*!
    \typedef quint32
    \relates <QtGlobal>

    Typedef for \c{unsigned int}. This type is guaranteed to
    be 32-bit on all platforms supported by Qt.
*/

/*! \typedef qint64
    \relates <QtGlobal>

    Typedef for \c{long long int} (\c __int64 on Windows). This type
    is guaranteed to be 64-bit on all platforms supported by Qt.

    Literals of this type can be created using the Q_INT64_C() macro:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 5

    \sa Q_INT64_C(), quint64, qlonglong
*/

/*!
    \typedef quint64
    \relates <QtGlobal>

    Typedef for \c{unsigned long long int} (\c{unsigned __int64} on
    Windows). This type is guaranteed to be 64-bit on all platforms
    supported by Qt.

    Literals of this type can be created using the Q_UINT64_C()
    macro:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 6

    \sa Q_UINT64_C(), qint64, qulonglong
*/

/*!
    \typedef quintptr
    \relates <QtGlobal>

    Integral type for representing a pointers (useful for hashing,
    etc.).

    Typedef for either quint32 or quint64. This type is guaranteed to
    be the same size as a pointer on all platforms supported by Qt. On
    a system with 32-bit pointers, quintptr is a typedef for quint32;
    on a system with 64-bit pointers, quintptr is a typedef for
    quint64.

    Note that quintptr is unsigned. Use qptrdiff for signed values.

    \sa qptrdiff, quint32, quint64
*/

/*!
    \typedef qptrdiff
    \relates <QtGlobal>

    Integral type for representing pointer differences.

    Typedef for either qint32 or qint64. This type is guaranteed to be
    the same size as a pointer on all platforms supported by Qt. On a
    system with 32-bit pointers, quintptr is a typedef for quint32; on
    a system with 64-bit pointers, quintptr is a typedef for quint64.

    Note that qptrdiff is signed. Use quintptr for unsigned values.

    \sa quintptr, qint32, qint64
*/

/*!
    \typedef QtMsgHandler
    \relates <QtGlobal>

    This is a typedef for a pointer to a function with the following
    signature:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 7

    \sa QtMsgType, qInstallMsgHandler()
*/

/*!
    \enum QtMsgType
    \relates <QtGlobal>

    This enum describes the messages that can be sent to a message
    handler (QtMsgHandler). You can use the enum to identify and
    associate the various message types with the appropriate
    actions.

    \value QtDebugMsg
           A message generated by the qDebug() function.
    \value QtWarningMsg
           A message generated by the qWarning() function.
    \value QtCriticalMsg
           A message generated by the qCritical() function.
    \value QtFatalMsg
           A message generated by the qFatal() function.
    \value QtSystemMsg


    \sa QtMsgHandler, qInstallMsgHandler()
*/

/*! \macro qint64 Q_INT64_C(literal)
    \relates <QtGlobal>

    Wraps the signed 64-bit integer \a literal in a
    platform-independent way. For example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 8

    \sa qint64, Q_UINT64_C()
*/

/*! \macro quint64 Q_UINT64_C(literal)
    \relates <QtGlobal>

    Wraps the unsigned 64-bit integer \a literal in a
    platform-independent way. For example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 9

    \sa quint64, Q_INT64_C()
*/

/*! \typedef qlonglong
    \relates <QtGlobal>

    Typedef for \c{long long int} (\c __int64 on Windows). This is
    the same as \l qint64.

    \sa qulonglong, qint64
*/

/*!
    \typedef qulonglong
    \relates <QtGlobal>

    Typedef for \c{unsigned long long int} (\c{unsigned __int64} on
    Windows). This is the same as \l quint64.

    \sa quint64, qlonglong
*/

/*! \fn const T &qAbs(const T &value)
    \relates <QtGlobal>

    Returns the absolute value of \a value. For example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 10
*/

/*! \fn int qRound(qreal value)
    \relates <QtGlobal>

    Rounds \a value to the nearest integer. For example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 11
*/

/*! \fn qint64 qRound64(qreal value)
    \relates <QtGlobal>

    Rounds \a value to the nearest 64-bit integer. For example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 12
*/

/*! \fn const T &qMin(const T &value1, const T &value2)
    \relates <QtGlobal>

    Returns the minimum of \a value1 and \a value2. For example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 13

    \sa qMax(), qBound()
*/

/*! \fn const T &qMax(const T &value1, const T &value2)
    \relates <QtGlobal>

    Returns the maximum of \a value1 and \a value2. For example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 14

    \sa qMin(), qBound()
*/

/*! \fn const T &qBound(const T &min, const T &value, const T &max)
    \relates <QtGlobal>

    Returns \a value bounded by \a min and \a max. This is equivalent
    to qMax(\a min, qMin(\a value, \a max)). For example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 15

    \sa qMin(), qMax()
*/

/*!
    \typedef Q_INT8
    \relates <QtGlobal>
    \compat

    Use \l qint8 instead.
*/

/*!
    \typedef Q_UINT8
    \relates <QtGlobal>
    \compat

    Use \l quint8 instead.
*/

/*!
    \typedef Q_INT16
    \relates <QtGlobal>
    \compat

    Use \l qint16 instead.
*/

/*!
    \typedef Q_UINT16
    \relates <QtGlobal>
    \compat

    Use \l quint16 instead.
*/

/*!
    \typedef Q_INT32
    \relates <QtGlobal>
    \compat

    Use \l qint32 instead.
*/

/*!
    \typedef Q_UINT32
    \relates <QtGlobal>
    \compat

    Use \l quint32 instead.
*/

/*!
    \typedef Q_INT64
    \relates <QtGlobal>
    \compat

    Use \l qint64 instead.
*/

/*!
    \typedef Q_UINT64
    \relates <QtGlobal>
    \compat

    Use \l quint64 instead.
*/

/*!
    \typedef Q_LLONG
    \relates <QtGlobal>
    \compat

    Use \l qint64 instead.
*/

/*!
    \typedef Q_ULLONG
    \relates <QtGlobal>
    \compat

    Use \l quint64 instead.
*/

/*!
    \typedef Q_LONG
    \relates <QtGlobal>
    \compat

    Use \c{void *} instead.
*/

/*!
    \typedef Q_ULONG
    \relates <QtGlobal>
    \compat

    Use \c{void *} instead.
*/

/*! \fn bool qSysInfo(int *wordSize, bool *bigEndian)
    \relates <QtGlobal>

    Use QSysInfo::WordSize and QSysInfo::ByteOrder instead.
*/

/*!
    \fn bool qt_winUnicode()
    \relates <QtGlobal>

    Use QSysInfo::WindowsVersion and QSysInfo::WV_DOS_based instead.

    \sa QSysInfo
*/

/*!
    \fn int qWinVersion()
    \relates <QtGlobal>

    Use QSysInfo::WindowsVersion instead.

    \sa QSysInfo
*/

/*!
    \fn int qMacVersion()
    \relates <QtGlobal>

    Use QSysInfo::MacintoshVersion instead.

    \sa QSysInfo
*/

/*!
    \macro QT_VERSION
    \relates <QtGlobal>

    This macro expands a numeric value of the form 0xMMNNPP (MM =
    major, NN = minor, PP = patch) that specifies Qt's version
    number. For example, if you compile your application against Qt
    4.1.2, the QT_VERSION macro will expand to 0x040102.

    You can use QT_VERSION to use the latest Qt features where
    available. For example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 16

    \sa QT_VERSION_STR, qVersion()
*/

/*!
    \macro QT_VERSION_STR
    \relates <QtGlobal>

    This macro expands to a string that specifies Qt's version number
    (for example, "4.1.2"). This is the version against which the
    application is compiled.

    \sa qVersion(), QT_VERSION
*/

/*!
    \relates <QtGlobal>

    Returns the version number of Qt at run-time as a string (for
    example, "4.1.2"). This may be a different version than the
    version the application was compiled against.

    \sa QT_VERSION_STR
*/

const char *qVersion()
{
    return QT_VERSION_STR;
}

bool qSharedBuild()
{
#ifdef QT_SHARED
    return true;
#else
    return false;
#endif
}

/*****************************************************************************
  System detection routines
 *****************************************************************************/

/*!
    \class QSysInfo
    \brief The QSysInfo class provides information about the system.

    \list
    \o \l WordSize specifies the size of a pointer for the platform
       on which the application is compiled.
    \o \l ByteOrder specifies whether the platform is big-endian or
       little-endian.
    \o \l WindowsVersion specifies the version of the Windows operating
       system on which the application is run (Windows only)
    \o \l MacintoshVersion specifies the version of the Macintosh
       operating system on which the application is run (Mac only).
    \endlist

    Some constants are defined only on certain platforms. You can use
    the preprocessor symbols Q_WS_WIN and Q_WS_MAC to test that
    the application is compiled under Windows or Mac.

    \sa QLibraryInfo
*/

/*!
    \enum QSysInfo::Sizes

    This enum provides platform-specific information about the sizes of data
    structures used by the underlying architecture.

    \value WordSize The size in bits of a pointer for the platform on which
           the application is compiled (32 or 64).
*/

/*!
    \variable QSysInfo::WindowsVersion
    \brief the version of the Windows operating system on which the
           application is run (Windows only)
*/

/*!
    \fn QSysInfo::WindowsVersion QSysInfo::windowsVersion()

    Returns the version of the Windows operating system on which the
    application is run (Windows only).
*/

/*!
    \variable QSysInfo::MacintoshVersion
    \brief the version of the Macintosh operating system on which
           the application is run (Mac only).
*/

/*!
    \enum QSysInfo::Endian

    \value BigEndian  Big-endian byte order (also called Network byte order)
    \value LittleEndian  Little-endian byte order
    \value ByteOrder  Equals BigEndian or LittleEndian, depending on
                      the platform's byte order.
*/

/*!
    \enum QSysInfo::WinVersion

    This enum provides symbolic names for the various versions of the
    Windows operating system. On Windows, the
    QSysInfo::WindowsVersion variable gives the version of the system
    on which the application is run.

    MS-DOS-based versions:

    \value WV_32s   Windows 3.1 with Win 32s
    \value WV_95    Windows 95
    \value WV_98    Windows 98
    \value WV_Me    Windows Me

    NT-based versions:

    \value WV_NT    Windows NT
    \value WV_2000  Windows 2000
    \value WV_XP    Windows XP
    \value WV_2003  Windows Server 2003
    \value WV_VISTA Windows Vista

    CE-based versions:

    \value WV_CE    Windows CE
    \value WV_CENET Windows CE .NET
    \value WV_CE_5  Windows CE 5.x
    \value WV_CE_6  Windows CE 6.x

    The following masks can be used for testing whether a Windows
    version is MS-DOS-based, NT-based, or CE-based:

    \value WV_DOS_based MS-DOS-based version of Windows
    \value WV_NT_based  NT-based version of Windows
    \value WV_CE_based  CE-based version of Windows

    \sa MacVersion
*/

/*!
    \enum QSysInfo::MacVersion

    This enum provides symbolic names for the various versions of the
    Macintosh operating system. On Mac, the
    QSysInfo::MacintoshVersion variable gives the version of the
    system on which the application is run.

    \value MV_9        Mac OS 9 (unsupported)
    \value MV_10_0     Mac OS X 10.0 (unsupported)
    \value MV_10_1     Mac OS X 10.1 (unsupported)
    \value MV_10_2     Mac OS X 10.2 (unsupported)
    \value MV_10_3     Mac OS X 10.3
    \value MV_10_4     Mac OS X 10.4
    \value MV_10_5     Mac OS X 10.5
    \value MV_Unknown  An unknown and currently unsupported platform

    \value MV_CHEETAH  Apple codename for MV_10_0
    \value MV_PUMA     Apple codename for MV_10_1
    \value MV_JAGUAR   Apple codename for MV_10_2
    \value MV_PANTHER  Apple codename for MV_10_3
    \value MV_TIGER    Apple codename for MV_10_4
    \value MV_LEOPARD  Apple codename for MV_10_5

    \sa WinVersion
*/

/*!
    \fn T qFromBigEndian(const uchar *src)
    \since 4.3
    \relates <QtGlobal>

    Reads a big-endian number from memory location \a src and returns the number in the
    host byte order representation.
    On CPU architectures where the host byte order is little-endian (such as x86) this
    will swap the byte order; otherwise it will just read from \a src.

    Note that template type \c{T} can only be an integer data type (signed or unsigned).

    There are no data alignment constraints for \a src.

    \sa qFromLittleEndian()
    \sa qToBigEndian()
    \sa qToLittleEndian()
*/
/*!
    \fn T qFromBigEndian(T src)
    \since 4.3
    \relates <QtGlobal>
    \overload

    Converts \a src from big-endian byte order and returns the number in host byte order
    representation of that number.
    On CPU architectures where the host byte order is little-endian (such as x86) this
    will return \a src with the byte order swapped; otherwise it will return \a src
    unmodified.
*/
/*!
    \fn T qFromLittleEndian(const uchar *src)
    \since 4.3
    \relates <QtGlobal>

    Reads a little-endian number from memory location \a src and returns the number in
    the host byte order representation.
    On CPU architectures where the host byte order is big-endian (such as PowerPC) this
    will swap the byte order; otherwise it will just read from \a src.

    Note that template type \c{T} can only be an integer data type (signed or unsigned).

    There are no data alignment constraints for \a src.

    \sa qFromBigEndian()
    \sa qToBigEndian()
    \sa qToLittleEndian()
*/
/*!
    \fn T qFromLittleEndian(T src)
    \since 4.3
    \relates <QtGlobal>
    \overload

    Converts \a src from little-endian byte order and returns the number in host byte
    order representation of that number.
    On CPU architectures where the host byte order is big-endian (such as PowerPC) this
    will return \a src with the byte order swapped; otherwise it will return \a src
    unmodified.
*/
/*!
    \fn void qToBigEndian(T src, uchar *dest)
    \since 4.3
    \relates <QtGlobal>

    Writes the number \a src with template type \c{T} to the memory location at \a dest
    in big-endian byte order.

    Note that template type \c{T} can only be an integer data type (signed or unsigned).

    There are no data alignment constraints for \a dest.

    \sa qFromBigEndian()
    \sa qFromLittleEndian()
    \sa qToLittleEndian()
*/
/*!
    \fn T qToBigEndian(T src)
    \since 4.3
    \relates <QtGlobal>
    \overload

    Converts \a src from host byte order and returns the number in big-endian byte order
    representation of that number.
    On CPU architectures where the host byte order is little-endian (such as x86) this
    will return \a src with the byte order swapped; otherwise it will return \a src
    unmodified.
*/
/*!
    \fn void qToLittleEndian(T src, uchar *dest)
    \since 4.3
    \relates <QtGlobal>

    Writes the number \a src with template type \c{T} to the memory location at \a dest
    in little-endian byte order.

    Note that template type \c{T} can only be an integer data type (signed or unsigned).

    There are no data alignment constraints for \a dest.

    \sa qFromBigEndian()
    \sa qFromLittleEndian()
    \sa qToBigEndian()
*/
/*!
    \fn T qToLittleEndian(T src)
    \since 4.3
    \relates <QtGlobal>
    \overload

    Converts \a src from host byte order and returns the number in little-endian byte
    order representation of that number.
    On CPU architectures where the host byte order is big-endian (such as PowerPC) this
    will return \a src with the byte order swapped; otherwise it will return \a src
    unmodified.
*/

/*!
    \macro Q_WS_MAC
    \relates <QtGlobal>

    Defined on Mac OS X.

    \sa Q_WS_WIN, Q_WS_X11, Q_WS_QWS
*/

/*!
    \macro Q_WS_WIN
    \relates <QtGlobal>

    Defined on Windows.

    \sa Q_WS_MAC, Q_WS_X11, Q_WS_QWS
*/

/*!
    \macro Q_WS_X11
    \relates <QtGlobal>

    Defined on X11.

    \sa Q_WS_MAC, Q_WS_WIN, Q_WS_QWS
*/

/*!
    \macro Q_WS_QWS
    \relates <QtGlobal>

    Defined on Qt for Embedded Linux.

    \sa Q_WS_MAC, Q_WS_WIN, Q_WS_X11
*/

/*!
    \macro Q_OS_DARWIN
    \relates <QtGlobal>

    Defined on Darwin OS (synonym for Q_OS_MAC).
*/

/*!
    \macro Q_OS_MSDOS
    \relates <QtGlobal>

    Defined on MS-DOS and Windows.
*/

/*!
    \macro Q_OS_OS2
    \relates <QtGlobal>

    Defined on OS/2.
*/

/*!
    \macro Q_OS_OS2EMX
    \relates <QtGlobal>

    Defined on XFree86 on OS/2 (not PM).
*/

/*!
    \macro Q_OS_WIN32
    \relates <QtGlobal>

    Defined on all supported versions of Windows.
*/

/*!
    \macro Q_OS_WINCE
    \relates <QtGlobal>

    Defined on Windows CE.
*/

/*!
    \macro Q_OS_CYGWIN
    \relates <QtGlobal>

    Defined on Cygwin.
*/

/*!
    \macro Q_OS_SOLARIS
    \relates <QtGlobal>

    Defined on Sun Solaris.
*/

/*!
    \macro Q_OS_HPUX
    \relates <QtGlobal>

    Defined on HP-UX.
*/

/*!
    \macro Q_OS_ULTRIX
    \relates <QtGlobal>

    Defined on DEC Ultrix.
*/

/*!
    \macro Q_OS_LINUX
    \relates <QtGlobal>

    Defined on Linux.
*/

/*!
    \macro Q_OS_FREEBSD
    \relates <QtGlobal>

    Defined on FreeBSD.
*/

/*!
    \macro Q_OS_NETBSD
    \relates <QtGlobal>

    Defined on NetBSD.
*/

/*!
    \macro Q_OS_OPENBSD
    \relates <QtGlobal>

    Defined on OpenBSD.
*/

/*!
    \macro Q_OS_BSDI
    \relates <QtGlobal>

    Defined on BSD/OS.
*/

/*!
    \macro Q_OS_IRIX
    \relates <QtGlobal>

    Defined on SGI Irix.
*/

/*!
    \macro Q_OS_OSF
    \relates <QtGlobal>

    Defined on HP Tru64 UNIX.
*/

/*!
    \macro Q_OS_SCO
    \relates <QtGlobal>

    Defined on SCO OpenServer 5.
*/

/*!
    \macro Q_OS_UNIXWARE
    \relates <QtGlobal>

    Defined on UnixWare 7, Open UNIX 8.
*/

/*!
    \macro Q_OS_AIX
    \relates <QtGlobal>

    Defined on AIX.
*/

/*!
    \macro Q_OS_HURD
    \relates <QtGlobal>

    Defined on GNU Hurd.
*/

/*!
    \macro Q_OS_DGUX
    \relates <QtGlobal>

    Defined on DG/UX.
*/

/*!
    \macro Q_OS_RELIANT
    \relates <QtGlobal>

    Defined on Reliant UNIX.
*/

/*!
    \macro Q_OS_DYNIX
    \relates <QtGlobal>

    Defined on DYNIX/ptx.
*/

/*!
    \macro Q_OS_QNX
    \relates <QtGlobal>

    Defined on QNX.
*/

/*!
    \macro Q_OS_QNX6
    \relates <QtGlobal>

    Defined on QNX RTP 6.1.
*/

/*!
    \macro Q_OS_LYNX
    \relates <QtGlobal>

    Defined on LynxOS.
*/

/*!
    \macro Q_OS_BSD4
    \relates <QtGlobal>

    Defined on Any BSD 4.4 system.
*/

/*!
    \macro Q_OS_UNIX
    \relates <QtGlobal>

    Defined on Any UNIX BSD/SYSV system.
*/

/*!
    \macro Q_CC_SYM
    \relates <QtGlobal>

    Defined if the application is compiled using Digital Mars C/C++
    (used to be Symantec C++).
*/

/*!
    \macro Q_CC_MWERKS
    \relates <QtGlobal>

    Defined if the application is compiled using Metrowerks
    CodeWarrior.
*/

/*!
    \macro Q_CC_MSVC
    \relates <QtGlobal>

    Defined if the application is compiled using Microsoft Visual
    C/C++, Intel C++ for Windows.
*/

/*!
    \macro Q_CC_BOR
    \relates <QtGlobal>

    Defined if the application is compiled using Borland/Turbo C++.
*/

/*!
    \macro Q_CC_WAT
    \relates <QtGlobal>

    Defined if the application is compiled using Watcom C++.
*/

/*!
    \macro Q_CC_GNU
    \relates <QtGlobal>

    Defined if the application is compiled using GNU C++.
*/

/*!
    \macro Q_CC_COMEAU
    \relates <QtGlobal>

    Defined if the application is compiled using Comeau C++.
*/

/*!
    \macro Q_CC_EDG
    \relates <QtGlobal>

    Defined if the application is compiled using Edison Design Group
    C++.
*/

/*!
    \macro Q_CC_OC
    \relates <QtGlobal>

    Defined if the application is compiled using CenterLine C++.
*/

/*!
    \macro Q_CC_SUN
    \relates <QtGlobal>

    Defined if the application is compiled using Forte Developer, or
    Sun Studio C++.
*/

/*!
    \macro Q_CC_MIPS
    \relates <QtGlobal>

    Defined if the application is compiled using MIPSpro C++.
*/

/*!
    \macro Q_CC_DEC
    \relates <QtGlobal>

    Defined if the application is compiled using DEC C++.
*/

/*!
    \macro Q_CC_HPACC
    \relates <QtGlobal>

    Defined if the application is compiled using HP aC++.
*/

/*!
    \macro Q_CC_USLC
    \relates <QtGlobal>

    Defined if the application is compiled using SCO OUDK and UDK.
*/

/*!
    \macro Q_CC_CDS
    \relates <QtGlobal>

    Defined if the application is compiled using Reliant C++.
*/

/*!
    \macro Q_CC_KAI
    \relates <QtGlobal>

    Defined if the application is compiled using KAI C++.
*/

/*!
    \macro Q_CC_INTEL
    \relates <QtGlobal>

    Defined if the application is compiled using Intel C++ for Linux,
    Intel C++ for Windows.
*/

/*!
    \macro Q_CC_HIGHC
    \relates <QtGlobal>

    Defined if the application is compiled using MetaWare High C/C++.
*/

/*!
    \macro Q_CC_PGI
    \relates <QtGlobal>

    Defined if the application is compiled using Portland Group C++.
*/

/*!
    \macro Q_CC_GHS
    \relates <QtGlobal>

    Defined if the application is compiled using Green Hills
    Optimizing C++ Compilers.
*/

#if defined(QT_BUILD_QMAKE)
// needed to bootstrap qmake
static const unsigned int qt_one = 1;
const int QSysInfo::ByteOrder = ((*((unsigned char *) &qt_one) == 0) ? BigEndian : LittleEndian);
#endif

#if !defined(QWS) && defined(Q_OS_MAC)

QT_BEGIN_INCLUDE_NAMESPACE
#include "private/qcore_mac_p.h"
#include "qnamespace.h"
QT_END_INCLUDE_NAMESPACE

Q_CORE_EXPORT OSErr qt_mac_create_fsref(const QString &file, FSRef *fsref)
{
    return FSPathMakeRef(reinterpret_cast<const UInt8 *>(file.toUtf8().constData()), fsref, 0);
}

// Don't use this function, it won't work in 10.5 (Leopard) and up
Q_CORE_EXPORT OSErr qt_mac_create_fsspec(const QString &file, FSSpec *spec)
{
    FSRef fsref;
    OSErr ret = qt_mac_create_fsref(file, &fsref);
    if (ret == noErr)
        ret = FSGetCatalogInfo(&fsref, kFSCatInfoNone, 0, 0, spec, 0);
    return ret;
}

Q_CORE_EXPORT void qt_mac_to_pascal_string(QString s, Str255 str, TextEncoding encoding=0, int len=-1)
{
    if(len == -1)
        len = s.length();
#if 0
    UnicodeMapping mapping;
    mapping.unicodeEncoding = CreateTextEncoding(kTextEncodingUnicodeDefault,
                                                 kTextEncodingDefaultVariant,
                                                 kUnicode16BitFormat);
    mapping.otherEncoding = (encoding ? encoding : );
    mapping.mappingVersion = kUnicodeUseLatestMapping;

    UnicodeToTextInfo info;
    OSStatus err = CreateUnicodeToTextInfo(&mapping, &info);
    if(err != noErr) {
        qDebug("Qt: internal: Unable to create pascal string '%s'::%d [%ld]",
               s.left(len).latin1(), (int)encoding, err);
        return;
    }
    const int unilen = len * 2;
    const UniChar *unibuf = (UniChar *)s.unicode();
    ConvertFromUnicodeToPString(info, unilen, unibuf, str);
    DisposeUnicodeToTextInfo(&info);
#else
    Q_UNUSED(encoding);
    CFStringGetPascalString(QCFString(s), str, 256, CFStringGetSystemEncoding());
#endif
}

Q_CORE_EXPORT QString qt_mac_from_pascal_string(const Str255 pstr) {
    return QCFString(CFStringCreateWithPascalString(0, pstr, CFStringGetSystemEncoding()));
}



static QSysInfo::MacVersion macVersion()
{
    SInt32 gestalt_version;
    if (Gestalt(gestaltSystemVersion, &gestalt_version) == noErr) {
        return QSysInfo::MacVersion(((gestalt_version & 0x00F0) >> 4) + 2);
    }
    return QSysInfo::MV_Unknown;
}
const QSysInfo::MacVersion QSysInfo::MacintoshVersion = macVersion();

#elif defined(Q_OS_WIN32) || defined(Q_OS_CYGWIN) || defined(Q_OS_WINCE)

QT_BEGIN_INCLUDE_NAMESPACE
#include "qt_windows.h"
QT_END_INCLUDE_NAMESPACE

QSysInfo::WinVersion QSysInfo::windowsVersion()
{
#ifndef VER_PLATFORM_WIN32s
#define VER_PLATFORM_WIN32s            0
#endif
#ifndef VER_PLATFORM_WIN32_WINDOWS
#define VER_PLATFORM_WIN32_WINDOWS  1
#endif
#ifndef VER_PLATFORM_WIN32_NT
#define VER_PLATFORM_WIN32_NT            2
#endif
#ifndef VER_PLATFORM_WIN32_CE
#define VER_PLATFORM_WIN32_CE            3
#endif

    static QSysInfo::WinVersion winver = QSysInfo::WV_NT;
#ifndef Q_OS_WINCE
    OSVERSIONINFOA osver;
    osver.dwOSVersionInfoSize = sizeof(osver);
    GetVersionExA(&osver);
#else
    DWORD qt_cever = 0;
    OSVERSIONINFOW osver;
    osver.dwOSVersionInfoSize = sizeof(osver);
    GetVersionEx(&osver);
    qt_cever = osver.dwMajorVersion * 100;
    qt_cever += osver.dwMinorVersion * 10;
#endif
    switch (osver.dwPlatformId) {
    case VER_PLATFORM_WIN32s:
        winver = QSysInfo::WV_32s;
        break;
    case VER_PLATFORM_WIN32_WINDOWS:
        // We treat Windows Me (minor 90) the same as Windows 98
        if (osver.dwMinorVersion == 90)
            winver = QSysInfo::WV_Me;
        else if (osver.dwMinorVersion == 10)
            winver = QSysInfo::WV_98;
        else
            winver = QSysInfo::WV_95;
        break;
#ifdef Q_OS_WINCE
    case VER_PLATFORM_WIN32_CE:
        if (qt_cever >= 600)
            winver = QSysInfo::WV_CE_6;
        if (qt_cever >= 500)
            winver = QSysInfo::WV_CE_5;
        else if (qt_cever >= 400)
            winver = QSysInfo::WV_CENET;
        else
            winver = QSysInfo::WV_CE;
        break;
#endif
    default: // VER_PLATFORM_WIN32_NT
        if (osver.dwMajorVersion < 5) {
            winver = QSysInfo::WV_NT;
        } else if (osver.dwMajorVersion == 6) {
            winver = QSysInfo::WV_VISTA;
        } else if (osver.dwMinorVersion == 0) {
            winver = QSysInfo::WV_2000;
        } else if (osver.dwMinorVersion == 1) {
            winver = QSysInfo::WV_XP;
        } else if (osver.dwMinorVersion == 2) {
            winver = QSysInfo::WV_2003;
        } else {
            qWarning("Qt: Untested Windows version detected!");
            winver = QSysInfo::WV_NT_based;
        }
    }

#ifdef QT_DEBUG
    {
        QByteArray override = qgetenv("QT_WINVER_OVERRIDE");
        if (override.isEmpty())
            return winver;

        if (override == "Me")
            winver = QSysInfo::WV_Me;
        if (override == "95")
            winver = QSysInfo::WV_95;
        else if (override == "98")
            winver = QSysInfo::WV_98;
        else if (override == "NT")
            winver = QSysInfo::WV_NT;
        else if (override == "2000")
            winver = QSysInfo::WV_2000;
        else if (override == "2003")
            winver = QSysInfo::WV_2003;
        else if (override == "XP")
            winver = QSysInfo::WV_XP;
        else if (override == "VISTA")
            winver = QSysInfo::WV_VISTA;
    }
#endif

    return winver;
}

const QSysInfo::WinVersion QSysInfo::WindowsVersion = QSysInfo::windowsVersion();

#endif

/*!
    \macro void Q_ASSERT(bool test)
    \relates <QtGlobal>

    Prints a warning message containing the source code file name and
    line number if \a test is false.

    Q_ASSERT() is useful for testing pre- and post-conditions
    during development. It does nothing if \c QT_NO_DEBUG was defined
    during compilation.

    Example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 17

    If \c b is zero, the Q_ASSERT statement will output the following
    message using the qFatal() function:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 18

    \sa Q_ASSERT_X(), qFatal(), {Debugging Techniques}
*/

/*!
    \macro void Q_ASSERT_X(bool test, const char *where, const char *what)
    \relates <QtGlobal>

    Prints the message \a what together with the location \a where,
    the source file name and line number if \a test is false.

    Q_ASSERT_X is useful for testing pre- and post-conditions during
    development. It does nothing if \c QT_NO_DEBUG was defined during
    compilation.

    Example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 19

    If \c b is zero, the Q_ASSERT_X statement will output the following
    message using the qFatal() function:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 20

    \sa Q_ASSERT(), qFatal(), {Debugging Techniques}
*/

/*!
    \macro void Q_CHECK_PTR(void *pointer)
    \relates <QtGlobal>

    If \a pointer is 0, prints a warning message containing the source
    code's file name and line number, saying that the program ran out
    of memory.

    Q_CHECK_PTR does nothing if \c QT_NO_DEBUG was defined during
    compilation.

    Example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 21

    \sa qWarning(), {Debugging Techniques}
*/

/*!
    \macro const char* Q_FUNC_INFO()
    \relates <QtGlobal>

    Expands to a string that describe the function the macro resides in. How this string looks
    more specifically is compiler dependent. With GNU GCC it is typically the function signature,
    while with other compilers it might be the line and column number.

    Q_FUNC_INFO can be conveniently used with qDebug(). For example, this function:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 22

    when instantiated with the integer type, will with the GCC compiler produce:

    \tt{const TInputType& myMin(const TInputType&, const TInputType&) [with TInputType = int] was called with value1: 3 value2: 4}

    If this macro is used outside a function, the behavior is undefined.
 */

/*
  The Q_CHECK_PTR macro calls this function if an allocation check
  fails.
*/
void qt_check_pointer(const char *n, int l)
{
    qWarning("In file %s, line %d: Out of memory", n, l);
}

/*
  The Q_ASSERT macro calls this this function when the test fails.
*/
void qt_assert(const char *assertion, const char *file, int line)
{
    qFatal("ASSERT: \"%s\" in file %s, line %d", assertion, file, line);
}

/*
  The Q_ASSERT_X macro calls this this function when the test fails.
*/
void qt_assert_x(const char *where, const char *what, const char *file, int line)
{
    qFatal("ASSERT failure in %s: \"%s\", file %s, line %d", where, what, file, line);
}


/*
    Dijkstra's bisection algorithm to find the square root of an integer.
    Deliberately not exported as part of the Qt API, but used in both
    qsimplerichtext.cpp and qgfxraster_qws.cpp
*/
Q_CORE_EXPORT unsigned int qt_int_sqrt(unsigned int n)
{
    // n must be in the range 0...UINT_MAX/2-1
    if (n >= (UINT_MAX>>2)) {
        unsigned int r = 2 * qt_int_sqrt(n / 4);
        unsigned int r2 = r + 1;
        return (n >= r2 * r2) ? r2 : r;
    }
    uint h, p= 0, q= 1, r= n;
    while (q <= n)
        q <<= 2;
    while (q != 1) {
        q >>= 2;
        h= p + q;
        p >>= 1;
        if (r >= h) {
            p += q;
            r -= h;
        }
    }
    return p;
}

#if defined(qMemCopy)
#  undef qMemCopy
#endif
#if defined(qMemSet)
#  undef qMemSet
#endif

void *qMemCopy(void *dest, const void *src, size_t n) { return memcpy(dest, src, n); }
void *qMemSet(void *dest, int c, size_t n) { return memset(dest, c, n); }

static QtMsgHandler handler = 0;                // pointer to debug handler
static const int QT_BUFFER_LENGTH = 8192;       // internal buffer length

#ifdef Q_CC_MWERKS
extern bool qt_is_gui_used;
static void mac_default_handler(const char *msg)
{
    if (qt_is_gui_used) {
        Str255 pmsg;
        qt_mac_to_pascal_string(msg, pmsg);
        DebugStr(pmsg);
    } else {
        fprintf(stderr, msg);
    }
}
#endif // Q_CC_MWERKS



QString qt_error_string(int errorCode)
{
    const char *s = 0;
    QString ret;
    if (errorCode == -1) {
#if defined(Q_OS_WIN32) || defined(Q_OS_WINCE)
        errorCode = GetLastError();
#else
        errorCode = errno;
#endif
    }
    switch (errorCode) {
    case 0:
        break;
    case EACCES:
        s = QT_TRANSLATE_NOOP("QIODevice", "Permission denied");
        break;
    case EMFILE:
        s = QT_TRANSLATE_NOOP("QIODevice", "Too many open files");
        break;
    case ENOENT:
        s = QT_TRANSLATE_NOOP("QIODevice", "No such file or directory");
        break;
    case ENOSPC:
        s = QT_TRANSLATE_NOOP("QIODevice", "No space left on device");
        break;
    default: {
#ifdef Q_OS_WIN
        QT_WA({
            unsigned short *string = 0;
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
                          NULL,
                          errorCode,
                          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                          (LPTSTR)&string,
                          0,
                          NULL);
            ret = QString::fromUtf16(string);
            LocalFree((HLOCAL)string);
        }, {
            char *string = 0;
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
                           NULL,
                           errorCode,
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           (LPSTR)&string,
                           0,
                           NULL);
            ret = QString::fromLocal8Bit(string);
            LocalFree((HLOCAL)string);
        });

        if (ret.isEmpty() && errorCode == ERROR_MOD_NOT_FOUND)
            ret = QString::fromLatin1("The specified module could not be found.");

#elif !defined(QT_NO_THREAD) && defined(_POSIX_THREAD_SAFE_FUNCTIONS) && _POSIX_VERSION >= 200112L && !defined(Q_OS_INTEGRITY)

        QByteArray buf(1024, '\0');
        strerror_r(errorCode, buf.data(), buf.size());
        ret = QString::fromLocal8Bit(buf.constData());
#else
        ret = QString::fromLocal8Bit(strerror(errorCode));
#endif
    break; }
    }
    if (s)
        // ######## this breaks moc build currently
//         ret = QCoreApplication::translate("QIODevice", s);
        ret = QString::fromLatin1(s);
    return ret.trimmed();
}


/*!
    \fn QtMsgHandler qInstallMsgHandler(QtMsgHandler handler)
    \relates <QtGlobal>

    Installs a Qt message \a handler which has been defined
    previously. Returns a pointer to the previous message handler
    (which may be 0).

    The message handler is a function that prints out debug messages,
    warnings, critical and fatal error messages. The Qt library (debug
    version) contains hundreds of warning messages that are printed
    when internal errors (usually invalid function arguments)
    occur. If you implement your own message handler, you get total
    control of these messages.

    The default message handler prints the message to the standard
    output under X11 or to the debugger under Windows. If it is a
    fatal message, the application aborts immediately.

    Only one message handler can be defined, since this is usually
    done on an application-wide basis to control debug output.

    To restore the message handler, call \c qInstallMsgHandler(0).

    Example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 23

    \sa qDebug(), qWarning(), qCritical(), qFatal(), QtMsgType,
    {Debugging Techniques}
*/
#if defined(Q_OS_WIN) && defined(QT_BUILD_CORE_LIB)
extern bool usingWinMain;
extern Q_CORE_EXPORT void qWinMsgHandler(QtMsgType t, const char* str);
#endif

QtMsgHandler qInstallMsgHandler(QtMsgHandler h)
{
    QtMsgHandler old = handler;
    handler = h;
#if defined(Q_OS_WIN) && defined(QT_BUILD_CORE_LIB)
    if (!handler && usingWinMain)
        handler = qWinMsgHandler;
#endif
    return old;
}

/*!
    \internal
*/
void qt_message_output(QtMsgType msgType, const char *buf)
{
    if (handler) {
        (*handler)(msgType, buf);
    } else {
#if defined(Q_CC_MWERKS)
        mac_default_handler(buf);
#elif defined(Q_OS_WINCE)
        QString fstr = QString::fromLatin1(buf);
        fstr += QLatin1String("\n");
        OutputDebugString(reinterpret_cast<const wchar_t *> (fstr.utf16()));
#else
        fprintf(stderr, "%s\n", buf);
        fflush(stderr);
#endif
    }

    if (msgType == QtFatalMsg
        || (msgType == QtWarningMsg
            && (!qgetenv("QT_FATAL_WARNINGS").isNull())) ) {

#if defined(Q_CC_MSVC) && defined(QT_DEBUG) && defined(_DEBUG) && defined(_CRT_ERROR)
        // get the current report mode
        int reportMode = _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
        _CrtSetReportMode(_CRT_ERROR, reportMode);
#if !defined(Q_OS_WINCE)
        int ret = _CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, QT_VERSION_STR, buf);
#else
        int ret = _CrtDbgReportW(_CRT_ERROR, _CRT_WIDE(__FILE__),
            __LINE__, _CRT_WIDE(QT_VERSION_STR), reinterpret_cast<const wchar_t *> (QString::fromLatin1(buf).utf16()));
#endif
        if (ret == 0  && reportMode & _CRTDBG_MODE_WNDW)
            return; // ignore
        else if (ret == 1)
            _CrtDbgBreak();
#endif

#if (defined(Q_OS_UNIX) || defined(Q_CC_MINGW)) && defined(QT_DEBUG)
        abort(); // trap; generates core dump
#else
        exit(1); // goodbye cruel world
#endif
    }
}

#undef qDebug
/*!
    \relates <QtGlobal>

    Calls the message handler with the debug message \a msg. If no
    message handler has been installed, the message is printed to
    stderr. Under Windows, the message is sent to the console, if it is a
    console application; otherwise, it is sent to the debugger. This
    function does nothing if \c QT_NO_DEBUG_OUTPUT was defined
    during compilation.

    If you pass the function a format string and a list of arguments,
    it works in similar way to the C printf() function.

    Example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 24

    If you include \c <QtDebug>, a more convenient syntax is also
    available:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 25

    This syntax automatically puts a single space between each item,
    and outputs a newline at the end. It supports many C++ and Qt
    types.

    \warning The internal buffer is limited to 8192 bytes, including
    the '\0'-terminator.

    \sa qWarning(), qCritical(), qFatal(), qInstallMsgHandler(),
        {Debugging Techniques}
*/
void qDebug(const char *msg, ...)
{
    char buf[QT_BUFFER_LENGTH];
    buf[QT_BUFFER_LENGTH - 1] = '\0';
    va_list ap;
    va_start(ap, msg);                        // use variable arg list
    if (msg)
        qvsnprintf(buf, QT_BUFFER_LENGTH - 1, msg, ap);
    va_end(ap);

    qt_message_output(QtDebugMsg, buf);
}

#undef qWarning
/*!
    \relates <QtGlobal>

    Calls the message handler with the warning message \a msg. If no
    message handler has been installed, the message is printed to
    stderr. Under Windows, the message is sent to the debugger. This
    function does nothing if \c QT_NO_WARNING_OUTPUT was defined
    during compilation; it exits if the environment variable \c
    QT_FATAL_WARNINGS is defined.

    This function takes a format string and a list of arguments,
    similar to the C printf() function.

    Example:
    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 26

    If you include <QtDebug>, a more convenient syntax is
    also available:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 27

    This syntax inserts a space between each item, and
    appends a newline at the end.

    \warning The internal buffer is limited to 8192 bytes, including
    the '\0'-terminator.

    \sa qDebug(), qCritical(), qFatal(), qInstallMsgHandler(),
        {Debugging Techniques}
*/
void qWarning(const char *msg, ...)
{
    char buf[QT_BUFFER_LENGTH];
    buf[QT_BUFFER_LENGTH - 1] = '\0';
    va_list ap;
    va_start(ap, msg); // use variable arg list
    if (msg)
        qvsnprintf(buf, QT_BUFFER_LENGTH - 1, msg, ap);
    va_end(ap);

    qt_message_output(QtWarningMsg, buf);
}

/*!
    \relates <QtGlobal>

    Calls the message handler with the critical message \a msg. If no
    message handler has been installed, the message is printed to
    stderr. Under Windows, the message is sent to the debugger.

    This function takes a format string and a list of arguments, similar
    to the C printf() function.

    Example:
    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 28

    If you include <QtDebug>, a more convenient syntax is
    also available:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 29

    A space is inserted between the items, and a newline is
    appended at the end.

    \warning The internal buffer is limited to 8192 bytes, including
    the '\0'-terminator.

    \sa qDebug(), qWarning(), qFatal(), qInstallMsgHandler(),
        {Debugging Techniques}
*/
void qCritical(const char *msg, ...)
{
    char buf[QT_BUFFER_LENGTH];
    buf[QT_BUFFER_LENGTH - 1] = '\0';
    va_list ap;
    va_start(ap, msg); // use variable arg list
    if (msg)
        qvsnprintf(buf, QT_BUFFER_LENGTH - 1, msg, ap);
    va_end(ap);

    qt_message_output(QtCriticalMsg, buf);
}
#ifdef QT3_SUPPORT
void qSystemWarning(const char *msg, int code)
   { qCritical("%s (%s)", msg, qt_error_string(code).toLocal8Bit().constData()); }
#endif // QT3_SUPPORT

void qErrnoWarning(const char *msg, ...)
{
    char buf[QT_BUFFER_LENGTH];
    buf[QT_BUFFER_LENGTH - 1] = '\0';
    va_list ap;
    va_start(ap, msg);
    if (msg)
        qvsnprintf(buf, QT_BUFFER_LENGTH - 1, msg, ap);
    va_end(ap);

    qCritical("%s (%s)", buf, qt_error_string(-1).toLocal8Bit().constData());
}

void qErrnoWarning(int code, const char *msg, ...)
{
    char buf[QT_BUFFER_LENGTH];
    buf[QT_BUFFER_LENGTH - 1] = '\0';
    va_list ap;
    va_start(ap, msg);
    if (msg)
        qvsnprintf(buf, QT_BUFFER_LENGTH - 1, msg, ap);
    va_end(ap);

    qCritical("%s (%s)", buf, qt_error_string(code).toLocal8Bit().constData());
}

/*!
    \relates <QtGlobal>

    Calls the message handler with the fatal message \a msg. If no
    message handler has been installed, the message is printed to
    stderr. Under Windows, the message is sent to the debugger.

    For a release library this function will exit the application
    with return value 1. For the debug version this function will
    abort on Unix systems to create a core dump, and report a
    _CRT_ERROR on Windows allowing to connect a debugger to the
    application.

    This function takes a format string and a list of arguments,
    similar to the C printf() function.

    Example:
    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 30

    \warning The internal buffer is limited to 8192 bytes, including
    the '\0'-terminator.

    \sa qDebug(), qCritical(), qWarning(), qInstallMsgHandler(),
        {Debugging Techniques}
*/
void qFatal(const char *msg, ...)
{
    char buf[QT_BUFFER_LENGTH];
    buf[QT_BUFFER_LENGTH - 1] = '\0';
    va_list ap;
    va_start(ap, msg); // use variable arg list
    if (msg)
        qvsnprintf(buf, QT_BUFFER_LENGTH - 1, msg, ap);
    va_end(ap);

    qt_message_output(QtFatalMsg, buf);
}

// getenv is declared as deprecated in VS2005. This function
// makes use of the new secure getenv function.
QByteArray qgetenv(const char *varName)
{
#if defined(_MSC_VER) && _MSC_VER >= 1400
    size_t requiredSize = 0;
    QByteArray buffer;
    getenv_s(&requiredSize, 0, 0, varName);
    if (requiredSize == 0)
        return buffer;
    buffer.resize(int(requiredSize));
    getenv_s(&requiredSize, buffer.data(), requiredSize, varName);
    // requiredSize includes the terminating null, which we don't want.
    Q_ASSERT(buffer.endsWith('\0'));
    buffer.chop(1);
    return buffer;
#else
    return QByteArray(::getenv(varName));
#endif
}

bool qputenv(const char *varName, const QByteArray& value)
{
#if defined(_MSC_VER) && _MSC_VER >= 1400
    return _putenv_s(varName, value.constData()) == 0;
#else
    QByteArray buffer(varName);
    buffer += "=";
    buffer += value;
    return putenv(qstrdup(buffer.constData())) == 0;
#endif
}

#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD)

#  if defined(Q_OS_INTEGRITY)
typedef long SeedStorageType;
#  else
typedef uint SeedStorageType;
#  endif

typedef QThreadStorage<SeedStorageType *> SeedStorage;
Q_GLOBAL_STATIC(SeedStorage, randTLS)  // Thread Local Storage for seed value

#endif

/*!
    \relates <QtGlobal>
    \since 4.2

    Thread-safe version of the standard C++ \c srand() function.

    Sets the argument \a seed to be used to generate a new random number sequence of
    pseudo random integers to be returned by qrand().

    If no seed value is provided, qrand() is automatically seeded with a value of 1.

    The sequence of random numbers generated is deterministic per thread. For example,
    if two threads call qsrand(1) and subsequently calls qrand(), the threads will get
    the same random number sequence.

    \sa qrand()
*/
void qsrand(uint seed)
{
#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD)
    SeedStorageType *pseed = randTLS()->localData();
    if (!pseed)
        randTLS()->setLocalData(pseed = new SeedStorageType);
    *pseed = seed;
#else
    // On Windows srand() and rand() already use Thread-Local-Storage
    // to store the seed between calls
    srand(seed);
#endif
}

/*!
    \relates <QtGlobal>
    \since 4.2

    Thread-safe version of the standard C++ \c rand() function.

    Returns a value between 0 and \c RAND_MAX (defined in \c <cstdlib> and
    \c <stdlib.h>), the next number in the current sequence of pseudo-random
    integers.

    Use \c qsrand() to initialize the pseudo-random number generator with
    a seed value.

    \sa qsrand()
*/
int qrand()
{
#if defined(Q_OS_UNIX) && !defined(QT_NO_THREAD)
    SeedStorageType *pseed = randTLS()->localData();
    if (!pseed) {
        randTLS()->setLocalData(pseed = new SeedStorageType);
        *pseed = 1;
    }
    return rand_r(pseed);
#else
    // On Windows srand() and rand() already use Thread-Local-Storage
    // to store the seed between calls
    return rand();
#endif
}

/*!
    \macro forever
    \relates <QtGlobal>

    This macro is provided for convenience for writing infinite
    loops.

    Example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 31

    It is equivalent to \c{for (;;)}.

    If you're worried about namespace pollution, you can disable this
    macro by adding the following line to your \c .pro file:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 32

    \sa Q_FOREVER
*/

/*!
    \macro Q_FOREVER
    \relates <QtGlobal>

    Same as \l{forever}.

    This macro is available even when \c no_keywords is specified
    using the \c .pro file's \c CONFIG variable.

    \sa foreach()
*/

/*!
    \macro foreach(variable, container)
    \relates <QtGlobal>

    This macro is used to implement Qt's \c foreach loop. The \a
    variable parameter is a variable name or variable definition; the
    \a container parameter is a Qt container whose value type
    corresponds to the type of the variable. See \l{The foreach
    Keyword} for details.

    If you're worried about namespace pollution, you can disable this
    macro by adding the following line to your \c .pro file:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 33

    \sa Q_FOREACH()
*/

/*!
    \macro Q_FOREACH(variable, container)
    \relates <QtGlobal>

    Same as foreach(\a variable, \a container).

    This macro is available even when \c no_keywords is specified
    using the \c .pro file's \c CONFIG variable.

    \sa foreach()
*/

/*!
    \macro QT_TR_NOOP(sourceText)
    \relates <QtGlobal>

    Marks the string literal \a sourceText for dynamic translation in
    the current context (class), i.e the stored \a sourceText will not
    be altered.

    The macro expands to \a sourceText.

    Example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 34

    \sa QT_TRANSLATE_NOOP(), {Internationalization with Qt}
*/

/*!
    \macro QT_TRANSLATE_NOOP(context, sourceText)
    \relates <QtGlobal>

    Marks the string literal \a sourceText for dynamic translation in
    the given \a context, i.e the stored \a sourceText will not be
    altered. The \a context is typically a class and also needs to
    be specified as string literal.

    The macro expands to \a sourceText.

    Example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 35

    \sa QT_TR_NOOP(), QT_TRANSLATE_NOOP3(), {Internationalization with Qt}
*/

/*!
    \macro QT_TRANSLATE_NOOP3(context, sourceText, comment)
    \relates <QtGlobal>

    Marks the string literal \a sourceText for dynamic translation in the
    given \a context and with \a comment, i.e the stored \a sourceText will
    not be altered. The \a context is typically a class and also needs to
    be specified as string literal. The string literal \a comment
    will be available for translators using e.g. Qt Linguist.

    The macro expands to anonymous struct of the two string
    literals passed as \a sourceText and \a comment.

    Example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 36

    \sa QT_TR_NOOP(), QT_TRANSLATE_NOOP(), {Internationalization with Qt}
*/

/*!
    \macro QT_POINTER_SIZE
    \relates <QtGlobal>

    Expands to the size of a pointer in bytes (4 or 8). This is
    equivalent to \c sizeof(void *) but can be used in a preprocessor
    directive.
*/

/*!
    \macro TRUE
    \relates <QtGlobal>
    \obsolete

    Synonym for \c true.

    \sa FALSE
*/

/*!
    \macro FALSE
    \relates <QtGlobal>
    \obsolete

    Synonym for \c false.

    \sa TRUE
*/

/*!
    \macro QABS(n)
    \relates <QtGlobal>
    \obsolete

    Use qAbs(\a n) instead.

    \sa QMIN(), QMAX()
*/

/*!
    \macro QMIN(x, y)
    \relates <QtGlobal>
    \obsolete

    Use qMin(\a x, \a y) instead.

    \sa QMAX(), QABS()
*/

/*!
    \macro QMAX(x, y)
    \relates <QtGlobal>
    \obsolete

    Use qMax(\a x, \a y) instead.

    \sa QMIN(), QABS()
*/

/*!
    \macro const char *qPrintable(const QString &str)
    \relates <QtGlobal>

    Returns \a str as a \c{const char *}. This is equivalent to
    \a{str}.toLocal8Bit().constData().

    Example:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 37

    \sa qDebug(), qWarning(), qCritical(), qFatal()
*/

/*!
    \macro Q_DECLARE_TYPEINFO(Type, Flags)
    \relates <QtGlobal>

    You can use this macro to specify information about a custom type
    \a Type. With accurate type information, Qt's \l{generic
    containers} can choose appropriate storage methods and algorithms.

    \a Flags can be one of the following:

    \list
    \o \c Q_PRIMITIVE_TYPE specifies that \a Type is a POD (plain old
       data) type with no constructor or destructor.
    \o \c Q_MOVABLE_TYPE specifies that \a Type has a constructor
       and/or a destructor but can be moved in memory using \c
       memcpy().
    \o \c Q_COMPLEX_TYPE (the default) specifies that \a Type has
       constructors and/or a destructor and that it may not be moved
       in memory.
    \endlist

    Example of a "primitive" type:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 38

    Example of a movable type:

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 39
*/

/*!
    \macro Q_UNUSED(name)
    \relates <QtGlobal>

    Indicates to the compiler that the parameter with the specified
    \a name is not used in the body of a function. This can be used to
    suppress compiler warnings while allowing functions to be defined
    with meaningful parameter names in their signatures.
*/

#if defined(QT3_SUPPORT) && !defined(QT_NO_SETTINGS)
QT_BEGIN_INCLUDE_NAMESPACE
#include <qlibraryinfo.h>
QT_END_INCLUDE_NAMESPACE

static const char *qInstallLocation(QLibraryInfo::LibraryLocation loc)
{
    static QByteArray ret;
    ret = QLibraryInfo::location(loc).toLatin1();
    return ret.constData();
}
const char *qInstallPath()
{
    return qInstallLocation(QLibraryInfo::PrefixPath);
}
const char *qInstallPathDocs()
{
    return qInstallLocation(QLibraryInfo::DocumentationPath);
}
const char *qInstallPathHeaders()
{
    return qInstallLocation(QLibraryInfo::HeadersPath);
}
const char *qInstallPathLibs()
{
    return qInstallLocation(QLibraryInfo::LibrariesPath);
}
const char *qInstallPathBins()
{
    return qInstallLocation(QLibraryInfo::BinariesPath);
}
const char *qInstallPathPlugins()
{
    return qInstallLocation(QLibraryInfo::PluginsPath);
}
const char *qInstallPathData()
{
    return qInstallLocation(QLibraryInfo::DataPath);
}
const char *qInstallPathTranslations()
{
    return qInstallLocation(QLibraryInfo::TranslationsPath);
}
const char *qInstallPathSysconf()
{
    return qInstallLocation(QLibraryInfo::SettingsPath);
}
#endif

struct QInternal_CallBackTable {
    QVector<QList<qInternalCallback> > callbacks;
};

Q_GLOBAL_STATIC(QInternal_CallBackTable, global_callback_table)

bool QInternal::registerCallback(Callback cb, qInternalCallback callback)
{
    if (cb >= 0 && cb < QInternal::LastCallback) {
        QInternal_CallBackTable *cbt = global_callback_table();
        cbt->callbacks.resize(cb + 1);
        cbt->callbacks[cb].append(callback);
        return true;
    }
    return false;
}

bool QInternal::unregisterCallback(Callback cb, qInternalCallback callback)
{
    if (cb >= 0 && cb < QInternal::LastCallback) {
        QInternal_CallBackTable *cbt = global_callback_table();
        return (bool) cbt->callbacks[cb].removeAll(callback);
    }
    return false;
}

bool QInternal::activateCallbacks(Callback cb, void **parameters)
{
    Q_ASSERT_X(cb >= 0, "QInternal::activateCallback()", "Callback id must be a valid id");

    QInternal_CallBackTable *cbt = global_callback_table();
    if (cbt && cb < cbt->callbacks.size()) {
        QList<qInternalCallback> callbacks = cbt->callbacks[cb];
        bool ret = false;
        for (int i=0; i<callbacks.size(); ++i)
            ret |= (callbacks.at(i))(parameters);
        return ret;
    }
    return false;
}

void qt_set_current_thread_to_main_thread();

bool QInternal::callFunction(InternalFunction func, void **args)
{
    Q_ASSERT_X(func >= 0,
               "QInternal::callFunction()", "Callback id must be a valid id");
#ifndef QT_NO_QOBJECT
    switch (func) {
#ifndef QT_NO_THREAD
    case QInternal::CreateThreadForAdoption:
        *args = QAdoptedThread::createThreadForAdoption();
        return true;
#endif
    case QInternal::RefAdoptedThread:
        QThreadData::get2((QThread *) *args)->ref();
        return true;
    case QInternal::DerefAdoptedThread:
        QThreadData::get2((QThread *) *args)->deref();
        return true;
    case QInternal::SetCurrentThreadToMainThread:
        qt_set_current_thread_to_main_thread();
        return true;
    case QInternal::SetQObjectSender: {
        QObject *receiver = (QObject *) args[0];
        QObjectPrivate::Sender *sender = new QObjectPrivate::Sender;
        sender->sender = (QObject *) args[1];
        sender->signal = *(int *) args[2];

        // Store the old sender as "return value"
        args[3] = QObjectPrivate::setCurrentSender(receiver, sender);
        args[4] = sender;
        return true;
    }
    case QInternal::GetQObjectSender: {
        QObject *receiver = (QObject *) args[0];
        QObjectPrivate *d = QObjectPrivate::get(receiver);
        args[1] = d->currentSender ? d->currentSender->sender : 0;
        return true;
    }
    case QInternal::ResetQObjectSender: {
        QObject *receiver = (QObject *) args[0];
        QObjectPrivate::Sender *oldSender = (QObjectPrivate::Sender *) args[1];
        QObjectPrivate::Sender *sender = (QObjectPrivate::Sender *) args[2];
        QObjectPrivate::resetCurrentSender(receiver, sender, oldSender);
        delete sender;
        return true;
    }

    default:
        break;
    }
#else
    Q_UNUSED(args);
    Q_UNUSED(func);
#endif

    return false;
}

/*!
    \macro Q_BYTE_ORDER
    \relates <QtGlobal>

    This macro can be used to determine the byte order your system
    uses for storing data in memory. i.e., whether your system is
    little-endian or big-endian. It is set by Qt to one of the macros
    Q_LITTLE_ENDIAN or Q_BIG_ENDIAN. You normally won't need to worry
    about endian-ness, but you might, for example if you need to know
    which byte of an integer or UTF-16 character is stored in the
    lowest address. Endian-ness is important in networking, where
    computers with different values for Q_BYTE_ORDER must pass data
    back and forth.

    Use this macro as in the following examples.

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 40

    \sa Q_BIG_ENDIAN, Q_LITTLE_ENDIAN
*/

/*!
    \macro Q_LITTLE_ENDIAN
    \relates <QtGlobal>

    This macro represents a value you can compare to the macro
    Q_BYTE_ORDER to determine the endian-ness of your system.  In a
    little-endian system, the least significant byte is stored at the
    lowest address. The other bytes follow in increasing order of
    significance.

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 41

    \sa Q_BYTE_ORDER, Q_BIG_ENDIAN
*/

/*!
    \macro Q_BIG_ENDIAN
    \relates <QtGlobal>

    This macro represents a value you can compare to the macro
    Q_BYTE_ORDER to determine the endian-ness of your system.  In a
    big-endian system, the most significant byte is stored at the
    lowest address. The other bytes follow in decreasing order of
    significance.

    \snippet doc/src/snippets/code/src.corelib.global.qglobal.cpp 42

    \sa Q_BYTE_ORDER, Q_LITTLE_ENDIAN
*/

/*!
    \macro Q_GLOBAL_STATIC(type, name)
    \internal

    Declares a global static variable with the given \a type and \a name.

    Use this macro to instantiate an object in a thread-safe way, creating
    a global pointer that can be used to refer to it.

    \warning This macro is subject to a race condition that can cause the object
    to be constructed twice. However, if this occurs, the second instance will
    be immediately deleted.

    See also
    \l{http://www.aristeia.com/publications.html}{"C++ and the perils of Double-Checked Locking"}
    by Scott Meyers and Andrei Alexandrescu.
*/

/*!
    \macro Q_GLOBAL_STATIC_WITH_ARGS(type, name, arguments)
    \internal

    Declares a global static variable with the specified \a type and \a name.

    Use this macro to instantiate an object using the \a arguments specified
    in a thread-safe way, creating a global pointer that can be used to refer
    to it.

    \warning This macro is subject to a race condition that can cause the object
    to be constructed twice. However, if this occurs, the second instance will
    be immediately deleted.

    See also
    \l{http://www.aristeia.com/publications.html}{"C++ and the perils of Double-Checked Locking"}
    by Scott Meyers and Andrei Alexandrescu.
*/

/*!
    \macro QT_NAMESPACE
    \internal

    If this macro is defined to \c ns all Qt classes are put in a namespace
    called \c ns. Also, moc will output code putting metaobjects etc.
    into namespace \c ns.

    \sa QT_BEGIN_NAMESPACE, QT_END_NAMESPACE,
    QT_PREPEND_NAMESPACE, QT_USE_NAMESPACE,
    QT_BEGIN_INCLUDE_NAMESPACE, QT_END_INCLUDE_NAMESPACE,
    QT_BEGIN_MOC_NAMESPACE, QT_END_MOC_NAMESPACE,
*/

/*!
    \macro QT_PREPEND_NAMESPACE(identifier)
    \internal

    This macro qualifies \a identifier with the full namespace.
    It expands to \c{::QT_NAMESPACE::identifier} if \c QT_NAMESPACE is defined
    and only \a identifier otherwise.

    \sa QT_NAMESPACE
*/

/*!
    \macro QT_USE_NAMESPACE
    \internal

    This macro expands to using QT_NAMESPACE if QT_NAMESPACE is defined
    and nothing otherwise.

    \sa QT_NAMESPACE
*/

/*!
    \macro QT_BEGIN_NAMESPACE
    \internal

    This macro expands to

    \snippet snippets/code/src.corelib.global.qglobal.cpp begin namespace macro

    if \c QT_NAMESPACE is defined and nothing otherwise. If should always
    appear in the file-level scope and be followed by \c QT_END_NAMESPACE
    at the same logical level with respect to preprocessor conditionals
    in the same file.

    As a rule of thumb, \c QT_BEGIN_NAMESPACE should appear in all Qt header
    and Qt source files after the last \c{#include} line and before the first
    declaration. In Qt headers using \c QT_BEGIN_HEADER, \c QT_BEGIN_NAMESPACE
    follows \c QT_BEGIN_HEADER immediately.

    If that rule can't be followed because, e.g., \c{#include} lines and
    declarations are wildly mixed, place \c QT_BEGIN_NAMESPACE before
    the first declaration and wrap the \c{#include} lines in
    \c QT_BEGIN_INCLUDE_NAMESPACE and \c QT_END_INCLUDE_NAMESPACE.

    When using the \c QT_NAMESPACE feature in user code
    (e.g., when building plugins statically linked to Qt) where
    the user code is not intended to go into the \c QT_NAMESPACE
    namespace, all forward declarations of Qt classes need to
    be wrapped in \c QT_BEGIN_NAMESPACE and \c QT_END_NAMESPACE.
    After that, a \c QT_USE_NAMESPACE should follow.
    No further changes should be needed.

    \sa QT_NAMESPACE
*/

/*!
    \macro QT_END_NAMESPACE
    \internal

    This macro expands to

    \snippet snippets/code/src.corelib.global.qglobal.cpp end namespace macro

    if \c QT_NAMESPACE is defined and nothing otherwise. It is used to cancel
    the effect of \c QT_BEGIN_NAMESPACE.

    If a source file ends with a \c{#include} directive that includes a moc file,
    \c QT_END_NAMESPACE should be placed before that \c{#include}.

    \sa QT_NAMESPACE
*/

/*!
    \macro QT_BEGIN_INCLUDE_NAMESPACE
    \internal

    This macro is equivalent to \c QT_END_NAMESPACE.
    It only serves as syntactic sugar and is intended
    to be used before #include lines within a
    \c QT_BEGIN_NAMESPACE ... \c QT_END_NAMESPACE block.

    \sa QT_NAMESPACE
*/

/*!
    \macro QT_END_INCLUDE_NAMESPACE
    \internal

    This macro is equivalent to \c QT_BEGIN_NAMESPACE.
    It only serves as syntactic sugar and is intended
    to be used after #include lines within a
    \c QT_BEGIN_NAMESPACE ... \c QT_END_NAMESPACE block.

    \sa QT_NAMESPACE
*/

/*!
    \macro QT_BEGIN_MOC_NAMESPACE
    \internal

    This macro is output by moc at the beginning of
    moc files. It is equivalent to \c QT_USE_NAMESPACE.

    \sa QT_NAMESPACE
*/

/*!
    \macro QT_END_MOC_NAMESPACE
    \internal

    This macro is output by moc at the beginning of
    moc files. It expands to nothing.

    \sa QT_NAMESPACE
*/

/*!
 \fn bool qFuzzyCompare(double p1, double p2) 
 \since 4.4
 \threadsafe
 
 Compares the floating point value \a p1 and \a p2 and
 returns \c true if they are considered equal, otherwise \c false.

 The two numbers are compared in a relative way, where the
 exactness is stronger the smaller the numbers are.
 */

/*!
 \fn bool qFuzzyCompare(float p1, float p2) 
 \since 4.4
 \threadsafe
 \overload
 */

QT_END_NAMESPACE
