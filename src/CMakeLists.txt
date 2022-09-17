set(SDL2_DIR "C:/devel/SDL2-VC/cmake")
find_package(SDL2 REQUIRED)

add_executable(facilitate
    main.cpp
    player.cpp
    timer.cpp
    object.cpp
)

target_include_directories(facilitate PRIVATE 
    .
)

target_include_directories(facilitate PUBLIC
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(facilitate PUBLIC
    ${SDL2_LIBRARIES}
)