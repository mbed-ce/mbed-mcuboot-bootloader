cmake_minimum_required(VERSION 3.19)
cmake_policy(VERSION 3.19)

set(MBED_APP_JSON_PATH mbed_app.json5)

include(mbed-os/tools/cmake/mbed_toolchain_setup.cmake)

project(mbed-mcuboot-bootloader
	LANGUAGES C CXX ASM)

include(mbed_project_setup)

add_subdirectory(mbed-os)

set(MBED_MCUBOOT_BOOTLOADER_SOURCES
	secondary_bd.cpp
	shared_data.c)

# Compile mcuboot sources
add_subdirectory(mcuboot/boot/bootutil)
add_subdirectory(mcuboot/boot/mbed)

add_executable(${CMAKE_PROJECT_NAME} ${MBED_MCUBOOT_BOOTLOADER_SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME}
	mbed-baremetal
	mbed-storage
	mbed-mcuboot)
mbed_set_post_build(${CMAKE_PROJECT_NAME})
