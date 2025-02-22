/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#ifndef QDATASTREAM_H
#define QDATASTREAM_H

#include <QtCore/qiodevice.h>
#include <QtCore/qglobal.h>

#ifdef Status
#error qdatastream.h must be included before any header file that defines Status
#endif

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Core)

class QByteArray;
class QIODevice;

template <typename T> class QList;
template <typename T> class QLinkedList;
template <typename T> class QVector;
template <typename T> class QSet;
template <class Key, class T> class QHash;
template <class Key, class T> class QMap;

class QDataStreamPrivate;

#ifndef QT_NO_DATASTREAM
class Q_CORE_EXPORT QDataStream
{
public:
    enum Version {
        Qt_1_0 = 1,
        Qt_2_0 = 2,
        Qt_2_1 = 3,
        Qt_3_0 = 4,
        Qt_3_1 = 5,
        Qt_3_3 = 6,
        Qt_4_0 = 7,
        Qt_4_1 = Qt_4_0,
        Qt_4_2 = 8,
        Qt_4_3 = 9,
        Qt_4_4 = 10
#if QT_VERSION >= 0x040500
#error Add Qt_4_5 = Qt_4_4
#endif
    };

    enum ByteOrder {
        BigEndian = QSysInfo::BigEndian,
        LittleEndian = QSysInfo::LittleEndian
    };

    enum Status {
        Ok,
        ReadPastEnd,
	ReadCorruptData
    };

    QDataStream();
    explicit QDataStream(QIODevice *);
#ifdef QT3_SUPPORT
    QDataStream(QByteArray *, int mode);
#endif
    QDataStream(QByteArray *, QIODevice::OpenMode flags);
    QDataStream(const QByteArray &);
    virtual ~QDataStream();

    QIODevice *device() const;
    void setDevice(QIODevice *);
    void unsetDevice();

    bool atEnd() const;
#ifdef QT3_SUPPORT
    inline QT3_SUPPORT bool eof() const { return atEnd(); }
#endif

    Status status() const;
    void setStatus(Status status);
    void resetStatus();

    ByteOrder byteOrder() const;
    void setByteOrder(ByteOrder);

    int version() const;
    void setVersion(int);

    QDataStream &operator>>(qint8 &i);
    QDataStream &operator>>(quint8 &i);
    QDataStream &operator>>(qint16 &i);
    QDataStream &operator>>(quint16 &i);
    QDataStream &operator>>(qint32 &i);
    QDataStream &operator>>(quint32 &i);
    QDataStream &operator>>(qint64 &i);
    QDataStream &operator>>(quint64 &i);

    QDataStream &operator>>(bool &i);
    QDataStream &operator>>(float &f);
    QDataStream &operator>>(double &f);
    QDataStream &operator>>(char *&str);

    QDataStream &operator<<(qint8 i);
    QDataStream &operator<<(quint8 i);
    QDataStream &operator<<(qint16 i);
    QDataStream &operator<<(quint16 i);
    QDataStream &operator<<(qint32 i);
    QDataStream &operator<<(quint32 i);
    QDataStream &operator<<(qint64 i);
    QDataStream &operator<<(quint64 i);
    QDataStream &operator<<(bool i);
    QDataStream &operator<<(float f);
    QDataStream &operator<<(double f);
    QDataStream &operator<<(const char *str);

    QDataStream &readBytes(char *&, uint &len);
    int readRawData(char *, int len);

    QDataStream &writeBytes(const char *, uint len);
    int writeRawData(const char *, int len);

    int skipRawData(int len);

#ifdef QT3_SUPPORT
    inline QT3_SUPPORT QDataStream &readRawBytes(char *str, uint len)
        { readRawData(str, static_cast<int>(len)); return *this; }
    inline QT3_SUPPORT QDataStream &writeRawBytes(const char *str, uint len)
        { writeRawData(str, static_cast<int>(len)); return *this; }
    inline QT3_SUPPORT bool isPrintableData() const { return false; }
    inline QT3_SUPPORT void setPrintableData(bool) {}
#endif

private:
    Q_DISABLE_COPY(QDataStream)

    QDataStreamPrivate *d;

    QIODevice *dev;
    bool owndev;
    bool noswap;
    ByteOrder byteorder;
    int ver;
    Status q_status;
};


/*****************************************************************************
  QDataStream inline functions
 *****************************************************************************/

inline QIODevice *QDataStream::device() const
{ return dev; }

inline QDataStream::ByteOrder QDataStream::byteOrder() const
{ return byteorder; }

inline int QDataStream::version() const
{ return ver; }

inline void QDataStream::setVersion(int v)
{ ver = v; }

inline QDataStream &QDataStream::operator>>(quint8 &i)
{ return *this >> reinterpret_cast<qint8&>(i); }

inline QDataStream &QDataStream::operator>>(quint16 &i)
{ return *this >> reinterpret_cast<qint16&>(i); }

inline QDataStream &QDataStream::operator>>(quint32 &i)
{ return *this >> reinterpret_cast<qint32&>(i); }

inline QDataStream &QDataStream::operator>>(quint64 &i)
{ return *this >> reinterpret_cast<qint64&>(i); }

inline QDataStream &QDataStream::operator<<(quint8 i)
{ return *this << qint8(i); }

inline QDataStream &QDataStream::operator<<(quint16 i)
{ return *this << qint16(i); }

inline QDataStream &QDataStream::operator<<(quint32 i)
{ return *this << qint32(i); }

inline QDataStream &QDataStream::operator<<(quint64 i)
{ return *this << qint64(i); }

template <typename T>
QDataStream& operator>>(QDataStream& s, QList<T>& l)
{
    l.clear();
    quint32 c;
    s >> c;
    for(quint32 i = 0; i < c; ++i)
    {
        T t;
        s >> t;
        l.append(t);
        if (s.atEnd())
            break;
    }
    return s;
}

template <typename T>
QDataStream& operator<<(QDataStream& s, const QList<T>& l)
{
    s << quint32(l.size());
    for (int i = 0; i < l.size(); ++i)
        s << l.at(i);
    return s;
}

template <typename T>
QDataStream& operator>>(QDataStream& s, QLinkedList<T>& l)
{
    l.clear();
    quint32 c;
    s >> c;
    for(quint32 i = 0; i < c; ++i)
    {
        T t;
        s >> t;
        l.append(t);
        if (s.atEnd())
            break;
    }
    return s;
}

template <typename T>
QDataStream& operator<<(QDataStream& s, const QLinkedList<T>& l)
{
    s << quint32(l.size());
    typename QLinkedList<T>::ConstIterator it = l.constBegin();
    for(; it != l.constEnd(); ++it)
        s << *it;
    return s;
}

template<typename T>
QDataStream& operator>>(QDataStream& s, QVector<T>& v)
{
    v.clear();
    quint32 c;
    s >> c;
    v.resize(c);
    for(quint32 i = 0; i < c; ++i) {
        T t;
        s >> t;
        v[i] = t;
    }
    return s;
}

template<typename T>
QDataStream& operator<<(QDataStream& s, const QVector<T>& v)
{
    s << quint32(v.size());
    for (typename QVector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
        s << *it;
    return s;
}

template <typename T>
QDataStream &operator>>(QDataStream &in, QSet<T> &set)
{
    set.clear();
    quint32 c;
    in >> c;
    for (quint32 i = 0; i < c; ++i) {
        T t;
        in >> t;
        set << t;
        if (in.atEnd())
            break;
    }
    return in;
}

template <typename T>
QDataStream& operator<<(QDataStream &out, const QSet<T> &set)
{
    out << quint32(set.size());
    typename QSet<T>::const_iterator i = set.constBegin();
    while (i != set.constEnd()) {
        out << *i;
        ++i;
    }
    return out;
}

template <class Key, class T>
Q_OUTOFLINE_TEMPLATE QDataStream &operator>>(QDataStream &in, QHash<Key, T> &hash)
{
    QDataStream::Status oldStatus = in.status();
    in.resetStatus();
    hash.clear();

    quint32 n;
    in >> n;

    for (quint32 i = 0; i < n; ++i) {
        if (in.status() != QDataStream::Ok)
            break;

        Key k;
        T t;
        in >> k >> t;
        hash.insertMulti(k, t);
    }

    if (in.status() != QDataStream::Ok)
        hash.clear();
    if (oldStatus != QDataStream::Ok)
        in.setStatus(oldStatus);
    return in;
}

template <class Key, class T>
Q_OUTOFLINE_TEMPLATE QDataStream &operator<<(QDataStream &out, const QHash<Key, T>& hash)
{
    out << quint32(hash.size());
    typename QHash<Key, T>::ConstIterator it = hash.end();
    typename QHash<Key, T>::ConstIterator begin = hash.begin();
    while (it != begin) {
        --it;
        out << it.key() << it.value();
    }
    return out;
}
#ifdef qdoc
template <class Key, class T>
Q_OUTOFLINE_TEMPLATE QDataStream &operator>>(QDataStream &in, QMap<Key, T> &map)
#else
template <class aKey, class aT>
Q_OUTOFLINE_TEMPLATE QDataStream &operator>>(QDataStream &in, QMap<aKey, aT> &map)
#endif
{
    QDataStream::Status oldStatus = in.status();
    in.resetStatus();
    map.clear();

    quint32 n;
    in >> n;

    map.detach();
    map.setInsertInOrder(true);
    for (quint32 i = 0; i < n; ++i) {
        if (in.status() != QDataStream::Ok)
            break;

        aKey key;
        aT value;
        in >> key >> value;
        map.insertMulti(key, value);
    }
    map.setInsertInOrder(false);
    if (in.status() != QDataStream::Ok)
        map.clear();
    if (oldStatus != QDataStream::Ok)
        in.setStatus(oldStatus);
    return in;
}

template <class Key, class T>
Q_OUTOFLINE_TEMPLATE QDataStream &operator<<(QDataStream &out, const QMap<Key, T> &map)
{
    out << quint32(map.size());
    typename QMap<Key, T>::ConstIterator it = map.end();
    typename QMap<Key, T>::ConstIterator begin = map.begin();
    while (it != begin) {
        --it;
        out << it.key() << it.value();
    }
    return out;
}

#endif // QT_NO_DATASTREAM

QT_END_NAMESPACE

QT_END_HEADER

#endif // QDATASTREAM_H
