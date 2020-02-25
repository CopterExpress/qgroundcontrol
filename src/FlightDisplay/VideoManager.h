/****************************************************************************
 *
 *   (c) 2009-2019 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#ifndef VideoManager_H
#define VideoManager_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QUrl>

#include "QGCMAVLink.h"
#include "QGCLoggingCategory.h"
#include "VideoReceiver.h"
#include "QGCToolbox.h"
#include "SubtitleWriter.h"

Q_DECLARE_LOGGING_CATEGORY(VideoManagerLog)

class VideoSettings;
class Vehicle;
class Joystick;

class VideoManager : public QGCTool
{
    Q_OBJECT

public:
    VideoManager    (QGCApplication* app, QGCToolbox* toolbox);
    ~VideoManager   ();

    Q_PROPERTY(bool             hasVideo                READ    hasVideo                                    NOTIFY hasVideoChanged)
    Q_PROPERTY(bool             isGStreamer             READ    isGStreamer                                 NOTIFY isGStreamerChanged)
    Q_PROPERTY(bool             isVideoEnabled          READ    isVideoEnabled                              NOTIFY isVideoEnabledChanged)
    Q_PROPERTY(bool             isCsVideoEnabled        READ    isCsVideoEnabled                            NOTIFY isCsVideoEnabledChanged)
    Q_PROPERTY(VideoReceiver*   videoReceiver           READ    videoReceiver                               CONSTANT)
    Q_PROPERTY(VideoReceiver*   thermalVideoReceiver    READ    thermalVideoReceiver                        CONSTANT)
    Q_PROPERTY(double           aspectRatio             READ    aspectRatio                                 NOTIFY aspectRatioChanged)
    Q_PROPERTY(double           thermalAspectRatio      READ    thermalAspectRatio                          NOTIFY aspectRatioChanged)
    Q_PROPERTY(double           hfov                    READ    hfov                                        NOTIFY aspectRatioChanged)
    Q_PROPERTY(double           thermalHfov             READ    thermalHfov                                 NOTIFY aspectRatioChanged)
    Q_PROPERTY(bool             autoStreamConfigured    READ    autoStreamConfigured                        NOTIFY autoStreamConfiguredChanged)
    Q_PROPERTY(bool             hasThermal              READ    hasThermal                                  NOTIFY aspectRatioChanged)
    Q_PROPERTY(bool             uvcEnabled              READ    uvcEnabled                                  NOTIFY uvcEnabledChanged)
    Q_PROPERTY(QString          videoSourceID           READ    videoSourceID                               NOTIFY videoSourceIDChanged)

    bool        hasVideo            ();
    bool        isGStreamer         ();
    bool        isVideoEnabled      ();
    bool        isCsVideoEnabled    ();
    bool        isAutoStream        ();
    bool        isTaisync           () { return _isTaisync; }
    bool        fullScreen          () { return _fullScreen; }
    QString     videoSourceID       () { return _videoSourceID; }
    double      aspectRatio         ();
    double      thermalAspectRatio  ();
    double      hfov                ();
    double      thermalHfov         ();
    bool        autoStreamConfigured();
    bool        hasThermal          ();
    void        restartVideo        ();

    VideoReceiver*  videoReceiver           () { return _videoReceiver; }
    VideoReceiver*  thermalVideoReceiver    () { return _thermalVideoReceiver; }
    Vehicle*        getActiveVehicle        () { return _activeVehicle; }

#if defined(QGC_DISABLE_UVC)
    bool        uvcEnabled          () { return false; }
#else
    bool        uvcEnabled          ();
#endif

    void        setfullScreen       (bool f) { _fullScreen = f; emit fullScreenChanged(); }
    void        setIsTaisync        (bool t) { _isTaisync = t;  emit isTaisyncChanged(); }

    // Override from QGCTool
    void        setToolbox          (QGCToolbox *toolbox);

    Q_INVOKABLE void startVideo     ();
    Q_INVOKABLE void stopVideo      ();

signals:
    void hasVideoChanged            ();
    void isGStreamerChanged         ();
    void isVideoEnabledChanged      ();
    void isCsVideoEnabledChanged    ();
    void videoSourceIDChanged       ();
    void fullScreenChanged          ();
    void isAutoStreamChanged        ();
    void isTaisyncChanged           ();
    void aspectRatioChanged         ();
    void autoStreamConfiguredChanged();
    void uvcEnabledChanged          ();

private slots:
    void _videoSourceChanged        ();
    void _csVehicleIDChanged        ();
    void _csVideoSourceChanged      ();
    void _udpPortChanged            ();
    void _csUdpPortChanged          ();
    void _rtspUrlChanged            ();
    void _csRtspUrlChanged          ();
    void _tcpUrlChanged             ();
    void _csTcpUrlChanged           ();
    void _updateUVC                 ();
    void _setActiveVehicle          (Vehicle* vehicle);
    void _aspectRatioChanged        ();

private:
    void _updateSettings            ();

private:
    SubtitleWriter  _subtitleWriter;
    bool            _isTaisync              = false;
    VideoReceiver*  _videoReceiver          = nullptr;
    VideoReceiver*  _thermalVideoReceiver   = nullptr;
    VideoSettings*  _videoSettings          = nullptr;
    QString         _videoSourceID;
    bool            _fullScreen             = false;
    Vehicle*        _activeVehicle          = nullptr;
};

#endif
