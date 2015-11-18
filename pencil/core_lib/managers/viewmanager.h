#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <QTransform>
#include "basemanager.h"


class ViewManager : public BaseManager
{
    Q_OBJECT
public:
    explicit ViewManager(QObject *parent = 0);
    bool init() override;
    
    QTransform getView();
    void resetView();

    QPointF mapCanvasToScreen( QPointF p );
    QPointF mapScreenToCanvas( QPointF p );

    QRectF mapCanvasToScreen( const QRectF& rect );
    QRectF mapScreenToCanvas( const QRectF& rect );

    QPainterPath mapCanvasToScreen( const QPainterPath& path );
    QPainterPath mapScreenToCanvas( const QPainterPath& path );

    QPointF translation() { return mTranslate; }
    void translate( float dx, float dy );
    void translate( QPointF offset );

    float rotation() { return mRotate; }
    void rotate( float degree );

    float scaling() { return mScale; }
    void scale( float scaleValue );

    void flipHorizontal( bool b ) { mIsFlipHorizontal = b; }
    void flipVertical( bool b ){ mIsFlipVertical = b; }
    bool isFlipHorizontal() { return mIsFlipHorizontal; }
    bool isFlipVertical() { return mIsFlipVertical; }

    void setCanvasSize( QSize size );

private:
    QTransform createViewTransform();

    QTransform mView;
    QTransform mCentre;

    QPointF mTranslate;
    float mRotate = 0.f;
    float mScale = 1.f;

    QSize mCanvasSize = { 1, 1 };

    bool mIsFlipHorizontal = false;
    bool mIsFlipVertical = false;
};

#endif // VIEWMANAGER_H
