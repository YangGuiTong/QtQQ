/****************************************************************************
** Meta object code from reading C++ file 'TitleBar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../TitleBar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TitleBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TitleBar_t {
    QByteArrayData data[10];
    char stringdata0[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TitleBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TitleBar_t qt_meta_stringdata_TitleBar = {
    {
QT_MOC_LITERAL(0, 0, 8), // "TitleBar"
QT_MOC_LITERAL(1, 9, 22), // "signalButtonMinClicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 26), // "signalButtonRestoreClicked"
QT_MOC_LITERAL(4, 60, 22), // "signalButtonMaxClicked"
QT_MOC_LITERAL(5, 83, 24), // "signalButtonCloseClicked"
QT_MOC_LITERAL(6, 108, 18), // "onButtonMinClicked"
QT_MOC_LITERAL(7, 127, 22), // "onButtonRestoreClicked"
QT_MOC_LITERAL(8, 150, 18), // "onButtonMaxClicked"
QT_MOC_LITERAL(9, 169, 20) // "onButtonCloseClicked"

    },
    "TitleBar\0signalButtonMinClicked\0\0"
    "signalButtonRestoreClicked\0"
    "signalButtonMaxClicked\0signalButtonCloseClicked\0"
    "onButtonMinClicked\0onButtonRestoreClicked\0"
    "onButtonMaxClicked\0onButtonCloseClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TitleBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    0,   56,    2, 0x06 /* Public */,
       5,    0,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TitleBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TitleBar *_t = static_cast<TitleBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalButtonMinClicked(); break;
        case 1: _t->signalButtonRestoreClicked(); break;
        case 2: _t->signalButtonMaxClicked(); break;
        case 3: _t->signalButtonCloseClicked(); break;
        case 4: _t->onButtonMinClicked(); break;
        case 5: _t->onButtonRestoreClicked(); break;
        case 6: _t->onButtonMaxClicked(); break;
        case 7: _t->onButtonCloseClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (TitleBar::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TitleBar::signalButtonMinClicked)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TitleBar::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TitleBar::signalButtonRestoreClicked)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TitleBar::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TitleBar::signalButtonMaxClicked)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (TitleBar::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TitleBar::signalButtonCloseClicked)) {
                *result = 3;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject TitleBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TitleBar.data,
      qt_meta_data_TitleBar,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TitleBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TitleBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TitleBar.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TitleBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void TitleBar::signalButtonMinClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TitleBar::signalButtonRestoreClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TitleBar::signalButtonMaxClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TitleBar::signalButtonCloseClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
