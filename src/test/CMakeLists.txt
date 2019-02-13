add_executable(compass-run main.cpp)
target_link_libraries(compass-run Compass)

install(TARGETS compass-run DESTINATION ${CMAKE_SYSTEM_NAME})
install(FILES "${EXAMPLE_ASSET}" DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_SYSTEM_NAME}")
