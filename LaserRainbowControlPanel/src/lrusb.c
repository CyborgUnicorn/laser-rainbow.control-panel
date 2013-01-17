#include <stdio.h>
#include <stdint.h>
#include "libusb.h"
#include "lrusb.h"

#define VALIDATE_CONTEXT_RETURN if(libusb_context_ptr==NULL){printf("No USB contect"); return 0;}
#define VALIDATE_CONTEXT if(libusb_context_ptr==NULL){printf("No USB contect"); return;}

libusb_context			*libusb_context_ptr = NULL;
uint16_t				lr_vendorIds[] = {
							0x03eb
						};

void lr_usb_init(){
	if ( libusb_context_ptr != NULL ){
		printf("LaserRainbow USB already initialized");
		return;
	}

	libusb_init(&libusb_context_ptr);
	if ( libusb_context_ptr == NULL )
		printf("Failed to initialize libusb");
}

void lr_usb_exit(){
	VALIDATE_CONTEXT;

	libusb_exit(libusb_context_ptr);
}

uint32_t lr_usb_get_devices( lr_device **device_list ){
	uint32_t			num_found_devices=0;
	libusb_device		**usb_device_list = NULL;
	libusb_device		*usb_device = NULL;
	uint32_t			num_usb_devices=0, 
						i=0,
						j=0;
	struct libusb_device_descriptor 
						usb_device_desc;
	
	VALIDATE_CONTEXT_RETURN;

	// Get a list of all USB devices and match them
	// agains the CyborgUnicorn vendor id
	num_usb_devices = libusb_get_device_list(libusb_context_ptr, &usb_device_list);

	for( i=0; i<num_usb_devices; ++i ){
		usb_device = usb_device_list[i];

		libusb_get_device_descriptor(usb_device, &usb_device_desc);
		for( j=0; j<(sizeof(lr_vendorIds)/sizeof(lr_vendorIds[0])); ++j) {
			if ( usb_device_desc.idVendor == lr_vendorIds[j] ) {
				// found a matching device, add it to the found list
				if (*device_list==NULL){
					*device_list = (lr_device*)calloc(1, sizeof(lr_device));
				} else {
					*device_list = (lr_device*)realloc(*device_list,(num_found_devices+1) * sizeof(lr_device));
				}
				// store the values we need to the device list
				//(*device_list[num_found_devices]) = (lr_device*)calloc(1, sizeof(lr_device));
				(*device_list)[num_found_devices].vendorId = usb_device_desc.idVendor;
				(*device_list)[num_found_devices].productId = usb_device_desc.idProduct;
				(*device_list)[num_found_devices]._libusb_device_ptr = usb_device;
				(*device_list)[num_found_devices]._libusb_handle_ptr = NULL;

				++num_found_devices;
			}
		}
	}

	libusb_free_device_list(usb_device_list, 0);

	return num_found_devices;
}

void lr_usb_free_devices( lr_device *device_list ){
	free(device_list);
}

void lr_usb_open( lr_device *device ){
	libusb_open((libusb_device*)device->_libusb_device_ptr, (libusb_device_handle**)&device->_libusb_handle_ptr);
}

void lr_usb_close( lr_device *device ){
	libusb_close((libusb_device_handle*)device->_libusb_handle_ptr);
	device->_libusb_handle_ptr = NULL;
}

int	lr_usb_send_message( lr_device *device, lr_msg *msg ){
	int result;
	result = libusb_control_transfer(
				(libusb_device_handle*)device->_libusb_handle_ptr, 
				msg->requestType, 
				msg->request,
				msg->value,
				msg->index,
				msg->data,
				msg->length,
				(uint16_t)5000);

	return result;
}