/****************************************************************************
** Meta object code from reading C++ file 'FrQwtPlotWidget.h'
**
** Created: Fri 21. Nov 13:58:59 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrQwtPlotWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrQwtPlotWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrQwtPlotWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_FrQwtPlotWidget[] = {
    "FrQwtPlotWidget\0"
};

const QMetaObject FrQwtPlotWidget::staticMetaObject = {
    { &QwtPlot::staticMetaObject, qt_meta_stringdata_FrQwtPlotWidget,
      qt_meta_data_FrQwtPlotWidget, 0 }
};

const QMetaObject *FrQwtPlotWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrQwtPlotWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrQwtPlotWidget))
	return static_cast<void*>(const_cast< FrQwtPlotWidget*>(this));
    return QwtPlot::qt_metacast(_clname);
}

int FrQwtPlotWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtPlot::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}