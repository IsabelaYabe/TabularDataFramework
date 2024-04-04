# Adiciona o executável principal
add_executable(PipelineFramework main.cpp)

# Vincula o executável com a biblioteca
target_link_libraries(PipelineFramework
    PRIVATE http_lib
)