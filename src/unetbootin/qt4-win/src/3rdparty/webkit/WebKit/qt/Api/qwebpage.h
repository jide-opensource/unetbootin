/*
    Copyright (C) 2007 Trolltech ASA
    Copyright (C) 2007 Staikos Computing Services Inc.

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

#ifndef QWEBPAGE_H
#define QWEBPAGE_H

#include "qwebsettings.h"
#include "qwebkitglobal.h"

#include <QtCore/qobject.h>
#include <QtGui/qwidget.h>

QT_BEGIN_NAMESPACE
class QNetworkProxy;
class QUndoStack;
class QUrl;
class QNetworkRequest;
class QNetworkReply;
class QNetworkAccessManager;
QT_END_NAMESPACE

class QWebFrame;
class QWebNetworkRequest;
class QWebHistory;

class QWebPagePrivate;
class QWebFrameData;
class QWebNetworkInterface;
class QWebHitTestResult;
class QWebPluginFactory;

namespace WebCore {
    class ChromeClientQt;
    class EditorClientQt;
    class FrameLoaderClientQt;
    class FrameLoadRequest;
    class InspectorClientQt;
    class ResourceHandle;
    class HitTestResult;
    class FrameView;
}

class QWEBKIT_EXPORT QWebPage : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool modified READ isModified)
    Q_PROPERTY(QString selectedText READ selectedText)
    Q_PROPERTY(QSize viewportSize READ viewportSize WRITE setViewportSize)
    Q_PROPERTY(bool forwardUnsupportedContent READ forwardUnsupportedContent WRITE setForwardUnsupportedContent)
    Q_PROPERTY(LinkDelegationPolicy linkDelegationPolicy READ linkDelegationPolicy WRITE setLinkDelegationPolicy)
    Q_PROPERTY(QPalette palette READ palette WRITE setPalette)
    Q_ENUMS(LinkDelegationPolicy NavigationType WebAction)
public:
    enum NavigationType {
        NavigationTypeLinkClicked,
        NavigationTypeFormSubmitted,
        NavigationTypeBackOrForward,
        NavigationTypeReload,
        NavigationTypeFormResubmitted,
        NavigationTypeOther
    };

    enum WebAction {
        NoWebAction = - 1,

        OpenLink,

        OpenLinkInNewWindow,
        OpenFrameInNewWindow,

        DownloadLinkToDisk,
        CopyLinkToClipboard,

        OpenImageInNewWindow,
        DownloadImageToDisk,
        CopyImageToClipboard,

        Back,
        Forward,
        Stop,
        Reload,

        Cut,
        Copy,
        Paste,

        Undo,
        Redo,
        MoveToNextChar,
        MoveToPreviousChar,
        MoveToNextWord,
        MoveToPreviousWord,
        MoveToNextLine,
        MoveToPreviousLine,
        MoveToStartOfLine,
        MoveToEndOfLine,
        MoveToStartOfBlock,
        MoveToEndOfBlock,
        MoveToStartOfDocument,
        MoveToEndOfDocument,
        SelectNextChar,
        SelectPreviousChar,
        SelectNextWord,
        SelectPreviousWord,
        SelectNextLine,
        SelectPreviousLine,
        SelectStartOfLine,
        SelectEndOfLine,
        SelectStartOfBlock,
        SelectEndOfBlock,
        SelectStartOfDocument,
        SelectEndOfDocument,
        DeleteStartOfWord,
        DeleteEndOfWord,

        SetTextDirectionDefault,
        SetTextDirectionLeftToRight,
        SetTextDirectionRightToLeft,

        ToggleBold,
        ToggleItalic,
        ToggleUnderline,

        InspectElement,

        WebActionCount
    };

    enum FindFlag {
        FindBackward = 1,
        FindCaseSensitively = 2,
        FindWrapsAroundDocument = 4
    };
    Q_DECLARE_FLAGS(FindFlags, FindFlag)

    enum LinkDelegationPolicy {
        DontDelegateLinks,
        DelegateExternalLinks,
        DelegateAllLinks
    };

    enum WebWindowType {
        WebBrowserWindow,
        WebModalDialog
    };

    explicit QWebPage(QObject *parent = 0);
    ~QWebPage();

    QWebFrame *mainFrame() const;
    QWebFrame *currentFrame() const;

    QWebHistory *history() const;
    QWebSettings *settings() const;

    void setView(QWidget *view);
    QWidget *view() const;

    bool isModified() const;
    QUndoStack *undoStack() const;

#if QT_VERSION < 0x040400 && !defined(qdoc)
    void setNetworkInterface(QWebNetworkInterface *interface);
    QWebNetworkInterface *networkInterface() const;

    // #### why is this in the page itself?
#ifndef QT_NO_NETWORKPROXY
    void setNetworkProxy(const QNetworkProxy& proxy);
    QNetworkProxy networkProxy() const;
#endif

#else
    void setNetworkAccessManager(QNetworkAccessManager *manager);
    QNetworkAccessManager *networkAccessManager() const;
#endif

    void setPluginFactory(QWebPluginFactory *factory);
    QWebPluginFactory *pluginFactory() const;

    quint64 totalBytes() const;
    quint64 bytesReceived() const;

    QString selectedText() const;

    QAction *action(WebAction action) const;
    virtual void triggerAction(WebAction action, bool checked = false);

    QSize viewportSize() const;
    void setViewportSize(const QSize &size) const;

    virtual bool event(QEvent*);
    bool focusNextPrevChild(bool next);

    QVariant inputMethodQuery(Qt::InputMethodQuery property) const;

    bool findText(const QString &subString, FindFlags options = 0);

    void setForwardUnsupportedContent(bool forward);
    bool forwardUnsupportedContent() const;

    void setLinkDelegationPolicy(LinkDelegationPolicy policy);
    LinkDelegationPolicy linkDelegationPolicy() const;

    void setPalette(const QPalette &palette);
    QPalette palette() const;

    bool swallowContextMenuEvent(QContextMenuEvent *event);
    void updatePositionDependentActions(const QPoint &pos);

    enum Extension {
    };
    class ExtensionOption
    {};
    class ExtensionReturn
    {};
    virtual bool extension(Extension extension, const ExtensionOption *option = 0, ExtensionReturn *output = 0);
    virtual bool supportsExtension(Extension extension) const;

Q_SIGNALS:
    void loadStarted();
    void loadProgress(int progress);
    void loadFinished(bool ok);

    void linkHovered(const QString &link, const QString &title, const QString &textContent);
    void statusBarMessage(const QString& text);
    void selectionChanged();
    void frameCreated(QWebFrame *frame);
    void geometryChangeRequested(const QRect& geom);
    void repaintRequested(const QRect& dirtyRect);
    void scrollRequested(int dx, int dy, const QRect& scrollViewRect);
    void windowCloseRequested();
    void printRequested(QWebFrame *frame);
    void linkClicked(const QUrl &url);

    void toolBarVisibilityChangeRequested(bool visible);
    void statusBarVisibilityChangeRequested(bool visible);
    void menuBarVisibilityChangeRequested(bool visible);

#if QT_VERSION >= 0x040400
    void unsupportedContent(QNetworkReply *reply);
    void downloadRequested(const QNetworkRequest &request);
#endif

    void microFocusChanged();

protected:
    virtual QWebPage *createWindow(WebWindowType type);
    virtual QObject *createPlugin(const QString &classid, const QUrl &url, const QStringList &paramNames, const QStringList &paramValues);

#if QT_VERSION >= 0x040400
    virtual bool acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type);
#else
    virtual bool acceptNavigationRequest(QWebFrame *frame, const QWebNetworkRequest &request, NavigationType type);
#endif
    virtual QString chooseFile(QWebFrame *originatingFrame, const QString& oldFile);
    virtual void javaScriptAlert(QWebFrame *originatingFrame, const QString& msg);
    virtual bool javaScriptConfirm(QWebFrame *originatingFrame, const QString& msg);
    virtual bool javaScriptPrompt(QWebFrame *originatingFrame, const QString& msg, const QString& defaultValue, QString* result);
    virtual void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID);

    virtual QString userAgentForUrl(const QUrl& url) const;

private:
    Q_PRIVATE_SLOT(d, void _q_onLoadProgressChanged(int))
    Q_PRIVATE_SLOT(d, void _q_webActionTriggered(bool checked))
    QWebPagePrivate *d;

    friend class QWebFrame;
    friend class QWebPagePrivate;
    friend class WebCore::ChromeClientQt;
    friend class WebCore::EditorClientQt;
    friend class WebCore::FrameLoaderClientQt;
    friend class WebCore::InspectorClientQt;
    friend class WebCore::ResourceHandle;
    friend class WebCore::FrameView;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QWebPage::FindFlags)

#endif
