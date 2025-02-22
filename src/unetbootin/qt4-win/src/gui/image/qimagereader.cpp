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

//#define QIMAGEREADER_DEBUG

/*!
    \class QImageReader
    \brief The QImageReader class provides a format independent interface
    for reading images from files or other devices.

    \reentrant
    \ingroup multimedia
    \ingroup io

    The most common way to read images is through QImage and QPixmap's
    constructors, or by calling QImage::load() and
    QPixmap::load(). QImageReader is a specialized class which gives
    you more control when reading images. For example, you can read an
    image into a specific size by calling setScaledSize(), and you can
    select a clip rect, effectively loading only parts of an image, by
    calling setClipRect(). Depending on the underlying support in the
    image format, this can save memory and speed up loading of images.

    To read an image, you start by constructing a QImageReader object.
    Pass either a file name or a device pointer, and the image format
    to QImageReader's constructor. You can then set several options,
    such as the clip rect (by calling setClipRect()) and scaled size
    (by calling setScaledSize()). canRead() returns the image if the
    QImageReader can read the image (i.e., the image format is
    supported and the device is open for reading). Call read() to read
    the image.

    If any error occurs when reading the image, read() will return a
    null QImage. You can then call error() to find the type of error
    that occurred, or errorString() to get a human readable
    description of what went wrong.

    Call supportedImageFormats() for a list of formats that
    QImageReader can read. QImageReader supports all built-in image
    formats, in addition to any image format plugins that support
    reading.

    \sa QImageWriter, QImageIOHandler, QImageIOPlugin
*/

/*!
    \enum QImageReader::ImageReaderError

    This enum describes the different types of errors that can occur
    when reading images with QImageReader.

    \value FileNotFoundError QImageReader was used with a file name,
    but not file was found with that name. This can also happen if the
    file name contained no extension, and the file with the correct
    extension is not supported by Qt.

    \value DeviceError QImageReader encountered a device error when
    reading the image. You can consult your particular device for more
    details on what went wrong.

    \value UnsupportedFormatError Qt does not support the requested
    image format.

    \value InvalidDataError The image data was invalid, and
    QImageReader was unable to read an image from it. The can happen
    if the image file is damaged.

    \value UnknownError An unknown error occurred. If you get this
    value after calling read(), it is most likely caused by a bug in
    QImageReader.
*/
#include "qimagereader.h"

#include <qbytearray.h>
#ifdef QIMAGEREADER_DEBUG
#include <qdebug.h>
#endif
#include <qfile.h>
#include <qfileinfo.h>
#include <qimage.h>
#include <qimageiohandler.h>
#include <qlist.h>
#include <qrect.h>
#include <qset.h>
#include <qsize.h>
#include <qcolor.h>
#include <qvariant.h>

// factory loader
#include <qcoreapplication.h>
#include <private/qfactoryloader_p.h>

// image handlers
#include <private/qbmphandler_p.h>
#include <private/qppmhandler_p.h>
#include <private/qxbmhandler_p.h>
#include <private/qxpmhandler_p.h>
#ifndef QT_NO_IMAGEFORMAT_PNG
#include <private/qpnghandler_p.h>
#endif

QT_BEGIN_NAMESPACE

#if !defined (QT_NO_LIBRARY) && !defined(QT_NO_SETTINGS)
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader,
                          (QImageIOHandlerFactoryInterface_iid, QLatin1String("/imageformats")))
#endif

enum _qt_BuiltInFormatType {
#ifndef QT_NO_IMAGEFORMAT_PNG
    _qt_PngFormat,
#endif
    _qt_BmpFormat,
#ifndef QT_NO_IMAGEFORMAT_PPM
    _qt_PpmFormat,
    _qt_PgmFormat,
    _qt_PbmFormat,
#endif
#ifndef QT_NO_IMAGEFORMAT_XBM
    _qt_XbmFormat,
#endif
#ifndef QT_NO_IMAGEFORMAT_XPM
    _qt_XpmFormat,
#endif
    _qt_NumFormats,
    _qt_NoFormat = -1
};

struct _qt_BuiltInFormatStruct
{
    _qt_BuiltInFormatType type;
    const char *extension;
};

static const _qt_BuiltInFormatStruct _qt_BuiltInFormats[] = {
#ifndef QT_NO_IMAGEFORMAT_PNG
    {_qt_PngFormat, "png"},
#endif
    {_qt_BmpFormat, "bmp"},
#ifndef QT_NO_IMAGEFORMAT_PPM
    {_qt_PpmFormat, "ppm"},
    {_qt_PgmFormat, "pgm"},
    {_qt_PbmFormat, "pbm"},
#endif
#ifndef QT_NO_IMAGEFORMAT_XBM
    {_qt_XbmFormat, "xbm"},
#endif
#ifndef QT_NO_IMAGEFORMAT_XPM
    {_qt_XpmFormat, "xpm"},
#endif
    {_qt_NoFormat, ""}
};

static QImageIOHandler *createReadHandlerHelper(QIODevice *device,
    const QByteArray &format)
{
    QByteArray form = format.toLower();
    QImageIOHandler *handler = 0;

#if !defined (QT_NO_LIBRARY) && !defined(QT_NO_SETTINGS)
    // check if we have plugins that support the image format
    QFactoryLoader *l = loader();
    QStringList keys = l->keys();
#endif
    QByteArray suffix;

#ifdef QIMAGEREADER_DEBUG
    qDebug() << "QImageReader::createReadHandler( device =" << (void *)device << ", format =" << format << "),"
             << keys.size() << "plugins available: " << keys;
#endif

#if !defined (QT_NO_LIBRARY) && !defined(QT_NO_SETTINGS)
    int suffixPluginIndex = -1;
    if (device && format.isEmpty()) {
        // if there's no format, see if \a device is a file, and if so, find
        // the file suffix and find support for that format among our plugins.
        // this allows plugins to override our built-in handlers.
        if (QFile *file = qobject_cast<QFile *>(device)) {
#ifdef QIMAGEREADER_DEBUG
            qDebug() << "QImageReader::createReadHandler: device is a file:" << file->fileName();
#endif
            if (!(suffix = QFileInfo(file->fileName()).suffix().toLower().toLatin1()).isEmpty()) {
                int index = keys.indexOf(QString::fromLatin1(suffix));
                if (index != -1) {
#ifdef QIMAGEREADER_DEBUG
                    qDebug() << "QImageReader::createReadHandler: suffix recognized; the"
                             << suffix << "plugin might be able to read this";
#endif
                    suffixPluginIndex = index;
                }
            }
        }
    }
#endif // QT_NO_LIBRARY

    QByteArray testFormat = !form.isEmpty() ? form : suffix;

#if !defined (QT_NO_LIBRARY) && !defined(QT_NO_SETTINGS)
    if (suffixPluginIndex != -1) {
        // check if the plugin that claims support for this format can load
        // from this device with this format.
        const qint64 pos = device ? device->pos() : 0;
        QImageIOPlugin *plugin = qobject_cast<QImageIOPlugin *>(l->instance(QString::fromLatin1(suffix)));
        if (plugin && plugin->capabilities(device, testFormat) & QImageIOPlugin::CanRead) {
            handler = plugin->create(device, testFormat);
#ifdef QIMAGEREADER_DEBUG
            qDebug() << "QImageReader::createReadHandler: using the" << suffix
                     << "plugin";
#endif
        }
        if (device && !device->isSequential())
            device->seek(pos);
    }

    if (!handler && !testFormat.isEmpty()) {
        // check if any plugin supports the format (they are not allowed to
        // read from the device yet).
        const qint64 pos = device ? device->pos() : 0;
        for (int i = 0; i < keys.size(); ++i) {
            if (i != suffixPluginIndex) {
                QImageIOPlugin *plugin = qobject_cast<QImageIOPlugin *>(l->instance(keys.at(i)));
                if (plugin && plugin->capabilities(device, testFormat) & QImageIOPlugin::CanRead) {
#ifdef QIMAGEREADER_DEBUG
                    qDebug() << "QImageReader::createReadHandler: the" << keys.at(i) << "plugin can read this format";
#endif
                    handler = plugin->create(device, testFormat);
                    break;
                }
            }
        }
        if (device && !device->isSequential())
            device->seek(pos);
    }
#endif // QT_NO_LIBRARY

    // if we don't have a handler yet, check if we have built-in support for
    // the format
    if (!handler && !testFormat.isEmpty()) {
        if (false) {
#ifndef QT_NO_IMAGEFORMAT_PNG
	} else if (testFormat == "png") {
            handler = new QPngHandler;
#endif
#ifndef QT_NO_IMAGEFORMAT_BMP
        } else if (testFormat == "bmp") {
            handler = new QBmpHandler;
#endif
#ifndef QT_NO_IMAGEFORMAT_XPM
        } else if (testFormat == "xpm") {
            handler = new QXpmHandler;
#endif
#ifndef QT_NO_IMAGEFORMAT_XBM
        } else if (testFormat == "xbm") {
            handler = new QXbmHandler;
            handler->setOption(QImageIOHandler::SubType, testFormat);
#endif
#ifndef QT_NO_IMAGEFORMAT_PPM
        } else if (testFormat == "pbm" || testFormat == "pbmraw" || testFormat == "pgm"
                   || testFormat == "pgmraw" || testFormat == "ppm" || testFormat == "ppmraw") {
            handler = new QPpmHandler;
            handler->setOption(QImageIOHandler::SubType, testFormat);
#endif
        }

#ifdef QIMAGEREADER_DEBUG
        if (handler)
            qDebug() << "QImageReader::createReadHandler: using the built-in handler for" << testFormat;
#endif
    }

#if !defined (QT_NO_LIBRARY) && !defined(QT_NO_SETTINGS)
    if (!handler) {
        // check if any of our plugins recognize the file from its contents.
        const qint64 pos = device ? device->pos() : 0;
        for (int i = 0; i < keys.size(); ++i) {
            if (i != suffixPluginIndex) {
                QImageIOPlugin *plugin = qobject_cast<QImageIOPlugin *>(l->instance(keys.at(i)));
                if (plugin && plugin->capabilities(device, QByteArray()) & QImageIOPlugin::CanRead) {
                    handler = plugin->create(device, testFormat);
#ifdef QIMAGEREADER_DEBUG
                    qDebug() << "QImageReader::createReadHandler: the" << keys.at(i) << "plugin can read this data";
#endif
                    break;
                }
            }
        }
        if (device && !device->isSequential())
            device->seek(pos);
    }
#endif

    if (!handler) {
        // check if any of our built-in handlers recognize the file from its
        // contents.
        int currentFormat = 0;
        if (!suffix.isEmpty()) {
            // If reading from a file with a suffix, start testing our
            // built-in handler for that suffix first.
            for (int i = 0; i < _qt_NumFormats; ++i) {
                if (_qt_BuiltInFormats[i].extension == suffix) {
                    currentFormat = i;
                    break;
                }
            }
        }

        QByteArray subType;
        int numFormats = _qt_NumFormats;
        while (device && numFormats >= 0) {
            const _qt_BuiltInFormatStruct *formatStruct = &_qt_BuiltInFormats[currentFormat];

            const qint64 pos = device->pos();
            switch (formatStruct->type) {
#ifndef QT_NO_IMAGEFORMAT_PNG
            case _qt_PngFormat:
                if (QPngHandler::canRead(device))
                    handler = new QPngHandler;
                break;
#endif
#ifndef QT_NO_IMAGEFORMAT_BMP
            case _qt_BmpFormat:
                if (QBmpHandler::canRead(device))
                    handler = new QBmpHandler;
                break;
#endif
#ifndef QT_NO_IMAGEFORMAT_XPM
            case _qt_XpmFormat:
                if (QXpmHandler::canRead(device))
                    handler = new QXpmHandler;
                break;
#endif
#ifndef QT_NO_IMAGEFORMAT_PPM
            case _qt_PbmFormat:
            case _qt_PgmFormat:
            case _qt_PpmFormat:
                if (QPpmHandler::canRead(device, &subType)) {
                    handler = new QPpmHandler;
                    handler->setOption(QImageIOHandler::SubType, subType);
                }
                break;
#endif
#ifndef QT_NO_IMAGEFORMAT_XBM
            case _qt_XbmFormat:
                if (QXbmHandler::canRead(device))
                    handler = new QXbmHandler;
                break;
#endif
            default:
                break;
            }
            if (!device->isSequential())
                device->seek(pos);

            if (handler) {
#ifdef QIMAGEREADER_DEBUG
                qDebug() << "QImageReader::createReadHandler: the" << formatStruct->extension
                         << "built-in handler can read this data";
#endif
                break;
            }

            --numFormats;
            ++currentFormat;
            currentFormat %= _qt_NumFormats;
        }
    }

    if (!handler) {
#ifdef QIMAGEREADER_DEBUG
        qDebug() << "QImageReader::createReadHandler: no handlers found. giving up.";
#endif
        // no handler: give up.
        return 0;
    }

    handler->setDevice(device);
    if (!form.isEmpty())
        handler->setFormat(form);
    return handler;
}

class QImageReaderPrivate
{
public:
    QImageReaderPrivate(QImageReader *qq);
    ~QImageReaderPrivate();

    // device
    QByteArray format;
    QIODevice *device;
    bool deleteDevice;
    QImageIOHandler *handler;
    bool initHandler();

    // image options
    QRect clipRect;
    QSize scaledSize;
    QRect scaledClipRect;
    int quality;
    QMap<QString, QString> text;
    void getText();

    // error
    QImageReader::ImageReaderError imageReaderError;
    QString errorString;

    QImageReader *q;
};

/*!
    \internal
*/
QImageReaderPrivate::QImageReaderPrivate(QImageReader *qq)
{
    device = 0;
    deleteDevice = false;
    handler = 0;
    quality = -1;
    imageReaderError = QImageReader::UnknownError;
    errorString = QLatin1String(QT_TRANSLATE_NOOP(QImageReader, "Unknown error"));

    q = qq;
}

/*!
    \internal
*/
QImageReaderPrivate::~QImageReaderPrivate()
{
    if (deleteDevice)
        delete device;
    delete handler;
}

/*!
    \internal
*/
bool QImageReaderPrivate::initHandler()
{
    // check some preconditions
    if (!device || (!deleteDevice && !device->isOpen())) {
        imageReaderError = QImageReader::DeviceError;
        errorString = QLatin1String(QT_TRANSLATE_NOOP(QImageReader, "Invalid device"));
        return false;
    }

    // probe the file extension
    if (deleteDevice && !device->isOpen() && !device->open(QIODevice::ReadOnly)) {
        QList<QByteArray> extensions = QImageReader::supportedImageFormats();
        if (!format.isEmpty()) {
            // Try the most probable extension first
            int currentFormatIndex = extensions.indexOf(format.toLower());
            if (currentFormatIndex > 0)
                extensions.swap(0, currentFormatIndex);
        }

        int currentExtension = 0;

        QFile *file = static_cast<QFile *>(device);
        QString fileName = file->fileName();

        do {
            file->setFileName(fileName + QLatin1Char('.')
                    + QString::fromLatin1(extensions.at(currentExtension++).constData()));
            file->open(QIODevice::ReadOnly);
        } while (!file->isOpen() && currentExtension < extensions.size());

        if (!device->isOpen()) {
            imageReaderError = QImageReader::FileNotFoundError;
            errorString = QLatin1String(QT_TRANSLATE_NOOP(QImageReader, "File not found"));
            return false;
        }
    }

    // assign a handler
    if (!handler && (handler = createReadHandlerHelper(device, format)) == 0) {
        imageReaderError = QImageReader::UnsupportedFormatError;
        errorString = QLatin1String(QT_TRANSLATE_NOOP(QImageReader, "Unsupported image format"));
        return false;
    }
    return true;
}

/*!
    \internal
*/
void QImageReaderPrivate::getText()
{
    if (!text.isEmpty() || (!handler && !initHandler()) || !handler->supportsOption(QImageIOHandler::Description))
        return;
    foreach (QString pair, handler->option(QImageIOHandler::Description).toString().split(
                QLatin1String("\n\n"))) {
        int index = pair.indexOf(QLatin1Char(':'));
        if (index >= 0 && pair.indexOf(QLatin1Char(' ')) < index) {
            text.insert(QLatin1String("Description"), pair.simplified());
        } else {
            QString key = pair.left(index);
            text.insert(key, pair.mid(index + 2).simplified());
        }
    }
}

/*!
    Constructs an empty QImageReader object. Before reading an image,
    call setDevice() or setFileName().
*/
QImageReader::QImageReader()
    : d(new QImageReaderPrivate(this))
{
}

/*!
    Constructs a QImageReader object with the device \a device and the
    image format \a format.
*/
QImageReader::QImageReader(QIODevice *device, const QByteArray &format)
    : d(new QImageReaderPrivate(this))
{
    d->device = device;
    d->format = format;
}

/*!
    Constructs a QImageReader object with the file name \a fileName
    and the image format \a format.

    \sa setFileName()
*/
QImageReader::QImageReader(const QString &fileName, const QByteArray &format)
    : d(new QImageReaderPrivate(this))
{
    QFile *file = new QFile(fileName);
    d->device = file;
    d->deleteDevice = true;
    d->format = format;
}

/*!
    Destructs the QImageReader object.
*/
QImageReader::~QImageReader()
{
    delete d;
}

/*!
    Sets the format QImageReader will use when reading images, to \a
    format. \a format is a case insensitive text string. Example:

    \snippet doc/src/snippets/code/src.gui.image.qimagereader.cpp 0

    You can call supportedImageFormats() for the full list of formats
    QImageReader supports.

    \sa format()
*/
void QImageReader::setFormat(const QByteArray &format)
{
    d->format = format;
}

/*!
    Returns the format QImageReader uses for reading images.

    You can call this function after assigning a device to the
    reader to determine the format of the device. For example:

    \snippet doc/src/snippets/code/src.gui.image.qimagereader.cpp 1

    If the reader cannot read any image from the device (e.g., there is no
    image there, or the image has already been read), or if the format is
    unsupported, this function returns an empty QByteArray().

    \sa setFormat(), supportedImageFormats()
*/
QByteArray QImageReader::format() const
{
    if (d->format.isEmpty()) {
        if (!d->initHandler())
            return QByteArray();
        return d->handler->canRead() ? d->handler->format() : QByteArray();
    }

    return d->format;
}

/*!
    Sets QImageReader's device to \a device. If a device has already
    been set, the old device is removed from QImageReader and is
    otherwise left unchanged.

    If the device is not already open, QImageReader will attempt to
    open the device in \l QIODevice::ReadOnly mode by calling
    open(). Note that this does not work for certain devices, such as
    QProcess, QTcpSocket and QUdpSocket, where more logic is required
    to open the device.

    \sa device(), setFileName()
*/
void QImageReader::setDevice(QIODevice *device)
{
    if (d->device && d->deleteDevice)
        delete d->device;
    d->device = device;
    d->deleteDevice = false;
    delete d->handler;
    d->handler = 0;
    d->text.clear();
}

/*!
    Returns the device currently assigned to QImageReader, or 0 if no
    device has been assigned.
*/
QIODevice *QImageReader::device() const
{
    return d->device;
}

/*!
    Sets the file name of QImageReader to \a fileName. Internally,
    QImageReader will create a QFile object and open it in \l
    QIODevice::ReadOnly mode, and use this when reading images.

    If \a fileName does not include a file extension (e.g., .png or .bmp),
    QImageReader will cycle through all supported extensions until it finds
    a matching file.

    \sa fileName(), setDevice(), supportedImageFormats()
*/
void QImageReader::setFileName(const QString &fileName)
{
    setDevice(new QFile(fileName));
    d->deleteDevice = true;
}

/*!
    If the currently assigned device is a QFile, or if setFileName()
    has been called, this function returns the name of the file
    QImageReader reads from. Otherwise (i.e., if no device has been
    assigned or the device is not a QFile), an empty QString is
    returned.

    \sa setFileName(), setDevice()
*/
QString QImageReader::fileName() const
{
    QFile *file = qobject_cast<QFile *>(d->device);
    return file ? file->fileName() : QString();
}

/*!
    \since 4.2

    This is an image format specific function that sets the quality
    level of the image to \a quality. For image formats that do not
    support setting the quality, this value is ignored.

    The value range of \a quality depends on the image format. For
    example, the "jpeg" format supports a quality range from 0 (low
    quality, high compression) to 100 (high quality, low compression).

    \sa quality()
*/
void QImageReader::setQuality(int quality)
{
    d->quality = quality;
}

/*!
    \since 4.2

    Returns the quality level of the image.

    \sa setQuality()
*/
int QImageReader::quality() const
{
    return d->quality;
}


/*!
    Returns the size of the image, without actually reading the image
    contents.

    If the image format does not support this feature, this function returns
    an invalid size. Qt's built-in image handlers all support this feature,
    but custom image format plugins are not required to do so.

    \sa QImageIOHandler::ImageOption, QImageIOHandler::option(), QImageIOHandler::supportsOption()
*/
QSize QImageReader::size() const
{
    if (!d->initHandler())
        return QSize();

    if (d->handler->supportsOption(QImageIOHandler::Size))
        return d->handler->option(QImageIOHandler::Size).toSize();

    return QSize();
}

/*!
    \since 4.1

    Returns the text keys for this image. You can use
    these keys with text() to list the image text for
    a certain key.

    Support for this option is implemented through
    QImageIOHandler::Description.

    \sa text(), QImageWriter::setText(), QImage::textKeys()
*/
QStringList QImageReader::textKeys() const
{
    d->getText();
    return d->text.keys();
}

/*!
    \since 4.1

    Returns the image text associated with \a key.

    Support for this option is implemented through
    QImageIOHandler::Description.

    \sa textKeys(), QImageWriter::setText()
*/
QString QImageReader::text(const QString &key) const
{
    d->getText();
    return d->text.value(key);
}

/*!
    Sets the image clip rect (also known as the ROI, or Region Of
    Interest) to \a rect. The coordinates of \a rect are relative to
    the untransformed image size, as returned by size().

    \sa clipRect(), setScaledSize(), setScaledClipRect()
*/
void QImageReader::setClipRect(const QRect &rect)
{
    d->clipRect = rect;
}

/*!
    Returns the clip rect (also known as the ROI, or Region Of
    Interest) of the image. If no clip rect has been set, an invalid
    QRect is returned.

    \sa setClipRect()
*/
QRect QImageReader::clipRect() const
{
    return d->clipRect;
}

/*!
    Sets the scaled size of the image to \a size. The scaling is
    performed after the initial clip rect, but before the scaled clip
    rect is applied. The algorithm used for scaling depends on the
    image format. By default (i.e., if the image format does not
    support scaling), QImageReader will use QImage::scale() with
    Qt::SmoothScaling.

    \sa scaledSize(), setClipRect(), setScaledClipRect()
*/
void QImageReader::setScaledSize(const QSize &size)
{
    d->scaledSize = size;
}

/*!
    Returns the scaled size of the image.

    \sa setScaledSize()
*/
QSize QImageReader::scaledSize() const
{
    return d->scaledSize;
}

/*!
    Sets the scaled clip rect to \a rect. The scaled clip rect is the
    clip rect (also known as ROI, or Region Of Interest) that is
    applied after the image has been scaled.

    \sa scaledClipRect(), setScaledSize()
*/
void QImageReader::setScaledClipRect(const QRect &rect)
{
    d->scaledClipRect = rect;
}

/*!
    Returns the scaled clip rect of the image.

    \sa setScaledClipRect()
*/
QRect QImageReader::scaledClipRect() const
{
    return d->scaledClipRect;
}

/*!
    \since 4.1

    Sets the background color to \a color.
    Image formats that support this operation are expected to
    initialize the background to \a color before reading an image.

    \sa backgroundColor(), read()
*/
void QImageReader::setBackgroundColor(const QColor &color)
{
    if (!d->initHandler())
        return;
    if (d->handler->supportsOption(QImageIOHandler::BackgroundColor))
        d->handler->setOption(QImageIOHandler::BackgroundColor, color);
}

/*!
    \since 4.1

    Returns the background color that's used when reading an image.
    If the image format does not support setting the background color
    an invalid color is returned.

    \sa setBackgroundColor(), read()
*/
QColor QImageReader::backgroundColor() const
{
    if (!d->initHandler())
        return QColor();
    if (d->handler->supportsOption(QImageIOHandler::BackgroundColor))
        return qVariantValue<QColor>(d->handler->option(QImageIOHandler::BackgroundColor));
    return QColor();
}

/*!
    \since 4.1

    Returns true if the image format supports animation;
    otherwise, false is returned.

    \sa QMovie::supportedFormats()
*/
bool QImageReader::supportsAnimation() const
{
    if (!d->initHandler())
        return false;
    if (d->handler->supportsOption(QImageIOHandler::Animation))
        return d->handler->option(QImageIOHandler::Animation).toBool();
    return false;
}

/*!
    Returns true if an image can be read for the device (i.e., the
    image format is supported, and the device seems to contain valid
    data); otherwise returns false.

    canRead() is a lightweight function that only does a quick test to
    see if the image data is valid. read() may still return false
    after canRead() returns true, if the image data is corrupt.

    For images that support animation, canRead() returns false when
    all frames have been read.

    \sa read(), supportedImageFormats()
*/
bool QImageReader::canRead() const
{
    if (!d->initHandler())
        return false;

    return d->handler->canRead();
}

/*!
    Reads an image from the device. On success, the image that was
    read is returned; otherwise, a null QImage is returned. You can
    then call error() to find the type of error that occurred, or
    errorString() to get a human readable description of the error.

    For image formats that support animation, calling read()
    repeatedly will return the next frame. When all frames have been
    read, a null image will be returned.

    \sa canRead(), supportedImageFormats(), supportsAnimation(), QMovie
*/
QImage QImageReader::read()
{
    // Because failed image reading might have side effects, we explicitly
    // return a null image instead of the image we've just created.
    QImage image;
    return read(&image) ? image : QImage();
}

/*!
    \overload

    Reads an image from the device into \a image, which must point to a
    QImage. Returns true on success; otherwise, returns false.

    If \a image has same format and size as the image data that is about to be
    read, this function may not need to allocate a new image before
    reading. Because of this, it can be faster than the other read() overload,
    which always constructs a new image; especially when reading several
    images with the same format and size.

    \snippet doc/src/snippets/code/src.gui.image.qimagereader.cpp 2

    For image formats that support animation, calling read() repeatedly will
    return the next frame. When all frames have been read, a null image will
    be returned.

    \sa canRead(), supportedImageFormats(), supportsAnimation(), QMovie
*/
bool QImageReader::read(QImage *image)
{
    if (!image) {
        qWarning("QImageReader::read: cannot read into null pointer");
        return false;
    }

    if (!d->handler && !d->initHandler())
        return false;

    // set the handler specific options.
    if (d->handler->supportsOption(QImageIOHandler::ScaledSize) && d->scaledSize.isValid()) {
        if ((d->handler->supportsOption(QImageIOHandler::ClipRect) && !d->clipRect.isNull())
            || d->clipRect.isNull()) {
            // Only enable the ScaledSize option if there is no clip rect, or
            // if the handler also supports ClipRect.
            d->handler->setOption(QImageIOHandler::ScaledSize, d->scaledSize);
        }
    }
    if (d->handler->supportsOption(QImageIOHandler::ClipRect) && !d->clipRect.isNull())
        d->handler->setOption(QImageIOHandler::ClipRect, d->clipRect);
    if (d->handler->supportsOption(QImageIOHandler::ScaledClipRect) && !d->scaledClipRect.isNull())
        d->handler->setOption(QImageIOHandler::ScaledClipRect, d->scaledClipRect);
    if (d->handler->supportsOption(QImageIOHandler::Quality))
        d->handler->setOption(QImageIOHandler::Quality, d->quality);

    // read the image
    if (!d->handler->read(image)) {
        d->imageReaderError = InvalidDataError;
        d->errorString = QLatin1String(QT_TRANSLATE_NOOP(QImageReader, "Unable to read image data"));
        return false;
    }

    // provide default implementations for any unsupported image
    // options
    if (d->handler->supportsOption(QImageIOHandler::ClipRect) && !d->clipRect.isNull()) {
        if (d->handler->supportsOption(QImageIOHandler::ScaledSize) && d->scaledSize.isValid()) {
            if (d->handler->supportsOption(QImageIOHandler::ScaledClipRect) && !d->scaledClipRect.isNull()) {
                // all features are supported by the handler; nothing to do.
            } else {
                // the image is already scaled, so apply scaled clipping.
                if (!d->scaledClipRect.isNull())
                    *image = image->copy(d->scaledClipRect);
            }
        } else {
            if (d->handler->supportsOption(QImageIOHandler::ScaledClipRect) && !d->scaledClipRect.isNull()) {
                // supports scaled clipping but not scaling, most
                // likely a broken handler.
            } else {
                if (d->scaledSize.isValid()) {
                    *image = image->scaled(d->scaledSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                }
                if (d->scaledClipRect.isValid()) {
                    *image = image->copy(d->scaledClipRect);
                }
            }
        }
    } else {
        if (d->handler->supportsOption(QImageIOHandler::ScaledSize) && d->scaledSize.isValid()) {
            // in this case, there's nothing we can do. if the
            // plugin supports scaled size but not ClipRect, then
            // we have to ignore ClipRect."

            if (d->handler->supportsOption(QImageIOHandler::ScaledClipRect) && !d->scaledClipRect.isNull()) {
                // nothing to do (ClipRect is ignored!)
            } else {
                // provide all workarounds.
                if (d->scaledClipRect.isValid()) {
                    *image = image->copy(d->scaledClipRect);
                }
            }
        } else {
            if (d->handler->supportsOption(QImageIOHandler::ScaledClipRect) && !d->scaledClipRect.isNull()) {
                // this makes no sense; a handler that supports
                // ScaledClipRect but not ScaledSize is broken, and we
                // can't work around it.
            } else {
                // provide all workarounds.
                if (d->clipRect.isValid())
                    *image = image->copy(d->clipRect);
                if (d->scaledSize.isValid())
                    *image = image->scaled(d->scaledSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                if (d->scaledClipRect.isValid())
                    *image = image->copy(d->scaledClipRect);
            }
        }
    }

    return true;
}

/*!
   For image formats that support animation, this function steps over the
   current image, returning true if successful or false if there is no
   following image in the animation.

   The default implementation calls read(), then discards the resulting
   image, but the image handler may have a more efficient way of implementing
   this operation.

   \sa jumpToImage(), QImageIOHandler::jumpToNextImage()
*/
bool QImageReader::jumpToNextImage()
{
    if (!d->initHandler())
        return false;
    return d->handler->jumpToNextImage();
}

/*!
   For image formats that support animation, this function skips to the image
   whose sequence number is \a imageNumber, returning true if successful
   or false if the corresponding image cannot be found.

   The next call to read() will attempt to read this image.

   \sa jumpToNextImage(), QImageIOHandler::jumpToImage()
*/
bool QImageReader::jumpToImage(int imageNumber)
{
    if (!d->initHandler())
        return false;
    return d->handler->jumpToImage(imageNumber);
}

/*!
    For image formats that support animation, this function returns
    the number of times the animation should loop. Otherwise, it
    returns -1.

    \sa supportsAnimation(), QImageIOHandler::loopCount()
*/
int QImageReader::loopCount() const
{
    if (!d->initHandler())
        return -1;
    return d->handler->loopCount();
}

/*!
    For image formats that support animation, this function returns
    the total number of images in the animation.

    Certain animation formats do not support this feature, in which
    case 0 is returned.

    \sa supportsAnimation(), QImageIOHandler::imageCount()
*/
int QImageReader::imageCount() const
{
    if (!d->initHandler())
        return -1;
    return d->handler->imageCount();
}

/*!
    For image formats that support animation, this function returns
    the number of milliseconds to wait until displaying the next frame
    in the animation. Otherwise, 0 is returned.

    \sa supportsAnimation(), QImageIOHandler::nextImageDelay()
*/
int QImageReader::nextImageDelay() const
{
    if (!d->initHandler())
        return -1;
    return d->handler->nextImageDelay();
}

/*!
    For image formats that support animation, this function returns
    the sequence number of the current frame. Otherwise, -1 is
    returned.

    \sa supportsAnimation(), QImageIOHandler::currentImageNumber()
*/
int QImageReader::currentImageNumber() const
{
    if (!d->initHandler())
        return -1;
    return d->handler->currentImageNumber();
}

/*!
    For image formats that support animation, this function returns
    the rect for the current frame. Otherwise, a null rect is returned.

    \sa supportsAnimation(), QImageIOHandler::currentImageRect()
*/
QRect QImageReader::currentImageRect() const
{
    if (!d->initHandler())
        return QRect();
    return d->handler->currentImageRect();
}

/*!
    Returns the type of error that occurred last.

    \sa ImageReaderError, errorString()
*/
QImageReader::ImageReaderError QImageReader::error() const
{
    return d->imageReaderError;
}

/*!
    Returns a human readable description of the last error that
    occurred.

    \sa error()
*/
QString QImageReader::errorString() const
{
    return d->errorString;
}

/*!
    \since 4.2

    Returns true if the reader supports \a option; otherwise returns
    false.

    Different image formats support different options. Call this function to
    determine whether a certain option is supported by the current format. For
    example, the PNG format allows you to embed text into the image's metadata
    (see text()), and the BMP format allows you to determine the image's size
    without loading the whole image into memory (see size()).

    \snippet doc/src/snippets/code/src.gui.image.qimagereader.cpp 3

    \sa QImageWriter::supportsOption()
*/
bool QImageReader::supportsOption(QImageIOHandler::ImageOption option) const
{
    if (!d->initHandler())
        return false;
    return d->handler->supportsOption(option);
}

/*!
    If supported, this function returns the image format of the file
    \a fileName. Otherwise, an empty string is returned.
*/
QByteArray QImageReader::imageFormat(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return QByteArray();

    return imageFormat(&file);
}

/*!
    If supported, this function returns the image format of the device
    \a device. Otherwise, an empty string is returned.
*/
QByteArray QImageReader::imageFormat(QIODevice *device)
{
    QByteArray format;
    QImageIOHandler *handler = createReadHandlerHelper(device, format);
    if (handler) {
        if (handler->canRead())
            format = handler->format();
        delete handler;
    }
    return format;
}

/*!
    Returns the list of image formats supported by QImageReader.

    By default, Qt can read the following formats:

    \table
    \header \o Format \o Description
    \row    \o BMP    \o Windows Bitmap
    \row    \o GIF    \o Graphic Interchange Format (optional)
    \row    \o JPG    \o Joint Photographic Experts Group
    \row    \o JPEG   \o Joint Photographic Experts Group
    \row    \o MNG    \o Multiple-image Network Graphics
    \row    \o PNG    \o Portable Network Graphics
    \row    \o PBM    \o Portable Bitmap
    \row    \o PGM    \o Portable Graymap
    \row    \o PPM    \o Portable Pixmap
    \row    \o TIFF   \o Tagged Image File Format
    \row    \o XBM    \o X11 Bitmap
    \row    \o XPM    \o X11 Pixmap
    \endtable

    Reading and writing SVG files is supported through Qt's
    \l{QtSvg Module}{SVG Module}.

    To configure Qt with GIF support, pass \c -qt-gif to the \c
    configure script or check the appropriate option in the graphical
    installer.

    \sa setFormat(), QImageWriter::supportedImageFormats(), QImageIOPlugin
*/
QList<QByteArray> QImageReader::supportedImageFormats()
{
    QSet<QByteArray> formats;
    for (int i = 0; i < _qt_NumFormats; ++i)
        formats << _qt_BuiltInFormats[i].extension;

#if !defined (QT_NO_LIBRARY) && !defined(QT_NO_SETTINGS)
    QFactoryLoader *l = loader();
    QStringList keys = l->keys();

    for (int i = 0; i < keys.count(); ++i) {
        QImageIOPlugin *plugin = qobject_cast<QImageIOPlugin *>(l->instance(keys.at(i)));
        if (plugin && plugin->capabilities(0, keys.at(i).toLatin1()) & QImageIOPlugin::CanRead)
            formats << keys.at(i).toLatin1();
    }
#endif // QT_NO_LIBRARY

    QList<QByteArray> sortedFormats;
    for (QSet<QByteArray>::ConstIterator it = formats.constBegin(); it != formats.constEnd(); ++it)
        sortedFormats << *it;

    qSort(sortedFormats);
    return sortedFormats;
}

QT_END_NAMESPACE
