#ifndef VIDEOEDITINTERFACE
#define VIDEOEDITINTERFACE

#include "qdockwidget.h"
class VideoEditInterface
{
public:
    virtual ~VideoEditInterface(){}
    virtual void showVideoEditMain(void)=0;
    virtual QDockWidget* getTheDialog()=0;
    virtual QDockWidget* getDockWidget()=0;

};

#define VideoEditInterface_iid "anobodykey.VideoEditInterface/2.0"

Q_DECLARE_INTERFACE(VideoEditInterface, VideoEditInterface_iid)

#endif // VIDEOEDITINTERFACE

