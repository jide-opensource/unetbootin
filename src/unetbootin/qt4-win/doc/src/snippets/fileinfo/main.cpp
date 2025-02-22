/****************************************************************************
**
** Copyright (C) 2005-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the documentation of the Qt Toolkit.
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

#include <QApplication>
#include <QPushButton>
#include <QFileInfo>
#include <QDir>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//! [0]
    QFileInfo fileInfo1("~/examples/191697/.");
    QFileInfo fileInfo2("~/examples/191697/..");
    QFileInfo fileInfo3("~/examples/191697/main.cpp");
//! [0]    
//! [1]    
    QFileInfo fileInfo4(".");
    QFileInfo fileInfo5("..");
    QFileInfo fileInfo6("main.cpp");
//! [1]    

    qDebug() << fileInfo1.fileName();
    qDebug() << fileInfo2.fileName();
    qDebug() << fileInfo3.fileName();
    qDebug() << fileInfo4.fileName();
    qDebug() << fileInfo5.fileName();
    qDebug() << fileInfo6.fileName();

    QPushButton* button1 = new QPushButton(fileInfo1.dir().path());
    QPushButton* button2 = new QPushButton(fileInfo2.dir().path());
    QPushButton* button3 = new QPushButton(fileInfo3.dir().path());
    QPushButton* button4 = new QPushButton(fileInfo4.dir().path());
    QPushButton* button5 = new QPushButton(fileInfo5.dir().path());
    QPushButton* button6 = new QPushButton(fileInfo6.dir().path());

    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(button1);
    vbox->addWidget(button2);
    vbox->addWidget(button3);
    vbox->addWidget(button4);
    vbox->addWidget(button5);
    vbox->addWidget(button6);
    vbox->addStretch(1);

    QGroupBox *groupBox = new QGroupBox("QFileInfo::dir() test");
    groupBox->setLayout(vbox);
    groupBox->show();

    return app.exec();
}
