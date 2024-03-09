/****************************************************************************
** Meta object code from reading C++ file 'QtQQ_Server.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../QtQQ_Server.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtQQ_Server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtQQ_Server_t {
    QByteArrayData data[11];
    char stringdata0[175];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtQQ_Server_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtQQ_Server_t qt_meta_stringdata_QtQQ_Server = {
    {
QT_MOC_LITERAL(0, 0, 11), // "QtQQ_Server"
QT_MOC_LITERAL(1, 12, 13), // "onUDPbroadMsg"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 11), // "QByteArray&"
QT_MOC_LITERAL(4, 39, 6), // "btData"
QT_MOC_LITERAL(5, 46, 9), // "onRefresh"
QT_MOC_LITERAL(6, 56, 29), // "on_queryDepartmentBtn_clicked"
QT_MOC_LITERAL(7, 86, 21), // "on_queryIDBtn_clicked"
QT_MOC_LITERAL(8, 108, 20), // "on_logoutBtn_clicked"
QT_MOC_LITERAL(9, 129, 27), // "on_selectPictureBtn_clicked"
QT_MOC_LITERAL(10, 157, 17) // "on_addBtn_clicked"

    },
    "QtQQ_Server\0onUDPbroadMsg\0\0QByteArray&\0"
    "btData\0onRefresh\0on_queryDepartmentBtn_clicked\0"
    "on_queryIDBtn_clicked\0on_logoutBtn_clicked\0"
    "on_selectPictureBtn_clicked\0"
    "on_addBtn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtQQ_Server[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    0,   55,    2, 0x08 /* Private */,
       9,    0,   56,    2, 0x08 /* Private */,
      10,    0,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QtQQ_Server::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QtQQ_Server *_t = static_cast<QtQQ_Server *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onUDPbroadMsg((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: _t->onRefresh(); break;
        case 2: _t->on_queryDepartmentBtn_clicked(); break;
        case 3: _t->on_queryIDBtn_clicked(); break;
        case 4: _t->on_logoutBtn_clicked(); break;
        case 5: _t->on_selectPictureBtn_clicked(); break;
        case 6: _t->on_addBtn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject QtQQ_Server::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QtQQ_Server.data,
      qt_meta_data_QtQQ_Server,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtQQ_Server::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtQQ_Server::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtQQ_Server.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int QtQQ_Server::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
