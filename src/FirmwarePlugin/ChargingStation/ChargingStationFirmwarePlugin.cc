#include "PX4ParameterMetaData.h"

#include "ChargingStationFirmwarePlugin.h"

ChargingStationFirmwarePlugin::ChargingStationFirmwarePlugin(void):
    _chargingStationFactGroup(this)
{
    _nameToFactGroupMap.insert("Charging Station", &_chargingStationFactGroup);
}

QString ChargingStationFirmwarePlugin::vehicleImageOpaque(const Vehicle* vehicle) const
{
    Q_UNUSED(vehicle);
    return QStringLiteral("/qmlimages/ChargingStation.svg");
}

QString ChargingStationFirmwarePlugin::vehicleImageOutline(const Vehicle* vehicle) const
{
    Q_UNUSED(vehicle);
    return QStringLiteral("/qmlimages/ChargingStation.svg");
}

bool ChargingStationFirmwarePlugin::isCapable(const Vehicle *vehicle, FirmwareCapabilities capabilities)
{
    Q_UNUSED(vehicle);
    return capabilities & SetFlightModeCapability;
}

QStringList ChargingStationFirmwarePlugin::flightModes(Vehicle* vehicle)
{
    Q_UNUSED(vehicle);

    QStringList flightModes;
    flightModes << tr("Stop") << tr("Open") << tr("Closed") << tr("Load drone") << tr("Unload drone") << tr("Get from user") << tr("Unload to user") << tr("Change battery") << tr("Reset") << tr("Service");
    return flightModes;
}

bool ChargingStationFirmwarePlugin::setFlightMode(const QString& flightMode, uint8_t* base_mode, uint32_t* custom_mode)
{
    *base_mode = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;

    if (flightMode.compare(tr("Open"), Qt::CaseInsensitive) == 0) {
        *custom_mode = CUSTOM_MODE_OPEN;
    } else if (flightMode.compare(tr("Closed"), Qt::CaseInsensitive) == 0) {
        *custom_mode = CUSTOM_MODE_CLOSED;
    } else if (flightMode.compare(tr("Load drone"), Qt::CaseInsensitive) == 0) {
        *custom_mode = CUSTOM_MODE_LOADING_DRONE;
    } else if (flightMode.compare(tr("Unload drone"), Qt::CaseInsensitive) == 0) {
        *custom_mode = CUSTOM_MODE_UNLOADING_DRONE;
    } else if (flightMode.compare(tr("Get from user"), Qt::CaseInsensitive) == 0) {
        *custom_mode = CUSTOM_MODE_GETTING_FROM_USER;
    } else if (flightMode.compare(tr("Unload to user"), Qt::CaseInsensitive) == 0) {
        *custom_mode = CUSTOM_MODE_UNLOADING_TO_USER;
    } else if (flightMode.compare(tr("Change battery"), Qt::CaseInsensitive) == 0) {
        *custom_mode = CUSTOM_MODE_CHANGING_BATTERY;
    } else if (flightMode.compare(tr("Reset"), Qt::CaseInsensitive) == 0) {
        *custom_mode = CUSTOM_MODE_RESET;
    } else if (flightMode.compare(tr("Service"), Qt::CaseInsensitive) == 0) {
        *custom_mode = CUSTOM_MODE_SERVICE;
    } else if (flightMode.compare(tr("Stop"), Qt::CaseInsensitive) == 0) {
        *custom_mode = CUSTOM_MODE_STOP;
    } else {
        return false;
    }

    return true;
}

QString ChargingStationFirmwarePlugin::flightMode(uint8_t base_mode, uint32_t custom_mode) const
{
    if (base_mode & MAV_MODE_FLAG_CUSTOM_MODE_ENABLED) {
        switch (custom_mode) {
            case CUSTOM_MODE_OPEN:
                return tr("Open");
            case CUSTOM_MODE_OPENING:
                return tr("Opening");
            case CUSTOM_MODE_CLOSED:
                return tr("Closed");
            case CUSTOM_MODE_CLOSING:
                return tr("Closing");
            case CUSTOM_MODE_LOADING_DRONE:
                return tr("Loading drone");
            case CUSTOM_MODE_UNLOADING_DRONE:
                return tr("Unloading drone");
            case CUSTOM_MODE_GETTING_FROM_USER:
                return tr("Getting from user");
            case CUSTOM_MODE_UNLOADING_TO_USER:
                return tr("Unloading to user");
            case CUSTOM_MODE_CHANGING_BATTERY:
                return tr("Changing battery");
            case CUSTOM_MODE_SERVICE:
                return tr("Service");
            case CUSTOM_MODE_ERROR:
                return tr("Error");
            case CUSTOM_MODE_STANDBY:
                return tr("Standby");
        }
    } else {
        qWarning() << "Charging station mode without custom mode enabled";
    }

    return tr("Unknown");
}

const QVariantList& ChargingStationFirmwarePlugin::toolBarIndicators(const Vehicle* vehicle)
{
    Q_UNUSED(vehicle);

    if(_toolBarIndicatorList.size() == 0) {
        _toolBarIndicatorList.append(QVariant::fromValue(QUrl::fromUserInput("qrc:/toolbar/MessageIndicator.qml")));
        _toolBarIndicatorList.append(QVariant::fromValue(QUrl::fromUserInput("qrc:/toolbar/GPSIndicator.qml")));
        _toolBarIndicatorList.append(QVariant::fromValue(QUrl::fromUserInput("qrc:/toolbar/BatteryIndicator.qml")));
        _toolBarIndicatorList.append(QVariant::fromValue(QUrl::fromUserInput("qrc:/toolbar/ModeIndicator.qml")));
        _toolBarIndicatorList.append(QVariant::fromValue(QUrl::fromUserInput("qrc:/toolbar/GPSRTKIndicator.qml")));
    }
    return _toolBarIndicatorList;
}

FactMetaData* ChargingStationFirmwarePlugin::getMetaDataForFact(QObject* parameterMetaData, const QString& name, MAV_TYPE vehicleType)
{
    PX4ParameterMetaData* px4MetaData = qobject_cast<PX4ParameterMetaData*>(parameterMetaData);

    if (px4MetaData) {
        return px4MetaData->getMetaDataForFact(name, vehicleType);
    } else {
        qWarning() << "Internal error: pointer passed to PX4FirmwarePlugin::getMetaDataForFact not PX4ParameterMetaData";
    }

    return NULL;
}

void ChargingStationFirmwarePlugin::addMetaDataToFact(QObject* parameterMetaData, Fact* fact, MAV_TYPE vehicleType)
{
    PX4ParameterMetaData* px4MetaData = qobject_cast<PX4ParameterMetaData*>(parameterMetaData);

    if (px4MetaData) {
        px4MetaData->addMetaDataToFact(fact, vehicleType);
    } else {
        qWarning() << "Internal error: pointer passed to PX4FirmwarePlugin::addMetaDataToFact not PX4ParameterMetaData";
    }
}

void ChargingStationFirmwarePlugin::getParameterMetaDataVersionInfo(const QString& metaDataFile, int& majorVersion, int& minorVersion)
{
    return PX4ParameterMetaData::getParameterMetaDataVersionInfo(metaDataFile, majorVersion, minorVersion);
}

QObject* ChargingStationFirmwarePlugin::loadParameterMetaData(const QString& metaDataFile)
{
    PX4ParameterMetaData* metaData = new PX4ParameterMetaData;
    if (!metaDataFile.isEmpty()) {
        metaData->loadParameterFactMetaDataFile(metaDataFile);
    }
    return metaData;
}

void ChargingStationFirmwarePlugin::_handleNamedValueInt(mavlink_message_t* message)
{
    mavlink_named_value_int_t namedValueInt;
    mavlink_msg_named_value_int_decode(message, &namedValueInt);

    if (QString(namedValueInt.name) == "rtk_surv")
        _chargingStationFactGroup.rtkSurveyIn()->setRawValue(namedValueInt.value + 1) ;
}

void ChargingStationFirmwarePlugin::_handleNamedValueFloat(mavlink_message_t *message)
{
    mavlink_named_value_float_t namedValueFloat;
    mavlink_msg_named_value_float_decode(message, &namedValueFloat);

    if (QString(namedValueFloat.name) == "rain")
        _chargingStationFactGroup.weatherRain()->setRawValue(namedValueFloat.value);
    else if (QString(namedValueFloat.name) == "humidity")
        _chargingStationFactGroup.weatherHumidity()->setRawValue(namedValueFloat.value * 100);
}

void ChargingStationFirmwarePlugin::_handleMavlinkMessage(mavlink_message_t* message)
{
    switch (message->msgid) {
    // Charging station RTK Survey In status message
    case MAVLINK_MSG_ID_NAMED_VALUE_INT:
        _handleNamedValueInt(message);
        break;
    case MAVLINK_MSG_ID_NAMED_VALUE_FLOAT:
        _handleNamedValueFloat(message);
        break;
    }
}

QMap<QString, FactGroup*>* ChargingStationFirmwarePlugin::factGroups(void) {
    return &_nameToFactGroupMap;
}

bool ChargingStationFirmwarePlugin::adjustIncomingMavlinkMessage(Vehicle* vehicle, mavlink_message_t* message)
{
    Q_UNUSED(vehicle);

    _handleMavlinkMessage(message);
    return true;
}

const char* ChargingStationFactGroup::_rtkSurveyInFactName = "rtkSurveyIn";
const char* ChargingStationFactGroup::_weatherRainFactName = "weatherRain";
const char* ChargingStationFactGroup::_weatherHumidityFactName = "weatherHumidity";

ChargingStationFactGroup::ChargingStationFactGroup(QObject* parent):
    FactGroup(1000, ":/json/Vehicle/ChargingStationFact.json", parent),
    _rtkSurveyInFact(0, _rtkSurveyInFactName, FactMetaData::valueTypeInt32),
    _weatherRainFact(0, _weatherRainFactName, FactMetaData::valueTypeFloat),
    _weatherHumidityFact(0, _weatherHumidityFactName, FactMetaData::valueTypeFloat)
{
    _addFact(&_rtkSurveyInFact, _rtkSurveyInFactName);
    _addFact(&_weatherRainFact, _weatherRainFactName);
    _addFact(&_weatherHumidityFact, _weatherHumidityFactName);

    _weatherRainFact.setRawValue(std::numeric_limits<float>::quiet_NaN());
    _weatherHumidityFact.setRawValue(std::numeric_limits<float>::quiet_NaN());
}
