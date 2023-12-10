cmake_minimum_required (VERSION 3.18.4)

# Executable
add_executable(unit1 
  "src/main.cpp"
 "src/TilemapMaker.h" "src/Unit1.h" "src/Unit1.cpp" "src/Scene1.h")

# Include the 'include' directory for common headers
target_include_directories(unit1 PRIVATE ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/include)
target_include_directories(unit1 PRIVATE ${CMAKE_SOURCE_DIR}/cs454/GameEngine/include)

target_link_libraries(unit1 ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2.lib)
target_link_libraries(unit1 ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2main.lib)
target_link_libraries(unit1 ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2_image.lib)
target_link_libraries(unit1 ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2_mixer.lib)
target_link_libraries(unit1 ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/sdl/SDL2_ttf.lib)
					  						   
target_link_libraries(unit1 ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/boost/libboost_chrono-vc143-mt-gd-x64-1_83.lib)
target_link_libraries(unit1 ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/boost/libboost_json-vc143-mt-gd-x64-1_83.lib)
target_link_libraries(unit1 ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/lib/boost/libboost_container-vc143-mt-gd-x64-1_83.lib)
											   
# Link the GameEngine source files
target_link_libraries(unit1 gameengine)