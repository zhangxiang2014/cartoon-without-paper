#include <QtWidgets>

#include <cmath>
#include <stdlib.h>
#include <QMessageBox>
#include <QtPlugin>
#include "drawplugin.h"
//用这种回调函数返回QWidget组件不能被使用
void DrawPlugin::LoadUI()
{

}


//! [0]
DrawPlugin::DrawPlugin(QObject *parent)
    : QObject(parent)
    , initialized(false)
{
}
//! [0]

//! [1]
void DrawPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}
//! [1]

//! [2]
bool DrawPlugin::isInitialized() const
{
    return initialized;
}
//! [2]

//! [3]
//! can forceChange
QWidget *DrawPlugin::createWidget(QWidget *parent)
{

    pencil = new MainWindow2(parent);
    return pencil;
}

//! [3]
void DrawPlugin::setBottomUI(QWidget *videoUI)
{
    videoUI->setObjectName( "TimeLine" );
    pencil->addDockWidget(Qt::BottomDockWidgetArea, (QDockWidget*)videoUI);
    videoUI->show();
}

//! [4]
QString DrawPlugin::name() const
{
    return "AnalogClock";
}
//! [4]

//! [5]
QString DrawPlugin::group() const
{
    return "Display Widgets [Examples]";
}
//! [5]

//! [6]
QIcon DrawPlugin::icon() const
{
    return QIcon();
}
//! [6]

//! [7]
QString DrawPlugin::toolTip() const
{
    return "";
}
//! [7]

//! [8]
QString DrawPlugin::whatsThis() const
{
    return "";
}
//! [8]

//! [9]
bool DrawPlugin::isContainer() const
{
    return false;
}
//! [9]

//! [10]
QString DrawPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"AnalogClock\" name=\"analogClock\">\n"
//! [11]
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>400</width>\n"
           "    <height>400</height>\n"
           "   </rect>\n"
           "  </property>\n"
//! [11]
           "  <property name=\"toolTip\" >\n"
           "   <string>The current time</string>\n"
           "  </property>\n"
           "  <property name=\"whatsThis\" >\n"
           "   <string>The analog clock widget displays the current time.</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//! [10]

//! [12]
QString DrawPlugin::includeFile() const
{
    return "mainwindow2.h";
}
//! [12]

