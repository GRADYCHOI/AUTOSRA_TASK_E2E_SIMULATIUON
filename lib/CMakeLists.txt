# /lib/CMakeList.txt

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(simulation STATIC ${SRC_FILES})

target_include_directories(simulation PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_compile_options(simulation PRIVATE -Wall -Werror -Wvirtual-inheritance)

target_compile_features(simulation PRIVATE cxx_std_17)

# target_link_libraries(simulation PRIVATE RapidJson)