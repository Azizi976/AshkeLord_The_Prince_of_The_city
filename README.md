# AshkeLord

A 2D Top-Down RPG game in C++ using SFML - A parody of Pokemon set in the Israeli city of Ashkelon.

## Project Overview

**Genre:** 2D RPG / Grid-based movement

**Protagonist:** An "Ars" (Israeli stereotype) who evolves his street status instead of leveling up (e.g., getting a better haircut, buying a Honda Civic, upgrading his flip-flops).

**Setting:** Different neighborhoods of Ashkelon

## Tech Stack

- **Language:** C++17
- **Library:** SFML 2.5+ (Graphics, Window, System, Audio)
- **Build System:** CMake 3.15+

## Building the Project

### Prerequisites

1. Install CMake (3.15 or higher)
2. Install SFML 2.5 or higher
   - On Windows: Download from [SFML website](https://www.sfml-dev.org/download.php)
   - On Linux: `sudo apt-get install libsfml-dev` (Ubuntu/Debian)
   - On macOS: `brew install sfml`

### Build Instructions

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

# Run the game
./bin/AshkeLord  # Linux/macOS
# or
bin\AshkeLord.exe  # Windows
```

## Project Structure

```
AshkeLord/
├── CMakeLists.txt          # CMake build configuration
├── README.md               # This file
├── src/                    # Source files
│   ├── main.cpp           # Entry point
│   ├── Game.cpp           # Main game class
│   ├── ResourceManager.cpp # Resource management
│   └── Entities/
│       ├── Entity.cpp     # Base entity class
│       └── Player.cpp     # Player class
├── include/                # Header files
│   ├── Game.hpp
│   ├── ResourceManager.hpp
│   └── Entities/
│       ├── Entity.hpp
│       └── Player.hpp
└── assets/                 # Game assets
    └── textures/
        └── ars_sprite.png  # Player sprite (you need to add this)
```

## Controls

- **Movement:** WASD or Arrow Keys
- **Close Window:** Click the X button or Alt+F4

## Game Terminology

- **Street Cred:** Experience points (replaces XP)
- **Nervim Level:** Energy/mana level (nervim = nerves/energy in Hebrew slang)

## Features

- ✅ Modular architecture with separate header/implementation files
- ✅ RAII memory management with smart pointers
- ✅ ResourceManager singleton for efficient texture/font loading
- ✅ Entity-Component system with base Entity class
- ✅ Player movement with WASD/Arrow keys
- ✅ Standard game loop (Input -> Update -> Render)

## Next Steps

To complete the game, you'll need to:

1. Add the player sprite (`ars_sprite.png`) to `assets/textures/`
2. Implement grid-based movement
3. Add NPCs and enemies
4. Create different neighborhoods/maps
5. Implement the "evolution" system (street status upgrades)
6. Add combat mechanics
7. Create a UI/HUD system

## License

This is a personal project for educational purposes.

