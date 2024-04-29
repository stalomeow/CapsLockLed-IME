cmake_minimum_required(VERSION 3.5)

project(CapsLockLed-IME)

add_library(capslockled-hook SHARED src/hook.c)
add_executable(capslockled-main src/main.c)

set_target_properties(capslockled-main PROPERTIES
    LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)

install(TARGETS capslockled-hook DESTINATION)
install(TARGETS capslockled-main DESTINATION)
