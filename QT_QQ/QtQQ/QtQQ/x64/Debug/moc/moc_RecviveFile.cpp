/****************************************************************************
** Meta object code from reading C++ file 'RecviveFile.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../RecviveFile.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RecviveFile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RecviveFile_t {
    QByteArrayData data[5];
    char stringdata0[62];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RecviveFile_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RecviveFile_t qt_meta_stringdata_RecviveFile = {
    {
QT_MOC_LITERAL(0, 0, 11), // "RecviveFile"
QT_MOC_LITERAL(1, 12, 10), // "refuseFile"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 20), // "on_cancelBtn_clicked"
QT_MOC_LITERAL(4, 45, 16) // "on_okBtn_clicked"

    },
    "RecviveFile\0refuseFile\0\0on_cancelBtn_clicked\0"
    "on_okBtn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RecviveFile[] = {

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
       1,    0,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   30,    2, 0x08 /* Private */,
       4,    0,   31,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RecviveFile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RecviveFile *_t = static_cast<RecviveFile *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->refuseFile(); break;
        case 1: _t->on_cancelBtn_clicked(); break;
        case 2: _t->on_okBtn_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (RecviveFile::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RecviveFile::refuseFile)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject RecviveFile::staticMetaObject = {
    { &BasicWindow::staticMetaObject, qt_meta_stringdata_RecviveFile.data,
      qt_meta_data_RecviveFile,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RecviveFile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RecviveFile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RecviveFile.stringdata0))
        return static_cast<void*>(this);
    return BasicWindow::qt_metacast(_clname);
}

int RecviveFile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void RecviveFile::refuseFile()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
