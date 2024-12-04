cmake_minimum_required(VERSION 3.19)
cmake_policy(VERSION 3.19)

set(MBED_APP_JSON_PATH mbed_app.json5)

include(mbed-os/tools/cmake/app.cmake)

add_subdirectory(mbed-os)

project(mbed-mcuboot-bootloader)

set(MBED_MCUBOOT_BOOTLOADER_SOURCES
	secondary_bd.cpp
	enc_key.c
	shared_data.c
	signing_keys.c)
	
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/signing_keys.c)
    message(FATAL_ERROR "Missing signing_keys.c in source directory!  Please follow the README instructions to generate the signing keys!")
endif()

# Compile mcuboot sources
add_subdirectory(mcuboot/boot/bootutil)
add_subdirectory(mcuboot/boot/mbed)

add_executable(${CMAKE_PROJECT_NAME} ${MBED_MCUBOOT_BOOTLOADER_SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME}
	mbed-baremetal
	mbed-storage
	mbed-mcuboot)
mbed_set_post_build(${CMAKE_PROJECT_NAME})

mbed_finalize_build()