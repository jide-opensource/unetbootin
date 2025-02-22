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

#include "qpixmapdatafactory_p.h"

#ifdef Q_WS_QWS
# include <QtGui/qscreen_qws.h>
#endif
#ifdef Q_WS_X11
# include <private/qpixmap_x11_p.h>
#endif
#ifdef Q_WS_WIN
# include <private/qpixmap_raster_p.h>
#endif
#ifdef Q_WS_MAC
# include <private/qpixmap_mac_p.h>
#endif

#include "private/qapplication_p.h"
#include "private/qgraphicssystem_p.h"

QT_BEGIN_NAMESPACE

#if !defined(Q_WS_QWS)

class QSimplePixmapDataFactory : public QPixmapDataFactory
{
public:
    ~QSimplePixmapDataFactory() {}
    QPixmapData* create(QPixmapData::PixelType type);
};

QPixmapData* QSimplePixmapDataFactory::create(QPixmapData::PixelType type)
{
    if (QApplicationPrivate::graphicsSystem())
        return QApplicationPrivate::graphicsSystem()->createPixmapData(type);

#if defined(Q_WS_X11)
    return new QX11PixmapData(type);
#elif defined(Q_WS_WIN)
    return new QRasterPixmapData(type);
#elif defined(Q_WS_MAC)
    return new QMacPixmapData(type);
#else
#error QSimplePixmapDataFactory::create() not implemented
#endif
}

Q_GLOBAL_STATIC(QSimplePixmapDataFactory, factory);

#endif // !defined(Q_WS_QWS)

QPixmapDataFactory::~QPixmapDataFactory()
{
}

QPixmapDataFactory* QPixmapDataFactory::instance(int screen)
{
    Q_UNUSED(screen);
#ifdef Q_WS_QWS
    return QScreen::instance()->pixmapDataFactory();
#else
    return factory();
#endif
}

QT_END_NAMESPACE
