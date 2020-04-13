/****************************************************************************
** Meta object code from reading C++ file 'kline_wall.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../kline_wall.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kline_wall.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_KLineWall_t {
    QByteArrayData data[7];
    char stringdata[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_KLineWall_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_KLineWall_t qt_meta_stringdata_KLineWall = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 14),
QT_MOC_LITERAL(2, 25, 0),
QT_MOC_LITERAL(3, 26, 23),
QT_MOC_LITERAL(4, 50, 15),
QT_MOC_LITERAL(5, 66, 20),
QT_MOC_LITERAL(6, 87, 16)
    },
    "KLineWall\0sigUpdateKwall\0\0"
    "slotOpenRelatedSubKwall\0slotUpdateKwall\0"
    "slotOpenStatisticDlg\0slotZoominSelect\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KLineWall[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       3,    1,   40,    2, 0x0a,
       4,    0,   43,    2, 0x0a,
       5,    1,   44,    2, 0x08,
       6,    1,   47,    2, 0x08,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void KLineWall::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        KLineWall *_t = static_cast<KLineWall *>(_o);
        switch (_id) {
        case 0: _t->sigUpdateKwall(); break;
        case 1: _t->slotOpenRelatedSubKwall((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->slotUpdateKwall(); break;
        case 3: _t->slotOpenStatisticDlg((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->slotZoominSelect((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (KLineWall::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&KLineWall::sigUpdateKwall)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject KLineWall::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_KLineWall.data,
      qt_meta_data_KLineWall,  qt_static_metacall, 0, 0}
};


const QMetaObject *KLineWall::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KLineWall::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KLineWall.stringdata))
        return static_cast<void*>(const_cast< KLineWall*>(this));
    return QWidget::qt_metacast(_clname);
}

int KLineWall::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void KLineWall::sigUpdateKwall()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
