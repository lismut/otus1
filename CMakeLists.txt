cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER}) 
    project(helloworld VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(helloworld VERSION 0.0.4)
endif()


find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package (Threads REQUIRED)

configure_file(version.h.in version.h)

add_subdirectory(
       "${CMAKE_CURRENT_SOURCE_DIR}/gtest"
       gtest
)
add_executable(helloworld.deb main.cpp)
add_library(helloworld lib.cpp)
add_executable(test_version test_version.cpp)
add_executable(test_version_g test_version1.cpp)

set_target_properties(helloworld.deb helloworld test_version test_version_g PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(helloworld
    PRIVATE "${CMAKE_BINARY_DIR}"
)
set_target_properties(test_version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

# set_target_properties(test_version_g PROPERTIES INCLUDE_DIRECTORIES ${GOOGLETEST_SOURCES} )

target_link_libraries(helloworld.deb PRIVATE
    helloworld
)
target_link_libraries(test_version
    ${Boost_LIBRARIES}
    helloworld
)

target_link_libraries(test_version_g
    gtest 
    ${CMAKE_THREAD_LIBS_INIT}
    helloworld gtest_main
)

if (MSVC)
    target_compile_options(helloworld.deb PRIVATE
        /W4
    )
    target_compile_options(helloworld PRIVATE
        /W4
    )
    target_compile_options(test_version PRIVATE
        /W4
    )
    target_compile_options(test_version_g PRIVATE
        /W4
    )
else ()
    target_compile_options(helloworld.deb PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(helloworld PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_version_g PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS helloworld.deb RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_version test_version)
add_test(test_version_g test_version_g)
