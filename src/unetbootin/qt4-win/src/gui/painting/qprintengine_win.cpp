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

#ifndef QT_NO_PRINTER

#include "qprinter_p.h"
#include "qprintengine_win_p.h"

#include <limits.h>

#include <private/qfont_p.h>
#include <private/qfontengine_p.h>
#include <private/qpainter_p.h>

#include <qbitmap.h>
#include <qdebug.h>
#include <qvector.h>
#include <qpicture.h>
#include <private/qpicture_p.h>

QT_BEGIN_NAMESPACE

// #define QT_DEBUG_DRAW

static const struct {
    int winSizeName;
    QPrinter::PaperSize qtSizeName;
} dmMapping[] = {
    { DMPAPER_LETTER,             QPrinter::Letter },
    { DMPAPER_LETTERSMALL,        QPrinter::Letter },
    { DMPAPER_TABLOID,            QPrinter::Tabloid },
    { DMPAPER_LEDGER,             QPrinter::Ledger },
    { DMPAPER_LEGAL,              QPrinter::Legal },
    { DMPAPER_EXECUTIVE,          QPrinter::Executive },
    { DMPAPER_A3,                 QPrinter::A3 },
    { DMPAPER_A4,                 QPrinter::A4 },
    { DMPAPER_A4SMALL,            QPrinter::A4 },
    { DMPAPER_A5,                 QPrinter::A5 },
    { DMPAPER_B4,                 QPrinter::B4 },
    { DMPAPER_B5,                 QPrinter::B5 },
    { DMPAPER_FOLIO,              QPrinter::Folio },
    { DMPAPER_ENV_10,             QPrinter::Comm10E },
    { DMPAPER_ENV_DL,             QPrinter::DLE },
    { DMPAPER_ENV_C3,             QPrinter::C5E },
    { DMPAPER_LETTER_EXTRA,       QPrinter::Letter },
    { DMPAPER_LEGAL_EXTRA,        QPrinter::Legal },
    { DMPAPER_TABLOID_EXTRA,      QPrinter::Tabloid },
    { DMPAPER_A4_EXTRA,           QPrinter::A4},
    { DMPAPER_LETTER_TRANSVERSE,  QPrinter::Letter},
    { DMPAPER_A4_TRANSVERSE,      QPrinter::A4},
    { DMPAPER_LETTER_EXTRA_TRANSVERSE, QPrinter::Letter },
    { DMPAPER_A_PLUS,             QPrinter::A4 },
    { DMPAPER_B_PLUS,             QPrinter::A3 },
    { DMPAPER_LETTER_PLUS,        QPrinter::Letter },
    { DMPAPER_A4_PLUS,            QPrinter::A4 },
    { DMPAPER_A5_TRANSVERSE,      QPrinter::A5 },
    { DMPAPER_B5_TRANSVERSE,      QPrinter::B5 },
    { DMPAPER_A3_EXTRA,           QPrinter::A3 },
    { DMPAPER_A5_EXTRA,           QPrinter::A5 },
    { DMPAPER_B5_EXTRA,           QPrinter::B5 },
    { DMPAPER_A2,                 QPrinter::A2 },
    { DMPAPER_A3_TRANSVERSE,      QPrinter::A3 },
    { DMPAPER_A3_EXTRA_TRANSVERSE,QPrinter::A3 },
    { 0, QPrinter::Custom }
};

QPrinter::PaperSize mapDevmodePaperSize(int s)
{
    int i = 0;
    while ((dmMapping[i].winSizeName > 0) && (dmMapping[i].winSizeName != s))
        i++;
    return dmMapping[i].qtSizeName;
}

static int mapPaperSizeDevmode(QPrinter::PaperSize s)
{
    int i = 0;
 while ((dmMapping[i].winSizeName > 0) && (dmMapping[i].qtSizeName != s))
        i++;
    return dmMapping[i].winSizeName;
}

static const struct {
    int winSourceName;
    QPrinter::PaperSource qtSourceName;
}  sources[] = {
    { DMBIN_ONLYONE,        QPrinter::OnlyOne },
    { DMBIN_LOWER,          QPrinter::Lower },
    { DMBIN_MIDDLE,         QPrinter::Middle },
    { DMBIN_MANUAL,         QPrinter::Manual },
    { DMBIN_ENVELOPE,       QPrinter::Envelope },
    { DMBIN_ENVMANUAL,      QPrinter::EnvelopeManual },
    { DMBIN_AUTO,           QPrinter::Auto },
    { DMBIN_TRACTOR,        QPrinter::Tractor },
    { DMBIN_SMALLFMT,       QPrinter::SmallFormat },
    { DMBIN_LARGEFMT,       QPrinter::LargeFormat },
    { DMBIN_LARGECAPACITY,  QPrinter::LargeCapacity },
    { DMBIN_CASSETTE,       QPrinter::Cassette },
    { DMBIN_FORMSOURCE,     QPrinter::FormSource },
    { 0, (QPrinter::PaperSource) -1 }
};

static QPrinter::PaperSource mapDevmodePaperSource(int s)
{
    int i = 0;
    while ((sources[i].winSourceName > 0) && (sources[i].winSourceName != s))
        i++;
    return sources[i].winSourceName ? sources[i].qtSourceName : (QPrinter::PaperSource) s;
}

static int mapPaperSourceDevmode(QPrinter::PaperSource s)
{
    int i = 0;
    while ((sources[i].qtSourceName >= 0) && (sources[i].qtSourceName != s))
        i++;
    return sources[i].winSourceName ? sources[i].winSourceName : s;
}

QAlphaPaintEngine::QAlphaPaintEngine(QAlphaPaintEnginePrivate &data, PaintEngineFeatures devcaps)
    : QPaintEngine(data, devcaps)
{

}

QAlphaPaintEngine::~QAlphaPaintEngine()
{

}

bool QAlphaPaintEngine::begin(QPaintDevice *pdev)
{
    Q_D(QAlphaPaintEngine);

    d->m_continueCall = true;
    if (d->m_pass != 0) {
        return true;
    }

    d->m_savedcaps = gccaps;
    d->m_pdev = pdev;

    d->m_alphaPen = false;
    d->m_alphaBrush = false;
    d->m_alphaOpacity = false;
    d->m_hasalpha = false;
    d->m_advancedPen = false;
    d->m_advancedBrush = false;
    d->m_complexTransform = false;

    // clear alpha region
    d->m_alphargn = QRegion();
    d->m_cliprgn = QRegion();
    d->m_pen = QPen();
    d->m_transform = QTransform();

    flushAndInit();

    return true;
}

extern int qt_defaultDpi();

bool QAlphaPaintEngine::end()
{
    Q_D(QAlphaPaintEngine);

    d->m_continueCall = true;
    if (d->m_pass != 0) {
        return true;
    }

    flushAndInit(false);
    return true;
}

void QAlphaPaintEngine::updateState(const QPaintEngineState &state)
{
    Q_D(QAlphaPaintEngine);

    DirtyFlags flags = state.state();
    if (flags & QPaintEngine::DirtyTransform) {
        d->m_transform = state.transform();
        d->m_complexTransform = (d->m_transform.type() > QTransform::TxScale);
    }
    if (flags & QPaintEngine::DirtyPen) {
        d->m_pen = state.pen();
        if (d->m_pen.style() == Qt::NoPen) {
            d->m_advancedPen = false;
            d->m_alphaPen = false;
        } else {
            d->m_advancedPen = (d->m_pen.brush().style() != Qt::SolidPattern);
            d->m_alphaPen = !d->m_pen.brush().isOpaque();
        }
    }

    if (d->m_pass != 0) {
        d->m_continueCall = true;
        return;
    }
    d->m_continueCall = false;

    if (flags & QPaintEngine::DirtyOpacity) {
        d->m_alphaOpacity = (state.opacity() != 1.0f);
    }

    if (flags & QPaintEngine::DirtyBrush) {
        if (state.brush().style() == Qt::NoBrush) {
            d->m_advancedBrush = false;
            d->m_alphaBrush = false;
        } else {
            d->m_advancedBrush = (state.brush().style() != Qt::SolidPattern);
            d->m_alphaBrush = !state.brush().isOpaque();
        }
    }


    d->m_hasalpha = d->m_alphaOpacity || d->m_alphaBrush || d->m_alphaPen;

    if (d->m_picengine)
        d->m_picengine->updateState(state);
}

void QAlphaPaintEngine::drawPath(const QPainterPath &path)
{
    Q_D(QAlphaPaintEngine);

    QRectF tr = d->addPenWidth(path);

    if (d->m_pass == 0) {
        d->m_continueCall = false;
        if (d->m_hasalpha || d->m_advancedPen || d->m_advancedBrush) {
            d->addAlphaRect(tr);
        }
        if (d->m_picengine)
            d->m_picengine->drawPath(path);
    } else {
        d->m_continueCall = !d->fullyContained(tr);
    }
}

void QAlphaPaintEngine::drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode)
{
    Q_D(QAlphaPaintEngine);

    QPolygonF poly;
    for (int i=0; i<pointCount; ++i)
        poly.append(points[i]);

    QPainterPath path;
    path.addPolygon(poly);
    QRectF tr = d->addPenWidth(path);

    if (d->m_pass == 0) {
        d->m_continueCall = false;
        if (d->m_hasalpha || d->m_advancedPen || d->m_advancedBrush) {
            d->addAlphaRect(tr);
        }

        if (d->m_picengine)
            d->m_picengine->drawPolygon(points, pointCount, mode);
    } else {
        d->m_continueCall = !d->fullyContained(tr);
    }
}

void QAlphaPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
    Q_D(QAlphaPaintEngine);

    QRectF tr = d->m_transform.mapRect(r);
    if (d->m_pass == 0) {
        d->m_continueCall = false;
        if (pm.hasAlpha() || d->m_alphaOpacity || d->m_complexTransform || pm.isQBitmap()) {
            d->addAlphaRect(tr);
        }

        if (d->m_picengine)
            d->m_picengine->drawPixmap(r, pm, sr);

    } else {
        d->m_continueCall = !d->fullyContained(tr);
    }
}

void QAlphaPaintEngine::drawTextItem(const QPointF &p, const QTextItem &textItem)
{
    Q_D(QAlphaPaintEngine);

    QRectF tr(p.x(), p.y() - textItem.ascent(), textItem.width() + 5, textItem.ascent() + textItem.descent() + 5);
    tr = d->m_transform.mapRect(tr);

    if (d->m_pass == 0) {
        d->m_continueCall = false;
        if (d->m_alphaPen || d->m_alphaOpacity || d->m_advancedPen) {
            d->addAlphaRect(tr);
        }
        if (d->m_picengine) {
            d->m_picengine->drawTextItem(p, textItem);
        }
    } else {
        d->m_continueCall = !d->fullyContained(tr);
    }
}

void QAlphaPaintEngine::drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s)
{
    Q_D(QAlphaPaintEngine);

    QRectF brect = d->m_transform.mapRect(r);

    if (d->m_pass == 0) {
        d->m_continueCall = false;
        if (pixmap.hasAlpha() || d->m_alphaOpacity || d->m_complexTransform || pixmap.isQBitmap()) {
            d->addAlphaRect(brect);
        }
        if (d->m_picengine)
            d->m_picengine->drawTiledPixmap(r, pixmap, s);
    } else {
        d->m_continueCall = !d->fullyContained(brect);
    }
}

QRegion QAlphaPaintEngine::alphaClipping() const
{
    Q_D(const QAlphaPaintEngine);
    return d->m_cliprgn;
}

bool QAlphaPaintEngine::continueCall() const
{
    Q_D(const QAlphaPaintEngine);
    return d->m_continueCall;
}

void QAlphaPaintEngine::flushAndInit(bool init)
{
    Q_D(QAlphaPaintEngine);
    Q_ASSERT(d->m_pass == 0);

    if (d->m_pic) {
        d->m_picpainter->end();

        // set clip region
        d->m_alphargn = d->m_alphargn.intersected(QRect(0, 0, d->m_pdev->width(), d->m_pdev->height()));
        d->m_cliprgn = d->m_alphargn;

        // now replay the QPicture
        ++d->m_pass; // we are now doing pass #2

        // reset states
        gccaps = d->m_savedcaps;

        painter()->save();
        d->resetState(painter());

        // make sure the output from QPicture is unscaled
        QTransform mtx;
        mtx.scale(1.0f / (qreal(d->m_pdev->logicalDpiX()) / qreal(qt_defaultDpi())),
                  1.0f / (qreal(d->m_pdev->logicalDpiY()) / qreal(qt_defaultDpi())));
        painter()->setTransform(mtx);
        painter()->drawPicture(0, 0, *d->m_pic);
        d->m_cliprgn = QRegion();
        d->resetState(painter());

        QVector<QRect> rects = d->m_alphargn.rects();
        if (rects.count() > 10) {
            d->drawAlphaImage(d->m_alphargn.boundingRect());
        } else {
            for (int i=0; i<rects.count(); ++i)
                d->drawAlphaImage(rects.at(i));
        }

        d->m_alphargn = QRegion();

        painter()->restore();

        --d->m_pass; // pass #2 finished

        cleanUp();
    }

    if (init) {
        gccaps = PaintEngineFeatures(AllFeatures & ~QPaintEngine::ObjectBoundingModeGradients);

        d->m_pic = new QPicture();
        d->m_pic->d_ptr->in_memory_only = true;
        d->m_picpainter = new QPainter(d->m_pic);
        d->m_picengine = d->m_picpainter->paintEngine();

        // When newPage() is called and the m_picpainter is recreated
        // we have to copy the current state of the original printer
        // painter back to the m_picpainter
        d->m_picpainter->setPen(painter()->pen());
        d->m_picpainter->setBrush(painter()->brush());
        d->m_picpainter->setBrushOrigin(painter()->brushOrigin());
        d->m_picpainter->setFont(painter()->font());
        d->m_picpainter->setOpacity(painter()->opacity());
        d->m_picpainter->setTransform(painter()->combinedTransform());
        d->m_picengine->syncState();
    }
}

void QAlphaPaintEngine::cleanUp()
{
    Q_D(QAlphaPaintEngine);

    delete d->m_picpainter;
    delete d->m_pic;

    d->m_picpainter = 0;
    d->m_pic = 0;
    d->m_picengine = 0;
}

QAlphaPaintEnginePrivate::QAlphaPaintEnginePrivate()
    :   m_pass(0),
        m_pic(0),
        m_picengine(0),
        m_picpainter(0),
        m_hasalpha(false),
        m_alphaPen(false),
        m_alphaBrush(false),
        m_alphaOpacity(false),
        m_advancedPen(false),
        m_advancedBrush(false),
        m_complexTransform(false)
{

}

QAlphaPaintEnginePrivate::~QAlphaPaintEnginePrivate()
{
    delete m_picpainter;
    delete m_pic;
}

QRectF QAlphaPaintEnginePrivate::addPenWidth(const QPainterPath &path)
{
    QPainterPath tmp = path;

    if (m_pen.style() == Qt::NoPen)
        return (path.controlPointRect() * m_transform).boundingRect();
    if (m_pen.isCosmetic())
        tmp = path * m_transform;

    QPainterPathStroker stroker;
    if (m_pen.widthF() == 0.0f)
        stroker.setWidth(1.0);
    else
        stroker.setWidth(m_pen.widthF());
    stroker.setJoinStyle(m_pen.joinStyle());
    stroker.setCapStyle(m_pen.capStyle());
    tmp = stroker.createStroke(tmp);
    if (m_pen.isCosmetic())
        return tmp.controlPointRect();

    return (tmp.controlPointRect() * m_transform).boundingRect();
}

QRect QAlphaPaintEnginePrivate::toRect(const QRectF &rect) const
{
    QRect r;
    r.setLeft(int(rect.left()));
    r.setTop(int(rect.top()));
    r.setRight(int(rect.right() + 1));
    r.setBottom(int(rect.bottom() + 1));
    return r;
}

void QAlphaPaintEnginePrivate::addAlphaRect(const QRectF &rect)
{
    m_alphargn |= toRect(rect);
}

void QAlphaPaintEnginePrivate::drawAlphaImage(const QRectF &rect)
{
    Q_Q(QAlphaPaintEngine);

    qreal dpiX = qMax(m_pdev->logicalDpiX(), 300);
    qreal dpiY = qMax(m_pdev->logicalDpiY(), 300);
    qreal xscale = (dpiX / m_pdev->logicalDpiX());
    qreal yscale = (dpiY / m_pdev->logicalDpiY());

    QTransform picscale;
    picscale.scale(xscale, yscale);

    const int tileSize = 2048;
    QSize size((int(rect.width() * xscale)), int(rect.height() * yscale));
    int divw = (size.width() / tileSize);
    int divh = (size.height() / tileSize);
    divw += 1;
    divh += 1;

    int incx = int(rect.width() / divw);
    int incy = int(rect.height() / divh);

    for (int y=0; y<divh; ++y) {
        int ypos = int((incy * y) + rect.y());
        int height = int((y == (divh - 1)) ? (rect.height() - (incy * y)) : incy) + 1;

        for (int x=0; x<divw; ++x) {
            int xpos = int((incx * x) + rect.x());
            int width = int((x == (divw - 1)) ? (rect.width() - (incx * x)) : incx) + 1;

            QSize imgsize(int(width * xscale), int(height * yscale));
            QImage img(imgsize, QImage::Format_ARGB32_Premultiplied);
            img.fill(0xffffffff);

            QPainter imgpainter(&img);
            imgpainter.setTransform(picscale);
            QPointF picpos(qreal(-xpos), qreal(-ypos));
            imgpainter.drawPicture(picpos, *m_pic);
            imgpainter.end();

            q->painter()->setTransform(QTransform());
            QRect r(xpos, ypos, width, height);
            q->painter()->drawImage(r, img);
        }
    }
}

bool QAlphaPaintEnginePrivate::fullyContained(const QRectF &rect) const
{
    QRegion r(toRect(rect));
    return (m_cliprgn.intersected(r) == r);
}

void QAlphaPaintEnginePrivate::resetState(QPainter *p)
{
    p->setPen(QPen());
    p->setBrush(QBrush());
    p->setBrushOrigin(0,0);
    p->setBackground(QBrush());
    p->setFont(QFont());
    p->setTransform(QTransform());
    // The view transform is already recorded and included in the
    // picture we're about to replay. If we don't turn if off,
    // the view matrix will be applied twice.
    p->setViewTransformEnabled(false);
    p->setClipRegion(QRegion(), Qt::NoClip);
    p->setClipPath(QPainterPath(), Qt::NoClip);
    p->setClipping(false);
    p->setOpacity(1.0f);
}

QWin32PrintEngine::QWin32PrintEngine(QPrinter::PrinterMode mode)
    : QAlphaPaintEngine(*(new QWin32PrintEnginePrivate),
                   PaintEngineFeatures(PrimitiveTransform
                                       | PixmapTransform
                                       | PerspectiveTransform
                                       | PainterPaths
                                       | Antialiasing
                                       | PaintOutsidePaintEvent))
{
    Q_D(QWin32PrintEngine);
    d->docName = QLatin1String("document1");
    d->mode = mode;
    d->queryDefault();
    d->initialize();
}

bool QWin32PrintEngine::begin(QPaintDevice *pdev)
{
    Q_D(QWin32PrintEngine);

    QAlphaPaintEngine::begin(pdev);
    if (!continueCall())
        return true;

    if (d->reinit) {
       d->resetDC();
       d->reinit = false;
    }

    // ### set default colors and stuff...

    bool ok = d->state == QPrinter::Idle;

    if (!d->hdc)
        return false;

    // Assign the FILE: to get the query...
    if (d->printToFile && d->fileName.isEmpty())
        d->fileName = d->port;

    QT_WA({
    d->devModeW()->dmCopies = d->num_copies;
        DOCINFO di;
        memset(&di, 0, sizeof(DOCINFO));
        di.cbSize = sizeof(DOCINFO);
        di.lpszDocName = reinterpret_cast<const wchar_t *>(d->docName.utf16());
        if (d->printToFile && !d->fileName.isEmpty())
            di.lpszOutput = reinterpret_cast<const wchar_t *>(d->fileName.utf16());
        if (ok && StartDoc(d->hdc, &di) == SP_ERROR) {
            qErrnoWarning("QWin32PrintEngine::begin: StartDoc failed");
            ok = false;
        }
    } , {
    d->devModeA()->dmCopies = d->num_copies;
        DOCINFOA di;
        memset(&di, 0, sizeof(DOCINFOA));
        di.cbSize = sizeof(DOCINFOA);
        QByteArray docNameA = d->docName.toLocal8Bit();
        di.lpszDocName = docNameA.data();
        QByteArray outfileA = d->fileName.toLocal8Bit();
        if (d->printToFile && !d->fileName.isEmpty())
            di.lpszOutput = outfileA;
        if (ok && StartDocA(d->hdc, &di) == SP_ERROR) {
            qErrnoWarning("QWin32PrintEngine::begin: StartDoc failed");
            ok = false;
        }
    });

    if (StartPage(d->hdc) <= 0) {
        qErrnoWarning("QWin32PrintEngine::begin: StartPage failed");
        ok = false;
    }

    if (!ok) {
        d->state = QPrinter::Idle;
    } else {
        d->state = QPrinter::Active;
    }

    d->matrix = QTransform();
    d->has_pen = true;
    d->pen = QColor(Qt::black);
    d->has_brush = false;

    d->complex_xform = false;

    updateMatrix(d->matrix);

    if (!ok)
        cleanUp();

    return ok;
}

bool QWin32PrintEngine::end()
{
    Q_D(QWin32PrintEngine);

    if (d->hdc) {
        if (d->state == QPrinter::Aborted) {
            cleanUp();
            AbortDoc(d->hdc);
            return true;
        }
    }

    QAlphaPaintEngine::end();
    if (!continueCall())
        return true;

    if (d->hdc) {
        EndPage(d->hdc);                 // end; printing done
        EndDoc(d->hdc);
    }

    d->state = QPrinter::Idle;
    return true;
}

bool QWin32PrintEngine::newPage()
{
    Q_D(QWin32PrintEngine);
    Q_ASSERT(isActive());

    Q_ASSERT(d->hdc);

    flushAndInit();

    bool transparent = GetBkMode(d->hdc) == TRANSPARENT;

    if (!EndPage(d->hdc)) {
        qErrnoWarning("QWin32PrintEngine::newPage: EndPage failed");
        return false;
    }

    if (d->reinit) {
        if (!d->resetDC()) {
            qErrnoWarning("QWin32PrintEngine::newPage: ResetDC failed");
            return false;
        }
        d->reinit = false;
    }

    if (!StartPage(d->hdc)) {
        qErrnoWarning("Win32PrintEngine::newPage: StartPage failed");
        return false;
    }

    SetTextAlign(d->hdc, TA_BASELINE);
    if (transparent)
        SetBkMode(d->hdc, TRANSPARENT);

    // ###
    return true;

    bool success = false;
    if (d->hdc && d->state == QPrinter::Active) {
//         bool restorePainter = false;
//         if ((qWinVersion()& Qt::WV_DOS_based) && painter && painter->isActive()) {
//             painter->save();               // EndPage/StartPage ruins the DC
//             restorePainter = true;
//         }
        if (EndPage(d->hdc) != SP_ERROR) {
            // reinitialize the DC before StartPage if needed,
            // because resetdc is disabled between calls to the StartPage and EndPage functions
            // (see StartPage documentation in the Platform SDK:Windows GDI)
//          state = PST_ACTIVEDOC;
//          reinit();
//          state = PST_ACTIVE;
            // start the new page now
            if (d->reinit) {
                if (!d->resetDC())
                    qErrnoWarning("QWin32PrintEngine::newPage(), ResetDC failed (2)");
                d->reinit = false;
            }
            success = (StartPage(d->hdc) != SP_ERROR);
        }
        if (!success)
            d->state = QPrinter::Aborted;

//         if (qWinVersion() & Qt::WV_DOS_based)
//         if (restorePainter) {
//             painter->restore();
//      }

        if (!success)
            return false;
    }
    return true;
}

bool QWin32PrintEngine::abort()
{
    // do nothing loop.
    return false;
}

extern void qt_draw_text_item(const QPointF &pos, const QTextItemInt &ti, HDC hdc,
                              bool convertToText, const QTransform &matrix, const QPointF &topLeft);

void QWin32PrintEngine::drawTextItem(const QPointF &p, const QTextItem &textItem)
{
    Q_D(const QWin32PrintEngine);

    QAlphaPaintEngine::drawTextItem(p, textItem);
    if (!continueCall())
        return;

    const QTextItemInt &ti = static_cast<const QTextItemInt &>(textItem);
    QRgb brushColor = state->pen().brush().color().rgb();
    bool fallBack = state->pen().brush().style() != Qt::SolidPattern
                    || qAlpha(brushColor) != 0xff
                    || QT_WA_INLINE(false, d->txop >= QTransform::TxScale)
                    || ti.fontEngine->type() != QFontEngine::Win;


    if (!fallBack) {
        QFontEngineWin *fe = static_cast<QFontEngineWin *>(ti.fontEngine);

        // Try selecting the font to see if we get a substitution font
        SelectObject(d->hdc, fe->hfont);

        if (GetDeviceCaps(d->hdc, TECHNOLOGY) != DT_CHARSTREAM) {
            QT_WA({
                TCHAR n[64];
                GetTextFaceW(d->hdc, 64, n);
                fallBack = QString::fromUtf16((ushort *)n)
                        != QString::fromUtf16((ushort *)fe->logfont.lfFaceName);
            } , {
                char an[64];
                GetTextFaceA(d->hdc, 64, an);
                fallBack = QString::fromLocal8Bit(an)
                        != QString::fromLocal8Bit(((LOGFONTA*)(&fe->logfont))->lfFaceName);
            });
        }
    }


    if (fallBack) {
        QPaintEngine::drawTextItem(p, textItem);
        return ;
    }

    // We only want to convert the glyphs to text if the entire string is latin1
    bool latin1String = true;
    for (int i=0;  i < ti.num_chars; ++i) {
        if (ti.chars[i].unicode() >= 0x100) {
            latin1String = false;
            break;
        }
    }

    COLORREF cf = RGB(qRed(brushColor), qGreen(brushColor), qBlue(brushColor));
    SelectObject(d->hdc, CreateSolidBrush(cf));
    SelectObject(d->hdc, CreatePen(PS_SOLID, 1, cf));
    SetTextColor(d->hdc, cf);

    qt_draw_text_item(p, ti, d->hdc, latin1String, d->matrix, d->devPaperRect.topLeft());
    DeleteObject(SelectObject(d->hdc,GetStockObject(HOLLOW_BRUSH)));
    DeleteObject(SelectObject(d->hdc,GetStockObject(BLACK_PEN)));
}

static inline qreal mmToInches(double mm)
{
    return mm*0.039370147;
}

static inline qreal inchesToMM(double in)
{
    return in/0.039370147;
}

int QWin32PrintEngine::metric(QPaintDevice::PaintDeviceMetric m) const
{
    Q_D(const QWin32PrintEngine);

    if (!d->hdc)
        return 0;

    int val;
    int res = d->resolution;

    switch (m) {
    case QPaintDevice::PdmWidth:
        if (d->has_custom_paper_size) {
            val =  qRound(d->paper_size.width());
        } else {
            int logPixelsX = GetDeviceCaps(d->hdc, LOGPIXELSX);
            if (logPixelsX == 0) {
                qWarning("QWin32PrintEngine::metric: GetDeviceCaps() failed, "
                        "might be a driver problem");
                logPixelsX = 600; // Reasonable default
            }
            val = res
                  * GetDeviceCaps(d->hdc, d->fullPage ? PHYSICALWIDTH : HORZRES)
                  / logPixelsX;
        }
        if (d->pageMarginsSet)
            val -= int(mmToInches(d->previousDialogMargins.left() +
                                  d->previousDialogMargins.width()));
        break;
    case QPaintDevice::PdmHeight:
        if (d->has_custom_paper_size) {
            val = qRound(d->paper_size.height());
        } else {
            int logPixelsY = GetDeviceCaps(d->hdc, LOGPIXELSY);
            if (logPixelsY == 0) {
                qWarning("QWin32PrintEngine::metric: GetDeviceCaps() failed, "
                        "might be a driver problem");
                logPixelsY = 600; // Reasonable default
            }
            val = res
                  * GetDeviceCaps(d->hdc, d->fullPage ? PHYSICALHEIGHT : VERTRES)
                  / logPixelsY;
        }
        if (d->pageMarginsSet)
            val -= int(mmToInches(d->previousDialogMargins.top() +
                                  d->previousDialogMargins.height()));
        break;
    case QPaintDevice::PdmDpiX:
        val = res;
        break;
    case QPaintDevice::PdmDpiY:
        val = res;
        break;
    case QPaintDevice::PdmPhysicalDpiX:
        val = GetDeviceCaps(d->hdc, LOGPIXELSX);
        break;
    case QPaintDevice::PdmPhysicalDpiY:
        val = GetDeviceCaps(d->hdc, LOGPIXELSY);
        break;
    case QPaintDevice::PdmWidthMM:
        if (d->has_custom_paper_size) {
            val = qRound(d->paper_size.width()*25.4/72);
        } else {
            if (!d->fullPage) {
                val = GetDeviceCaps(d->hdc, HORZSIZE);
            } else {
                float wi = 25.4 * GetDeviceCaps(d->hdc, PHYSICALWIDTH);
                int logPixelsX = GetDeviceCaps(d->hdc,  LOGPIXELSX);
                if (logPixelsX == 0) {
                    qWarning("QWin32PrintEngine::metric: GetDeviceCaps() failed, "
                            "might be a driver problem");
                    logPixelsX = 600; // Reasonable default
                }
                val = qRound(wi / logPixelsX);
            }
        }
        if (d->pageMarginsSet)
            val -= d->previousDialogMargins.left() +
                   d->previousDialogMargins.width();
        break;
    case QPaintDevice::PdmHeightMM:
        if (d->has_custom_paper_size) {
            val = qRound(d->paper_size.height()*25.4/72);
        } else {
            if (!d->fullPage) {
                val = GetDeviceCaps(d->hdc, VERTSIZE);
            } else {
                float hi = 25.4 * GetDeviceCaps(d->hdc, PHYSICALHEIGHT);
                int logPixelsY = GetDeviceCaps(d->hdc,  LOGPIXELSY);
                if (logPixelsY == 0) {
                    qWarning("QWin32PrintEngine::metric: GetDeviceCaps() failed, "
                            "might be a driver problem");
                    logPixelsY = 600; // Reasonable default
                }
                val = qRound(hi / logPixelsY);
            }
        }
        if (d->pageMarginsSet)
            val -= d->previousDialogMargins.top() +
                   d->previousDialogMargins.height();
        break;
    case QPaintDevice::PdmNumColors:
        {
            int bpp = GetDeviceCaps(d->hdc, BITSPIXEL);
            if(bpp==32)
                val = INT_MAX;
            else if(bpp<=8)
                val = GetDeviceCaps(d->hdc, NUMCOLORS);
            else
                val = 1 << (bpp * GetDeviceCaps(d->hdc, PLANES));
        }
        break;
    case QPaintDevice::PdmDepth:
        val = GetDeviceCaps(d->hdc, PLANES);
        break;
    default:
        qWarning("QPrinter::metric: Invalid metric command");
        return 0;
    }
    return val;
}

void QWin32PrintEngine::updateState(const QPaintEngineState &state)
{
    Q_D(QWin32PrintEngine);

    QAlphaPaintEngine::updateState(state);
    if (!continueCall())
        return;

    if (state.state() & DirtyTransform) {
        updateMatrix(state.transform());
    }

    if (state.state() & DirtyPen) {
        d->pen = state.pen();
        d->has_pen = d->pen.style() != Qt::NoPen && d->pen.isSolid();
    }

    if (state.state() & DirtyBrush) {
        QBrush brush = state.brush();
        d->has_brush = brush.style() == Qt::SolidPattern;
        d->brush_color = brush.color();
    }

    if (state.state() & DirtyClipEnabled) {
        if (state.isClipEnabled())
            updateClipPath(painter()->clipPath(), Qt::ReplaceClip);
        else
            updateClipPath(QPainterPath(), Qt::NoClip);
    }

    if (state.state() & DirtyClipPath) {
        updateClipPath(state.clipPath(), state.clipOperation());
    }

    if (state.state() & DirtyClipRegion) {
        QPainterPath clipPath;
        QRegion clipRegion = state.clipRegion();
        clipPath.addRegion(clipRegion);
        updateClipPath(clipPath, state.clipOperation());
    }
}

void QWin32PrintEngine::updateClipPath(const QPainterPath &clipPath, Qt::ClipOperation op)
{
    Q_D(QWin32PrintEngine);

    bool doclip = true;
    if (op == Qt::NoClip) {
        SelectClipRgn(d->hdc, 0);
        doclip = false;
    }

    if (doclip) {
        QPainterPath xformed = clipPath * d->matrix;

        if (xformed.isEmpty()) {
            QRegion empty(-0x1000000, -0x1000000, 1, 1);
            SelectClipRgn(d->hdc, empty.handle());
        } else {
            d->composeGdiPath(xformed);
            const int ops[] = {
                -1,         // Qt::NoClip, covered above
                RGN_COPY,   // Qt::ReplaceClip
                RGN_AND,    // Qt::IntersectClip
                RGN_OR      // Qt::UniteClip
            };
            Q_ASSERT(op > 0 && unsigned(op) <= sizeof(ops) / sizeof(int));
            SelectClipPath(d->hdc, ops[op]);
        }
    }

    QPainterPath aclip;
    aclip.addRegion(alphaClipping());
    if (!aclip.isEmpty()) {
        QTransform tx(d->stretch_x, 0, 0, d->stretch_y, d->origin_x, d->origin_y);
        d->composeGdiPath(tx.map(aclip));
        SelectClipPath(d->hdc, RGN_DIFF);
    }
}

void QWin32PrintEngine::updateMatrix(const QTransform &m)
{
    Q_D(QWin32PrintEngine);

    QTransform stretch(d->stretch_x, 0, 0, d->stretch_y, d->origin_x, d->origin_y);
    d->painterMatrix = m;
    d->matrix = d->painterMatrix * stretch;
    d->txop = d->matrix.type();
    d->complex_xform = (d->txop > QTransform::TxScale);
}

void QWin32PrintEngine::drawPixmap(const QRectF &targetRect,
                                   const QPixmap &originalPixmap,
                                   const QRectF &sourceRect)
{
    Q_D(QWin32PrintEngine);

    QAlphaPaintEngine::drawPixmap(targetRect, originalPixmap, sourceRect);
    if (!continueCall())
        return;

    const int tileSize = 2048;

    QRectF r = targetRect;
    QRectF sr = sourceRect;

    QPixmap pixmap = originalPixmap;
    if (sr.size() != pixmap.size()) {
        pixmap = pixmap.copy(sr.toRect());
    }

    qreal scaleX = 1.0f;
    qreal scaleY = 1.0f;

    QTransform scaleMatrix;
    scaleMatrix.scale(r.width() / pixmap.width(), r.height() / pixmap.height());
    QTransform adapted = QPixmap::trueMatrix(d->painterMatrix * scaleMatrix,
                                             pixmap.width(), pixmap.height());

    qreal xform_offset_x = adapted.dx();
    qreal xform_offset_y = adapted.dy();

    if (d->complex_xform) {
        pixmap = pixmap.transformed(adapted);
        scaleX = d->stretch_x;
        scaleY = d->stretch_y;
    } else {
        scaleX = d->stretch_x * (r.width() / pixmap.width()) * d->painterMatrix.m11();
        scaleY = d->stretch_y * (r.height() / pixmap.height()) * d->painterMatrix.m22();
    }

    QPointF topLeft = r.topLeft() * d->painterMatrix;
    int tx = int(topLeft.x() * d->stretch_x + d->origin_x);
    int ty = int(topLeft.y() * d->stretch_y + d->origin_y);
    int tw = qAbs(int(pixmap.width() * scaleX));
    int th = qAbs(int(pixmap.height() * scaleY));

    xform_offset_x *= d->stretch_x;
    xform_offset_y *= d->stretch_y;

    int dc_state = SaveDC(d->hdc);

    int tilesw = pixmap.width() / tileSize;
    int tilesh = pixmap.height() / tileSize;
    ++tilesw;
    ++tilesh;

    int txinc = tileSize*scaleX;
    int tyinc = tileSize*scaleY;

    for (int y = 0; y < tilesh; ++y) {
        int tposy = ty + (y * tyinc);
        int imgh = tileSize;
        int height = tyinc;
        if (y == (tilesh - 1)) {
            imgh = pixmap.height() - (y * tileSize);
            height = (th - (y * tyinc));
        }
        for (int x = 0; x < tilesw; ++x) {
            int tposx = tx + (x * txinc);
            int imgw = tileSize;
            int width = txinc;
            if (x == (tilesw - 1)) {
                imgw = pixmap.width() - (x * tileSize);
                width = (tw - (x * txinc));
            }

            QPixmap p = pixmap.copy(tileSize * x, tileSize * y, imgw, imgh);
            HBITMAP hbitmap = p.toWinHBITMAP(QPixmap::NoAlpha);
            HDC hbitmap_hdc = CreateCompatibleDC(qt_win_display_dc());
            HGDIOBJ null_bitmap = SelectObject(hbitmap_hdc, hbitmap);

            if (!StretchBlt(d->hdc, qRound(tposx - xform_offset_x), qRound(tposy - xform_offset_y), width, height,
                            hbitmap_hdc, 0, 0, p.width(), p.height(), SRCCOPY))
                qErrnoWarning("QWin32PrintEngine::drawPixmap, StretchBlt failed");

            SelectObject(hbitmap_hdc, null_bitmap);
            DeleteObject(hbitmap);
            DeleteDC(hbitmap_hdc);
        }
    }

    RestoreDC(d->hdc, dc_state);
}


void QWin32PrintEngine::drawTiledPixmap(const QRectF &r, const QPixmap &pm, const QPointF &pos)
{
    Q_D(QWin32PrintEngine);

    QAlphaPaintEngine::drawTiledPixmap(r, pm, pos);
    if (!continueCall())
        return;

    if (d->complex_xform || !pos.isNull()) {
        QPaintEngine::drawTiledPixmap(r, pm, pos);
    } else {
        int dc_state = SaveDC(d->hdc);

        HBITMAP hbitmap = pm.toWinHBITMAP(QPixmap::NoAlpha);
        HDC hbitmap_hdc = CreateCompatibleDC(qt_win_display_dc());
        HGDIOBJ null_bitmap = SelectObject(hbitmap_hdc, hbitmap);

        QRectF trect = d->painterMatrix.mapRect(r);
        int tx = int(trect.left() * d->stretch_x + d->origin_x);
        int ty = int(trect.top() * d->stretch_y + d->origin_y);

        int xtiles = int(trect.width() / pm.width()) + 1;
        int ytiles = int(trect.height() / pm.height()) + 1;
        int xinc = int(pm.width() * d->stretch_x);
        int yinc = int(pm.height() * d->stretch_y);

        for (int y = 0; y < ytiles; ++y) {
            int ity = ty + (yinc * y);
            int ith = pm.height();
            if (y == (ytiles - 1)) {
                ith = int(trect.height() - (pm.height() * y));
            }

            for (int x = 0; x < xtiles; ++x) {
                int itx = tx + (xinc * x);
                int itw = pm.width();
                if (x == (xtiles - 1)) {
                    itw = int(trect.width() - (pm.width() * x));
                }

                if (!StretchBlt(d->hdc, itx, ity, int(itw * d->stretch_x), int(ith * d->stretch_y),
                                hbitmap_hdc, 0, 0, itw, ith, SRCCOPY))
                    qErrnoWarning("QWin32PrintEngine::drawPixmap, StretchBlt failed");

            }
        }

        SelectObject(hbitmap_hdc, null_bitmap);
        DeleteObject(hbitmap);
        DeleteDC(hbitmap_hdc);

        RestoreDC(d->hdc, dc_state);
    }
}


void QWin32PrintEnginePrivate::composeGdiPath(const QPainterPath &path)
{
    if (!BeginPath(hdc))
        qErrnoWarning("QWin32PrintEnginePrivate::drawPath: BeginPath failed");

    // Drawing the subpaths
    int start = -1;
    for (int i=0; i<path.elementCount(); ++i) {
        const QPainterPath::Element &elm = path.elementAt(i);
        switch (elm.type) {
        case QPainterPath::MoveToElement:
            if (start >= 0
                && path.elementAt(start).x == path.elementAt(i-1).x
                && path.elementAt(start).y == path.elementAt(i-1).y)
                CloseFigure(hdc);
            start = i;
            MoveToEx(hdc, qRound(elm.x), qRound(elm.y), 0);
            break;
        case QPainterPath::LineToElement:
            LineTo(hdc, qRound(elm.x), qRound(elm.y));
            break;
        case QPainterPath::CurveToElement: {
            POINT pts[3] = {
                { qRound(elm.x), qRound(elm.y) },
                { qRound(path.elementAt(i+1).x), qRound(path.elementAt(i+1).y) },
                { qRound(path.elementAt(i+2).x), qRound(path.elementAt(i+2).y) }
            };
            i+=2;
            PolyBezierTo(hdc, pts, 3);
            break;
        }
        default:
            qFatal("QWin32PaintEngine::drawPath: Unhandled type: %d", elm.type);
        }
    }

    if (start >= 0
        && path.elementAt(start).x == path.elementAt(path.elementCount()-1).x
        && path.elementAt(start).y == path.elementAt(path.elementCount()-1).y)
        CloseFigure(hdc);

    if (!EndPath(hdc))
        qErrnoWarning("QWin32PaintEngine::drawPath: EndPath failed");

    SetPolyFillMode(hdc, path.fillRule() == Qt::WindingFill ? WINDING : ALTERNATE);
}


void QWin32PrintEnginePrivate::fillPath_dev(const QPainterPath &path, const QColor &color)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << " --- QWin32PrintEnginePrivate::fillPath() bound:" << path.boundingRect() << color;
#endif

    composeGdiPath(path);

    HBRUSH brush = CreateSolidBrush(RGB(color.red(), color.green(), color.blue()));
    HGDIOBJ old_brush = SelectObject(hdc, brush);
    FillPath(hdc);
    DeleteObject(SelectObject(hdc, old_brush));
}

void QWin32PrintEnginePrivate::strokePath_dev(const QPainterPath &path, const QColor &color,
                                              Qt::PenStyle penStyle)
{
    composeGdiPath(path);

    int gdiPenStyle;
    switch (penStyle)
    {
    case Qt::DashLine:
        gdiPenStyle = PS_DASH;
        break;
    case Qt::DashDotLine:
        gdiPenStyle = PS_DASHDOT;
        break;
    case Qt::DashDotDotLine:
        gdiPenStyle = PS_DASHDOTDOT;
        break;
    case Qt::DotLine:
        gdiPenStyle = PS_DOT;
        break;
    default:
        gdiPenStyle = PS_SOLID;
        break;
    };

    HPEN pen = CreatePen(gdiPenStyle, 0, RGB(color.red(), color.green(), color.blue()));
    HGDIOBJ old_pen = SelectObject(hdc, pen);
    StrokePath(hdc);
    DeleteObject(SelectObject(hdc, old_pen));
}


void QWin32PrintEnginePrivate::fillPath(const QPainterPath &path, const QColor &color)
{
    fillPath_dev(path * matrix, color);
}

void QWin32PrintEnginePrivate::strokePath(const QPainterPath &path, const QColor &color)
{
    QPainterPathStroker stroker;
    stroker.setDashPattern(pen.dashPattern());
    stroker.setDashOffset(pen.dashOffset());
    stroker.setCapStyle(pen.capStyle());
    stroker.setJoinStyle(pen.joinStyle());
    stroker.setMiterLimit(pen.miterLimit());

    QPainterPath stroke;

    qreal width = pen.widthF();
    if (pen.isCosmetic()) {
        // We do not support custom dash patterns for stroked paths with solid colored, cosmetic pens
        strokePath_dev(path * matrix, color, pen.style());
    } else {
        stroker.setWidth(width);
        stroke = stroker.createStroke(path) * matrix;
        if (stroke.isEmpty())
            return;

        fillPath_dev(stroke, color);
    }
}


void QWin32PrintEngine::drawPath(const QPainterPath &path)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << " - QWin32PrintEngine::drawPath(), bounds: " << path.boundingRect();
#endif

    Q_D(QWin32PrintEngine);

    QAlphaPaintEngine::drawPath(path);
    if (!continueCall())
        return;

    if (d->has_brush)
        d->fillPath(path, d->brush_color);

    if (d->has_pen)
        d->strokePath(path, d->pen.color());
}


void QWin32PrintEngine::drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << " - QWin32PrintEngine::drawPolygon(), pointCount: " << pointCount;
#endif

    QAlphaPaintEngine::drawPolygon(points, pointCount, mode);
    if (!continueCall())
        return;

    Q_ASSERT(pointCount > 1);

    QPainterPath path(points[0]);

    for (int i=1; i<pointCount; ++i) {
        path.lineTo(points[i]);
    }

    Q_D(QWin32PrintEngine);

    bool has_brush = d->has_brush;

    if (mode == PolylineMode)
        d->has_brush = false; // No brush for polylines
    else
        path.closeSubpath(); // polygons are should always be closed.

    drawPath(path);
    d->has_brush = has_brush;
}

void QWin32PrintEnginePrivate::queryDefault()
{
    /* Read the default printer name, driver and port with the intuitive function
     * Strings "windows" and "device" are specified in the MSDN under EnumPrinters()
     */
    QString noPrinters(QLatin1String("qt_no_printers"));
    QString output;
    QT_WA({
        ushort buffer[256];
        GetProfileStringW(L"windows", L"device",
                          reinterpret_cast<const wchar_t *>(noPrinters.utf16()),
                          reinterpret_cast<wchar_t *>(buffer), 256);
        output = QString::fromUtf16(buffer);
        if (output == noPrinters) { // no printers
            qWarning("QPrinter: System has no default printer, are any printers installed?");
            return;
        }
    }, {
        char buffer[256];
        GetProfileStringA("windows", "device", noPrinters.toLatin1(), buffer, 256);
        output = QString::fromLocal8Bit(buffer);
        if (output == noPrinters) { // no printers
            qWarning("QPrinter: System has no default printer, are any printers installed?");
            return;
        }
    });
    QStringList info = output.split(QLatin1Char(','));
    if(name.isEmpty())
        name = info.at(0);
    if(program.isEmpty())
        program = info.at(1);
    if(port.isEmpty())
        port = info.at(2);
}

QWin32PrintEnginePrivate::~QWin32PrintEnginePrivate()
{
    if (hdc)
        release();
}

void QWin32PrintEnginePrivate::initialize()
{
    if (hdc)
        release();
    Q_ASSERT(!hPrinter);
    Q_ASSERT(!hdc);
    Q_ASSERT(!devMode);
    Q_ASSERT(!pInfo);

    if (name.isEmpty())
        return;

    txop = QTransform::TxNone;

    bool ok;
    QT_WA( {
        ok = OpenPrinterW((LPWSTR)name.utf16(), (LPHANDLE)&hPrinter, 0);
    }, {
        ok = OpenPrinterA((LPSTR)name.toLatin1().data(), (LPHANDLE)&hPrinter, 0);
    } );

    if (!ok) {
        qErrnoWarning("QWin32PrintEngine::initialize: OpenPrinter failed");
        return;
    }

    // Fetch the PRINTER_INFO_2 with DEVMODE data containing the
    // printer settings.
    DWORD infoSize, numBytes;
    ok = true;
    QT_WA( {
        GetPrinterW(hPrinter, 2, NULL, 0, &infoSize);
        hMem = GlobalAlloc(GHND, infoSize);
        pInfo = GlobalLock(hMem);
        if (!GetPrinterW(hPrinter, 2, (LPBYTE)pInfo, infoSize, &numBytes)) {
            ok = false;
        }
    }, {
        GetPrinterA(hPrinter, 2, NULL, 0, &infoSize);
        hMem = GlobalAlloc(GHND, infoSize);
        pInfo = GlobalLock(hMem);
        if (!GetPrinterA(hPrinter, 2, (LPBYTE)pInfo, infoSize, &numBytes)) {
            ok = false;
        }
    });

    if (!ok) {
        qErrnoWarning("QWin32PrintEngine::initialize: GetPrinter failed");
        GlobalUnlock(pInfo);
        GlobalFree(hMem);
        ClosePrinter(hPrinter);
        pInfo = 0;
        hMem = 0;
        hPrinter = 0;
        return;
    }

    QT_WA( {
        devMode = pInfoW()->pDevMode;
    }, {
        devMode = pInfoA()->pDevMode;
    } );

    QT_WA( {
        hdc = CreateDC(reinterpret_cast<const wchar_t *>(program.utf16()),
                       reinterpret_cast<const wchar_t *>(name.utf16()), 0, devModeW());
    }, {
        hdc = CreateDCA(program.toLatin1(), name.toLatin1(), 0, devModeA());
    } );

    Q_ASSERT(hPrinter);
    Q_ASSERT(pInfo);

    if (devMode) {
        QT_WA( {
            num_copies = devModeW()->dmCopies;
        }, {
            num_copies = devModeA()->dmCopies;
        } );
    }

    initHDC();

#ifdef QT_DEBUG_DRAW
    qDebug() << "QWin32PrintEngine::initialize()" << endl
             << " - paperRect" << devPaperRect << endl
             << " - pageRect" << devPageRect << endl
             << " - stretch_x" << stretch_x << endl
             << " - stretch_y" << stretch_y << endl
             << " - origin_x" << origin_x << endl
             << " - origin_y" << origin_y << endl;
#endif
}

void QWin32PrintEnginePrivate::initHDC()
{
    Q_ASSERT(hdc);

    dpi_x = GetDeviceCaps(hdc, LOGPIXELSX);
    dpi_y = GetDeviceCaps(hdc, LOGPIXELSY);
    dpi_display = GetDeviceCaps(qt_win_display_dc(), LOGPIXELSY);
    if (dpi_display == 0) {
        qWarning("QWin32PrintEngine::metric: GetDeviceCaps() failed, "
                "might be a driver problem");
        dpi_display = 96; // Reasonable default
    }

    switch(mode) {
    case QPrinter::ScreenResolution:
        resolution = dpi_display;
        stretch_x = dpi_x / double(dpi_display);
        stretch_y = dpi_y / double(dpi_display);
        break;
    case QPrinter::PrinterResolution:
    case QPrinter::HighResolution:
        resolution = dpi_y;
        stretch_x = 1;
        stretch_y = 1;
        break;
    default:
        break;
    }

    initDevRects();
}

void QWin32PrintEnginePrivate::initDevRects()
{
    devPaperRect = QRect(0, 0,
                         GetDeviceCaps(hdc, PHYSICALWIDTH),
                         GetDeviceCaps(hdc, PHYSICALHEIGHT));
    devPhysicalPageRect = QRect(GetDeviceCaps(hdc, PHYSICALOFFSETX),
                                GetDeviceCaps(hdc, PHYSICALOFFSETY),
                                GetDeviceCaps(hdc, HORZRES),
                                GetDeviceCaps(hdc, VERTRES));
    if (!pageMarginsSet)
        devPageRect = devPhysicalPageRect;
    else
        devPageRect = devPaperRect.adjusted(qRound(mmToInches(previousDialogMargins.left() / 100.0) * dpi_x),
                                            qRound(mmToInches(previousDialogMargins.top() / 100.0) * dpi_y),
                                            -qRound(mmToInches(previousDialogMargins.width() / 100.0) * dpi_x),
                                            -qRound(mmToInches(previousDialogMargins.height() / 100.0) * dpi_y));
    updateOrigin();
}

void QWin32PrintEnginePrivate::setPageMargins(int marginLeft, int marginTop, int marginRight, int marginBottom)
{
    pageMarginsSet = true;
    previousDialogMargins = QRect(marginLeft, marginTop, marginRight, marginBottom);

    devPageRect = devPaperRect.adjusted(qRound(mmToInches(marginLeft / 100.0) * dpi_x),
                                        qRound(mmToInches(marginTop / 100.0) * dpi_y),
                                        - qRound(mmToInches(marginRight / 100.0) * dpi_x),
                                        - qRound(mmToInches(marginBottom / 100.0) * dpi_y));
    updateOrigin();
}

QRect QWin32PrintEnginePrivate::getPageMargins() const
{
    if (pageMarginsSet)
        return previousDialogMargins;
    else
        return QRect(qRound(inchesToMM(devPhysicalPageRect.left()) * 100.0 / dpi_x),
                     qRound(inchesToMM(devPhysicalPageRect.top()) * 100.0 / dpi_y),
                     qRound(inchesToMM(devPaperRect.right() - devPhysicalPageRect.right()) * 100.0 / dpi_x),
                     qRound(inchesToMM(devPaperRect.bottom() - devPhysicalPageRect.bottom()) * 100.0 / dpi_y));
}

void QWin32PrintEnginePrivate::release()
{
    if (hdc == 0)
        return;

    if (globalDevMode) { // Devmode comes from print dialog
        GlobalUnlock(globalDevMode);
    } else {            // Devmode comes from initialize...
        // devMode is a part of the same memory block as pInfo so one free is enough...
        GlobalUnlock(hMem);
        GlobalFree(hMem);
        ClosePrinter(hPrinter);
    }
    DeleteDC(hdc);

    hdc = 0;
    hPrinter = 0;
    pInfo = 0;
    hMem = 0;
    devMode = 0;
}

QList<QVariant> QWin32PrintEnginePrivate::queryResolutions() const
{
    // Read the supported resolutions of the printer.
    DWORD numRes;
    LONG *enumRes;
    DWORD errRes;
    QList<QVariant> list;

    QT_WA({
        numRes = DeviceCapabilities(reinterpret_cast<const wchar_t *>(name.utf16()),
                                    reinterpret_cast<const wchar_t *>(port.utf16()),
                                    DC_ENUMRESOLUTIONS, 0, 0);
        if (numRes == (DWORD)-1)
            return list;
        enumRes = (LONG*)malloc(numRes * 2 * sizeof(LONG));
        errRes = DeviceCapabilities(reinterpret_cast<const wchar_t *>(name.utf16()),
                                    reinterpret_cast<const wchar_t *>(port.utf16()),
                                    DC_ENUMRESOLUTIONS, (LPWSTR)enumRes, 0);
    }, {
        numRes = DeviceCapabilitiesA(name.toLocal8Bit(), port.toLocal8Bit(), DC_ENUMRESOLUTIONS, 0, 0);
        if (numRes == (DWORD)-1)
            return list;
        enumRes = (LONG*)malloc(numRes * 2 * sizeof(LONG));
        errRes = DeviceCapabilitiesA(name.toLocal8Bit(), port.toLocal8Bit(), DC_ENUMRESOLUTIONS, (LPSTR)enumRes, 0);
    });

    if (errRes == (DWORD)-1) {
        qErrnoWarning("QWin32PrintEngine::queryResolutions: DeviceCapabilities failed");
        return list;
    }

    for (uint i=0; i<numRes; ++i)
        list.append(int(enumRes[i*2]));
    return list;
}

void QWin32PrintEnginePrivate::doReinit()
{
    if (state == QPrinter::Active) {
        reinit = true;
    } else {
        resetDC();
        initDevRects();
        reinit = false;
    }
}

void QWin32PrintEnginePrivate::updateOrigin()
{
    if (fullPage) {
        // subtract physical margins to make (0,0) absolute top corner of paper
        // then add user defined margins
        origin_x = -devPhysicalPageRect.x();
        origin_y = -devPhysicalPageRect.y();
        if (pageMarginsSet) {
            origin_x += devPageRect.left();
            origin_y += devPageRect.top();
        }
    } else {
        origin_x = 0;
        origin_y = 0;
        if (pageMarginsSet) {
            origin_x = devPageRect.left() - devPhysicalPageRect.x();
            origin_y = devPageRect.top() - devPhysicalPageRect.y();
        }
    }
}

void QWin32PrintEngine::setProperty(PrintEnginePropertyKey key, const QVariant &value)
{
    Q_D(QWin32PrintEngine);
    switch (key) {
    case PPK_CollateCopies:
        {
            if (!d->devMode)
                break;
            short collate = value.toBool() ? DMCOLLATE_TRUE : DMCOLLATE_FALSE;
            QT_WA( { d->devModeW()->dmCollate = collate; },
                   { d->devModeA()->dmCollate = collate; } );
            d->doReinit();
        }
        break;

    case PPK_ColorMode:
        {
            if (!d->devMode)
                break;
            int cm = value.toInt() == QPrinter::Color ? DMCOLOR_COLOR : DMCOLOR_MONOCHROME;
            QT_WA( { d->devModeW()->dmColor = cm; }, { d->devModeA()->dmColor = cm; } );
            d->doReinit();
        }
        break;

    case PPK_Creator:

        break;

    case PPK_DocumentName:
        if (isActive()) {
            qWarning("QWin32PrintEngine: Cannot change document name while printing is active");
            return;
        }
        d->docName = value.toString();
        break;

    case PPK_FullPage:
        d->fullPage = value.toBool();
        d->updateOrigin();
        break;

    case PPK_NumberOfCopies:
        if (!d->devMode)
            break;
        d->num_copies = value.toInt();
        QT_WA( { d->devModeW()->dmCopies = d->num_copies; },
               { d->devModeA()->dmCopies = d->num_copies; });
        d->doReinit();
        break;

    case PPK_Orientation:
        {
            if (!d->devMode)
                break;
            int orientation = value.toInt() == QPrinter::Landscape ? DMORIENT_LANDSCAPE : DMORIENT_PORTRAIT;
            int old_orientation;
            QT_WA( {
                old_orientation = d->devModeW()->dmOrientation;
                d->devModeW()->dmOrientation = orientation;
            }, {
                old_orientation = d->devModeA()->dmOrientation;
                d->devModeA()->dmOrientation = orientation;
            } );
            if (d->has_custom_paper_size && old_orientation != orientation)
                d->paper_size = QSizeF(d->paper_size.height(), d->paper_size.width());
            d->doReinit();
        }
        break;

    case PPK_OutputFileName:
        if (isActive()) {
            qWarning("QWin32PrintEngine: Cannot change filename while printing");
        } else {
            d->fileName = value.toString();
            d->printToFile = !value.toString().isEmpty();
        }
        break;

    case PPK_PaperSize:
        if (!d->devMode)
            break;
        QT_WA( {
            d->devModeW()->dmPaperSize = mapPaperSizeDevmode(QPrinter::PaperSize(value.toInt()));
        }, {
            d->devModeA()->dmPaperSize = mapPaperSizeDevmode(QPrinter::PaperSize(value.toInt()));
        } );
        d->has_custom_paper_size = (QPrinter::PaperSize(value.toInt()) == QPrinter::Custom);
        d->doReinit();
        break;

    case PPK_PaperSource:
        {
            if (!d->devMode)
                break;
            int dmMapped = DMBIN_AUTO;

            QList<QVariant> v = property(PPK_PaperSources).toList();
            if (v.contains(value))
                dmMapped = mapPaperSourceDevmode(QPrinter::PaperSource(value.toInt()));

            QT_WA( {
                d->devModeW()->dmDefaultSource = dmMapped;
            }, {
                d->devModeA()->dmDefaultSource = dmMapped;
            } );
            d->doReinit();
        }
        break;

    case PPK_PrinterName:
        d->name = value.toString();
        if(d->name.isEmpty())
            d->queryDefault();
        d->initialize();
        break;

    case PPK_Resolution:
        {
            d->resolution = value.toInt();

            d->stretch_x = d->dpi_x / double(d->resolution);
            d->stretch_y = d->dpi_y / double(d->resolution);
        }
        break;

    case PPK_SelectionOption:

        break;

    case PPK_SupportedResolutions:

        break;


    case PPK_WindowsPageSize:
        if (!d->devMode)
            break;
        d->has_custom_paper_size = false;
        QT_WA( {
            d->devModeW()->dmPaperSize = value.toInt();
        }, {
            d->devModeA()->dmPaperSize = value.toInt();
        } );
        d->doReinit();
        break;

    case PPK_CustomPaperSize:
    {
        d->has_custom_paper_size = true;
        d->paper_size = value.toSizeF();
        if (!d->devMode)
            break;
        int orientation;
        QT_WA( {
            orientation = d->devModeW()->dmOrientation;
        }, {
            orientation = d->devModeA()->dmOrientation;
        } );
        if (orientation != DMORIENT_PORTRAIT)
            d->paper_size = QSizeF(d->paper_size.height(), d->paper_size.width());
        break;
    }

    case PPK_PageMargins:
    {
        QList<QVariant> margins(value.toList());
        Q_ASSERT(margins.size() == 4);
        int left, top, right, bottom;
        // specified in 1/100 mm
        left = (margins.at(0).toDouble()*25.4/72.0) * 100;
        top = (margins.at(1).toDouble()*25.4/72.0) * 100;
        right = (margins.at(2).toDouble()*25.4/72.0) * 100;
        bottom = (margins.at(3).toDouble()*25.4/72.0) * 100;
        d->setPageMargins(left, top, right, bottom);
        break;
    }
    default:
        // Do nothing
        break;
    }
}

QVariant QWin32PrintEngine::property(PrintEnginePropertyKey key) const
{
    Q_D(const QWin32PrintEngine);
    QVariant value;
    switch (key) {

    case PPK_CollateCopies:
        value = false;
        break;

    case PPK_ColorMode:
        {
            if (!d->devMode) {
                value = QPrinter::Color;
            } else {
                int mode;
                QT_WA( {
                    mode = d->devModeW()->dmColor;
                }, {
                    mode = d->devModeA()->dmColor;
                } );
                value = mode == DMCOLOR_COLOR ? QPrinter::Color : QPrinter::GrayScale;
            }
        }
        break;

    case PPK_DocumentName:
        value = d->docName;
        break;

    case PPK_FullPage:
        value = d->fullPage;
        break;

    case PPK_NumberOfCopies:
        value = 1;
        break;

    case PPK_Orientation:
        {
            if (!d->devMode) {
                value = QPrinter::Portrait;
            } else {
                int o;
                QT_WA( { o = d->devModeW()->dmOrientation; }, { o = d->devModeA()->dmOrientation; } );
                value = o == DMORIENT_LANDSCAPE ? QPrinter::Landscape : QPrinter::Portrait;
            }
        }
        break;

    case PPK_OutputFileName:
        value = d->fileName;
        break;

    case PPK_PageRect:
        if (d->has_custom_paper_size) {
            QRect rect(0, 0, qRound(d->paper_size.width()), qRound(d->paper_size.height()));
            if (d->pageMarginsSet)
                rect = rect.adjusted(qRound(mmToInches(d->previousDialogMargins.left())),
                                     qRound(mmToInches(d->previousDialogMargins.top())),
                                     qRound(mmToInches(d->previousDialogMargins.width())),
                                     qRound(mmToInches(d->previousDialogMargins.height())));
            value = rect;
        } else {
            value = QTransform(1/d->stretch_x, 0, 0, 1/d->stretch_y, 0, 0)
                    .mapRect(d->fullPage ? d->devPaperRect : d->devPageRect);
        }
        break;

    case PPK_PaperSize:
        if (!d->devMode) {
            value = QPrinter::A4;
        } else {
            QT_WA( {
                value = mapDevmodePaperSize(d->devModeW()->dmPaperSize);
            }, {
                value = mapDevmodePaperSize(d->devModeA()->dmPaperSize);
            } );
        }
        break;

    case PPK_PaperRect:
        if (d->has_custom_paper_size)
            value = QRect(0, 0, qRound(d->paper_size.width()), qRound(d->paper_size.height()));
        else
            value = QTransform(1/d->stretch_x, 0, 0, 1/d->stretch_y, 0, 0).mapRect(d->devPaperRect);
        break;

    case PPK_PaperSource:
        if (!d->devMode) {
            value = QPrinter::Auto;
        } else {
            QT_WA( {
                value = mapDevmodePaperSource(d->devModeW()->dmDefaultSource);
            }, {
                value = mapDevmodePaperSource(d->devModeA()->dmDefaultSource);
            } );
        }
        break;

    case PPK_PrinterName:
        value = d->name;
        break;

    case PPK_Resolution:
        if (d->resolution || !d->name.isEmpty())
            value = d->resolution;
        break;

    case PPK_SupportedResolutions:
        value = d->queryResolutions();
        break;

    case PPK_WindowsPageSize:
        if (!d->devMode) {
            value = -1;
        } else {
            QT_WA( {
                value = d->devModeW()->dmPaperSize;
            }, {
                value = d->devModeA()->dmPaperSize;
            } );
        }
        break;

    case PPK_PaperSources:
        {
            int available, count;
            WORD *data;

            QT_WA({
                available = DeviceCapabilitiesW((const WCHAR *)d->name.utf16(), (const WCHAR *)d->port.utf16(), DC_BINS, 0,
                                                d->devModeW());
            }, {
                available = DeviceCapabilitiesA(d->name.toLatin1(), d->port.toLatin1(), DC_BINS, 0,
                                                d->devModeA());
            });

            if (available <= 0)
                break;
            data = (WORD *) malloc(available * sizeof(WORD));

            QT_WA({
                count = DeviceCapabilitiesW((const WCHAR *)d->name.utf16(), (const WCHAR *)d->port.utf16(), DC_BINS, (WCHAR *)data,
                                            d->devModeW());
            }, {
                count = DeviceCapabilitiesA(d->name.toLatin1(), d->port.toLatin1(), DC_BINS,
                                            (char *) data, d->devModeA());
            });

            QList<QVariant> out;
            for (int i=0; i<count; ++i) {
                QPrinter::PaperSource src = mapDevmodePaperSource(data[i]);
                if (src != -1)
                    out << (int) src;
            }
            value = out;
            free(data);
        }
        break;

    case PPK_CustomPaperSize:
        value = d->paper_size;
        break;

    case PPK_PageMargins:
    {
        QList<QVariant> margins;
        QRect pageMargins(d->getPageMargins());

        // specified in 1/100 mm
        margins << (mmToInches(pageMargins.left()/100.0) * 72)
                << (mmToInches(pageMargins.top()/100.0) * 72)
                << (mmToInches(pageMargins.width()/100.0) * 72)
                << (mmToInches(pageMargins.height()/100.0) * 72);
        value = margins;
        break;
    }
    default:
        // Do nothing
        break;
    }
    return value;
}

QPrinter::PrinterState QWin32PrintEngine::printerState() const
{
    return d_func()->state;
}

HDC QWin32PrintEngine::getDC() const
{
    return d_func()->hdc;
}

void QWin32PrintEngine::releaseDC(HDC) const
{

}

HGLOBAL *QWin32PrintEnginePrivate::createDevNames()
{
    QT_WA( {
        int size = sizeof(DEVNAMES)
                   + program.length() * 2 + 2
                   + name.length() * 2 + 2
                   + port.length() * 2 + 2;
        HGLOBAL *hGlobal = (HGLOBAL *) GlobalAlloc(GMEM_MOVEABLE, size);
        DEVNAMES *dn = (DEVNAMES*) GlobalLock(hGlobal);

        dn->wDriverOffset = sizeof(DEVNAMES) / sizeof(TCHAR);
        dn->wDeviceOffset = dn->wDriverOffset + program.length() + 1;
        dn->wOutputOffset = dn->wDeviceOffset + name.length() + 1;

        memcpy((ushort*)dn + dn->wDriverOffset, program.utf16(), program.length() * 2 + 2);
        memcpy((ushort*)dn + dn->wDeviceOffset, name.utf16(), name.length() * 2 + 2);
        memcpy((ushort*)dn + dn->wOutputOffset, port.utf16(), port.length() * 2 + 2);
        dn->wDefault = 0;

        GlobalUnlock(hGlobal);

//         printf("QPrintDialogWinPrivate::createDevNames()\n"
//                " -> wDriverOffset: %d\n"
//                " -> wDeviceOffset: %d\n"
//                " -> wOutputOffset: %d\n",
//                dn->wDriverOffset,
//                dn->wDeviceOffset,
//                dn->wOutputOffset);

//         printf("QPrintDialogWinPrivate::createDevNames(): %s, %s, %s\n",
//                QString::fromUtf16((ushort*)(dn) + dn->wDriverOffset).latin1(),
//                QString::fromUtf16((ushort*)(dn) + dn->wDeviceOffset).latin1(),
//                QString::fromUtf16((ushort*)(dn) + dn->wOutputOffset).latin1());

        return hGlobal;
    }, {
        int size = sizeof(DEVNAMES)
                   + program.length() + 2
                   + name.length() + 2
                   + port.length() + 2;
        HGLOBAL *hGlobal = (HGLOBAL *) GlobalAlloc(GMEM_MOVEABLE, size);
        DEVNAMES *dn = (DEVNAMES*) GlobalLock(hGlobal);

        dn->wDriverOffset = sizeof(DEVNAMES);
        dn->wDeviceOffset = dn->wDriverOffset + program.length() + 1;
        dn->wOutputOffset = dn->wDeviceOffset + name.length() + 1;

        memcpy((char*)dn + dn->wDriverOffset, program.toLatin1(), program.length() + 2);
        memcpy((char*)dn + dn->wDeviceOffset, name.toLatin1(), name.length() + 2);
        memcpy((char*)dn + dn->wOutputOffset, port.toLatin1(), port.length() + 2);
        dn->wDefault = 0;

        GlobalUnlock(hGlobal);
        return hGlobal;
    } );
}

void QWin32PrintEnginePrivate::readDevnames(HGLOBAL globalDevnames)
{
    if (globalDevnames) {
        QT_WA( {
            DEVNAMES *dn = (DEVNAMES*) GlobalLock(globalDevnames);
            name = QString::fromUtf16((ushort*)(dn) + dn->wDeviceOffset);
            port = QString::fromUtf16((ushort*)(dn) + dn->wOutputOffset);
            program = QString::fromUtf16((ushort*)(dn) + dn->wDriverOffset);
            GlobalUnlock(globalDevnames);
        }, {
            DEVNAMES *dn = (DEVNAMES*) GlobalLock(globalDevnames);
            name = QString::fromLatin1((char*)(dn) + dn->wDeviceOffset);
            port = QString::fromLatin1((char*)(dn) + dn->wOutputOffset);
            program = QString::fromLatin1((char*)(dn) + dn->wDriverOffset);
            GlobalUnlock(globalDevnames);
        } );
    }
}

void QWin32PrintEnginePrivate::readDevmode(HGLOBAL globalDevmode)
{
    if (globalDevmode) {
        QT_WA( {
            DEVMODE *dm = (DEVMODE*) GlobalLock(globalDevmode);
            release();
            globalDevMode = globalDevmode;
            devMode = dm;
            hdc = CreateDC(reinterpret_cast<const wchar_t *>(program.utf16()),
                           reinterpret_cast<const wchar_t *>(name.utf16()), 0, dm);

            num_copies = devModeW()->dmCopies;
        }, {
            DEVMODEA *dm = (DEVMODEA*) GlobalLock(globalDevmode);
            release();
            globalDevMode = globalDevmode;
            devMode = dm;
            hdc = CreateDCA(program.toLatin1(), name.toLatin1(), 0, dm);

            num_copies = devModeA()->dmCopies;
        } );
    }

    if (hdc)
        initHDC();
}

QT_END_NAMESPACE

#endif // QT_NO_PRINTER
