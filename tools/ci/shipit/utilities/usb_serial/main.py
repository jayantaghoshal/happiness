""" Example code to demo the use of usb_enum class"""

import argparse
from usb_enum import UsbSerialEnum

def main():
    """Main Function """
    parser = argparse.ArgumentParser(description="gives usb device path for each slave")
    parser.add_argument("file", help='Provide a .json file containing a list of slaves and corresponding ports on the USB hub')
    args = parser.parse_args()
    usb_serial = UsbSerialEnum(args.file)

    slave_usb_dict = usb_serial.get_slave_dict()
    usb_serial_dict = usb_serial.get_device_list()

    print("Slave List: {}".format(slave_usb_dict))
    print("Port List: {}".format(usb_serial_dict))

    print("Slave_1_mp: {}".format(usb_serial_dict[slave_usb_dict['Slave_1_mp']]))


if __name__ == "__main__":
    main()
