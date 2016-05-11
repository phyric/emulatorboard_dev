/****************************************************************************
** Meta object code from reading C++ file 'RadTestWorker.h'
**
** Created: Tue Jul 23 16:35:00 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/RadTestWorker.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RadTestWorker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RadTestWorker[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      38,   32,   14,   14, 0x05,
      53,   14,   14,   14, 0x25,
      67,   65,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      83,   14,   14,   14, 0x08,
      93,   65,   14,   14, 0x0a,
     108,   65,   14,   14, 0x0a,
     123,   65,   14,   14, 0x0a,
     138,   65,   14,   14, 0x0a,
     153,   65,   14,   14, 0x0a,
     169,   65,   14,   14, 0x0a,
     185,   65,   14,   14, 0x0a,
     200,   65,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RadTestWorker[] = {
    "RadTestWorker\0\0counterchanged()\0level\0"
    "needReset(int)\0needReset()\0e\0"
    "f7_needed(bool)\0runTest()\0turnOnF0(bool)\0"
    "turnOnF1(bool)\0turnOnF2(bool)\0"
    "turnOnF3(bool)\0turnOnF4a(bool)\0"
    "turnOnF4b(bool)\0turnOnF5(bool)\0"
    "turnOnF7(bool)\0"
};

const QMetaObject RadTestWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RadTestWorker,
      qt_meta_data_RadTestWorker, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RadTestWorker::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RadTestWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RadTestWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RadTestWorker))
        return static_cast<void*>(const_cast< RadTestWorker*>(this));
    return QObject::qt_metacast(_clname);
}

int RadTestWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: counterchanged(); break;
        case 1: needReset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: needReset(); break;
        case 3: f7_needed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: runTest(); break;
        case 5: turnOnF0((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: turnOnF1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: turnOnF2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: turnOnF3((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: turnOnF4a((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: turnOnF4b((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: turnOnF5((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: turnOnF7((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void RadTestWorker::counterchanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void RadTestWorker::needReset(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 3
void RadTestWorker::f7_needed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
