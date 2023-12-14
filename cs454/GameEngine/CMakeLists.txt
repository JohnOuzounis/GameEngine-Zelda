# Define the sources for the GameEngine
set(ENGINE_SOURCES
  include/GameEngine/JSON/Property.h
  include/GameEngine/JSON/StringProperty.h
  src/GameEngine/JSON/StringProperty.cpp
  include/GameEngine/JSON/AggregateProperty.h
  src/GameEngine/JSON/AggregateProperty.cpp
  include/GameEngine/JSON/NumericProperty.h
  src/GameEngine/JSON/NumericProperty.cpp
  include/GameEngine/JSON/BooleanProperty.h
  src/GameEngine/JSON/BooleanProperty.cpp
  include/GameEngine/JSON/Configurable.h
  include/GameEngine/JSON/Configurator.h
  src/GameEngine/JSON/Configurator.cpp
  include/GameEngine/Graphics/Event.h
  src/GameEngine/Graphics/Event.cpp
  include/GameEngine/Graphics/Window.h
  src/GameEngine/Graphics/Window.cpp
  include/GameEngine/Graphics/Renderer.h
  src/GameEngine/Graphics/Renderer.cpp
  include/GameEngine/Graphics/Image.h
  include/GameEngine/Graphics/Rect.h
  include/GameEngine/Graphics/Color.h
  src/GameEngine/Graphics/Image.cpp
  include/GameEngine/Collider.h
  include/GameEngine/CircleCollider2D.h
  "include/GameEngine/BoxCollider2D.h" 
  "src/GameEngine/BoxCollider2D.cpp" 
  "src/GameEngine/CircleCollider2D.cpp"
  include/GameEngine/Math.h
  include/GameEngine/System.h
  include/GameEngine/Audio.h
  src/GameEngine/Audio.cpp
  include/GameEngine/Time.h
  include/GameEngine/Graphics/Text.h
  src/GameEngine/Graphics/Text.cpp
  include/GameEngine/Graphics/Texture.h
  include/GameEngine/Camera.h
  include/GameEngine/Input.h
  src/GameEngine/Input.cpp
  include/GameEngine/Graphics/Light2D.h
  src/GameEngine/Graphics/Light2D.cpp
  include/GameEngine/Graphics/Button.h
  src/GameEngine/Graphics/Button.cpp
  include/GameEngine/Graphics/Panel.h
  include/GameEngine/Graphics/UIElement.h
  include/GameEngine/ImageLoader.h
  src/GameEngine/Time.cpp
  include/GameEngine/JSON/ArrayProperty.h
  src/GameEngine/JSON/ArrayProperty.cpp
 include/GameEngine/App.h "include/GameEngine/Game.h" "src/GameEngine/Game.cpp" "include/GameEngine/Resources.h" "include/GameEngine/Graphics/Tilemap.h" "src/GameEngine/Graphics/Tilemap.cpp" "include/GameEngine/Scene.h" "include/GameEngine/SceneManager.h" "include/GameEngine/Graphics/Gridmap.h" "src/GameEngine/Graphics/Gridmap.cpp" "include/GameEngine/CharacterController.h" "src/GameEngine/CharacterController.cpp" "include/GameEngine/Graphics/CircuralBackground.h")


# Create a library for the GameEngine
add_library(gameengine ${ENGINE_SOURCES})

# Include the 'include' directory for common headers
target_include_directories(gameengine PRIVATE ${CMAKE_SOURCE_DIR}/cs454/ThirdParty/include)
target_include_directories(gameengine PRIVATE ${CMAKE_SOURCE_DIR}/cs454/GameEngine/include)