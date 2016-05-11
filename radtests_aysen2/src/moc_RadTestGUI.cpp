/****************************************************************************
** Meta object code from reading C++ file 'RadTestGUI.h'
**
** Created: Tue Jul 23 16:35:01 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/RadTestGUI.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RadTestGUI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyQTextEdit[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_MyQTextEdit[] = {
    "MyQTextEdit\0"
};

const QMetaObject MyQTextEdit::staticMetaObject = {
    { &QTextEdit::staticMetaObject, qt_meta_stringdata_MyQTextEdit,
      qt_meta_data_MyQTextEdit, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyQTextEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyQTextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyQTextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyQTextEdit))
        return static_cast<void*>(const_cast< MyQTextEdit*>(this));
    return QTextEdit::qt_metacast(_clname);
}

int MyQTextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_FunctionBar[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_FunctionBar[] = {
    "FunctionBar\0"
};

const QMetaObject FunctionBar::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FunctionBar,
      qt_meta_data_FunctionBar, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FunctionBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FunctionBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FunctionBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FunctionBar))
        return static_cast<void*>(const_cast< FunctionBar*>(this));
    return QObject::qt_metacast(_clname);
}

int FunctionBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_RadTestGUI[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      20,   11,   11,   11, 0x08,
      32,   11,   11,   11, 0x08,
      48,   11,   11,   11, 0x08,
      64,   11,   11,   11, 0x08,
      77,   71,   11,   11, 0x08,
      97,   11,   11,   11, 0x28,
     116,  114,   11,   11, 0x08,
     146,  142,   11,   11, 0x08,
     166,  114,   11,   11, 0x08,
     181,  114,   11,   11, 0x08,
     196,  114,   11,   11, 0x08,
     211,  114,   11,   11, 0x08,
     226,  114,   11,   11, 0x08,
     242,  114,   11,   11, 0x08,
     258,  114,   11,   11, 0x08,
     273,  114,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RadTestGUI[] = {
    "RadTestGUI\0\0start()\0pausestep()\0"
    "insertComment()\0updatenumbers()\0stop()\0"
    "level\0resetRequested(int)\0resetRequested()\0"
    "e\0disableAllFunctions(bool)\0i,e\0"
    "turnOnBar(int,bool)\0turnOnF0(bool)\0"
    "turnOnF1(bool)\0turnOnF2(bool)\0"
    "turnOnF3(bool)\0turnOnF4a(bool)\0"
    "turnOnF4b(bool)\0turnOnF5(bool)\0"
    "turnOnF7(bool)\0"
};

const QMetaObject RadTestGUI::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RadTestGUI,
      qt_meta_data_RadTestGUI, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RadTestGUI::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RadTestGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RadTestGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RadTestGUI))
        return static_cast<void*>(const_cast< RadTestGUI*>(this));
    return QWidget::qt_metacast(_clname);
}

int RadTestGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: start(); break;
        case 1: pausestep(); break;
        case 2: insertComment(); break;
        case 3: updatenumbers(); break;
        case 4: stop(); break;
        case 5: resetRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: resetRequested(); break;
        case 7: disableAllFunctions((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: turnOnBar((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 9: turnOnF0((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: turnOnF1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: turnOnF2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: turnOnF3((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: turnOnF4a((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: turnOnF4b((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: turnOnF5((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: turnOnF7((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
