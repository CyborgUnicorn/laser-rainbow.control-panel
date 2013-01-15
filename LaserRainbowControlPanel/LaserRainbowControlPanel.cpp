
#include <stdio.h>
#include <cstdio>

#include "libusb-1.0/libusb.h"

libusb_context *ptrContext;
libusb_device_handle *ptrDeviceHandle;

//http://sourceforge.net/projects/libwdi/files/zadig/

int main(int argc, void* argv[])
{
	const unsigned short vendorId = 0x03eb;

	libusb_device **deviceList = nullptr;

	libusb_init(&ptrContext);

	uint32_t numDevices = libusb_get_device_list(ptrContext, &deviceList);

	printf("Get device list, found %d devices\n", numDevices);

	ptrDeviceHandle = nullptr;

	for ( uint32_t i = 0; i < numDevices; ++i ) {
		libusb_device *device = deviceList[i];

		libusb_device_descriptor desc;
		libusb_get_device_descriptor(device, &desc);

		if ( desc.idVendor == vendorId ) {
			printf("Found device matching vendorId %x\n", vendorId);

			libusb_open(device, &ptrDeviceHandle);
			if ( ptrDeviceHandle != nullptr ) {
				printf("Device open\n");
				break;
			} else {
				printf("Failed to open device\n");
			}
            
		}
	}

	libusb_free_device_list(deviceList, 0);

	if ( ptrDeviceHandle != nullptr ) {
		printf("Sending command\n");
		
		libusb_control_transfer(ptrDeviceHandle, 0x40, 0x7, 0, 1, 0, 0, (uint16_t)5000);
		
		libusb_control_transfer(ptrDeviceHandle, 0x40, 0x7, 1, 2, 0, 0, (uint16_t)5000);

		// Close the device
		libusb_close(ptrDeviceHandle);
		ptrDeviceHandle = nullptr;
	}

	std::puts("Press any key to continue...");
	std::getchar();
	
	return 0;
}

