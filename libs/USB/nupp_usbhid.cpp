/**
 * @file This file implements the NU++ header nupp/usbhid.hpp, which specifies
 * an interface for USB-HID-Custom-class devices.
 */

#include "nupp/usbhid.hpp"
#include "nu/platform.h"
#include "nu/compiler.h"
#include <cstdint>


#if NU_PLATFORM==NU_PLATFORM_UNKNOWN
#error "Unknown NU_PLATFORM in USB/nupp_usbhid.hpp!"

#elif NU_PLATFORM==NU_PLATFORM_GENERIC /* Generic USB-HID code */
#error "No GENERIC USB code in USB/nupp_usbhid.hpp!"

#elif NU_PLATFORM==NU_PLATFORM_PIC32MX /* PIC32MX-specific USB-HID code */

#ifdef __cplusplus
extern "C" {
#endif

#include "USB/usb.h"
#include "USB/usb_device.h"
#include "USB/usb_function_hid.h"
#include "usb_config.h"

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * Initialize USB In & Out Handles to 0!!
 */
nu::UsbHid::UsbHid(): USBOutHandle(NULL), USBInHandle(NULL) {
	USBDeviceInit();
	#if defined(USB_INTERRUPT)
		USBDeviceAttach();
	#endif
}

/**
 * This method checks if a packet was received, and, if so, calls the
 * callback and passes it a pointer to the buffer, and the number of
 * bytes in the buffer.
 * @param callback The callback, which takes a pointer and a size_t
 */
void nu::UsbHid::try_rx(void (*callback)(unsigned char *, size_t len)) {
	if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

	if(!HIDRxHandleBusy(USBOutHandle))
	{
		callback(ReceivedDataBuffer, 64);
		//Re-arm the OUT endpoint, so we can receive the next OUT data packet
		//that the host may try to send us.
		USBOutHandle = HIDRxPacket(HID_EP, (BYTE*)&ReceivedDataBuffer, 64);
	}
}

/**
 * This method checks if we can send a packet, and, if so, calls the
 * callback and passes it a pointer to the TX buffer, and the number of
 * bytes in the buffer.
 * @param callback The callback, which takes a pointer and a size_t
 */
void nu::UsbHid::try_tx(void (*callback)(unsigned char *, size_t len)) {
	if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

	if(!HIDTxHandleBusy(USBInHandle))
	{
		callback(ReceivedDataBuffer, 64);

		//Prepare the USB module to send the data packet to the host
		USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
	}
}

extern "C" {
BOOL USER_USB_CALLBACK_EVENT_HANDLER(int event, void *pdata, WORD size);
}

/** @todo Implement USB Event Handler */
BOOL USER_USB_CALLBACK_EVENT_HANDLER(int event, void *pdata, WORD size)
{
    switch(event)
    {
        default:
            break;
    }
    return TRUE;
}


#endif /* PLATFORM code */
