add_executable(compass-build main.cpp)
target_link_libraries(compass-build CompassCore CompassFilesystem CompassCompiler)

install(TARGETS compass-build DESTINATION ${CMAKE_SYSTEM_NAME})
