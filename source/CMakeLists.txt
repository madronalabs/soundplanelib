

set(SP_LIB_SOURCES
  SensorFrame.cpp
  ThreadUtility.cpp
)

if(SP_USE_LIBUSB)
  set(SP_LIB_SOURCES
    ${SP_LIB_SOURCES}
    LibusbSoundplaneDriver.cpp
    LibusbSoundplaneDriver.h
  )
else(SP_USE_LIBUSB)
  set(SP_LIB_SOURCES
    ${SP_LIB_SOURCES}
    MacSoundplaneDriver.cpp
    MacSoundplaneDriver.h
  )
endif()

#--------------------------------------------------------------------
# Add include directories
#--------------------------------------------------------------------

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

if(SP_USE_LIBUSB)
  pkg_check_modules(LIBUSB REQUIRED libusb-1.0)
  include_directories(${LIBUSB_INCLUDE_DIRS})
endif()

#--------------------------------------------------------------------
# create and install library
#--------------------------------------------------------------------

add_library(soundplanelib STATIC ${SP_LIB_SOURCES})
set_target_properties(soundplanelib PROPERTIES
                      OUTPUT_NAME "${soundplanelib_NAME}"
                      VERSION ${SP_VERSION}
                      SOVERSION ${SP_VERSION_MAJOR}
                      POSITION_INDEPENDENT_CODE ON)


# ?
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(soundplanelib atomic)
endif()

include(GNUInstallDirs)

install(
    TARGETS soundplanelib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

file(GLOB SP_LIB_INCLUDES "${CMAKE_SOURCE_DIR}/include/*.h")
install(FILES ${SP_LIB_INCLUDES} DESTINATION include/soundplane)

#--------------------------------------------------------------------
# Link platform libraries
#--------------------------------------------------------------------

if(SP_USE_LIBUSB)
  target_link_libraries(soundplanelib ${LIBUSB_LINK_LIBRARIES})
else()
  target_link_libraries(soundplanelib "-framework Foundation")
  target_link_libraries(soundplanelib "-framework IOKit")
endif()

