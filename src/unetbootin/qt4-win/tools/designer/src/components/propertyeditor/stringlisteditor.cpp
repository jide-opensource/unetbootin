/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Designer of the Qt Toolkit.
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

#include "stringlisteditor.h"
#include <iconloader_p.h>
#include <QtGui/QStringListModel>

QT_BEGIN_NAMESPACE

using namespace qdesigner_internal;

StringListEditor::StringListEditor(QWidget *parent)
    : QDialog(parent), m_model(new QStringListModel(this))
{
    setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    listView->setModel(m_model);

    connect(listView->selectionModel(),
            SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));
    connect(listView->itemDelegate(),
            SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)),
            this, SLOT(currentValueChanged()));

    QIcon upIcon = createIconSet(QString::fromUtf8("up.png"));
    QIcon downIcon = createIconSet(QString::fromUtf8("down.png"));
    QIcon minusIcon = createIconSet(QString::fromUtf8("minus.png"));
    QIcon plusIcon = createIconSet(QString::fromUtf8("plus.png"));
    upButton->setIcon(upIcon);
    downButton->setIcon(downIcon);
    newButton->setIcon(plusIcon);
    deleteButton->setIcon(minusIcon);

    updateUi();
}

StringListEditor::~StringListEditor()
{
}

QStringList StringListEditor::getStringList(QWidget *parent, const QStringList &init, int *result)
{
    StringListEditor dlg(parent);
    dlg.setStringList(init);
    int res = dlg.exec();
    if (result)
        *result = res;
    return (res == QDialog::Accepted) ? dlg.stringList() : init;
}

void StringListEditor::setStringList(const QStringList &stringList)
{
    m_model->setStringList(stringList);
    updateUi();
}

QStringList StringListEditor::stringList() const
{
    return m_model->stringList();
}

void StringListEditor::currentIndexChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    setCurrentIndex(current.row());
    updateUi();
}

void StringListEditor::currentValueChanged()
{
    setCurrentIndex(currentIndex());
    updateUi();
}

void StringListEditor::on_upButton_clicked()
{
    int from = currentIndex();
    int to = currentIndex() - 1;
    QString value = stringAt(from);
    removeString(from);
    insertString(to, value);
    setCurrentIndex(to);
    updateUi();
}

void StringListEditor::on_downButton_clicked()
{
    int from = currentIndex();
    int to = currentIndex() + 1;
    QString value = stringAt(from);
    removeString(from);
    insertString(to, value);
    setCurrentIndex(to);
    updateUi();
}

void StringListEditor::on_newButton_clicked()
{
    int to = currentIndex();
    if (to == -1)
        to = count() - 1;
    ++to;
    insertString(to, QString());
    setCurrentIndex(to);
    updateUi();
    editString(to);
}

void StringListEditor::on_deleteButton_clicked()
{
    removeString(currentIndex());
    setCurrentIndex(currentIndex());
    updateUi();
}

void StringListEditor::on_valueEdit_textEdited(const QString &text)
{
    setStringAt(currentIndex(), text);
}

void StringListEditor::updateUi()
{
    upButton->setEnabled((count() > 1) && (currentIndex() > 0));
    downButton->setEnabled((count() > 1) && (currentIndex() >= 0) && (currentIndex() < (count() - 1)));
    deleteButton->setEnabled(currentIndex() != -1);
    valueEdit->setEnabled(currentIndex() != -1);
}

int StringListEditor::currentIndex() const
{
    return listView->currentIndex().row();
}

void StringListEditor::setCurrentIndex(int index)
{
    QModelIndex modelIndex = m_model->index(index, 0);
    if (listView->currentIndex() != modelIndex)
        listView->setCurrentIndex(modelIndex);
    valueEdit->setText(stringAt(index));
}

int StringListEditor::count() const
{
    return m_model->rowCount();
}

QString StringListEditor::stringAt(int index) const
{
    return qvariant_cast<QString>(m_model->data(m_model->index(index, 0), Qt::DisplayRole));
}

void StringListEditor::setStringAt(int index, const QString &value)
{
    m_model->setData(m_model->index(index, 0), value);
}

void StringListEditor::removeString(int index)
{
    m_model->removeRows(index, 1);
}

void StringListEditor::insertString(int index, const QString &value)
{
    m_model->insertRows(index, 1);
    m_model->setData(m_model->index(index, 0), value);
}

void StringListEditor::editString(int index)
{
    listView->edit(m_model->index(index, 0));
}

QT_END_NAMESPACE
