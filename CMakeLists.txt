cmake_minimum_required(VERSION 3.20)
project(jpeg_meta LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(jpegmeta src/main.cpp)

# Ostrzeżenia i standardowa „higiena”
if (MSVC)
  target_compile_options(jpegmeta PRIVATE
    /W4 /permissive- /Zc:__cplusplus /Zc:preprocessor /Zc:inline /Zc:throwingNew /EHsc /utf-8
  )
else()
  target_compile_options(jpegmeta PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Exiv2 z vcpkg (CONFIG mode)
find_package(exiv2 CONFIG REQUIRED)

# Różne pakiety Exiv2 eksportują różne nazwy targetów – pokryjmy najczęstsze:
if (TARGET Exiv2::exiv2)
  target_link_libraries(jpegmeta PRIVATE Exiv2::exiv2)
elseif (TARGET exiv2lib)
  target_link_libraries(jpegmeta PRIVATE exiv2lib)
elseif (TARGET exiv2)
  target_link_libraries(jpegmeta PRIVATE exiv2)
else()
  message(FATAL_ERROR "Nie znaleziono importowanego targetu exiv2.*")
endif()

# Test „dymny” – uruchomienie bez argumentów powinno zwrócić kod błędu != 0
include(CTest)
enable_testing()
add_test(NAME run_without_args COMMAND jpegmeta)
set_tests_properties(run_without_args PROPERTIES WILL_FAIL TRUE)

# Instalacja i paczkowanie (CPack)
install(TARGETS jpegmeta)
include(CPack)
