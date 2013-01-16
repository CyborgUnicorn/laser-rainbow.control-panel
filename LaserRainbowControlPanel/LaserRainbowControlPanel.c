
#include <stdio.h>
#include <stdint.h>

#include "libusb-1.0/libusb.h"

libusb_context *ptrContext;
libusb_device_handle *ptrDeviceHandle;

int main(int argc, char* argv[])
{
	const uint16_t vendorId = 0x03eb;
	uint32_t numDevices, i;

	libusb_device **deviceList = NULL;

	libusb_init(&ptrContext);

	numDevices = libusb_get_device_list(ptrContext, &deviceList);

	printf("Get device list, found %d devices\n", numDevices);

	ptrDeviceHandle = NULL;

	
	for ( i = 0; i < numDevices; ++i ) {
		libusb_device *device = NULL;
		struct libusb_device_descriptor desc;
		
		device = deviceList[i];

		libusb_get_device_descriptor(device, &desc);

		if ( desc.idVendor == vendorId ) {
			printf("Found device matching vendorId %x\n", vendorId);

			libusb_open(device, &ptrDeviceHandle);
			if ( ptrDeviceHandle != NULL ) {
				printf("Device open\n");
				break;
			} else {
				printf("Failed to open device\n");
			}
            
		}
	}

	libusb_free_device_list(deviceList, 0);

	if ( ptrDeviceHandle != NULL ) {
		printf("Sending command\n");
		
		libusb_control_transfer(ptrDeviceHandle, 0x40, 0x7, 0, 1, 0, 0, (uint16_t)5000);
		
		libusb_control_transfer(ptrDeviceHandle, 0x40, 0x7, 1, 2, 0, 0, (uint16_t)5000);

		// Close the device
		libusb_close(ptrDeviceHandle);
		ptrDeviceHandle = NULL;
	} else {
		printf("Found NO device matching vendorId %x\n", vendorId);
	}

	printf("Press any key to continue...");
	getchar();
	
	return 0;
}

