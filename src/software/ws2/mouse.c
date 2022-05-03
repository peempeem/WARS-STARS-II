#include "mouse.h"

#include <stdio.h>
#include "system.h"
#include "altera_avalon_spi.h"
#include "altera_avalon_spi_regs.h"
#include "sys/alt_irq.h"
#include "usb_kb/GenericMacros.h"
#include "usb_kb/GenericTypeDefs.h"
#include "usb_kb/HID.h"
#include "usb_kb/MAX3421E.h"
#include "usb_kb/transfer.h"
#include "usb_kb/usb_ch9.h"
#include "usb_kb/USB.h"
#include "timer.h"

extern HID_DEVICE hid_device;
static BYTE addr = 1;

BYTE GetDriverandReport() {
	BYTE i;
	BYTE rcode;
	BYTE device = 0xFF;
	BYTE tmpbyte;

	DEV_RECORD* tpl_ptr;
	printf("Reached USB_STATE_RUNNING (0x40)\n");
	for (i = 1; i < USB_NUMDEVICES; i++) {
		tpl_ptr = GetDevtable(i);
		if (tpl_ptr->epinfo != NULL) {
			printf("Device: %d", i);
			device = tpl_ptr->devclass;
		}
	}
	//Query rate and protocol
	rcode = XferGetIdle(addr, 0, hid_device.interface, 0, &tmpbyte);
	if (rcode) {   //error handling
		printf("GetIdle Error. Error code: ");
		printf("%x \n", rcode);
	} else {
		printf("Update rate: ");
		printf("%x \n", tmpbyte);
	}
	printf("Protocol: ");
	rcode = XferGetProto(addr, 0, hid_device.interface, &tmpbyte);
	if (rcode) {   //error handling
		printf("GetProto Error. Error code ");
		printf("%x \n", rcode);
	} else {
		printf("%d \n", tmpbyte);
	}
	return device;
}


void init_mouse() {
    MAX3421E_init();
    USB_init();
}

mouse_t new_mouse() {
    mouse_t mouse = {
        0, 0,
        0, 0,
        0, 0        
    };
    return mouse;
}

int poll_mouse(mouse_t* mouse) {
    static BOOT_MOUSE_REPORT buf;
    static BYTE device;
    static BYTE runningdebugflag = 0;

    MAX3421E_Task();
    USB_Task();

    if (GetUsbTaskState() == USB_STATE_RUNNING) {
        if (!runningdebugflag) {
            runningdebugflag = 1;
            device = GetDriverandReport();
        } else if (device == 2) {
            BYTE rcode = mousePoll(&buf);
            if (rcode == hrNAK) {
                //NAK means no new data
                return 0;
            } else if (rcode) {
                printf("Rcode: ");
                printf("%x \n", rcode);
                return 0;
            }

            mouse->dx = (signed char) buf.x;
            mouse->dy = (signed char) buf.y;
            mouse->x += (signed char) buf.x;
            mouse->y += (signed char) buf.y;
            mouse->left_click = buf.button & 0x01;
            mouse->right_click = buf.button & 0x02;
            return 1;
        }
    } else {
        if (runningdebugflag) {
            runningdebugflag = 0;
            printf("initing\n");
            init_mouse();
        }
    }

    
    return 0;
}
