# Copyright (c) Minmin Gong. All rights reserved.
# Licensed under the MIT License.

if(MSVC)
    string(REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
else()
    string(REPLACE "-frtti" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

add_executable(Test
    Test.cpp
)

if(MSVC)
    target_compile_options(Test
        PRIVATE
            /GR-
    )
else()
    target_compile_options(Test
        PRIVATE
            -fno-rtti
    )
endif()

target_link_libraries(Test
    PRIVATE
        NanoRtti
)

set_target_properties(Test PROPERTIES FOLDER "Tests")
