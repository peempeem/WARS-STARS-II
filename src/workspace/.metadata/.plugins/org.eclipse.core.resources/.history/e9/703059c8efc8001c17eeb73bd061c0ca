/* HID support header */

#ifndef _HID_h_
#define _HID_h

/* HID device structure */
typedef struct {
	BYTE addr;
	BYTE interface;
} HID_DEVICE;
/* Boot mouse report 8 bytes */
typedef struct {
//    struct {
//        unsigned one:1;
//        unsigned two:1;
//        unsigned three:1;
//        unsigned :5;
//        } button;
	BYTE button;
	BYTE b1;
	BYTE x;
	BYTE b3;
	BYTE y;
	BYTE b5;
	BYTE b6;
	BYTE b7;
} BOOT_MOUSE_REPORT;
/* boot keyboard report 8 bytes */
typedef struct {
	BYTE mod;
//    struct {
//        unsigned LCtrl:1;
//        unsigned LShift:1;
//        unsigned LAlt:1;
//        unsigned LWin:1;
//        /**/
//        unsigned RCtrl:1;
//        unsigned RShift:1;
//        unsigned RAlt:1;
//        unsigned RWin:1;
//        } mod;
	BYTE reserved;
	BYTE keycode[6];
} BOOT_KBD_REPORT;

/* Function prototypes */
BOOL HIDMProbe(BYTE address, DWORD flags);
BOOL HIDKProbe(BYTE address, DWORD flags);
void HID_init(void);
BYTE mousePoll(BOOT_MOUSE_REPORT* buf);
BYTE kbdPoll(BOOT_KBD_REPORT* buf);
BOOL HIDMEventHandler(BYTE addr, BYTE event, void *data, DWORD size);
BOOL HIDKEventHandler(BYTE addr, BYTE event, void *data, DWORD size);
#endif // _HID_h_
