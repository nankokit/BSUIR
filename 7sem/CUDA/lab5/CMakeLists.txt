cmake_minimum_required(VERSION 3.18)

project(lab5 LANGUAGES CXX CUDA)
enable_language(CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CUDA_ARCHITECTURES native)

find_package(CUDAToolkit REQUIRED)
find_package(OpenCV REQUIRED)

set(SOURCE main.cpp emboss.cu emboss.h)

add_executable(${PROJECT_NAME} ${SOURCE})

# Provide absolute paths for input/output images so the binary can find them when
# run from the build directory.
target_compile_definitions(${PROJECT_NAME} PRIVATE
    INPUT_IMAGE="${CMAKE_SOURCE_DIR}/input.png"
    OUTPUT_CPU_IMAGE="${CMAKE_SOURCE_DIR}/output_cpu.png"
    OUTPUT_CUDA_IMAGE="${CMAKE_SOURCE_DIR}/output_cuda.png"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CUDAToolkit_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE CUDA::cudart CUDA::curand ${OpenCV_LIBS})
