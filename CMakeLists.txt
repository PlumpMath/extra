cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules/")

project(SGE)

find_package(Threads REQUIRED)
find_package(DL)
find_package(SIEGE)

if(UNIX)
    set(AUX_INCLUDES "")
    set(AUX_LIBRARIES "${DL_LIBRARY}" "m" "rt" ${CMAKE_THREAD_LIBS_INIT})
else()
    set(AUX_INCLUDES "")
    set(AUX_LIBRARIES "")
endif()

include_directories(${SIEGE_INCLUDE_DIR} include)

file(GLOB_RECURSE ${PROJECT_NAME}_files src/*.c)
add_library(${PROJECT_NAME}        SHARED ${${PROJECT_NAME}_files})
add_library(${PROJECT_NAME}.static STATIC ${${PROJECT_NAME}_files})

target_link_libraries(${PROJECT_NAME}        ${AUX_LIBRARIES} ${SIEGE_LIBRARY})
#todo: static deps
target_link_libraries(${PROJECT_NAME}.static ${AUX_LIBRARIES} ${SIEGE_LIBRARY})

set_target_properties(${PROJECT_NAME} ${PROJECT_NAME}.static PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    ENABLE_EXPORTS ON)

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}.static DESTINATION lib)

file(GLOB_RECURSE ${PROJECT_NAME}_examples examples/*.c)
foreach(example ${${PROJECT_NAME}_examples})
    get_filename_component(ename ${example} NAME_WE)
    add_executable(${PROJECT_NAME}-${ename} ${example})
    target_link_libraries(${PROJECT_NAME}-${ename} ${AUX_LIBRARIES} ${SIEGE_LIBRARY} ${PROJECT_NAME}.static)

    install(TARGETS ${PROJECT_NAME}-${ename} DESTINATION bin)
endforeach()
