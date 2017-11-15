cmake_minimum_required(VERSION 3.0)

project (device_sim)

add_subdirectory(iot-sdk)

set(nanopb_c_files
	src/nanopb/pb_common.c
	src/nanopb/pb_encode.c
	src/nanopb/pb_decode.c
	)

set(device_sim_c_files
	src/config.c
	src/sim_d2c.c
	src/sim_main.c
	src/slogging.c
	src/messages.c
	src/contoso.pb.c
	)

include_directories(${AZURE_C_SHARED_UTILITY_INCLUDES})
include_directories(${AZURE_C_SHARED_UTILITY_INCLUDES})
include_directories(${SHARED_UTIL_INC_FOLDER})
include_directories(${IOTHUB_CLIENT_INC_FOLDER})
include_directories(${IOTHUB_SERVICE_CLIENT_INC_FOLDER})
include_directories(src)
include_directories(src/inc)
include_directories(src/nanopb)

add_executable(
	device_sim ${device_sim_c_files} ${nanopb_c_files}
)

target_link_libraries(device_sim  
	iothub_client_mqtt_transport
	iothub_client 
	${OPENSSL_LIBRARIES}
	"-luuid"
)