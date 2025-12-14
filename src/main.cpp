#include "Game.hpp"
#include <iostream>

/**
 * AshkeLord - A 2D Top-Down RPG
 * A parody of Pokemon set in the Israeli city of Ashkelon
 * 
 * Main entry point for the game
 */
int main() {
    try {
        // Create and initialize the game
        Game game;
        
        if (!game.initialize()) {
            std::cerr << "Failed to initialize game. Exiting." << std::endl;
            return 1;
        }
        
        // Run the game loop
        game.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

