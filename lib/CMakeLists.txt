
set(target_name "sleeping_dog")

find_package(OpenSSL REQUIRED)


file(GLOB_RECURSE sb_hpp  CONFIGURE_DEPENDS sb/*.hpp sb/*.ipp)
file(GLOB_RECURSE src_hpp CONFIGURE_DEPENDS include/*.hpp)
file(GLOB_RECURSE src_cpp CONFIGURE_DEPENDS src/*.cpp)


add_library(${target_name} STATIC
  ${src_cpp} ${src_hpp}
  ${sb_hpp}
)
target_include_directories(${target_name} PRIVATE
  include
)
target_include_directories(${target_name} PUBLIC
  .   # for `sb/sleeping_dog/...`
)
target_link_libraries(${target_name} PUBLIC
  Boost::beast
  OpenSSL::SSL
  fmt::fmt
  spdlog::spdlog
)
