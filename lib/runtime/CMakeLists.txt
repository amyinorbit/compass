add_library(CompassRuntime STATIC game.cpp run.cpp sentence.cpp)
target_link_libraries(CompassRuntime CompassCore CompassLanguage CompassFilesystem)
target_include_directories(CompassRuntime INTERFACE ${PROJECT_SOURCE_DIR}/include)
