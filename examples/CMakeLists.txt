#--------------------------------------------------------------------
# soundplanelib/examples/CMakeLists.txt
# CMake file for soundplanelib examples.
#--------------------------------------------------------------------

link_libraries(soundplanelib)


#--------------------------------------------------------------------
# Add the other examples.
#--------------------------------------------------------------------

add_executable(helloSoundplane helloSoundplane.cpp)

target_include_directories(helloSoundplane PUBLIC ../include)
