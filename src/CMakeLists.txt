# Adiciona biblioteca `minha_lib` com base nos arquivos fonte
add_library( http_lib
    http/http_server.cpp
    http/http_session.cpp
)

target_include_directories(http_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(http_lib
    ${Boost_LIBRARIES}
)