/****************************************************************************
** Meta object code from reading C++ file 'qgraphicsitemanimation.h'
**
** Created: Thu Mar 5 18:57:43 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../graphicsview/qgraphicsitemanimation.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgraphicsitemanimation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QGraphicsItemAnimation[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      26,   24,   23,   23, 0x0a,
      41,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QGraphicsItemAnimation[] = {
    "QGraphicsItemAnimation\0\0x\0setStep(qreal)\0"
    "reset()\0"
};

const QMetaObject QGraphicsItemAnimation::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QGraphicsItemAnimation,
      qt_meta_data_QGraphicsItemAnimation, 0 }
};

const QMetaObject *QGraphicsItemAnimation::metaObject() const
{
    return &staticMetaObject;
}

void *QGraphicsItemAnimation::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QGraphicsItemAnimation))
        return static_cast<void*>(const_cast< QGraphicsItemAnimation*>(this));
    return QObject::qt_metacast(_clname);
}

int QGraphicsItemAnimation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setStep((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: reset(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
