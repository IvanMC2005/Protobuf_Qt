cmake_minimum_required(VERSION 3.5)
set(PROJECTNAME PROTOAPI)
set(SOURCE_FOLDER ${CMAKE_SOURCE_DIR}/src)
set(PROTO_OUT_DIR ${SOURCE_FOLDER})
project(${PROJECTNAME})
option(BUILD_EX1 "Build Example 1" ON) # нужно для возможности отключения сборки примера
find_package(Protobuf REQUIRED)
enable_testing()
find_package(GTest REQUIRED)
include_directories(${CMAKE_SOURCE_DIR})
# команда для добавления файлом генерации proto.exe в папку с исходными файлами
add_custom_command(
    OUTPUT ${PROTO_OUT_DIR}/mes.pb.cc ${PROTO_OUT_DIR}/mes.pb.h
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
    ARGS --cpp_out=${PROTO_OUT_DIR} --proto_path=${SOURCE_FOLDER}/protocol mes.proto
    DEPENDS ${SOURCE_FOLDER}
)
add_custom_target(generate_protobuf_files DEPENDS ${PROTO_OUT_DIR}/mes.pb.cc ${PROTO_OUT_DIR}/mes.pb.h)

# Условная сборка примера работы с классом DelimitedMessagesStreamParser
if(BUILD_EX1)
    add_executable(${PROJECTNAME}_Ex1 ${SOURCE_FOLDER}/Examples/Ex1.cpp ${PROTO_OUT_DIR}/mes.pb.cc)
    target_link_libraries(${PROJECTNAME}_Ex1 protobuf::libprotobuf)
    target_include_directories(${PROJECTNAME}_Ex1 PUBLIC ${SOURCE_FOLDER})
	target_compile_definitions(${PROJECTNAME}_Ex1 PRIVATE EnEx)
endif()

# Тестирование
add_executable(${PROJECTNAME}_tests ${SOURCE_FOLDER}/Testing/tests.cpp ${PROTO_OUT_DIR}/mes.pb.cc)
target_link_libraries(${PROJECTNAME}_tests GTest::GTest protobuf::libprotobuf)
target_include_directories(${PROJECTNAME}_tests PUBLIC ${SOURCE_FOLDER})
# Добавляем тесты в CMake
add_test(Test ${PROJECTNAME}_tests COMMAND ${PROJECTNAME}_tests)