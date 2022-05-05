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

mouse_t new_mouse(int x, int y) {
    mouse_t mouse = {
        {x, y},
        {0, 0}
    };
    return mouse;
}

int poll_mouse(mouse_t* mouse, int reverse_x, int reverse_y) {
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

            if (reverse_x)
                mouse->pos.x -= (signed char) buf.x;
            else
                mouse->pos.x += (signed char) buf.x;
            if (reverse_y)
                mouse->pos.y -= (signed char) buf.y;
            else
                mouse->pos.y += (signed char) buf.y;

            mouse_buttons_t buttons;
            buttons.left    = buf.button & 0x01;
            buttons.right   = buf.button & 0x02;

            if (mouse->buttons.left && !buttons.left)
                mouse->clicked.left = 1;
            if (mouse->buttons.right && !buttons.right)
                mouse->clicked.right = 1;
            
            mouse->buttons = buttons;
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

int is_clicked(mouse_t* mouse, int button) {
    switch (button) {
        case MOUSE_BUTTON_LEFT:
            if (mouse->clicked.left) {
                mouse->clicked.left = 0;
                return 1;
            }
            break;
        case MOUSE_BUTTON_RIGHT:
            if (mouse->clicked.right) {
                mouse->clicked.right = 0;
                return 1;
            }
            break;
    }
    return 0;
}