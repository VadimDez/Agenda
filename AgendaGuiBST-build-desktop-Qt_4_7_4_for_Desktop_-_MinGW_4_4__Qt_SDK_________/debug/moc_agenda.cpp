/****************************************************************************
** Meta object code from reading C++ file 'agenda.h'
**
** Created: Sun 15. Apr 20:58:52 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AgendaGuiBST/agenda.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'agenda.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Agenda[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,    8,    7,    7, 0x08,
      40,    7,    7,    7, 0x08,
      69,   64,    7,    7, 0x08,
     119,   64,    7,    7, 0x08,
     163,   64,    7,    7, 0x08,
     212,  207,    7,    7, 0x08,
     239,  207,    7,    7, 0x08,
     270,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Agenda[] = {
    "Agenda\0\0date\0on_calendar_clicked(QDate)\0"
    "on_buttonEdit_clicked()\0item\0"
    "on_listWidget_itemDoubleClicked(QListWidgetItem*)\0"
    "on_listWidget_itemChanged(QListWidgetItem*)\0"
    "on_listWidget_itemClicked(QListWidgetItem*)\0"
    "arg1\0on_hours_valueChanged(int)\0"
    "on_spinBox_2_valueChanged(int)\0"
    "on_pushButton_clicked()\0"
};

const QMetaObject Agenda::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Agenda,
      qt_meta_data_Agenda, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Agenda::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Agenda::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Agenda::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Agenda))
        return static_cast<void*>(const_cast< Agenda*>(this));
    return QWidget::qt_metacast(_clname);
}

int Agenda::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_calendar_clicked((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 1: on_buttonEdit_clicked(); break;
        case 2: on_listWidget_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 3: on_listWidget_itemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 4: on_listWidget_itemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 5: on_hours_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: on_spinBox_2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: on_pushButton_clicked(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
