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

#include "qwebhistory.h"
#include "qwebhistory_p.h"

#include "DeprecatedString.h"
#include "PlatformString.h"
#include "Image.h"
#include "Page.h"

#include <QSharedData>

/*!
  \class QWebHistoryItem
  \ingroup explicitly-shared
  \since 4.4
  \brief The QWebHistoryItem class represents one item in the history of a QWebPage

  Each QWebHistoryItem instance represents an entry in the history stack of a Web page,
  containing information about the page, its location, and when it was last visited.

  The following table shows the properties of the page held by the history item, and
  the functions used to access them.

  \table
  \header \o Function      \o Description
  \row    \o title()       \o The page title.
  \row    \o url()         \o The location of the page.
  \row    \o originalUrl() \o The URL used to access the page.
  \row    \o lastVisited() \o The date and time of the user's last visit to the page.
  \row    \o icon()        \o The icon associated with the page that was provided by the server.
  \endtable

  \note QWebHistoryItem objects are value based and \l{explicitly shared}.

  \sa QWebHistory, QWebPage::history(), QWebHistoryInterface
*/

/*!
  Constructs a history item from \a other.
*/
QWebHistoryItem::QWebHistoryItem(const QWebHistoryItem &other)
    : d(other.d)
{
}

/*!
  Assigns the \a other history item to this.
*/
QWebHistoryItem &QWebHistoryItem::operator=(const QWebHistoryItem &other)
{
    d = other.d;
    return *this;
}

/*!
  Destroys the history item.
*/
QWebHistoryItem::~QWebHistoryItem()
{
}

/*!
 Returns the original URL associated with the history item.

 \sa url()
*/
QUrl QWebHistoryItem::originalUrl() const
{
    return QUrl(d->item->originalURL().url());
}


/*!
 Returns the URL associated with the history item.

 \sa originalUrl(), title(), lastVisited()
*/
QUrl QWebHistoryItem::url() const
{
    return QUrl(d->item->url().url());
}


/*!
 Returns the title of the page associated with the history item.

 \sa icon(), url(), lastVisited()
*/
QString QWebHistoryItem::title() const
{
    return d->item->title();
}


/*!
 Returns the date and time that the page associated with the item was last visited.

 \sa title(), icon(), url()
*/
QDateTime QWebHistoryItem::lastVisited() const
{
    //FIXME : this will be wrong unless we correctly set lastVisitedTime ourselves
    return QDateTime::fromTime_t((uint)d->item->lastVisitedTime());
}


/*!
 Returns the icon associated with the history item.

 \sa title(), url(), lastVisited()
*/
QIcon QWebHistoryItem::icon() const
{
    return *d->item->icon()->getPixmap();
}

/*!
  \internal
*/
QWebHistoryItem::QWebHistoryItem(QWebHistoryItemPrivate *priv)
{
    d = priv;
}

/*!
  \class QWebHistory
  \since 4.4
  \brief The QWebHistory class represents the history of a QWebPage

  Each QWebPage instance contains a history of visited pages that can be accessed
  by QWebPage::history(). QWebHistory represents this history and makes it possible
  to navigate it.

  The history uses the concept of a \e{current item}, dividing the pages visited
  into those that can be visited by navigating \e back and \e forward using the
  back() and forward() functions. The current item can be obtained by calling
  currentItem(), and an arbitrary item in the history can be made the current
  item by passing it to goToItem().

  A list of items describing the pages that can be visited by going back can be
  obtained by calling the backItems() function; similarly, items describing the
  pages ahead of the current page can be obtained with the forwardItems() function.
  The total list of items is obtained with the items() function.

  Just as with containers, functions are available to examine the history in terms
  of a list. Arbitrary items in the history can be obtained with itemAt(), the total
  number of items is given by count(), and the history can be cleared with the
  clear() function.

  \sa QWebHistoryItem, QWebHistoryInterface, QWebPage
*/


QWebHistory::QWebHistory()
    : d(0)
{
}

QWebHistory::~QWebHistory()
{
    delete d;
}

/*!
  Clears the history.

  \sa count(), items()
*/
void QWebHistory::clear()
{
    RefPtr<WebCore::HistoryItem> current = d->lst->currentItem();
    int capacity = d->lst->capacity();
    d->lst->setCapacity(0);    
    d->lst->setCapacity(capacity);
    d->lst->addItem(current.get());
    d->lst->goToItem(current.get());
}

/*!
  Returns a list of all items currently in the history.

  \sa count(), clear()
*/
QList<QWebHistoryItem> QWebHistory::items() const
{
    const WebCore::HistoryItemVector &items = d->lst->entries();

    QList<QWebHistoryItem> ret;
    for (int i = 0; i < items.size(); ++i) {
        QWebHistoryItemPrivate *priv = new QWebHistoryItemPrivate(items[i].get());
        ret.append(QWebHistoryItem(priv));
    }
    return ret;
}

/*!
  Returns the list of items in the backwards history list.
  At most \a maxItems entries are returned.

  \sa forwardItems()
*/
QList<QWebHistoryItem> QWebHistory::backItems(int maxItems) const
{
    WebCore::HistoryItemVector items(maxItems);
    d->lst->backListWithLimit(maxItems, items);

    QList<QWebHistoryItem> ret;
    for (int i = 0; i < items.size(); ++i) {
        QWebHistoryItemPrivate *priv = new QWebHistoryItemPrivate(items[i].get());
        ret.append(QWebHistoryItem(priv));
    }
    return ret;
}

/*!
  Returns the list of items in the forward history list.
  At most \a maxItems entries are returned.

  \sa backItems()
*/
QList<QWebHistoryItem> QWebHistory::forwardItems(int maxItems) const
{
    WebCore::HistoryItemVector items(maxItems);
    d->lst->forwardListWithLimit(maxItems, items);

    QList<QWebHistoryItem> ret;
    for (int i = 0; i < items.size(); ++i) {
        QWebHistoryItemPrivate *priv = new QWebHistoryItemPrivate(items[i].get());
        ret.append(QWebHistoryItem(priv));
    }
    return ret;
}

/*!
  Returns true if there is an item preceding the current item in the history;
  otherwise returns false.

  \sa canGoForward()
*/
bool QWebHistory::canGoBack() const
{
    return d->lst->backListCount() > 0;
}

/*!
  Returns true if we have an item to go forward to; otherwise returns false.

  \sa canGoBack()
*/
bool QWebHistory::canGoForward() const
{
    return d->lst->forwardListCount() > 0;
}

/*!
  Set the current item to be the previous item in the history and goes to the
  corresponding page; i.e., goes back one history item.

  \sa forward(), goToItem()
*/
void QWebHistory::back()
{
    d->lst->goBack();
    WebCore::Page *page = d->lst->page();
    page->goToItem(currentItem().d->item, WebCore::FrameLoadTypeIndexedBackForward);
}

/*!
  Sets the current item to be the next item in the history and goes to the
  corresponding page; i.e., goes forward one history item.

  \sa back(), goToItem()
*/
void QWebHistory::forward()
{
    d->lst->goBack();
    WebCore::Page *page = d->lst->page();
    page->goToItem(currentItem().d->item, WebCore::FrameLoadTypeIndexedBackForward);
}

/*!
  Sets the current item to be the specified \a item in the history and goes to the page.

  \sa back(), forward()
*/
void QWebHistory::goToItem(const QWebHistoryItem &item)
{
    d->lst->goToItem(item.d->item);
    WebCore::Page *page = d->lst->page();
    page->goToItem(currentItem().d->item, WebCore::FrameLoadTypeIndexedBackForward);
}

/*!
  Returns the item before the current item in the history.
*/
QWebHistoryItem QWebHistory::backItem() const
{
    WebCore::HistoryItem *i = d->lst->backItem();
    QWebHistoryItemPrivate *priv = new QWebHistoryItemPrivate(i);
    return QWebHistoryItem(priv);
}

/*!
  Returns the current item in the history.
*/
QWebHistoryItem QWebHistory::currentItem() const
{
    WebCore::HistoryItem *i = d->lst->currentItem();
    QWebHistoryItemPrivate *priv = new QWebHistoryItemPrivate(i);
    return QWebHistoryItem(priv);
}

/*!
  Returns the item after the current item in the history.
*/
QWebHistoryItem QWebHistory::forwardItem() const
{
    WebCore::HistoryItem *i = d->lst->forwardItem();
    QWebHistoryItemPrivate *priv = new QWebHistoryItemPrivate(i);
    return QWebHistoryItem(priv);
}

/*!
  Returns the item at index \a i in the history.
*/
QWebHistoryItem QWebHistory::itemAt(int i) const
{
    WebCore::HistoryItem *item = d->lst->itemAtIndex(i);

    QWebHistoryItemPrivate *priv = new QWebHistoryItemPrivate(item);
    return QWebHistoryItem(priv);
}

/*!
    Returns the total number of items in the history.
*/
int QWebHistory::count() const
{
    return d->lst->entries().size();
}

