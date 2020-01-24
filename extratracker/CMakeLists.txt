set(TARGET_NAME driver_extratracker)

add_library(${TARGET_NAME} SHARED
  driverlog.cpp
  driverlog.h
  driver_sample.cpp
  )

target_compile_definitions(${TARGET_NAME} PRIVATE
  DRIVER_SAMPLE_EXPORTS=1
  )

target_include_directories(${TARGET_NAME} PRIVATE
  ${OPENVR_DIR}/headers
  )

target_link_libraries(${TARGET_NAME}
  )
