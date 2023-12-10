/****************************************************************************
** Meta object code from reading C++ file 'BasicWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../BasicWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BasicWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BasicWindow_t {
    QByteArrayData data[13];
    char stringdata0[178];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BasicWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BasicWindow_t qt_meta_stringdata_BasicWindow = {
    {
QT_MOC_LITERAL(0, 0, 11), // "BasicWindow"
QT_MOC_LITERAL(1, 12, 11), // "onShowClose"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 9), // "onShowMin"
QT_MOC_LITERAL(4, 35, 10), // "onShowHide"
QT_MOC_LITERAL(5, 46, 12), // "onShowNormal"
QT_MOC_LITERAL(6, 59, 10), // "onShowQuit"
QT_MOC_LITERAL(7, 70, 19), // "onSingalSkinChanged"
QT_MOC_LITERAL(8, 90, 5), // "color"
QT_MOC_LITERAL(9, 96, 18), // "onButtonMinClicked"
QT_MOC_LITERAL(10, 115, 22), // "onButtonRestoreClicked"
QT_MOC_LITERAL(11, 138, 18), // "onButtonMaxClicked"
QT_MOC_LITERAL(12, 157, 20) // "onButtonCloseClicked"

    },
    "BasicWindow\0onShowClose\0\0onShowMin\0"
    "onShowHide\0onShowNormal\0onShowQuit\0"
    "onSingalSkinChanged\0color\0onButtonMinClicked\0"
    "onButtonRestoreClicked\0onButtonMaxClicked\0"
    "onButtonCloseClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BasicWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x0a /* Public */,
       3,    1,   67,    2, 0x0a /* Public */,
       4,    1,   70,    2, 0x0a /* Public */,
       5,    1,   73,    2, 0x0a /* Public */,
       6,    1,   76,    2, 0x0a /* Public */,
       7,    1,   79,    2, 0x0a /* Public */,
       9,    0,   82,    2, 0x0a /* Public */,
      10,    0,   83,    2, 0x0a /* Public */,
      11,    0,   84,    2, 0x0a /* Public */,
      12,    0,   85,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QColor,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BasicWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BasicWindow *_t = static_cast<BasicWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onShowClose((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->onShowMin((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->onShowHide((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onShowNormal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onShowQuit((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->onSingalSkinChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 6: _t->onButtonMinClicked(); break;
        case 7: _t->onButtonRestoreClicked(); break;
        case 8: _t->onButtonMaxClicked(); break;
        case 9: _t->onButtonCloseClicked(); break;
        default: ;
        }
    }
}

const QMetaObject BasicWindow::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_BasicWindow.data,
      qt_meta_data_BasicWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BasicWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BasicWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BasicWindow.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int BasicWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
