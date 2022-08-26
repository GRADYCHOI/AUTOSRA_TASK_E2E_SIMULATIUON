# /app/CMakeList.txt

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(DAG_Simulation ${SRC_FILES})
target_link_libraries(DAG_Simulation PUBLIC simulation)