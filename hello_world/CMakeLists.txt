cmake_minimum_required(VERSION 3.16)
include(ProcessorCount)
get_filename_component(SDK_INCLUDE
                       "${CMAKE_CURRENT_SOURCE_DIR}/../pico/pico-sdk/pico_sdk_init.cmake"
                       ABSOLUTE)

include(${SDK_INCLUDE})

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)

project(HelloWorld C CXX ASM)
ProcessorCount(JOBS)

###############
set(TARGET_ARCH cortex-m0plus)
set(TARGET cortex_m_generic)

set(PICO_BOARD pico_w)

# initialize the Pico SDK
pico_sdk_init()
#######################################################

set(BUILD_TYPE default)

#set abs path to the tflite-micro repository
get_filename_component(TENSORFLOW_ROOT
                       "${CMAKE_CURRENT_SOURCE_DIR}/../pico/tflite-micro/"
                       ABSOLUTE)
#set all used PATHs
set(TFLITE_MICROLIB_PATH "${TENSORFLOW_ROOT}/gen/${TARGET}_${TARGET_ARCH}_${BUILD_TYPE}/lib/libtensorflow-microlite.a")
set(TARGET_GENERATOR_INPUTS ${CMAKE_CURRENT_SOURCE_DIR}/hello_world.tflite)
set(TARGET_GENERATED_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/hello_world_model_data.cc)
set(TARGET_GENERATED_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/hello_world_model_data.h)



set(HELLO_WORLD_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/main_functions.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/output_handler.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/constants.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../RP2040_common/debug_log.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../RP2040_common/micro_time.cc
    )

include_directories(
    "${TENSORFLOW_ROOT}"
    "${TENSORFLOW_ROOT}/tensorflow/lite/micro/tools/make/downloads/flatbuffers/include"
    "${TENSORFLOW_ROOT}/tensorflow/lite/micro/tools/make/downloads/gemmlowp"
    )


#create example MAIN executable
add_executable(hello_world ${HELLO_WORLD_SRCS} ${TARGET_GENERATED_SRCS})
target_link_libraries(hello_world 
    ${TFLITE_MICROLIB_PATH} 
    pico_stdlib 
    hardware_pwm
    pico_cyw43_arch_none
    )
add_dependencies(hello_world tflitelib)

pico_enable_stdio_usb(hello_world 1)
pico_enable_stdio_uart(hello_world 0)
pico_add_extra_outputs(hello_world)

set(CXX_ADDITIONAL_FLAGS   -Werror -fno-unwind-tables -ffunction-sections -fdata-sections -fmessage-length=0 -DTF_LITE_STATIC_MEMORY -DTF_LITE_DISABLE_X86_NEON)
target_compile_options(hello_world PRIVATE ${CXX_ADDITIONAL_FLAGS})

set_property(
        TARGET hello_world
        APPEND
        PROPERTY ADDITIONAL_CLEAN_FILES ${TENSORFLOW_ROOT}/gen
                                        ${TENSORFLOW_ROOT}/tensorflow/lite/micro/tools/make/downloads
)

add_custom_command(
    OUTPUT  ${TARGET_GENERATED_SRCS} ${TARGET_GENERATED_HDRS}
    COMMAND python3 ${TENSORFLOW_ROOT}/tensorflow/lite/micro/tools/generate_cc_arrays.py ${CMAKE_CURRENT_SOURCE_DIR} ${TARGET_GENERATOR_INPUTS}
    DEPENDS ${HELLO_WORLD_SRCS}
    COMMENT "Generating network input as array..."
)




add_custom_target(
    tflitelib
    COMMAND make -f ${TENSORFLOW_ROOT}/tensorflow/lite/micro/tools/make/Makefile TARGET=${TARGET} TARGET_ARCH=${TARGET_ARCH} -j${JOBS} microlite
    COMMENT "Compiling tensorflowlite micro library..."
    WORKING_DIRECTORY  ${TENSORFLOW_ROOT}
)
