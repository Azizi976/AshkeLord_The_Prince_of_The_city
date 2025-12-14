#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Entities/Player.hpp"
#include "Entities/NPC.hpp"
#include "Entities/HealerNPC.hpp"
#include "Entities/MerchantNPC.hpp"
#include "Entities/EnemyNPC.hpp"
#include "Entities/MysticNPC.hpp"
#include "Entities/QuestNPC.hpp"

/**
 * Game - Main game class that manages the window, game loop, and game state
 * Implements the standard game loop: Handle Input -> Update -> Render
 */
class Game {
public:
    /**
     * GameState - Enumeration of possible game states
     */
    enum class GameState {
        Menu,     // Main menu screen
        Settings, // Settings menu
        Playing   // In-game state
    };

    Game();
    ~Game();

    /**
     * Initialize the game (load resources, set up initial state)
     * @return true if initialization successful, false otherwise
     */
    bool initialize();

    /**
     * Run the main game loop
     */
    void run();

private:
    /**
     * Handle window events (close, resize, etc.)
     */
    void handleEvents();

    /**
     * Update game logic based on current state
     * @param deltaTime Time elapsed since last frame in seconds
     */
    void update(float deltaTime);

    /**
     * Render everything to the window based on current state
     */
    void render();

    /**
     * Update menu state (handle mouse input, button interactions)
     * @param deltaTime Time elapsed since last frame
     */
    void updateMenu(float deltaTime);

    /**
     * Render the menu screen (title and buttons)
     */
    void drawMenu();

    /**
     * Update settings menu state
     * @param deltaTime Time elapsed since last frame
     */
    void updateSettings(float deltaTime);

    /**
     * Render the settings menu screen
     */
    void drawSettings();

    /**
     * Update playing state (game logic)
     * @param deltaTime Time elapsed since last frame
     */
    void updatePlaying(float deltaTime);

    /**
     * Render the playing state (game entities)
     */
    void drawPlaying();

    /**
     * Check if mouse is hovering over a text button
     * @param text The text object to check
     * @return true if mouse is over the text
     */
    bool isMouseOverText(const sf::Text& text) const;

    /**
     * Handle NPC interaction (check if player is near NPC and presses E/Enter)
     */
    void handleNPCInteraction();

    // Window and rendering
    std::unique_ptr<sf::RenderWindow> window_;
    sf::Clock clock_; // For calculating delta time
    
    // Game entities
    std::unique_ptr<Player> player_;
    std::vector<std::unique_ptr<NPC>> m_npcs; // All NPCs in the game
    
    // Game state
    GameState m_gameState;
    bool isRunning_;
    
    // Interaction
    bool interactionKeyPressed_; // Track if E/Enter was pressed this frame
    
    // Menu elements
    std::shared_ptr<sf::Font> m_font;
    sf::Text titleText_;           // "ASHKELORD" title
    sf::Text yallaButton_;         // "YALLA (Start)" button
    sf::Text settingsButton_;      // "SETTINGS" button
    sf::Text habaytaButton_;       // "HABAYTA (Exit)" button
    
    // Settings menu elements
    sf::Text settingsTitleText_;           // "SETTINGS" title
    sf::Text fullscreenLabel_;             // "Fullscreen:" label
    sf::Text fullscreenToggle_;            // "ON" / "OFF" toggle
    sf::Text resolutionLabel_;             // "Resolution:" label
    sf::Text resolutionValue_;             // Current resolution display
    sf::Text resolutionLeftArrow_;         // "<" arrow
    sf::Text resolutionRightArrow_;        // ">" arrow
    sf::Text backButton_;                  // "BACK" button
    
    // Settings data
    struct Settings {
        bool fullscreen;
        unsigned int resolutionIndex; // Index into available resolutions
        std::vector<sf::VideoMode> availableResolutions;
        
        Settings() : fullscreen(true), resolutionIndex(0) {}
    } gameSettings_;
    
    /**
     * Apply current settings (recreate window if needed)
     */
    void applySettings();
    
    /**
     * Initialize available resolutions
     */
    void initializeResolutions();
    
    // Menu colors
    static constexpr sf::Color GOLD_COLOR = sf::Color(255, 215, 0);
    static constexpr sf::Color WHITE_COLOR = sf::Color::White;
    
    // Window constants
    static constexpr unsigned int WINDOW_WIDTH = 1920;
    static constexpr unsigned int WINDOW_HEIGHT = 1080;
    static constexpr const char* WINDOW_TITLE = "AshkeLord - The Ars Adventure";
    
    // Get current window size (for dynamic resolution support)
    sf::Vector2u getWindowSize() const;
};

#endif // GAME_HPP

