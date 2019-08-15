#include "MyDevice.h"

#define DLM	"\r\n"

MyDevice::MyDevice() : _Led(LED1, 0), _Button(USER_BUTTON)
{
	_ButtonValue = false;
	_PushCount = 0;
}

void MyDevice::DoWork()
{
	AzureDeviceClient::DoWork();

	bool preButtonValue = _ButtonValue;
	_ButtonValue = _Button.read() ? true : false;

	if (!preButtonValue && _ButtonValue)
	{
		_PushCount++;

		printf("Send D2C message." DLM);
		JSON_Value* telemetryValue = json_value_init_object();
		JSON_Object* telemetryObject = json_value_get_object(telemetryValue);
		json_object_dotset_number(telemetryObject, "pushCount", _PushCount);
		auto jsonString = json_serialize_to_string(telemetryValue);
		printf(" %s" DLM, jsonString);
		json_free_serialized_string(jsonString);
		SendTelemetryAsync(telemetryObject);
		json_value_free(telemetryValue);
	}
}

void MyDevice::DeviceTwinReceived(JSON_Object* deviceTwinObject)
{
	printf("Received DeviceTwin." DLM);
	auto jsonString = json_serialize_to_string(json_object_get_wrapping_value(deviceTwinObject));
	printf(" %s" DLM, jsonString);
	json_free_serialized_string(jsonString);
}

void MyDevice::DesiredPropertyReceived(JSON_Object* desiredObject)
{
	printf("Received DesiredProperty." DLM);
	auto jsonString = json_serialize_to_string(json_object_get_wrapping_value(desiredObject));
	printf(" %s" DLM, jsonString);
	json_free_serialized_string(jsonString);
}
