
#include <QTimer>
#include "editor.h"
#include "layermanager.h"
#include "playbackmanager.h"

PlaybackManager::PlaybackManager( QObject* parent ) : BaseManager( parent )
{
}

bool PlaybackManager::init()
{
    m_pTimer = new QTimer( this );
    connect( m_pTimer, &QTimer::timeout, this, &PlaybackManager::timerTick );
    return true;
}

void PlaybackManager::play()
{
    int projectLength = editor()->layers()->projectLength();

    mStartFrame = ( m_isRangedPlayback ) ? m_markInFrame : 1;
    mEndFrame = ( m_isRangedPlayback ) ? m_markOutFrame : projectLength;

    if ( editor()->currentFrame() >= mEndFrame )
    {
        editor()->scrubTo( mStartFrame );
    }

    m_pTimer->setInterval( 1000.0f / m_fps );
    m_pTimer->start();
}

void PlaybackManager::stop()
{
    m_pTimer->stop();
}

void PlaybackManager::setFps( int fps )
{
    if ( m_fps != fps )
    {
        m_fps = fps;
        emit fpsChanged( m_fps );
    }
}

void PlaybackManager::timerTick()
{
    if ( editor()->currentFrame() > mEndFrame )
    {
        if ( m_isLooping )
        {
            editor()->scrubTo( mStartFrame );
        }
        else
        {
            stop();
        }
        return;
    }

    editor()->scrubTo( editor()->currentFrame() + 1 );

    // TODO: play sound if any
}

void PlaybackManager::setLooping( bool isLoop )
{
    if ( m_isLooping != isLoop )
    {
        m_isLooping = isLoop;
        emit loopStateChanged( m_isLooping );
    }
}

void PlaybackManager::enableRangedPlayback( bool b )
{
    if ( m_isRangedPlayback != b )
    {
        m_isRangedPlayback = b;
        emit rangedPlaybackStateChanged( m_isRangedPlayback );
    }
}


