add_library(libwsq STATIC IMPORTED GLOBAL)
add_library(libutil STATIC IMPORTED GLOBAL)
add_library(libioutil STATIC IMPORTED GLOBAL)
add_library(libimage STATIC IMPORTED GLOBAL)
add_library(libjpegl STATIC IMPORTED GLOBAL)
add_library(libihead STATIC IMPORTED GLOBAL)
add_library(libfet STATIC IMPORTED GLOBAL)

if(BUILD_FOR_UN20)
        message("Building for un20")
        set(LIBWSQ_DIR "un20")
else()
        message("Building for x64")
        set(LIBWSQ_DIR "x64")
endif()

set_target_properties(libwsq PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/${LIBWSQ_DIR}/libwsq.a
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include")

set_target_properties(libutil PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/${LIBWSQ_DIR}/libutil.a
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include")

set_target_properties(libioutil PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/${LIBWSQ_DIR}/libioutil.a
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include")

set_target_properties(libimage PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/${LIBWSQ_DIR}/libimage.a
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include")

set_target_properties(libjpegl PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/${LIBWSQ_DIR}/libjpegl.a
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include")

set_target_properties(libihead PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/${LIBWSQ_DIR}/libihead.a
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include")

set_target_properties(libfet PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/${LIBWSQ_DIR}/libfet.a
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include")

set(WSQ_LIB_SOURCES libwsq libjpegl libfet libioutil libutil PARENT_SCOPE)
