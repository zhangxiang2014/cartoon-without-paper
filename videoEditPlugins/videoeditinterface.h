#ifndef VIDEOEDITINTERFACE
#define VIDEOEDITINTERFACE
#define VideoEditInterface_iid "anobodykey.VideoEditInterface/2.0"
//声明纯虚函数的一般形式是 virtual 函数类型 函数名 (参数表列) =0;
#include "qdockwidget.h"
class VideoEditInterface
{
public:
    virtual ~VideoEditInterface(){}
    virtual void showVideoEditMain(void)=0;
    virtual QDockWidget* getTheDialog()=0;
    virtual QDockWidget* getDockWidget()=0;

};
Q_DECLARE_INTERFACE(VideoEditInterface, VideoEditInterface_iid)
#endif // VIDEOEDITINTERFACE

