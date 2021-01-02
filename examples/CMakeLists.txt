#--------------------------------------------------------------------
# soundplanelib/examples/CMakeLists.txt
# CMake file for soundplanelib examples.
#--------------------------------------------------------------------

link_libraries(soundplanelib)

if(SP_USE_LIBUSB)
  link_libraries(pthread ${LIBUSB_LINK_LIBRARIES})
endif()


#--------------------------------------------------------------------
# Add the other examples.
#--------------------------------------------------------------------

add_executable(helloSoundplane helloSoundplane.cpp)

target_include_directories(helloSoundplane PUBLIC ../include)
