#include "pico/unique_id.h"
#include "tusb.h"
#include "tusb_common.h"
#include "tusb_types.h"

#define USB_PID 0x4004
#define USB_VID 0xC01D // Cold
#define USB_BCD 0x0110 // USB spec 1.1

// Device Descriptor
tusb_desc_device_t const desc_device = {.bLength = sizeof(tusb_desc_device_t),
                                        .bDescriptorType = TUSB_DESC_DEVICE,
                                        .bcdUSB = USB_BCD,
                                        .bDeviceClass = 0x00,
                                        .bDeviceSubClass = 0x00,
                                        .bDeviceProtocol = 0x00,
                                        .bMaxPacketSize0 =
                                            CFG_TUD_ENDPOINT0_SIZE,

                                        .idVendor = USB_VID,
                                        .idProduct = USB_PID,
                                        .bcdDevice = 0x0100,

                                        .iManufacturer = 0x01,
                                        .iProduct = 0x02,
                                        .iSerialNumber = 0x03,

                                        .bNumConfigurations = 0x01};

uint8_t const *tud_descriptor_device_cb(void) {
  return (uint8_t const *)&desc_device;
}

// HID Report Descriptor
uint8_t const desc_hid_report[] = {TUD_HID_REPORT_DESC_GAMEPAD()};

uint8_t const *tud_hid_descriptor_report_cb(uint8_t _instance) {
  return desc_hid_report;
}

// Configuration + Interface + Endpoint descriptor

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

#define EIN 1
#define EOUT 0
#define EP_ADDR(ep_number, direction) ((direction << 7) | (ep_number))
#define SUBCLASS_PROTOCOL_NONE 0

uint8_t const desc_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1,                // bConfigurationValue
                          1,                // bNumInterfaces
                          0,                // iConfiguration
                          CONFIG_TOTAL_LEN, // wTotalLength
                          TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, // bmAttributes
                          100                                 // bMaxPower
                          ),

    TUD_HID_DESCRIPTOR(
        0,                       // bInterfaceNumber
        0,                       // iInterface
        SUBCLASS_PROTOCOL_NONE,  // bInterfaceSubClass and bInterfaceProtocol
        sizeof(desc_hid_report), // wDescriptorLength
        EP_ADDR(EIN, 1),         // bEndpointAddress
        CFG_TUD_HID_EP_BUFSIZE,  // wMaxPacketSize
        5                        // bInterval
        ),
};

uint8_t const *tud_descriptor_configuration_cb(uint8_t _index) {
  return desc_configuration;
}

// String Descriptors
char serial[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];

// array of pointer to string descriptors
char const *string_desc_arr[] = {
    (const char[]){0x09, 0x04}, // 0: is supported language is English (0x0409)
    "Veloid",                   // 1: Manufacturer
    "JoyShtick",                // 2: Product
    serial,                     // 3: Serials, uses the flash ID
};

static uint16_t _desc_str[32];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long
// enough for transfer to complete
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
  (void)langid;

  uint8_t chr_count;

  if (index == 0) {
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
  } else {
    // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
    // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

    if (index == 3)
      pico_get_unique_board_id_string(serial, sizeof(serial));

    if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])))
      return NULL;

    const char *str = string_desc_arr[index];

    // Cap at max char
    chr_count = strlen(str);
    if (chr_count > 31)
      chr_count = 31;

    // Convert ASCII string into UTF-16
    for (uint8_t i = 0; i < chr_count; i++) {
      _desc_str[1 + i] = str[i];
    }
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);

  return _desc_str;
}

// For some reason these are two are required to be implemented by tinyusb
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen) {
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;

  return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize) {
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
}
