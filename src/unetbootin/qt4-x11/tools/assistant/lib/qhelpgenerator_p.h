/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt Assistant of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QHELPGENERATOR_H
#define QHELPGENERATOR_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists for the convenience
// of the help generator tools. This header file may change from version
// to version without notice, or even be removed.
//
// We mean it.
//

#include "qhelp_global.h"
#include "qhelpdatainterface_p.h"

#include <QtCore/QObject>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QHelpGeneratorPrivate;

class QHELP_EXPORT QHelpGenerator : public QObject
{
    Q_OBJECT

public:
    QHelpGenerator(QObject *parent = 0);
    ~QHelpGenerator();

    bool generate(QHelpDataInterface *helpData,
        const QString &outputFileName);
    QString error() const;

Q_SIGNALS:
    void statusChanged(const QString &msg);
    void progressChanged(double progress);
    void warning(const QString &msg);

private:
    struct FileNameTableData
    {
        QString name;
        int fileId;
        QString title;
    };

    void writeTree(QDataStream &s, QHelpDataContentItem *item, int depth);
    bool createTables();
    bool insertFileNotFoundFile();
    bool registerCustomFilter(const QString &filterName,
        const QStringList &filterAttribs, bool forceUpdate = false);
    bool registerVirtualFolder(const QString &folderName, const QString &ns);
    bool insertFilterAttributes(const QStringList &attributes);
    bool insertKeywords(const QList<QHelpDataIndexItem> keywords,
        const QStringList &filterAttributes);
    bool insertFiles(const QStringList &files, const QString &rootPath,
        const QStringList &filterAttributes);
    bool insertContents(const QByteArray &ba,
        const QStringList &filterAttributes);
    bool insertMetaData(const QMap<QString, QVariant> &metaData);
    void cleanupDB();
    void setupProgress(QHelpDataInterface *helpData);
    void addProgress(double step);

    QHelpGeneratorPrivate *d;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif
