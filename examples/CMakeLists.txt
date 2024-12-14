
set(target_name example)


add_executable(${target_name} example.cpp)
target_link_libraries(${target_name} PRIVATE
  sleeping_dog
)
