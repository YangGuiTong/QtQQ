/****************************************************************************
** Meta object code from reading C++ file 'TalkWindowSheel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../TalkWindowSheel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TalkWindowSheel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TalkWindowSheel_t {
    QByteArrayData data[14];
    char stringdata0[168];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TalkWindowSheel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TalkWindowSheel_t qt_meta_stringdata_TalkWindowSheel = {
    {
QT_MOC_LITERAL(0, 0, 15), // "TalkWindowSheel"
QT_MOC_LITERAL(1, 16, 19), // "onEmotionBtnClicked"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 16), // "updateSendTcpMsg"
QT_MOC_LITERAL(4, 54, 8), // "QString&"
QT_MOC_LITERAL(5, 63, 7), // "strData"
QT_MOC_LITERAL(6, 71, 4), // "int&"
QT_MOC_LITERAL(7, 76, 7), // "msgType"
QT_MOC_LITERAL(8, 84, 5), // "sFile"
QT_MOC_LITERAL(9, 90, 23), // "onTalkWindowItemClicked"
QT_MOC_LITERAL(10, 114, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(11, 131, 4), // "item"
QT_MOC_LITERAL(12, 136, 20), // "onEmotionItemClicked"
QT_MOC_LITERAL(13, 157, 10) // "emotionNum"

    },
    "TalkWindowSheel\0onEmotionBtnClicked\0"
    "\0updateSendTcpMsg\0QString&\0strData\0"
    "int&\0msgType\0sFile\0onTalkWindowItemClicked\0"
    "QListWidgetItem*\0item\0onEmotionItemClicked\0"
    "emotionNum"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TalkWindowSheel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       3,    3,   42,    2, 0x0a /* Public */,
       3,    2,   49,    2, 0x2a /* Public | MethodCloned */,
       9,    1,   54,    2, 0x08 /* Private */,
      12,    1,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6, QMetaType::QString,    5,    7,    8,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6,    5,    7,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::Int,   13,

       0        // eod
};

void TalkWindowSheel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TalkWindowSheel *_t = static_cast<TalkWindowSheel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onEmotionBtnClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->updateSendTcpMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 2: _t->updateSendTcpMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->onTalkWindowItemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->onEmotionItemClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject TalkWindowSheel::staticMetaObject = {
    { &BasicWindow::staticMetaObject, qt_meta_stringdata_TalkWindowSheel.data,
      qt_meta_data_TalkWindowSheel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TalkWindowSheel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TalkWindowSheel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TalkWindowSheel.stringdata0))
        return static_cast<void*>(this);
    return BasicWindow::qt_metacast(_clname);
}

int TalkWindowSheel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BasicWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
