# /lib/CMakeList.txt

file(GLOB_RECURSE CPU_SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

file(GLOB_RECURSE GPU_SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cu
)

enable_language("CUDA")

add_library(simulation STATIC ${CPU_SRC_FILES} ${GPU_SRC_FILES})

target_include_directories(simulation PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_compile_options(simulation PRIVATE -Wall -Werror -Wvirtual-inheritance)

add_definitions(-DUSE_CUDA)

target_compile_features(simulation PRIVATE cxx_std_17)

# target_link_libraries(simulation PRIVATE RapidJson)