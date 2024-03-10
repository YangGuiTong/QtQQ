/****************************************************************************
** Meta object code from reading C++ file 'SendFile.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../SendFile.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SendFile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SendFile_t {
    QByteArrayData data[10];
    char stringdata0[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SendFile_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SendFile_t qt_meta_stringdata_SendFile = {
    {
QT_MOC_LITERAL(0, 0, 8), // "SendFile"
QT_MOC_LITERAL(1, 9, 15), // "sendFileClicked"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 8), // "QString&"
QT_MOC_LITERAL(4, 35, 7), // "strData"
QT_MOC_LITERAL(5, 43, 4), // "int&"
QT_MOC_LITERAL(6, 48, 7), // "msgType"
QT_MOC_LITERAL(7, 56, 8), // "fileName"
QT_MOC_LITERAL(8, 65, 18), // "on_openBtn_clicked"
QT_MOC_LITERAL(9, 84, 18) // "on_sendBtn_clicked"

    },
    "SendFile\0sendFileClicked\0\0QString&\0"
    "strData\0int&\0msgType\0fileName\0"
    "on_openBtn_clicked\0on_sendBtn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SendFile[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   36,    2, 0x08 /* Private */,
       9,    0,   37,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, QMetaType::QString,    4,    6,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SendFile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SendFile *_t = static_cast<SendFile *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendFileClicked((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: _t->on_openBtn_clicked(); break;
        case 2: _t->on_sendBtn_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SendFile::*_t)(QString & , int & , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SendFile::sendFileClicked)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject SendFile::staticMetaObject = {
    { &BasicWindow::staticMetaObject, qt_meta_stringdata_SendFile.data,
      qt_meta_data_SendFile,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SendFile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SendFile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SendFile.stringdata0))
        return static_cast<void*>(this);
    return BasicWindow::qt_metacast(_clname);
}

int SendFile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BasicWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void SendFile::sendFileClicked(QString & _t1, int & _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
