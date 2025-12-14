cmake_minimum_required(VERSION 3.15)
project(AshkeLord VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find SFML
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/ResourceManager.cpp
    src/Entities/Entity.cpp
    src/Entities/Player.cpp
    src/Entities/NPC.cpp
    src/Entities/HealerNPC.cpp
    src/Entities/MerchantNPC.cpp
    src/Entities/EnemyNPC.cpp
    src/Entities/MysticNPC.cpp
    src/Entities/QuestNPC.cpp
)

# Header files (for IDE support)
set(HEADERS
    include/Game.hpp
    include/ResourceManager.hpp
    include/Entities/Entity.hpp
    include/Entities/Player.hpp
    include/Entities/NPC.hpp
    include/Entities/HealerNPC.hpp
    include/Entities/MerchantNPC.hpp
    include/Entities/EnemyNPC.hpp
    include/Entities/MysticNPC.hpp
    include/Entities/QuestNPC.hpp
)

# Create executable
add_executable(AshkeLord ${SOURCES} ${HEADERS})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME}
    SFML::System
    SFML::Window
    SFML::Graphics
    SFML::Audio
)

# Copy assets to build directory (optional, for easier asset loading)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

