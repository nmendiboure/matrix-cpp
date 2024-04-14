enable_testing()

add_executable(matrix_test matrix_test.cc ../core/component/matrix.cpp ../core/component/proto/matrix.pb.cc)
target_link_libraries(matrix_test GTest::gtest_main ${PROTOBUF_LIBRARY})

include(GoogleTest)
gtest_discover_tests(hello_test matrix_test)
