#ifndef _LASER_RAINBOW_USB_H
#define _LASER_RAINBOW_USB_H

typedef struct {
	uint16_t vendorId;
	uint16_t productId;

	/**private**/
	void *_libusb_device_ptr;
	void *_libusb_handle_ptr;
} lr_device;

typedef struct {
	uint8_t			requestType;
	uint8_t			request;
	uint16_t		value;
	uint16_t		index;
	unsigned char	*data;
	uint16_t		length;
} lr_msg;

void		lr_usb_init();
void		lr_usb_exit();
uint32_t	lr_usb_get_devices( lr_device **device_list );
void		lr_usb_free_devices( lr_device *device_list );
void		lr_usb_open( lr_device *device );
void		lr_usb_close( lr_device *device );

int			lr_usb_send_message( lr_device *device, lr_msg *msg );

#endif