cmake_minimum_required(VERSION 3.18)

project(Lab3 LANGUAGES CXX CUDA)
enable_language(CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CUDA_ARCHITECTURES native)

find_package(CUDAToolkit REQUIRED)

set(SOURCE main.cu utils.h utils.cu triple.h triple.cu prefix.h prefix.cu)

add_executable(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CUDAToolkit_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE CUDA::cudart CUDA::curand)
