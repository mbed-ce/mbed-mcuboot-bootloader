/*
 * default_bd.cpp
 *
 *  Created on: Jul 30, 2020
 *      Author: gdbeckstein
 */

#include "BlockDevice.h"

#include "SlicingBlockDevice.h"
#include "FlashIAPBlockDevice.h"
#include "BufferedBlockDevice.h"

#if MBED_CONF_APP_SECONDARY_SLOT_IN_FLASH

mbed::BlockDevice* get_secondary_bd(void) {

    // Use a section of FlashIAP immediately after the secondary slot
    static FlashIAPBlockDevice flashBD(MCUBOOT_PRIMARY_SLOT_START_ADDR + MCUBOOT_SLOT_SIZE, MCUBOOT_SLOT_SIZE);
    return &flashBD;
}

#else

/**
 * You can override this function to suit your hardware/memory configuration
 * By default it simply returns what is returned by BlockDevice::get_default_instance();
 */
mbed::BlockDevice* get_secondary_bd(void) {

    // Use the PlatformStorage API to get the "default" block device for this project.
    // This will return the (O/Q)SPI flash or SD card instance if those components are available.
    // Otherwise it will return the flash IAP block device.
    mbed::BlockDevice* default_bd = mbed::BlockDevice::get_default_instance();

    // If this assert fails, there is no block def
    MBED_ASSERT(default_bd != nullptr);

    // mcuboot assumes that the read size of the secondary block device is the same as the read size
    // of flash, so use a BufferedBlockDevice to wrap the underlying BD and ensure this is the case.
    static mbed::BufferedBlockDevice buffered_bd(default_bd);

    // In this case, our flash is much larger than a single image so
    // slice it into the size of an image slot
    static mbed::SlicingBlockDevice sliced_bd(&buffered_bd, 0x0, MCUBOOT_SLOT_SIZE);

    return &sliced_bd;
}
#endif
