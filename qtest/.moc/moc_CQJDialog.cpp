/****************************************************************************
** Meta object code from reading C++ file 'CQJDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CQJDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQJDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQJDialog_t {
    QByteArrayData data[10];
    char stringdata0[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CQJDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CQJDialog_t qt_meta_stringdata_CQJDialog = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CQJDialog"
QT_MOC_LITERAL(1, 10, 11), // "historySlot"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(4, 40, 4), // "item"
QT_MOC_LITERAL(5, 45, 7), // "execCmd"
QT_MOC_LITERAL(6, 53, 3), // "cmd"
QT_MOC_LITERAL(7, 57, 8), // "loadFile"
QT_MOC_LITERAL(8, 66, 13), // "loadVariables"
QT_MOC_LITERAL(9, 80, 13) // "loadFunctions"

    },
    "CQJDialog\0historySlot\0\0QListWidgetItem*\0"
    "item\0execCmd\0cmd\0loadFile\0loadVariables\0"
    "loadFunctions"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQJDialog[] = {

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
       1,    1,   39,    2, 0x08 /* Private */,
       5,    1,   42,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,
       8,    0,   46,    2, 0x08 /* Private */,
       9,    0,   47,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CQJDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQJDialog *_t = static_cast<CQJDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->historySlot((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->execCmd((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->loadFile(); break;
        case 3: _t->loadVariables(); break;
        case 4: _t->loadFunctions(); break;
        default: ;
        }
    }
}

const QMetaObject CQJDialog::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_CQJDialog.data,
      qt_meta_data_CQJDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CQJDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQJDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CQJDialog.stringdata0))
        return static_cast<void*>(const_cast< CQJDialog*>(this));
    return QFrame::qt_metacast(_clname);
}

int CQJDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
