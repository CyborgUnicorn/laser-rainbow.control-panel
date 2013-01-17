
#include <stdio.h>
#include <stdint.h>
#include <gtk/gtk.h>

#include "laserrainbow.h"
#include "lrusb.h"

lr_device *device=NULL;
	
int main(int argc, char** argv)
{
	lr_device *device_list=NULL;
	uint32_t num_devices=0;
	
	lr_usb_init();

	num_devices = lr_usb_get_devices( &device_list );

	device = &device_list[0];

	lr_usb_open(device);
	
	lr_gtk_test(argc,argv);

	
	lr_usb_close(device);
	lr_usb_free_devices(device_list);
	lr_usb_exit();

	return 0;
}

static void led_button_clicked( GtkWidget *widget, gpointer data )
{
	lr_msg msg;
	msg.requestType = 0x40;
	msg.request = 0x7;
	msg.index = (int)data;
	msg.data = 0;
	msg.length = 0;

	if(GTK_TOGGLE_BUTTON(widget)->active) {
		msg.value = 1;
	} else {
		msg.value = 0;
	}

	lr_usb_send_message( device, &msg );
}

void lr_gtk_test(int argc, char** argv)
{
	GtkWidget *window;
	GtkWidget *box;
	GtkWidget *led1Button, *led2Button;

	gtk_init( &argc, &argv );

	window = gtk_window_new( GTK_WINDOW_TOPLEVEL );

	g_signal_connect( G_OBJECT( window ), "destroy", G_CALLBACK( gtk_main_quit ), NULL );
	
	gtk_container_set_border_width(GTK_CONTAINER (window), 10);

	box = gtk_hbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), box);

	led1Button = gtk_toggle_button_new_with_label("LED1");
	led2Button = gtk_toggle_button_new_with_label("LED2");

	gtk_container_add(GTK_CONTAINER(box), led1Button);
	gtk_container_add(GTK_CONTAINER(box), led2Button);

	g_signal_connect(led1Button, "clicked", G_CALLBACK(led_button_clicked), (gpointer)1);
	g_signal_connect(led2Button, "clicked", G_CALLBACK(led_button_clicked), (gpointer)2);

	gtk_widget_show(box);

	gtk_widget_show(led1Button);
	gtk_widget_show(led2Button);

	gtk_widget_show(window);

	gtk_main ();
}