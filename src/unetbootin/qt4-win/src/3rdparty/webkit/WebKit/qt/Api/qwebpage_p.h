/*
    Copyright (C) 2007 Trolltech ASA

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef QWEBPAGE_P_H
#define QWEBPAGE_P_H

#include <qbasictimer.h>
#include <qnetworkproxy.h>
#include <qpointer.h>

#include "qwebpage.h"
#include "qwebhistory.h"
#include "qwebframe.h"

#include "KURL.h"
#include "PlatformString.h"

#include <wtf/RefPtr.h>

namespace WebCore
{
    class ChromeClientQt;
    class ContextMenuClientQt;
    class ContextMenuItem;
    class ContextMenu;
    class EditorClientQt;
    class Element;
    class Node;
    class Page;
}

QT_BEGIN_NAMESPACE
class QUndoStack;
class QMenu;
QT_END_NAMESPACE

class QWebPagePrivate
{
public:
    QWebPagePrivate(QWebPage *);
    ~QWebPagePrivate();
    void createMainFrame();
    QMenu *createContextMenu(const WebCore::ContextMenu *webcoreMenu, const QList<WebCore::ContextMenuItem> *items);

    QWebFrame *frameAt(const QPoint &pos) const;

    void _q_onLoadProgressChanged(int);
    void _q_webActionTriggered(bool checked);
    void updateAction(QWebPage::WebAction action);
    void updateNavigationActions();
    void updateEditorActions();
    void updateContextMenuActions(const WebCore::ContextMenu *webcoreMenu, const QList<WebCore::ContextMenuItem> *items);

    void timerEvent(QTimerEvent*);
    
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);
    void mouseTripleClickEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void contextMenuEvent(QContextMenuEvent*);
    void wheelEvent(QWheelEvent*);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void focusInEvent(QFocusEvent*);
    void focusOutEvent(QFocusEvent*);

    void dragEnterEvent(QDragEnterEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

    void inputMethodEvent(QInputMethodEvent*);

    void shortcutOverrideEvent(QKeyEvent*);

    WebCore::ChromeClientQt *chromeClient;
    WebCore::ContextMenuClientQt *contextMenuClient;
    WebCore::EditorClientQt *editorClient;
    WebCore::Page *page;

    QPointer<QWebFrame> mainFrame;

    QWebPage *q;
    QUndoStack *undoStack;
    QWidget *view;

    bool modified;

    bool insideOpenCall;
    bool hasFocus;
    quint64 m_totalBytes;
    quint64 m_bytesReceived;
    
    QPoint tripleClick;
    QBasicTimer tripleClickTimer;

#if QT_VERSION < 0x040400
    bool acceptNavigationRequest(QWebFrame *frame, const QWebNetworkRequest &request, QWebPage::NavigationType type);

    QWebNetworkInterface *networkInterface;
#ifndef QT_NO_NETWORKPROXY
    QNetworkProxy networkProxy;
#endif

#else
    bool acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, QWebPage::NavigationType type);
    QNetworkAccessManager *networkManager;
#endif

    bool forwardUnsupportedContent;
    QWebPage::LinkDelegationPolicy linkPolicy;

    QSize viewportSize;
    QWebHistory history;
    QWebHitTestResult hitTestResult;
    QMenu *currentContextMenu;
    QWebSettings *settings;
    QPalette palette;

    QAction *actions[QWebPage::WebActionCount];

    QWebPluginFactory *pluginFactory;
};

#endif
