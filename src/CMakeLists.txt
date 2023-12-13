file(GLOB WS_SOURCE_FILES "ws/*.cpp")
file(GLOB CORE_SOURCE_FILES "core/*.cpp")
file(GLOB GLOBAL_SOURCE_FILES "global/*.cpp")
file(GLOB ROUTE_SOURCE_FILES "route/*.cpp")
file(GLOB HTTP_SOURCE_FILES "http/*.cpp")

add_executable(${PROJECT_NAME} main.cpp example/example.cpp ${HTTP_SOURCE_FILES} ${WS_SOURCE_FILES} ${CORE_SOURCE_FILES} ${GLOBAL_SOURCE_FILES} ${ROUTE_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} pthread)

add_subdirectory(tests)