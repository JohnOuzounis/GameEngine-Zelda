cmake_minimum_required (VERSION 3.18.4)

# Executable
add_executable(zelda 
  "src/main.cpp" "src/Zelda.h" "src/Zelda.cpp" "src/GameScene/GameScene.h" "src/GameScene/GameScene.cpp" "src/GameScene/Player.h" "src/GameScene/Elevator.h" "src/GameScene/Door.h" "src/GameScene/Item.h" "src/AudioManager.h" "src/AudioManager.cpp" "src/GameScene/Enemy.h" "src/GameScene/Wosu.h" "src/GameScene/Health.h" "src/GameScene/Bot.h" "src/GameScene/Stalfos.h" "src/GameScene/PauseMenu.h" "src/MenuScene/MenuScene.h" "src/MenuScene/MainMenu.h" "src/MenuScene/MenuScene.cpp" "src/MenuScene/ControlsMenu.h" "src/GameScene/PlayerHub.h" "src/GameScene/Guma.h")

# Include the 'include' directory for common headers
target_include_directories(zelda PRIVATE ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/include)
target_include_directories(zelda PRIVATE ${CMAKE_SOURCE_DIR}/cs454/GameEngine/include)

target_link_libraries(zelda ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2.lib)
target_link_libraries(zelda ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2main.lib)
target_link_libraries(zelda ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2_image.lib)
target_link_libraries(zelda ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2_mixer.lib)
target_link_libraries(zelda ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2_ttf.lib)
					  						   
target_link_libraries(zelda ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/boost/libboost_chrono-vc143-mt-gd-x64-1_83.lib)
target_link_libraries(zelda ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/boost/libboost_json-vc143-mt-gd-x64-1_83.lib)
target_link_libraries(zelda ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/boost/libboost_container-vc143-mt-gd-x64-1_83.lib)
											   
# Link the GameEngine source files
target_link_libraries(zelda gameengine)

# Executable
add_executable(zeldagrid "src/GridMaker.cpp" "../Unit2/src/GridMaker.h" "../Unit1/src/TilemapMaker.h" "../Unit2/src/TileColorHolder.h")

# Include the 'include' directory for common headers
target_include_directories(zeldagrid PRIVATE ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/include)
target_include_directories(zeldagrid PRIVATE ${CMAKE_SOURCE_DIR}/cs454/GameEngine/include)

target_link_libraries(zeldagrid ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2.lib)
target_link_libraries(zeldagrid ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2main.lib)
target_link_libraries(zeldagrid ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2_image.lib)
target_link_libraries(zeldagrid ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2_mixer.lib)
target_link_libraries(zeldagrid ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2_ttf.lib)
					  						   
target_link_libraries(zeldagrid ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/boost/libboost_chrono-vc143-mt-gd-x64-1_83.lib)
target_link_libraries(zeldagrid ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/boost/libboost_json-vc143-mt-gd-x64-1_83.lib)
target_link_libraries(zeldagrid ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/boost/libboost_container-vc143-mt-gd-x64-1_83.lib)
											   
# Link the GameEngine source files
target_link_libraries(zeldagrid gameengine)
