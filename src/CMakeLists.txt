set(SDL2_DIR "C:/devel/SDL2-VC/cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(facilitate
    main.cpp
    timer.cpp
    object.cpp
    texture.cpp
)

target_sources(facilitate PRIVATE
    ecs/systems/physics_system.cpp
    ecs/systems/render_system.cpp
    ecs/systems/player_control_system.cpp
    ecs/systems/collision_system.cpp
)

target_include_directories(facilitate PRIVATE 
    .

)

target_include_directories(facilitate PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

target_link_libraries(facilitate PUBLIC
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)
