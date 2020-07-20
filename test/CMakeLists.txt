# Copyright (C) 2020 - Roman Byshliaha

add_subdirectory("${PROJECT_SOURCE_DIR}/external/googletest" "googletest")

add_executable(${PROJECT_NAME}_test ${PROJECT_NAME}_test.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE gtest_main ${PROJECT_NAME})

add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)
