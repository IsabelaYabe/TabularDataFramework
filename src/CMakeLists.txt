# src

add_library( app_lib
    http/http_server.cpp
    http/http_session.cpp
    handler/parser.cpp
)

target_include_directories(app_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(app_lib
    ${Boost_LIBRARIES}
)