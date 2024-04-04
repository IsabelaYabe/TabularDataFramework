cmake_minimum_required(VERSION 3.10)
project(MeuProjeto VERSION 1.0)

# Define C++ Standard Globalmente
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define a localização dos binários (executáveis e bibliotecas)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Inclui Boost.Asio
find_package(Boost 1.65 REQUIRED COMPONENTS system)

# Inclui o diretório 'include' para disponibilizar os cabeçalhos globalmente
include_directories(include/**)

# Adiciona subdiretório para a biblioteca/aplicação
add_subdirectory(src)
add_subdirectory(app)