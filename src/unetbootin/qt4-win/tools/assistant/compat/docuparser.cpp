/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Assistant of the Qt Toolkit.
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

#include "docuparser.h"
#include "profile.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegExp>
#include <QString>
#include <QDataStream>

QT_BEGIN_NAMESPACE

QDataStream &operator>>(QDataStream &s, ContentItem &ci)
{
    s >> ci.title;
    s >> ci.reference;
    s >> ci.depth;
    return s;
}

QDataStream &operator<<(QDataStream &s, const ContentItem &ci)
{
    s << ci.title;
    s << ci.reference;
    s << ci.depth;
    return s;
}

const QString DocuParser::DocumentKey = QLatin1String("/Qt Assistant/") + QLatin1String(QT_VERSION_STR) + QLatin1String("/");

DocuParser *DocuParser::createParser(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)) {
        return 0;
    }

    QString str;
    int maxlen = 1024;
    int majVer = 0, minVer = 0, serVer = 0;
    static QRegExp re(QLatin1String("assistantconfig +version=\"(\\d)\\.(\\d)\\.(\\d)\""), Qt::CaseInsensitive);
    Q_ASSERT(re.isValid());
    while(!(str = QLatin1String(file.readLine(maxlen))).isEmpty()) {
        if(re.indexIn(str) >= 0) {
            majVer = re.cap(1).toInt();
            minVer = re.cap(2).toInt();
            serVer = re.cap(3).toInt();
            break;
        }
    }

    if (majVer < 3 || majVer == 3 && minVer < 2) {
        return new DocuParser310;
    }

    return new DocuParser320;
}


bool DocuParser::parse(QFile *file)
{
    QXmlInputSource source(file);
    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    reader.setErrorHandler(this);
    setFileName(QFileInfo(*file).absoluteFilePath());
    return reader.parse(source);
}


QString DocuParser::errorProtocol() const
{
    return errorProt;
}


QList<ContentItem> DocuParser::getContentItems()
{
    return contentList;
}


QList<IndexItem*> DocuParser::getIndexItems()
{
    return indexList;
}

QString DocuParser::absolutify(const QString &name, bool makeUrl) const
{
    if (!name.isEmpty()) {
        QString s = name;
        s.replace(QLatin1String("\\"), QLatin1String("/"));
        QFileInfo orgPath(name);
        if(orgPath.isRelative())
            s = QFileInfo(fname).path() + QLatin1Char('/') + name;
        if (makeUrl)
            s.prepend(QLatin1String("file:"));
        return s;
    }
    return name;
}


void DocuParser310::addTo(Profile *p)
{
    p->addDCFTitle(fname, docTitle);
    p->addDCFIcon(docTitle, iconName);
    p->addDCFIndexPage(docTitle, conURL);
}


bool DocuParser310::startDocument()
{
    state = StateInit;
    errorProt = QLatin1String("");

    contentRef = QLatin1String("");
    indexRef = QLatin1String("");
    depth = 0;

    contentList.clear();
    qDeleteAll(indexList);
    indexList.clear();

    return true;
}


bool DocuParser310::startElement(const QString &, const QString &,
                               const QString &qname,
                               const QXmlAttributes &attr)
{
    if (qname == QLatin1String("DCF") && state == StateInit) {
        state = StateContent;
        contentRef = absolutify(attr.value(QLatin1String("ref")), false);
        conURL = contentRef;
        docTitle = attr.value(QLatin1String("title"));
        iconName = absolutify(attr.value(QLatin1String("icon")), false);
        contentList.append(ContentItem(docTitle, absolutify(contentRef), depth));
    } else if (qname == QLatin1String("section") && (state == StateContent || state == StateSect)) {
        state = StateSect;
        contentRef = absolutify(attr.value(QLatin1String("ref")));
        title = attr.value(QLatin1String("title"));
        depth++;
        contentList.append(ContentItem(title, contentRef, depth));
    } else if (qname == QLatin1String("keyword") && state == StateSect) {
        state = StateKeyword;
        indexRef = absolutify(attr.value(QLatin1String("ref")));
    } else
        return false;
    return true;
}

bool DocuParser310::endElement(const QString &nameSpace, const QString &localName,
                             const QString &qName)
{
    Q_UNUSED(nameSpace);
    Q_UNUSED(localName);
    Q_UNUSED(qName);

    switch(state) {
    case StateInit:
        break;
    case StateContent:
        state = StateInit;
        break;
    case StateSect:
        state = --depth ? StateSect : StateContent;
        break;
    case StateKeyword:
        state = StateSect;
        break;
    default:
        break;
    }
    return true;
}


bool DocuParser310::characters(const QString& ch)
{
    QString str = ch.simplified();
    if (str.isEmpty())
        return true;

    switch (state) {
        case StateInit:
        case StateContent:
        case StateSect:
            return false;
            break;
        case StateKeyword:
            indexList.append(new IndexItem(str, indexRef));
            break;
        default:
            return false;
    }
    return true;
}


bool DocuParser310::fatalError(const QXmlParseException& exception)
{
    errorProt += QString::fromLatin1("fatal parsing error: %1 in line %2, column %3\n")
        .arg(exception.message())
        .arg(exception.lineNumber())
        .arg(exception.columnNumber());

    return QXmlDefaultHandler::fatalError(exception);
}


DocuParser320::DocuParser320()
    : prof(new Profile)
{
}


void DocuParser320::addTo(Profile *p)
{
    QMap<QString,QString>::ConstIterator it;

    for (it = prof->dcfTitles.constBegin(); it != prof->dcfTitles.constEnd(); ++it)
        p->dcfTitles[it.key()] = *it;

    for (it = prof->icons.constBegin(); it != prof->icons.constEnd(); ++it)
        p->icons[it.key()] = *it;

    for (it = prof->indexPages.constBegin(); it != prof->indexPages.constEnd(); ++it)
        p->indexPages[it.key()] = *it;
}


bool DocuParser320::startDocument()
{
    state = StateInit;
    errorProt = QLatin1String("");

    contentRef = QLatin1String("");
    indexRef = QLatin1String("");
    depth = 0;
    contentList.clear();
    indexList.clear();

    prof->addDCF(fname);

    return true;
}

bool DocuParser320::startElement(const QString &, const QString &,
                               const QString &qname,
                               const QXmlAttributes &attr)
{
    QString lower = qname.toLower();

    switch(state) {

    case StateInit:
        if(lower == QLatin1String("assistantconfig"))
            state = StateDocRoot;
        break;

    case StateDocRoot:
        if(lower == QLatin1String("dcf")) {
            state = StateContent;
            contentRef = absolutify(attr.value(QLatin1String("ref")));
            conURL = contentRef;
            docTitle = attr.value(QLatin1String("title"));
            iconName = absolutify(attr.value(QLatin1String("icon")));
            contentList.append(ContentItem(docTitle, contentRef, depth));
        } else if(lower == QLatin1String("profile")) {
            state = StateProfile;
        }
        break;

    case StateSect:
        if (lower == QLatin1String("keyword") && state == StateSect) {
            state = StateKeyword;
            indexRef = absolutify(attr.value(QLatin1String("ref")));
            break;
        } // else if (lower == "section")
    case StateContent:
        if(lower == QLatin1String("section")) {
            state = StateSect;
            contentRef = absolutify(attr.value(QLatin1String("ref")));
            title = attr.value(QLatin1String("title"));
            depth++;
            contentList.append(ContentItem(title, contentRef, depth));
        }
        break;

    case StateProfile:
        if(lower == QLatin1String("property")) {
            state = StateProperty;
            propertyName = attr.value(QLatin1String("name"));
        }
        break;

    case StateProperty:
        break;

    default:
        break;
    }

    return true;
}

bool DocuParser320::endElement(const QString &nameSpace,
                                const QString &localName,
                                const QString &qName)
{
    Q_UNUSED(nameSpace);
    Q_UNUSED(localName);
    Q_UNUSED(qName);

    switch(state) {
    case StateInit:
        break;
    case StateDocRoot:
        state = StateInit;
        break;
    case StateProfile:
        state = StateDocRoot;
        break;
    case StateProperty:
        state = StateProfile;
        if(propertyName.isEmpty() || propertyValue.isEmpty())
            return false;
        {
            static const QStringList lst = QStringList()
                << QLatin1String("startpage") << QLatin1String("abouturl")
                << QLatin1String("applicationicon") << QLatin1String("assistantdocs");

            if (lst.contains(propertyName))
                propertyValue = absolutify(propertyValue);
        }
        prof->addProperty(propertyName, propertyValue);
        break;
    case StateContent:
        if(!iconName.isEmpty())
            prof->addDCFIcon(docTitle, iconName);
        if(contentRef.isEmpty())
            return false;
        prof->addDCFIndexPage(docTitle, conURL);
        prof->addDCFTitle(fname, docTitle);
        state = StateDocRoot;
        break;
    case StateSect:
        state = --depth ? StateSect : StateContent;
        break;
    case StateKeyword:
        state = StateSect;
        break;
    }
    return true;
}

bool DocuParser320::characters(const QString& ch)
{
    QString str = ch.simplified();
    if (str.isEmpty())
        return true;

    switch (state) {
    case StateInit:
    case StateContent:
    case StateSect:
        return false;
        break;
    case StateKeyword:
        indexList.append(new IndexItem(str, indexRef));
        break;
    case StateProperty:
        propertyValue = ch;
        break;
    default:
        return false;
    }
    return true;
}

bool DocuParser320::fatalError(const QXmlParseException& exception)
{
    errorProt += QString::fromLatin1("fatal parsing error: %1 in line %2, column %3\n")
        .arg(exception.message())
        .arg(exception.lineNumber())
        .arg(exception.columnNumber());
    return QXmlDefaultHandler::fatalError(exception);
}

QT_END_NAMESPACE
