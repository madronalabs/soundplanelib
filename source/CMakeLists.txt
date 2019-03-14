



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

install(FILES ../include/libsoundplane.h DESTINATION include/soundplane)


#--------------------------------------------------------------------
# Link platform libraries
#--------------------------------------------------------------------

if(SP_USE_LIBUSB)
  target_link_libraries(soundplanelib libusb)
else()
  target_link_libraries(soundplanelib "-framework Foundation")
  target_link_libraries(soundplanelib "-framework IOKit")
endif()

#--------------------------------------------------------------------
# Add include directories
#--------------------------------------------------------------------

target_include_directories(soundplanelib PUBLIC .)
target_include_directories(soundplanelib PUBLIC ../include)

