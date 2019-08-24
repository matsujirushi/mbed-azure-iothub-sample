#include <mbed.h>
#include <EthernetInterface.h>
#include <ntp-client/NTPClient.h>
#include <MyDevice.h>

#define DLM	"\r\n"

#define LOOP_INTERVAL					(10)

NetworkInterface* _defaultSystemNetwork;

int main()
{
	printf(DLM "# START" DLM);

	printf("Initialize the network adapter." DLM);

	NetworkInterface* net;
#if defined TARGET_NUCLEO_F746ZG || defined TARGET_K64F
	EthernetInterface eth;
	printf("Connect ethernet." DLM);
	nsapi_error_t status = eth.connect();
	if (status != NSAPI_ERROR_OK) abort();
	net = &eth;
#elif defined TARGET_MTB_MXCHIP_EMW3166
	net = NetworkInterface::get_default_instance();
	if (net == NULL) abort();
	if (net->connect() != NSAPI_ERROR_OK) abort();
#else
#error This target board not supported.
#endif

	printf(" MAC    : %s" DLM, net->get_mac_address());
	printf(" IP     : %s" DLM, net->get_ip_address());
	printf(" Netmask: %s" DLM, net->get_netmask());
	printf(" Gateway: %s" DLM, net->get_gateway());

	printf("Synchronize time." DLM);

	NTPClient ntp(net);
	time_t timestamp = ntp.get_timestamp();
	printf(" Current time is %s" DLM, ctime(&timestamp));
	set_time(timestamp);

	printf("Setup AzureDeviceClient." DLM);
	_defaultSystemNetwork = net;
	MyDevice device;
#if not defined NO_LOGGING
	device.SetLogTrace(true);
#endif
	if (!device.ConnectIoTHub(MBED_CONF_APP_AZURE_IOTHUB_DEVICE_CONNECTION_STRING)) abort();

	while (true)
	{
		device.DoWork();
		wait_ms(LOOP_INTERVAL);
	}
}
