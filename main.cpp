#include <iostream>
#include <stdio.h>

#include <libusb/include/libusb.h>


static void print_device(libusb_device *dev, libusb_device_handle *handle = nullptr);

int main(){

    libusb_context *ctx = nullptr;
    int ret;


    ret = libusb_init(&ctx);
    if (ret < 0){
        std::cerr << "Error: initialization failed, code: " << ret << "." << std::endl;
        return 1;
    }

    libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_INFO);


    libusb_device **devs;
    ssize_t cnt;
    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        std::cerr << "Error: The list of USB devices has not been received." << std::endl;
        libusb_exit(ctx);
        return 1;
    }

    std::cout << "Number of devices found: " << cnt << std::endl;
    for (ssize_t i = 0; i < cnt; i++) {
        std::cout << "Device #" << i + 1 << " info:" << std::endl;
        print_device(devs[i]);
    }

    libusb_free_device_list(devs, 1);
    
    libusb_exit(ctx);

    return 0;
}

static void print_device(libusb_device *dev, libusb_device_handle *handle)
{
	struct libusb_device_descriptor desc;
	unsigned char string[256];
	int ret;

	ret = libusb_get_device_descriptor(dev, &desc);
	if (ret < 0) {
		std::cerr << "failed to get device descriptor" << std::endl;
		return;
	}

	if (!handle)
		libusb_open(dev, &handle);

	if (handle) {

        if ((int)desc.bDeviceClass == 9) {
            std::cout << "\tDevice class:              Hub" << std::endl;
        }
        else if ((int)desc.bDeviceClass == 224) {
            std::cout << "\tDevice class:              Wireless controller" << std::endl;
        } 
        else if ((int)desc.bDeviceClass == 239) {
            std::cout << "\tDevice class:              Other" << std::endl;
        } 
        else { 
            printf("\tDevice class:              %02x", desc.bDeviceClass);
            std::cout << std::endl;
        }

		if (desc.iManufacturer) {
			ret = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, string, sizeof(string));
			if (ret > 0) {
				std::cout << "\tManufacturer:              " << string << std::endl;
            }
		}

		if (desc.iProduct) {
			ret = libusb_get_string_descriptor_ascii(handle, desc.iProduct, string, sizeof(string));
			if (ret > 0) {
				std::cout << "\tProduct:                   " << string << std::endl;
            }
		}

		if (desc.iSerialNumber) {
			ret = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, string, sizeof(string));
			if (ret > 0) {
				std::cout << "\tSerial Number:             " << string << std::endl;
            }
		}
	}
    else {
        std::cout << "\tThe device information could not be opened.\n"
             << "\tMost likely, it does not have a driver for interaction or its version is outdated.\n"
             << "\tCheck the device driver and try again" 
             << std::endl;
    }

    std::cout << std::endl;

	if (handle)
		libusb_close(handle);
}
