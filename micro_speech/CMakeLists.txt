cmake_minimum_required(VERSION 3.16)
include(ProcessorCount)

option(BUILD_X86 "Build micro speech example for x86 target" OFF)

get_filename_component(SDK_INCLUDE
                       "${CMAKE_CURRENT_SOURCE_DIR}/../pico/pico-sdk/pico_sdk_init.cmake"
                       ABSOLUTE)

if(NOT BUILD_X86)
include(${SDK_INCLUDE})
endif()

set(CMAKE_SYSTEM_NAME Generic)

project(HelloWorld C CXX ASM)
ProcessorCount(JOBS)

###############
if(BUILD_X86)

#run uname -m if host machine is different than x86
set(TARGET_ARCH x86_64) 
set(TARGET linux)

else()

set(TARGET_ARCH cortex-m0plus)
set(TARGET cortex_m_generic)

set(PICO_BOARD pico_w)

# initialize the Pico SDK
pico_sdk_init()

endif(BUILD_X86)
#######################################################

set(BUILD_TYPE default)

#set abs path to the tflite-micro repository
get_filename_component(TENSORFLOW_ROOT
                       "${CMAKE_CURRENT_SOURCE_DIR}/../pico/tflite-micro/"
                       ABSOLUTE)

#set all used PATHs
set(TFLITE_MICROLIB_PATH "${TENSORFLOW_ROOT}/gen/${TARGET}_${TARGET_ARCH}_${BUILD_TYPE}/lib/libtensorflow-microlite.a")


set(TARGET_GENERATOR_INPUTS ${CMAKE_CURRENT_SOURCE_DIR}/micro_speech.tflite
                            ${CMAKE_CURRENT_SOURCE_DIR}/testdata/no_1000ms.wav
                            ${CMAKE_CURRENT_SOURCE_DIR}/testdata/no_30ms.wav
                            ${CMAKE_CURRENT_SOURCE_DIR}/testdata/yes_1000ms.wav
                            ${CMAKE_CURRENT_SOURCE_DIR}/testdata/yes_30ms.wav
                             
)

set(TARGET_GENERATED_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/micro_speech_model_data.cc
                          ${CMAKE_CURRENT_SOURCE_DIR}/testdata/no_1000ms_audio_data.cc
                          ${CMAKE_CURRENT_SOURCE_DIR}/testdata/no_30ms_audio_data.cc
                          ${CMAKE_CURRENT_SOURCE_DIR}/testdata/yes_1000ms_audio_data.cc
                          ${CMAKE_CURRENT_SOURCE_DIR}/testdata/yes_30ms_audio_data.cc
                          )
set(TARGET_GENERATED_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/micro_speech_model_data.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/testdata/no_1000ms_audio_data.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/testdata/no_30ms_audio_data.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/testdata/yes_1000ms_audio_data.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/testdata/yes_30ms_audio_data.h
)

set(TARGET_MIC_GENERATOR_INPUTS ${CMAKE_CURRENT_SOURCE_DIR}/micro_speech.tflite)
set(TARGET_MIC_GENERATOR_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/micro_speech_model_data.cc)
set(TARGET_MIC_GENERATOR_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/micro_speech_model_data.h)

set(MICRO_SPEECH_PDM_MIC_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/main_functions.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/microphone/pdm_audio_provider.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/feature_provider.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/micro_features/no_micro_features_data.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/micro_features/yes_micro_features_data.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/recognize_commands.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/microphone/command_responder.cc

    ${CMAKE_CURRENT_SOURCE_DIR}/micro_features/micro_features_generator.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/micro_features/micro_model_settings.cc

    ${CMAKE_CURRENT_SOURCE_DIR}/../RP2040_common/debug_log.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../RP2040_common/micro_time.cc

    ${CMAKE_CURRENT_SOURCE_DIR}/console_app/console_handler.cc

)


set(MICRO_SPEECH_MOCK_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/main_functions.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_provider_mock.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/feature_provider.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/micro_features/no_micro_features_data.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/micro_features/yes_micro_features_data.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/recognize_commands.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/command_responder.cc

    ${CMAKE_CURRENT_SOURCE_DIR}/micro_features/micro_features_generator.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/micro_features/micro_model_settings.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/console_app/console_handler.cc
    )

if(NOT BUILD_X86)
set(MICRO_SPEECH_MOCK_SRC
    ${MICRO_SPEECH_MOCK_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../RP2040_common/debug_log.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../RP2040_common/micro_time.cc
    )
endif()

set(TARGET_LIB_MICROFRONTEND_SRCS
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/fft.cc
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/fft_util.cc
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/filterbank.c
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/filterbank_util.c
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/frontend.c
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/frontend_util.c
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/kiss_fft_int16.cc
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/log_lut.c
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/log_scale.c
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/log_scale_util.c
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/noise_reduction.c
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/noise_reduction_util.c
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/pcan_gain_control.c
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/pcan_gain_control_util.c
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/window.c
    ${TENSORFLOW_ROOT}/tensorflow/lite/experimental/microfrontend/lib/window_util.c
    )
set(TARGET_LIB_KISSFFT_SRCS
    ${TENSORFLOW_ROOT}/tensorflow/lite/micro/tools/make/downloads/kissfft/kiss_fft.c
    ${TENSORFLOW_ROOT}/tensorflow/lite/micro/tools/make/downloads/kissfft/tools/kiss_fftr.c
    )

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/microphone"
    "${TENSORFLOW_ROOT}"
    "${TENSORFLOW_ROOT}/tensorflow/lite/micro/tools/make/downloads/flatbuffers/include"
    #KissFFT INCLUDES
    "${TENSORFLOW_ROOT}//tensorflow/lite/micro/tools/make/downloads/kissfft"
    "${TENSORFLOW_ROOT}/tensorflow/lite/micro/tools/make/downloads/gemmlowp"
    )

#configure&add libraries needed by main target
add_library(kiss_fft_lib STATIC ${TARGET_LIB_KISSFFT_SRCS})
set_source_files_properties(${TARGET_LIB_KISSFFT_SRCS} PROPERTIES GENERATED TRUE)
target_compile_options(kiss_fft_lib PRIVATE ${ARCH_FLAGS})
add_dependencies(kiss_fft_lib tflitelib)

add_library(microfrontend_lib STATIC ${TARGET_LIB_MICROFRONTEND_SRCS})
target_compile_options(microfrontend_lib PRIVATE ${ARCH_FLAGS})
add_dependencies(microfrontend_lib kiss_fft_lib)



######################################
if(BUILD_X86)

#create example micro-speech mock executable for X86
add_executable(micro-speech-mock ${MICRO_SPEECH_MOCK_SRCS} ${TARGET_GENERATED_SRCS})
target_link_libraries(micro-speech-mock 
    kiss_fft_lib 
    microfrontend_lib 
    ${TFLITE_MICROLIB_PATH}
    )
add_dependencies(micro-speech-mock tflitelib)

set(CXX_ADDITIONAL_FLAGS   -Werror -fno-unwind-tables -ffunction-sections -fdata-sections -fmessage-length=0 -DTF_LITE_STATIC_MEMORY -DTF_LITE_DISABLE_X86_NEON -DTARGET_X86)
target_compile_options(micro-speech-mock PRIVATE ${CXX_ADDITIONAL_FLAGS})

else()

#create miro-speech digital mic target
add_executable(micro-speech-pdm-mic ${MICRO_SPEECH_PDM_MIC_SRCS} ${TARGET_MIC_GENERATOR_SRCS})

target_link_libraries(micro-speech-pdm-mic
    kiss_fft_lib 
    microfrontend_lib 
    pico_stdlib 
    pico_cyw43_arch_none
    pico_pdm_microphone
    ${TFLITE_MICROLIB_PATH}
    )
add_dependencies(micro-speech-pdm-mic tflitelib)

set(CXX_ADDITIONAL_FLAGS   -Werror -fno-unwind-tables -ffunction-sections -fdata-sections -fmessage-length=0 -DTF_LITE_STATIC_MEMORY -DTF_LITE_DISABLE_X86_NEON)
target_compile_options(micro-speech-pdm-mic PRIVATE ${CXX_ADDITIONAL_FLAGS})

pico_enable_stdio_usb(micro-speech-pdm-mic 1)
pico_enable_stdio_uart(micro-speech-pdm-mic 0)
pico_add_extra_outputs(micro-speech-pdm-mic)


############################################
#create example micro-speech mock executable
add_executable(micro-speech-mock ${MICRO_SPEECH_MOCK_SRCS} ${TARGET_GENERATED_SRCS})
target_link_libraries(micro-speech-mock 
    kiss_fft_lib 
    microfrontend_lib 
    pico_stdlib 
    ${TFLITE_MICROLIB_PATH}
    )
add_dependencies(micro-speech-mock tflitelib)

set(CXX_ADDITIONAL_FLAGS   -Werror -fno-unwind-tables -ffunction-sections -fdata-sections -fmessage-length=0 -DTF_LITE_STATIC_MEMORY -DTF_LITE_DISABLE_X86_NEON)
target_compile_options(micro-speech-mock PRIVATE ${CXX_ADDITIONAL_FLAGS})

pico_enable_stdio_usb(micro-speech-mock 1)
pico_enable_stdio_uart(micro-speech-mock 0)
pico_add_extra_outputs(micro-speech-mock)

############################################################################################################################################
endif()

add_custom_command(
    OUTPUT  ${TARGET_GENERATED_SRCS} ${TARGET_GENERATED_HDRS}
    COMMAND python3 ${TENSORFLOW_ROOT}/tensorflow/lite/micro/tools/generate_cc_arrays.py ${CMAKE_CURRENT_SOURCE_DIR} ${TARGET_GENERATOR_INPUTS}
    DEPENDS ${MICRO_SPEECH_MOCK_SRCS}
    COMMENT "Generating network input as array..."
)

if(NOT BUILD_X86)
    set(ARG_OPTIMIZED_KERNEL_DIR "OPTIMIZED_KERNEL_DIR=cmsis_nn")
endif()

add_custom_target(
    tflitelib
    COMMAND make VERBOSE=1 -f ${TENSORFLOW_ROOT}/tensorflow/lite/micro/tools/make/Makefile ${ARG_OPTIMIZED_KERNEL_DIR} TARGET=${TARGET} TARGET_ARCH=${TARGET_ARCH} BUILD_TYPE=${BUILD_TYPE} -j${JOBS}
    COMMENT "Compiling tensorflowlite micro library..."
    WORKING_DIRECTORY  ${TENSORFLOW_ROOT}
)

set_property(
        TARGET micro-speech-mock
        APPEND
        PROPERTY ADDITIONAL_CLEAN_FILES ${TENSORFLOW_ROOT}/tensorflow/lite/micro/tools/make/gen
                                        ${TENSORFLOW_ROOT}/tensorflow/lite/micro/tools/make/downloads
)
if(NOT BUILD_X86)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../pico/microphone-library-for-pico" microphone-library-for-pico)
endif()