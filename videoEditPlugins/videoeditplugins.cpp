#include "videoeditplugins.h"
#include "dialog.h"

VideoEditPlugins::VideoEditPlugins()
{
    w = new Dialog;
}

void VideoEditPlugins::showVideoEditMain()
{
    w->show();
}

QDockWidget* VideoEditPlugins::getTheDialog()
{
    return w;
}

QDockWidget *VideoEditPlugins::getDockWidget()
{
    return w->getDockWidget();
}
