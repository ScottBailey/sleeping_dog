# @copyright See `LICENSE` in the root directory of this project.
# third_party/CMakeLists.txt


# Default FetchContent to Verbose. But don't override a user's explicit setting.
if(NOT FETCHCONTENT_QUIET)
  set(FETCHCONTENT_QUIET Off)
endif()


# Download CPM.cmake
#  Switch this to upstream if/when keyword_PATCHES is accepted.
file(
  DOWNLOAD      https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.2/CPM.cmake ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=c8cdc32c03816538ce22781ed72964dc864b2a34a310d3b7104812a5ca2d835d
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)



# fmt
CPMAddPackage(
  NAME     fmt
  URL      https://github.com/fmtlib/fmt/archive/refs/tags/11.0.2.tar.gz
  URL_HASH SHA256=6cb1e6d37bdcb756dbbe59be438790db409cdb4868c66e888d5df9f13f7c027f
  SYSTEM True
  OPTIONS
    "FMT_INSTALL OFF"
    "FMT_TEST OFF"
    "FMT_UNICODE OFF"
)
# spdlog
CPMAddPackage(
  NAME     spdlog
  URL      https://github.com/gabime/spdlog/archive/refs/tags/v1.14.1.tar.gz
  URL_HASH SHA256=1586508029a7d0670dfcb2d97575dcdc242d3868a259742b69f100801ab4e16b
  SYSTEM True
  OPTIONS
    "SPDLOG_FMT_EXTERNAL ON"
)
# sqlite
CPMAddPackage(
  NAME     SQLite3
  URL      https://www.sqlite.org/2024/sqlite-amalgamation-3460000.zip
  URL_HASH SHA256=712a7d09d2a22652fb06a49af516e051979a3984adb067da86760e60ed51a7f5
  SYSTEM True
  PATCHES  sqlite.patch
)

# CLI11
CPMAddPackage(
  NAME     CLI11
  URL      https://github.com/CLIUtils/CLI11/archive/refs/tags/v2.4.2.tar.gz
  URL_HASH SHA256=f2d893a65c3b1324c50d4e682c0cdc021dd0477ae2c048544f39eed6654b699a
  SYSTEM   True
)



# Boost libraries
CPMAddPackage(
  NAME Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.87.0/boost-1.87.0-cmake.tar.xz
  URL_HASH SHA256=7da75f171837577a52bbf217e17f8ea576c7c246e4594d617bfde7fafd408be5
  SYSTEM True
  OPTIONS
    "BOOST_ENABLE_CMAKE ON"
    "BOOST_SKIP_INSTALL_RULES ON" # Set `OFF` for installation
    "BUILD_SHARED_LIBS OFF"
    #"BOOST_INCLUDE_LIBRARIES headers\\\;system\\\;url\\\;beast\\\;json" # Note the escapes!
)
# Boost SQLite
CPMAddPackage(
  NAME boost_sqlite
  URL https://github.com/klemens-morgenstern/sqlite/archive/refs/heads/post-review.tar.gz
  URL_HASH SHA256=182646d186f4614001c778747359f411de79071085eea6fdaa9857f5a9e08c7c
  SYSTEM True
)

# magic_enum
#  Reflection for enumerations, pretty cool!
#  Note the limitations: https://github.com/Neargye/magic_enum/blob/master/doc/limitations.md
set(CMAKE_FOLDER "${BASE_FOLDER}/magic_enum")
CPMAddPackage(
  NAME magic_enum
  URL      https://github.com/Neargye/magic_enum/archive/refs/tags/v0.9.5.tar.gz
  URL_HASH SHA256=44ad80db5a72f5047e01d90e18315751d9ac90c0ab42cbea7a6f9ec66a4cd679
  SYSTEM   True
)
# Google's benchmarking tool
CPMAddPackage(
  NAME     benchmark
  URL      https://github.com/google/benchmark/archive/refs/tags/v1.8.3.tar.gz
  URL_HASH SHA256=6bc180a57d23d4d9515519f92b0c83d61b05b5bab188961f36ac7b06b0d9e9ce
  SYSTEM   True
  OPTIONS
    "BENCHMARK_ENABLE_INSTALL Off"
    "BENCHMARK_ENABLE_TESTING Off"
    "BENCHMARK_INSTALL_DOCS Off"
    "BENCHMARK_ENABLE_GTEST_TESTS Off"
)


# Catch2
CPMAddPackage(
  NAME     catch2
  URL      https://github.com/catchorg/Catch2/archive/refs/tags/v3.6.0.tar.gz
  URL_HASH SHA256=485932259a75c7c6b72d4b874242c489ea5155d17efa345eb8cc72159f49f356
  OPTIONS
    "CATCH_INSTALL_DOCS Off"
    "CATCH_INSTALL_EXTRAS Off"
    "CATCH_DEVELOPMENT_BUILD Off"
    "CATCH_ENABLE_REPRODUCIBLE_BUILD Off"
    "CATCH_BUILD_TESTING Off"
    "CATCH_ENABLE_WERROR Off"
)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)
