/****************************************************************************
** Meta object code from reading C++ file 'da_dialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../NetworkPlot/da_dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'da_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_da_dialog_t {
    QByteArrayData data[15];
    char stringdata0[285];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_da_dialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_da_dialog_t qt_meta_stringdata_da_dialog = {
    {
QT_MOC_LITERAL(0, 0, 9), // "da_dialog"
QT_MOC_LITERAL(1, 10, 15), // "da_trans_packet"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 14), // "da_trans_array"
QT_MOC_LITERAL(4, 42, 26), // "on_pushButton_load_clicked"
QT_MOC_LITERAL(5, 69, 21), // "on_buttonBox_accepted"
QT_MOC_LITERAL(6, 91, 20), // "on_buttonBox_clicked"
QT_MOC_LITERAL(7, 112, 16), // "QAbstractButton*"
QT_MOC_LITERAL(8, 129, 6), // "button"
QT_MOC_LITERAL(9, 136, 18), // "on_timer_over_time"
QT_MOC_LITERAL(10, 155, 25), // "on_net_notify_dac_hand_ok"
QT_MOC_LITERAL(11, 181, 21), // "on_buttonBox_rejected"
QT_MOC_LITERAL(12, 203, 26), // "on_pushButton_Send_clicked"
QT_MOC_LITERAL(13, 230, 27), // "on_pushButton_start_clicked"
QT_MOC_LITERAL(14, 258, 26) // "on_pushButton_stop_clicked"

    },
    "da_dialog\0da_trans_packet\0\0da_trans_array\0"
    "on_pushButton_load_clicked\0"
    "on_buttonBox_accepted\0on_buttonBox_clicked\0"
    "QAbstractButton*\0button\0on_timer_over_time\0"
    "on_net_notify_dac_hand_ok\0"
    "on_buttonBox_rejected\0on_pushButton_Send_clicked\0"
    "on_pushButton_start_clicked\0"
    "on_pushButton_stop_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_da_dialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       3,    1,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   75,    2, 0x08 /* Private */,
       5,    0,   76,    2, 0x08 /* Private */,
       6,    1,   77,    2, 0x08 /* Private */,
       9,    0,   80,    2, 0x08 /* Private */,
      10,    1,   81,    2, 0x08 /* Private */,
      11,    0,   84,    2, 0x08 /* Private */,
      12,    0,   85,    2, 0x08 /* Private */,
      13,    0,   86,    2, 0x08 /* Private */,
      14,    0,   87,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void da_dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        da_dialog *_t = static_cast<da_dialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->da_trans_packet((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: _t->da_trans_array((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->on_pushButton_load_clicked(); break;
        case 3: _t->on_buttonBox_accepted(); break;
        case 4: _t->on_buttonBox_clicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 5: _t->on_timer_over_time(); break;
        case 6: _t->on_net_notify_dac_hand_ok((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_buttonBox_rejected(); break;
        case 8: _t->on_pushButton_Send_clicked(); break;
        case 9: _t->on_pushButton_start_clicked(); break;
        case 10: _t->on_pushButton_stop_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (da_dialog::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&da_dialog::da_trans_packet)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (da_dialog::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&da_dialog::da_trans_array)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject da_dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_da_dialog.data,
      qt_meta_data_da_dialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *da_dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *da_dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_da_dialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int da_dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void da_dialog::da_trans_packet(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void da_dialog::da_trans_array(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE