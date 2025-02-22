/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#ifndef QCOMPLETER_P_H
#define QCOMPLETER_P_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "private/qobject_p.h"

#ifndef QT_NO_COMPLETER

#include "QtGui/qtreeview.h"
#include "QtGui/qabstractproxymodel.h"
#include "qcompleter.h"
#include "QtGui/qitemdelegate.h"
#include "QtGui/qpainter.h"
#include "private/qabstractproxymodel_p.h"

QT_BEGIN_NAMESPACE

class QCompletionModel;

class QCompleterPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QCompleter)

public:
    QCompleterPrivate();
    ~QCompleterPrivate() { delete popup; }
    void init(QAbstractItemModel *model = 0);

    QPointer<QWidget> widget;
    QCompletionModel *proxy;
    QAbstractItemView *popup;
    QCompleter::CompletionMode mode;

    QString prefix;
    Qt::CaseSensitivity cs;
    int role;
    int column;
    QCompleter::ModelSorting sorting;
    bool wrap;

    bool eatFocusOut;
    QRect popupRect;

    void showPopup(const QRect&);
    void _q_complete(QModelIndex, bool = false);
    void _q_completionSelected(const QItemSelection&);
    void _q_autoResizePopup();
    void setCurrentIndex(QModelIndex, bool = true);
};

class QIndexMapper
{
public:
    QIndexMapper() : v(false), f(0), t(-1) { }
    QIndexMapper(int f, int t) : v(false), f(f), t(t) { }
    QIndexMapper(QVector<int> vec) : v(true), vector(vec), f(-1), t(-1) { }

    inline int count() const { return v ? vector.count() : t - f + 1; }
    inline int operator[] (int index) const { return v ? vector[index] : f + index; }
    inline int indexOf(int x) const { return v ? vector.indexOf(x) : ((t < f) ? -1 : x - f); }
    inline bool isValid() const { return !isEmpty(); }
    inline bool isEmpty() const { return v ? vector.isEmpty() : (t < f); }
    inline void append(int x) { Q_ASSERT(v); vector.append(x); }
    inline int first() const { return v ? vector.first() : f; }
    inline int last() const { return v ? vector.last() : t; }
    inline int from() const { Q_ASSERT(!v); return f; }
    inline int to() const { Q_ASSERT(!v); return t; }
    inline int cost() const { return vector.count()+2; }

private:
    bool v;
    QVector<int> vector;
    int f, t;
};

struct QMatchData {
    QMatchData() : exactMatchIndex(-1) { }
    QMatchData(const QIndexMapper& indices, int em, bool p) :
        indices(indices), exactMatchIndex(em), partial(p) { }
    QIndexMapper indices;
    inline bool isValid() const { return indices.isValid(); }
    int  exactMatchIndex;
    bool partial;
};

class QCompletionEngine
{
public:
    typedef QMap<QString, QMatchData> CacheItem;
    typedef QMap<QModelIndex, CacheItem> Cache;

    QCompletionEngine(QCompleterPrivate *c) : c(c), curRow(-1), cost(0) { }
    virtual ~QCompletionEngine() { }

    void filter(const QStringList &parts);

    QMatchData filterHistory();
    bool matchHint(QString, const QModelIndex&, QMatchData*);

    void saveInCache(QString, const QModelIndex&, const QMatchData&);
    bool lookupCache(QString part, const QModelIndex& parent, QMatchData *m);

    virtual void filterOnDemand(int) { }
    virtual QMatchData filter(const QString&, const QModelIndex&, int) = 0;

    int matchCount() const { return curMatch.indices.count() + historyMatch.indices.count(); }

    QMatchData curMatch, historyMatch;
    QCompleterPrivate *c;
    QStringList curParts;
    QModelIndex curParent;
    int curRow;

    Cache cache;
    int cost;
};

class QSortedModelEngine : public QCompletionEngine
{
public:
    QSortedModelEngine(QCompleterPrivate *c) : QCompletionEngine(c) { }
    QMatchData filter(const QString&, const QModelIndex&, int);
    QIndexMapper indexHint(QString, const QModelIndex&, Qt::SortOrder);
    Qt::SortOrder sortOrder(const QModelIndex&) const;
};

class QUnsortedModelEngine : public QCompletionEngine
{
public:
    QUnsortedModelEngine(QCompleterPrivate *c) : QCompletionEngine(c) { }

    void filterOnDemand(int);
    QMatchData filter(const QString&, const QModelIndex&, int);
private:
    int buildIndices(const QString& str, const QModelIndex& parent, int n,
                     const QIndexMapper& iv, QMatchData* m);
};

class QCompleterItemDelegate : public QItemDelegate
{
public:
    QCompleterItemDelegate(QAbstractItemView *view)
        : QItemDelegate(view), view(view) { }
    void paint(QPainter *p, const QStyleOptionViewItem& opt, const QModelIndex& idx) const {
        QStyleOptionViewItem optCopy = opt;
        optCopy.showDecorationSelected = true;
        if (view->currentIndex() == idx)
            optCopy.state |= QStyle::State_HasFocus;
        QItemDelegate::paint(p, optCopy, idx);
    }

private:
    QAbstractItemView *view;
};

class QCompletionModelPrivate;

class QCompletionModel : public QAbstractProxyModel
{
    Q_OBJECT

public:
    QCompletionModel(QCompleterPrivate *c, QObject *parent);
    ~QCompletionModel() { delete engine; }

    void createEngine();
    void setFiltered(bool);
    void filter(const QStringList& parts);
    int completionCount() const;
    int currentRow() const { return engine->curRow; }
    bool setCurrentRow(int row);
    QModelIndex currentIndex(bool) const;
    void resetModel();

    QModelIndex index(int row, int column, const QModelIndex & = QModelIndex()) const;
    int rowCount(const QModelIndex &index = QModelIndex()) const;
    int columnCount(const QModelIndex &index = QModelIndex()) const;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex & = QModelIndex()) const { return QModelIndex(); }
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    void setSourceModel(QAbstractItemModel *sourceModel);
    QModelIndex mapToSource(const QModelIndex& proxyIndex) const;
    QModelIndex mapFromSource(const QModelIndex& sourceIndex) const;

    QCompleterPrivate *c;
    QCompletionEngine *engine;
    bool showAll;

    Q_DECLARE_PRIVATE(QCompletionModel)

signals:
    void rowsAdded();

public Q_SLOTS:
    void invalidate();
    void rowsInserted();
    void modelDestroyed();
};

class QCompletionModelPrivate : public QAbstractProxyModelPrivate
{
    Q_DECLARE_PUBLIC(QCompletionModel)
};

QT_END_NAMESPACE

#endif // QT_NO_COMPLETER

#endif // QCOMPLETER_P_H
