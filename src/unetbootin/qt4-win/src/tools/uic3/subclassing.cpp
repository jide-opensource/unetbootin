/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the tools applications of the Qt Toolkit.
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

#include "ui3reader.h"
#include "parser.h"
#include "domtool.h"
#include "globaldefs.h"
#include <QFile>
#include <QStringList>
#include <QDateTime>
#include <QRegExp>
#include <stdio.h>
#include <stdlib.h>

QT_BEGIN_NAMESPACE

/*!
  Creates a declaration ( headerfile ) for a subclass \a subClass
  of the form given in \a e

  \sa createSubImpl()
 */
void Ui3Reader::createSubDecl( const QDomElement &e, const QString& subClass )
{
    QDomElement n;
    QDomNodeList nl;
    int i;

    QString objClass = getClassName( e );
    if ( objClass.isEmpty() )
        return;

    out << "class " << subClass << " : public " << nameOfClass << endl;
    out << "{" << endl;

/* tmake ignore Q_OBJECT */
    out << "    Q_OBJECT" << endl;
    out << endl;
    out << "public:" << endl;

    // constructor
    if ( objClass == QLatin1String("QDialog") || objClass == QLatin1String("QWizard") ) {
        out << "    " << subClass << "( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0 );" << endl;
    } else { // standard QWidget
        out << "    " << subClass << "( QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = 0 );" << endl;
    }

    // destructor
    out << "    ~" << subClass << "();" << endl;
    out << endl;

    // find additional functions
    QStringList publicSlots, protectedSlots, privateSlots;
    QStringList publicSlotTypes, protectedSlotTypes, privateSlotTypes;
    QStringList publicSlotSpecifier, protectedSlotSpecifier, privateSlotSpecifier;
    QStringList publicFuncts, protectedFuncts, privateFuncts;
    QStringList publicFunctRetTyp, protectedFunctRetTyp, privateFunctRetTyp;
    QStringList publicFunctSpec, protectedFunctSpec, privateFunctSpec;

    nl = e.parentNode().toElement().elementsByTagName(QLatin1String("slot"));
    for ( i = 0; i < (int) nl.length(); i++ ) {
        n = nl.item(i).toElement();
        if ( n.parentNode().toElement().tagName() != QLatin1String("slots")
             && n.parentNode().toElement().tagName() != QLatin1String("connections") )
            continue;
        if ( n.attribute(QLatin1String("language"), QLatin1String("C++")) != QLatin1String("C++") )
            continue;
        QString returnType = n.attribute(QLatin1String("returnType"), QLatin1String("void"));
        QString functionName = n.firstChild().toText().data().trimmed();
        if ( functionName.endsWith(QLatin1String(";")))
            functionName = functionName.left( functionName.length() - 1 );
        QString specifier = n.attribute(QLatin1String("specifier"));
        QString access = n.attribute(QLatin1String("access"));
        if ( access == QLatin1String("protected") ) {
            protectedSlots += functionName;
            protectedSlotTypes += returnType;
            protectedSlotSpecifier += specifier;
        } else if ( access == QLatin1String("private") ) {
            privateSlots += functionName;
            privateSlotTypes += returnType;
            privateSlotSpecifier += specifier;
        } else {
            publicSlots += functionName;
            publicSlotTypes += returnType;
            publicSlotSpecifier += specifier;
        }
    }

    nl = e.parentNode().toElement().elementsByTagName(QLatin1String("function"));
    for ( i = 0; i < (int) nl.length(); i++ ) {
        n = nl.item(i).toElement();
        if ( n.parentNode().toElement().tagName() != QLatin1String("functions") )
            continue;
        if ( n.attribute(QLatin1String("language"), QLatin1String("C++")) != QLatin1String("C++") )
            continue;
        QString returnType = n.attribute(QLatin1String("returnType"), QLatin1String("void"));
        QString functionName = n.firstChild().toText().data().trimmed();
        if ( functionName.endsWith(QLatin1String(";")) )
            functionName = functionName.left( functionName.length() - 1 );
        QString specifier = n.attribute(QLatin1String("specifier"));
        QString access = n.attribute(QLatin1String("access"));
        if ( access == QLatin1String("protected") ) {
            protectedFuncts += functionName;
            protectedFunctRetTyp += returnType;
            protectedFunctSpec += specifier;
        } else if ( access == QLatin1String("private") ) {
            privateFuncts += functionName;
            privateFunctRetTyp += returnType;
            privateFunctSpec += specifier;
        } else {
            publicFuncts += functionName;
            publicFunctRetTyp += returnType;
            publicFunctSpec += specifier;
        }
    }

    if ( !publicFuncts.isEmpty() )
        writeFunctionsSubDecl( publicFuncts, publicFunctRetTyp, publicFunctSpec );

    // create public additional slots
    if ( !publicSlots.isEmpty() ) {
        out << "public slots:" << endl;
        writeFunctionsSubDecl( publicSlots, publicSlotTypes, publicSlotSpecifier );
    }

    if ( !protectedFuncts.isEmpty() ) {
        out << "protected:" << endl;
        writeFunctionsSubDecl( protectedFuncts, protectedFunctRetTyp, protectedFunctSpec );
    }

    // create protected additional slots
    if ( !protectedSlots.isEmpty() ) {
        out << "protected slots:" << endl;
        writeFunctionsSubDecl( protectedSlots, protectedSlotTypes, protectedSlotSpecifier );
    }

    if ( !privateFuncts.isEmpty() ) {
        out << "private:" << endl;
        writeFunctionsSubDecl( privateFuncts, privateFunctRetTyp, privateFunctSpec );
    }

    // create private additional slots
    if ( !privateSlots.isEmpty() ) {
        out << "private slots:" << endl;
        writeFunctionsSubDecl( privateSlots, privateSlotTypes, privateSlotSpecifier );
    }
    out << "};" << endl;
}

void Ui3Reader::writeFunctionsSubDecl( const QStringList &fuLst, const QStringList &typLst, const QStringList &specLst )
{
    QStringList::ConstIterator it, it2, it3;
    for ( it = fuLst.begin(), it2 = typLst.begin(), it3 = specLst.begin();
          it != fuLst.end(); ++it, ++it2, ++it3 ) {
        QString type = fixDeclaration(*it2);
        if ( type.isEmpty() )
            type = QLatin1String("void");
        if ( *it3 == QLatin1String("non virtual") )
            continue;
        out << "    " << type << " " << fixDeclaration(*it) << ";" << endl;
    }
    out << endl;
}

/*!
  Creates an implementation for a subclass \a subClass of the form
  given in \a e

  \sa createSubDecl()
 */
void Ui3Reader::createSubImpl( const QDomElement &e, const QString& subClass )
{
    QDomElement n;
    QDomNodeList nl;
    int i;

    QString objClass = getClassName( e );
    if ( objClass.isEmpty() )
        return;

    // constructor
    if ( objClass == QLatin1String("QDialog") || objClass == QLatin1String("QWizard") ) {
        out << "/* " << endl;
        out << " *  Constructs a " << subClass << " which is a child of 'parent', with the " << endl;
        out << " *  name 'name' and widget flags set to 'f' " << endl;
        out << " *" << endl;
        out << " *  The " << objClass.mid(1).toLower() << " will by default be modeless, unless you set 'modal' to" << endl;
        out << " *  true to construct a modal " << objClass.mid(1).toLower() << "." << endl;
        out << " */" << endl;
        out << subClass << "::" << subClass << "( QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl )" << endl;
        out << "    : " << nameOfClass << "( parent, name, modal, fl )" << endl;
    } else { // standard QWidget
        out << "/* " << endl;
        out << " *  Constructs a " << subClass << " which is a child of 'parent', with the " << endl;
        out << " *  name 'name' and widget flags set to 'f' " << endl;
        out << " */" << endl;
        out << subClass << "::" << subClass << "( QWidget* parent, const char* name, Qt::WindowFlags fl )" << endl;
        out << "    : " << nameOfClass << "( parent, name, fl )" << endl;
    }
    out << "{" << endl;
    out << "}" << endl;
    out << endl;

    // destructor
    out << "/*  " << endl;
    out << " *  Destroys the object and frees any allocated resources" << endl;
    out << " */" << endl;
    out << subClass << "::~" << subClass << "()" << endl;
    out << "{" << endl;
    out << "    // no need to delete child widgets, Qt does it all for us" << endl;
    out << "}" << endl;
    out << endl;


    // find additional functions
    QStringList publicSlots, protectedSlots, privateSlots;
    QStringList publicSlotTypes, protectedSlotTypes, privateSlotTypes;
    QStringList publicSlotSpecifier, protectedSlotSpecifier, privateSlotSpecifier;
    QStringList publicFuncts, protectedFuncts, privateFuncts;
    QStringList publicFunctRetTyp, protectedFunctRetTyp, privateFunctRetTyp;
    QStringList publicFunctSpec, protectedFunctSpec, privateFunctSpec;

    nl = e.parentNode().toElement().elementsByTagName(QLatin1String("slot"));
    for ( i = 0; i < (int) nl.length(); i++ ) {
        n = nl.item(i).toElement();
        if ( n.parentNode().toElement().tagName() != QLatin1String("slots")
             && n.parentNode().toElement().tagName() != QLatin1String("connections") )
            continue;
        if ( n.attribute(QLatin1String("language"), QLatin1String("C++")) != QLatin1String("C++") )
            continue;
        QString returnType = n.attribute(QLatin1String("returnType"), QLatin1String("void"));
        QString functionName = n.firstChild().toText().data().trimmed();
        if ( functionName.endsWith(QLatin1String(";")) )
            functionName = functionName.left( functionName.length() - 1 );
        QString specifier = n.attribute(QLatin1String("specifier"));
        QString access = n.attribute(QLatin1String("access"));
        if ( access == QLatin1String("protected") ) {
            protectedSlots += functionName;
            protectedSlotTypes += returnType;
            protectedSlotSpecifier += specifier;
        } else if ( access == QLatin1String("private") ) {
            privateSlots += functionName;
            privateSlotTypes += returnType;
            privateSlotSpecifier += specifier;
        } else {
            publicSlots += functionName;
            publicSlotTypes += returnType;
            publicSlotSpecifier += specifier;
        }
    }

    nl = e.parentNode().toElement().elementsByTagName(QLatin1String("function"));
    for ( i = 0; i < (int) nl.length(); i++ ) {
        n = nl.item(i).toElement();
        if ( n.parentNode().toElement().tagName() != QLatin1String("functions") )
            continue;
        if ( n.attribute(QLatin1String("language"), QLatin1String("C++")) != QLatin1String("C++") )
            continue;
        QString returnType = n.attribute(QLatin1String("returnType"), QLatin1String("void"));
        QString functionName = n.firstChild().toText().data().trimmed();
        if ( functionName.endsWith(QLatin1String(";")) )
            functionName = functionName.left( functionName.length() - 1 );
        QString specifier = n.attribute(QLatin1String("specifier"));
        QString access = n.attribute(QLatin1String("access"));
        if ( access == QLatin1String("protected") ) {
            protectedFuncts += functionName;
            protectedFunctRetTyp += returnType;
            protectedFunctSpec += specifier;
        } else if ( access == QLatin1String("private") ) {
            privateFuncts += functionName;
            privateFunctRetTyp += returnType;
            privateFunctSpec += specifier;
        } else {
            publicFuncts += functionName;
            publicFunctRetTyp += returnType;
            publicFunctSpec += specifier;
        }
    }

    if ( !publicFuncts.isEmpty() )
        writeFunctionsSubImpl( publicFuncts, publicFunctRetTyp, publicFunctSpec, subClass, QLatin1String("public function"));

    // create stubs for public additional slots
    if ( !publicSlots.isEmpty() )
        writeFunctionsSubImpl( publicSlots, publicSlotTypes, publicSlotSpecifier, subClass, QLatin1String("public slot"));

    if ( !protectedFuncts.isEmpty() )
        writeFunctionsSubImpl( protectedFuncts, protectedFunctRetTyp, protectedFunctSpec, subClass, QLatin1String("protected function"));

    // create stubs for protected additional slots
    if ( !protectedSlots.isEmpty() )
        writeFunctionsSubImpl( protectedSlots, protectedSlotTypes, protectedSlotSpecifier, subClass, QLatin1String("protected slot"));

    if ( !privateFuncts.isEmpty() )
        writeFunctionsSubImpl( privateFuncts, privateFunctRetTyp, privateFunctSpec, subClass, QLatin1String("private function"));

    // create stubs for private additional slots
    if ( !privateSlots.isEmpty() )
        writeFunctionsSubImpl( privateSlots, privateSlotTypes, privateSlotSpecifier, subClass, QLatin1String("private slot"));
}

void Ui3Reader::writeFunctionsSubImpl( const QStringList &fuLst, const QStringList &typLst, const QStringList &specLst,
                                 const QString &subClass, const QString &descr )
{
    QStringList::ConstIterator it, it2, it3;
    for ( it = fuLst.begin(), it2 = typLst.begin(), it3 = specLst.begin();
          it != fuLst.end(); ++it, ++it2, ++it3 ) {
        QString type = fixDeclaration(*it2);
        if ( type.isEmpty() )
            type = QLatin1String("void");
        if ( *it3 == QLatin1String("non virtual") )
            continue;
        out << "/*" << endl;
        out << " * " << descr << endl;
        out << " */" << endl;
        out << type << " " << subClass << "::" << fixDeclaration(*it) << endl;
        out << "{" << endl;
        out << "    qWarning( \"" << subClass << "::" << fixDeclaration(*it) << " not yet implemented!\" );" << endl;
        out << "}" << endl << endl;
    }
    out << endl;
}

QT_END_NAMESPACE
