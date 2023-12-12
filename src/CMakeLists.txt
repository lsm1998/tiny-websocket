file(GLOB WS_SOURCE_FILES "ws/*.cpp")
file(GLOB CORE_SOURCE_FILES "core/*.cpp")
file(GLOB GLOBAL_SOURCE_FILES "global/*.cpp")
file(GLOB ROUTE_SOURCE_FILES "route/*.cpp")

add_executable(${PROJECT_NAME} main.cpp example/example.cpp ${WS_SOURCE_FILES} ${CORE_SOURCE_FILES} ${GLOBAL_SOURCE_FILES} ${ROUTE_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} pthread)

add_subdirectory(tests)