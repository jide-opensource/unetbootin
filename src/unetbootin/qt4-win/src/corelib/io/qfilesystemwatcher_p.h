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

#ifndef QFILESYSTEMWATCHER_P_H
#define QFILESYSTEMWATCHER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "qfilesystemwatcher.h"

#ifndef QT_NO_FILESYSTEMWATCHER

#include <private/qobject_p.h>

#include <QtCore/qstringlist.h>
#include <QtCore/qthread.h>

QT_BEGIN_NAMESPACE

class QFileSystemWatcherEngine : public QThread
{
    Q_OBJECT

protected:
    inline QFileSystemWatcherEngine()
    {
        moveToThread(this);
    }

public:
    // fills \a files and \a directires with the \a paths it could
    // watch, and returns a list of paths this engine could not watch
    virtual QStringList addPaths(const QStringList &paths,
                                 QStringList *files,
                                 QStringList *directories) = 0;
    // removes \a paths from \a files and \a directories, and returns
    // a list of paths this engine does not know about (either addPath
    // failed or wasn't called)
    virtual QStringList removePaths(const QStringList &paths,
                                    QStringList *files,
                                    QStringList *directories) = 0;

    virtual void stop() = 0;

Q_SIGNALS:
    void fileChanged(const QString &path, bool removed);
    void directoryChanged(const QString &path, bool removed);
};

class QFileSystemWatcherPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QFileSystemWatcher)

    static QFileSystemWatcherEngine *createNativeEngine();

public:
    QFileSystemWatcherPrivate();
    void init();
    void initPollerEngine();
    void initForcedEngine(const QString &);

    QFileSystemWatcherEngine *native, *poller, *forced;
    QStringList files, directories;

    // private slots
    void _q_fileChanged(const QString &path, bool removed);
    void _q_directoryChanged(const QString &path, bool removed);
};


QT_END_NAMESPACE
#endif // QT_NO_FILESYSTEMWATCHER
#endif // QFILESYSTEMWATCHER_P_H
