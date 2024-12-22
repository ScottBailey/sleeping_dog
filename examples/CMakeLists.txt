
set(target_name puppy)
add_executable(puppy puppy.cpp)
target_link_libraries(puppy PRIVATE
  sleeping_dog
  CLI11::CLI11
)
