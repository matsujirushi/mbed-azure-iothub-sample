#pragma once

#include <mbed.h>
#include <AzureDeviceClient.h>

class MyDevice : public AzureDeviceClient
{
private:
	DigitalOut _Led;
	DigitalIn _Button;

	bool _ButtonValue;
	int _PushCount;

public:
	MyDevice();
	virtual void DoWork();

protected:
	virtual void DeviceTwinReceived(JSON_Object* deviceTwinObject);
	virtual void DesiredPropertyReceived(JSON_Object* desiredObject);

};
