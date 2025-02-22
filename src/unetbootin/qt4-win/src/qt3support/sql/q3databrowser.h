/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt3Support module of the Qt Toolkit.
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

#ifndef Q3DATABROWSER_H
#define Q3DATABROWSER_H

#include <QtGui/qwidget.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtSql/qsql.h>
#include <QtSql/qsqlindex.h>
#include <Qt3Support/q3sqlcursor.h>
#include <QtSql/qsqlerror.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Qt3Support)

#ifndef QT_NO_SQL_VIEW_WIDGETS

class Q3SqlForm;
class Q3DataBrowserPrivate;

class Q_COMPAT_EXPORT Q3DataBrowser : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool boundaryChecking READ boundaryChecking WRITE setBoundaryChecking)
    Q_PROPERTY(QString filter READ filter WRITE setFilter)
    Q_PROPERTY(QStringList sort READ sort WRITE setSort)
    Q_PROPERTY(bool confirmEdits READ confirmEdits WRITE setConfirmEdits)
    Q_PROPERTY(bool confirmInsert READ confirmInsert WRITE setConfirmInsert)
    Q_PROPERTY(bool confirmUpdate READ confirmUpdate WRITE setConfirmUpdate)
    Q_PROPERTY(bool confirmDelete READ confirmDelete WRITE setConfirmDelete)
    Q_PROPERTY(bool confirmCancels READ confirmCancels WRITE setConfirmCancels)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(bool autoEdit READ autoEdit WRITE setAutoEdit)

public:
    Q3DataBrowser(QWidget* parent=0, const char* name=0, Qt::WindowFlags fl = 0);
    ~Q3DataBrowser();

    enum Boundary {
        Unknown,
        None,
        BeforeBeginning,
        Beginning,
        End,
        AfterEnd
    };

    Boundary boundary();
    void setBoundaryChecking(bool active);
    bool boundaryChecking() const;

    void setSort(const QSqlIndex& sort);
    void setSort(const QStringList& sort);
    QStringList  sort() const;
    void setFilter(const QString& filter);
    QString filter() const;
    virtual void setSqlCursor(Q3SqlCursor* cursor, bool autoDelete = false);
    Q3SqlCursor* sqlCursor() const;
    virtual void setForm(Q3SqlForm* form);
    Q3SqlForm* form();

    virtual void setConfirmEdits(bool confirm);
    virtual void setConfirmInsert(bool confirm);
    virtual void setConfirmUpdate(bool confirm);
    virtual void setConfirmDelete(bool confirm);
    virtual void setConfirmCancels(bool confirm);
    bool confirmEdits() const;
    bool confirmInsert() const;
    bool confirmUpdate() const;
    bool confirmDelete() const;
    bool confirmCancels() const;

    virtual void setReadOnly(bool active);
    bool isReadOnly() const;
    virtual void setAutoEdit(bool autoEdit);
    bool autoEdit() const;

    virtual bool seek(int i, bool relative = false);

Q_SIGNALS:
    void firstRecordAvailable(bool available);
    void lastRecordAvailable(bool available);
    void nextRecordAvailable(bool available);
    void prevRecordAvailable(bool available);

    void currentChanged(const QSqlRecord* record);
    void primeInsert(QSqlRecord* buf);
    void primeUpdate(QSqlRecord* buf);
    void primeDelete(QSqlRecord* buf);
    void beforeInsert(QSqlRecord* buf);
    void beforeUpdate(QSqlRecord* buf);
    void beforeDelete(QSqlRecord* buf);
    void cursorChanged(Q3SqlCursor::Mode mode);

public Q_SLOTS:
    virtual void refresh();

    virtual void insert();
    virtual void update();
    virtual void del();

    virtual void first();
    virtual void last();
    virtual void next();
    virtual void prev();

    virtual void readFields();
    virtual void writeFields();
    virtual void clearValues();

    void updateBoundary();

protected:
    virtual bool insertCurrent();
    virtual bool updateCurrent();
    virtual bool deleteCurrent();
    virtual bool currentEdited();

    virtual QSql::Confirm confirmEdit(QSql::Op m);
    virtual QSql::Confirm confirmCancel(QSql::Op m);

    virtual void handleError(const QSqlError& error);

private:
    typedef bool (Q3SqlCursor::*Nav)();
    bool preNav();
    void postNav(bool primeUpd);
    void nav(Nav nav);
    Q3DataBrowserPrivate* d;

    Q_DISABLE_COPY(Q3DataBrowser)
};

#endif // QT_NO_SQL_VIEW_WIDGETS

QT_END_NAMESPACE

QT_END_HEADER

#endif // Q3DATABROWSER_H
