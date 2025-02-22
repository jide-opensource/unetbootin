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

#include <private/qemulationpaintengine_p.h>
#include <private/qpainter_p.h>
#include <private/qtextengine_p.h>
#include <qdebug.h>

QT_BEGIN_NAMESPACE

QEmulationPaintEngine::QEmulationPaintEngine(QPaintEngineEx *engine)
    : real_engine(engine)
{
    QPaintEngine::state = real_engine->state();
}


QPaintEngine::Type QEmulationPaintEngine::type() const
{
    return real_engine->type();
}

bool QEmulationPaintEngine::begin(QPaintDevice *)
{
    return true;
}

bool QEmulationPaintEngine::end()
{
    return true;
}


QPainterState *QEmulationPaintEngine::createState(QPainterState *orig) const
{
    return real_engine->createState(orig);
}

void QEmulationPaintEngine::fill(const QVectorPath &path, const QBrush &brush)
{
    QPainterState *s = state();

    if (s->bgMode == Qt::OpaqueMode) {
        Qt::BrushStyle style = brush.style();
        if (style >= Qt::Dense1Pattern && style <= Qt::DiagCrossPattern)
            real_engine->fill(path, s->bgBrush);
    }

    Qt::BrushStyle style = qbrush_style(brush);
    if (style >= Qt::LinearGradientPattern && style <= Qt::ConicalGradientPattern) {
        const QGradient *g = brush.gradient();

        if (g->coordinateMode() > QGradient::LogicalMode) {
            if (g->coordinateMode() == QGradient::StretchToDeviceMode) {
                QBrush copy = brush;
                QTransform mat = copy.transform();
                mat.scale(real_engine->paintDevice()->width(), real_engine->paintDevice()->height());
                copy.setTransform(mat);
                real_engine->fill(path, copy);
                return;
            } else if (g->coordinateMode() == QGradient::ObjectBoundingMode) {
                QBrush copy = brush;
                QTransform mat = copy.transform();
                QRealRect r = path.controlPointRect();
                mat.translate(r.x1, r.y1);
                mat.scale(r.x2 - r.x1, r.y2 - r.y1);
                copy.setTransform(mat);
                real_engine->fill(path, copy);
                return;
            }
        }
    }

    real_engine->fill(path, brush);
}

void QEmulationPaintEngine::stroke(const QVectorPath &path, const QPen &pen)
{
    QPainterState *s = state();

    if (s->bgMode == Qt::OpaqueMode && pen.style() > Qt::SolidLine) {
        QPen bgPen = pen;
        bgPen.setBrush(s->bgBrush);
        bgPen.setStyle(Qt::SolidLine);
        real_engine->stroke(path, bgPen);
    }


    QBrush brush = pen.brush();
    Qt::BrushStyle style = qbrush_style(brush);
    if (style >= Qt::LinearGradientPattern && style <= Qt::ConicalGradientPattern) {
        const QGradient *g = brush.gradient();
        if (g->coordinateMode() > QGradient::LogicalMode) {
            QPaintEngineEx::stroke(path, pen);
            return;
        }
    }

    real_engine->stroke(path, pen);
}

void QEmulationPaintEngine::clip(const QVectorPath &path, Qt::ClipOperation op)
{
    real_engine->clip(path, op);
}

void QEmulationPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
    if (state()->bgMode == Qt::OpaqueMode && pm.isQBitmap())
        fillBGRect(r);
    real_engine->drawPixmap(r, pm, sr);
}

void QEmulationPaintEngine::drawTextItem(const QPointF &p, const QTextItem &textItem)
{
    if (state()->bgMode == Qt::OpaqueMode) {
        const QTextItemInt &ti = static_cast<const QTextItemInt &>(textItem);
        QRectF rect(p.x(), p.y() - ti.ascent.toReal(), ti.width.toReal(), (ti.ascent + ti.descent + 1).toReal());
        fillBGRect(rect);
    }
    real_engine->drawTextItem(p, textItem);
}

void QEmulationPaintEngine::drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s)
{
    if (state()->bgMode == Qt::OpaqueMode && pixmap.isQBitmap())
        fillBGRect(r);
    real_engine->drawTiledPixmap(r, pixmap, s);
}

void QEmulationPaintEngine::drawImage(const QRectF &r, const QImage &pm, const QRectF &sr, Qt::ImageConversionFlags flags)
{
    real_engine->drawImage(r, pm, sr, flags);
}

void QEmulationPaintEngine::clipEnabledChanged()
{
    real_engine->clipEnabledChanged();
}

void QEmulationPaintEngine::penChanged()
{
    real_engine->penChanged();
}

void QEmulationPaintEngine::brushChanged()
{
    real_engine->brushChanged();
}

void QEmulationPaintEngine::brushOriginChanged()
{
    real_engine->brushOriginChanged();
}

void QEmulationPaintEngine::opacityChanged()
{
    real_engine->opacityChanged();
}

void QEmulationPaintEngine::compositionModeChanged()
{
    real_engine->compositionModeChanged();
}

void QEmulationPaintEngine::renderHintsChanged()
{
    real_engine->renderHintsChanged();
}

void QEmulationPaintEngine::transformChanged()
{
    real_engine->transformChanged();
}

void QEmulationPaintEngine::setState(QPainterState *s)
{
    QPaintEngine::state = s;
    real_engine->setState(s);
}


void QEmulationPaintEngine::fillBGRect(const QRectF &r)
{
    qreal pts[] = { r.x(), r.y(), r.x() + r.width(), r.y(),
                    r.x() + r.width(), r.y() + r.height(), r.x(), r.y() + r.height() };
    QVectorPath vp(pts, 4, 0, QVectorPath::RectangleHint);
    real_engine->fill(vp, state()->bgBrush);
}

QT_END_NAMESPACE
