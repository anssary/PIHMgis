/****************************************************************************
** Meta object code from reading C++ file 'importproject.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "1ProjectManagement/3ImportProject/importproject.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'importproject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ImportProject_t {
    QByteArrayData data[6];
    char stringdata[126];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ImportProject_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ImportProject_t qt_meta_stringdata_ImportProject = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 25),
QT_MOC_LITERAL(2, 40, 0),
QT_MOC_LITERAL(3, 41, 27),
QT_MOC_LITERAL(4, 69, 26),
QT_MOC_LITERAL(5, 96, 28)
    },
    "ImportProject\0on_pushButtonHelp_clicked\0"
    "\0on_pushButtonImport_clicked\0"
    "on_pushButtonClose_clicked\0"
    "on_pushButtonProject_clicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImportProject[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08,
       3,    0,   35,    2, 0x08,
       4,    0,   36,    2, 0x08,
       5,    0,   37,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ImportProject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ImportProject *_t = static_cast<ImportProject *>(_o);
        switch (_id) {
        case 0: _t->on_pushButtonHelp_clicked(); break;
        case 1: _t->on_pushButtonImport_clicked(); break;
        case 2: _t->on_pushButtonClose_clicked(); break;
        case 3: _t->on_pushButtonProject_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ImportProject::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ImportProject.data,
      qt_meta_data_ImportProject,  qt_static_metacall, 0, 0}
};


const QMetaObject *ImportProject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImportProject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ImportProject.stringdata))
        return static_cast<void*>(const_cast< ImportProject*>(this));
    return QDialog::qt_metacast(_clname);
}

int ImportProject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
