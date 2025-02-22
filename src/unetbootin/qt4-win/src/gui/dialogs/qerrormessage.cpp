/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#include "qerrormessage.h"

#ifndef QT_NO_ERRORMESSAGE

#include "qapplication.h"
#include "qcheckbox.h"
#include "qlabel.h"
#include "qlayout.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "qstringlist.h"
#include "qtextedit.h"
#include "qdialog_p.h"
#include "qpixmap.h"
#include "qmetaobject.h"
#include "qthread.h"
#include <qhash.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef Q_OS_WINCE
extern bool qt_wince_is_mobile();    //defined in qguifunctions_wince.cpp
extern bool qt_wince_is_high_dpi();  //defined in qguifunctions_wince.cpp

#include "qguifunctions_wince.h"
#endif

QT_BEGIN_NAMESPACE

class QErrorMessagePrivate : public QDialogPrivate
{
    Q_DECLARE_PUBLIC(QErrorMessage)
public:
    QPushButton * ok;
    QCheckBox * again;
    QTextEdit * errors;
    QLabel * icon;
    QStringList pending;
    QHash<QString, int> doNotShow;
    QString currentMessage;

    bool nextPending();
    void retranslateStrings();
};

class QErrorMessageTextView : public QTextEdit
{
public:
    QErrorMessageTextView(QWidget *parent)
        : QTextEdit(parent) { setReadOnly(true); }

    virtual QSize minimumSizeHint() const;
    virtual QSize sizeHint() const;
};

QSize QErrorMessageTextView::minimumSizeHint() const
{
#ifdef Q_OS_WINCE
    if (qt_wince_is_mobile())
         if (qt_wince_is_high_dpi())
            return QSize(200, 200);
         else
             return QSize(100, 100);
    else
      return QSize(70, 70);
#else
    return QSize(50, 50);
#endif
}

QSize QErrorMessageTextView::sizeHint() const
{
#ifdef Q_OS_WINCE
    if (qt_wince_is_mobile())
         if (qt_wince_is_high_dpi())
            return QSize(400, 200);
         else
             return QSize(320, 120);
    else
      return QSize(300, 100);
#else
    return QSize(250, 75);
#endif
}

/*!
    \class QErrorMessage

    \brief The QErrorMessage class provides an error message display dialog.

    \ingroup dialogs
    \ingroup misc

    An error message widget consists of a text label and a checkbox. The
    checkbox lets the user control whether the same error message will be
    displayed again in the future, typically displaying the text,
    "Show this message again" translated into the appropriate local
    language.

    For production applications, the class can be used to display messages which
    the user only needs to see once. To use QErrorMessage like this, you create
    the dialog in the usual way, and show it by calling the showMessage() slot or
    connecting signals to it.

    The static qtHandler() function installs a message handler
    using qInstallMsgHandler() and creates a QErrorMessage that displays
    qDebug(), qWarning() and qFatal() messages. This is most useful in
    environments where no console is available to display warnings and
    error messages.

    In both cases QErrorMessage will queue pending messages and display
    them in order, with each new message being shown as soon as the user
    has accepted the previous message. Once the user has specified that a
    message is not to be shown again it is automatically skipped, and the
    dialog will show the next appropriate message in the queue.

    The \l{dialogs/standarddialogs}{Standard Dialogs} example shows
    how to use QErrorMessage as well as other built-in Qt dialogs.

    \img qerrormessage.png

    \sa QMessageBox, QStatusBar::showMessage(), {Standard Dialogs Example}
*/

static QErrorMessage * qtMessageHandler = 0;

static void deleteStaticcQErrorMessage() // post-routine
{
    if (qtMessageHandler) {
        delete qtMessageHandler;
        qtMessageHandler = 0;
    }
}

static bool metFatal = false;

static void jump(QtMsgType t, const char * m)
{
    if (!qtMessageHandler)
        return;

    QString rich;

    switch (t) {
    case QtDebugMsg:
    default:
        rich = QErrorMessage::tr("Debug Message:");
        break;
    case QtWarningMsg:
        rich = QErrorMessage::tr("Warning:");
        break;
    case QtFatalMsg:
        rich = QErrorMessage::tr("Fatal Error:");
    }
    rich = QString::fromLatin1("<p><b>%1</b></p>").arg(rich);
    rich += Qt::convertFromPlainText(QLatin1String(m), Qt::WhiteSpaceNormal);

    // ### work around text engine quirk
    if (rich.endsWith(QLatin1String("</p>")))
        rich.chop(4);

    if (!metFatal) {
        if (QThread::currentThread() == qApp->thread()) {
            qtMessageHandler->showMessage(rich);
        } else {
            QMetaObject::invokeMethod(qtMessageHandler,
                                      "showMessage",
                                      Qt::QueuedConnection,
                                      Q_ARG(QString, rich));
        }
        metFatal = (t == QtFatalMsg);
    }
}


/*!
    Constructs and installs an error handler window with the given \a
    parent.
*/

QErrorMessage::QErrorMessage(QWidget * parent)
    : QDialog(*new QErrorMessagePrivate, parent)
{
    Q_D(QErrorMessage);
    QGridLayout * grid = new QGridLayout(this);
    d->icon = new QLabel(this);
#ifndef QT_NO_MESSAGEBOX
    d->icon->setPixmap(QMessageBox::standardIcon(QMessageBox::Information));
    d->icon->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
#endif
    grid->addWidget(d->icon, 0, 0, Qt::AlignTop);
    d->errors = new QErrorMessageTextView(this);
    grid->addWidget(d->errors, 0, 1);
    d->again = new QCheckBox(this);
    d->again->setChecked(true);
    grid->addWidget(d->again, 1, 1, Qt::AlignTop);
    d->ok = new QPushButton(this);
#ifdef Q_OS_WINCE
    d->ok->setFixedSize(0,0);
#endif
    connect(d->ok, SIGNAL(clicked()), this, SLOT(accept()));
    d->ok->setFocus();
    grid->addWidget(d->ok, 2, 0, 1, 2, Qt::AlignCenter);
    grid->setColumnStretch(1, 42);
    grid->setRowStretch(0, 42);
    d->retranslateStrings();
}


/*!
    Destroys the error message dialog.
*/

QErrorMessage::~QErrorMessage()
{
    if (this == qtMessageHandler) {
        qtMessageHandler = 0;
        QtMsgHandler tmp = qInstallMsgHandler(0);
        // in case someone else has later stuck in another...
        if (tmp != jump)
            qInstallMsgHandler(tmp);
    }
}


/*! \reimp */

void QErrorMessage::done(int a)
{
    Q_D(QErrorMessage);
    if (!d->again->isChecked() && !d->currentMessage.isEmpty())
        d->doNotShow.insert(d->currentMessage, 0);
    d->currentMessage.clear();
    if (!d->nextPending()) {
        QDialog::done(a);
        if (this == qtMessageHandler && metFatal)
            exit(1);
    }
}


/*!
    Returns a pointer to a QErrorMessage object that outputs the
    default Qt messages. This function creates such an object, if there
    isn't one already.
*/

QErrorMessage * QErrorMessage::qtHandler()
{
    if (!qtMessageHandler) {
        qtMessageHandler = new QErrorMessage(0);
        qAddPostRoutine(deleteStaticcQErrorMessage); // clean up
        qtMessageHandler->setWindowTitle(qApp->applicationName());
        qInstallMsgHandler(jump);
    }
    return qtMessageHandler;
}


/*! \internal */

bool QErrorMessagePrivate::nextPending()
{
    while (!pending.isEmpty()) {
        QString p = pending.takeFirst();
        if (!p.isEmpty() && !doNotShow.contains(p)) {
#ifndef QT_NO_TEXTHTMLPARSER
            errors->setHtml(p);
#else
            errors->setPlainText(p);
#endif
            currentMessage = p;
            return true;
        }
    }
    return false;
}


/*!
    Shows the given \a message, and returns immediately. This function does
    nothing if the user has requested that \a message should not be shown again.

    Normally, \a message is shown at once, but if there are pending messages,
    \a message is queued for later display.
*/

void QErrorMessage::showMessage(const QString &message)
{
    Q_D(QErrorMessage);
    if (d->doNotShow.contains(message))
        return;
    d->pending.append(message);
    if (!isVisible() && d->nextPending())
        show();
}

/*!
    \reimp
*/
void QErrorMessage::changeEvent(QEvent *e)
{
    Q_D(QErrorMessage);
    if (e->type() == QEvent::LanguageChange) {
        d->retranslateStrings();
    }
    QDialog::changeEvent(e);
}

void QErrorMessagePrivate::retranslateStrings()
{
    again->setText(QErrorMessage::tr("&Show this message again"));
    ok->setText(QErrorMessage::tr("&OK"));
}

/*!
    \fn void QErrorMessage::message(const QString & message)

    Use showMessage(\a message) instead.
*/

QT_END_NAMESPACE

#endif // QT_NO_ERRORMESSAGE
