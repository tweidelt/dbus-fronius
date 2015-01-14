#ifndef DBUS_INVERTER_SETTINGSBRIDGE_H
#define DBUS_INVERTER_SETTINGSBRIDGE_H

#include "dbus_bridge.h"

class InverterSettings;

class DBusInverterSettingsBridge : public DBusBridge
{
	Q_OBJECT
public:
	DBusInverterSettingsBridge(InverterSettings *settings, QObject *parent = 0);

protected:
	virtual bool toDBus(const QString &path, QVariant &v);

	virtual bool fromDBus(const QString &path, QVariant &v);
};

#endif // DBUS_INVERTER_SETTINGSBRIDGE_H
