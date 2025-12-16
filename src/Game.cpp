#include "Game.hpp"
#include "ResourceManager.hpp"
#include "UI/DialogueBox.hpp" // הוספנו את זה
#include <iostream>
#include <algorithm>

Game::Game() : m_gameState(GameState::Menu), isRunning_(false), interactionKeyPressed_(false) {
    // Create window in fullscreen mode at 1920x1080
    // Try fullscreen first, fallback to windowed if fullscreen fails
    auto videoMode = sf::VideoMode::getFullscreenModes();
    bool fullscreenCreated = false;
    
    // Try to find 1920x1080 fullscreen mode
    for (const auto& mode : videoMode) {
        if (mode.width == WINDOW_WIDTH && mode.height == WINDOW_HEIGHT) {
            window_ = std::make_unique<sf::RenderWindow>(
                mode,
                WINDOW_TITLE,
                sf::Style::Fullscreen
            );
            fullscreenCreated = true;
            break;
        }
    }
    
    // If fullscreen 1920x1080 not available, try any fullscreen mode
    if (!fullscreenCreated && !videoMode.empty()) {
        window_ = std::make_unique<sf::RenderWindow>(
            videoMode[0],
            WINDOW_TITLE,
            sf::Style::Fullscreen
        );
        fullscreenCreated = true;
    }
    
    // Fallback to windowed mode if fullscreen failed
    if (!fullscreenCreated) {
        window_ = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
            WINDOW_TITLE,
            sf::Style::Close | sf::Style::Titlebar
        );
        std::cout << "Fullscreen not available, using windowed mode." << std::endl;
    } else {
        std::cout << "Game running in fullscreen mode: " 
                  << window_->getSize().x << "x" << window_->getSize().y << std::endl;
    }
    
    // Set framerate limit (optional, for consistent performance)
    window_->setFramerateLimit(60);
}

Game::~Game() {
    // Smart pointers will handle cleanup automatically
}

bool Game::initialize() {
    if (!window_) {
        std::cerr << "Error: Failed to create window." << std::endl;
        return false;
    }

    // Load game resources
    auto& resourceManager = ResourceManager::getInstance();
    
    // Load default font - try multiple common system font paths
    bool fontLoaded = false;
    
    // Try custom font first
    if (resourceManager.loadFont("default", "assets/fonts/arial.ttf")) {
        fontLoaded = true;
    }
    // Try Windows system fonts
    else if (resourceManager.loadFont("default", "C:/Windows/Fonts/arial.ttf")) {
        fontLoaded = true;
    }
    else if (resourceManager.loadFont("default", "C:/Windows/Fonts/calibri.ttf")) {
        fontLoaded = true;
    }
    // Try Linux system fonts
    else if (resourceManager.loadFont("default", "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        fontLoaded = true;
    }
    // Try macOS system fonts
    else if (resourceManager.loadFont("default", "/System/Library/Fonts/Helvetica.ttc")) {
        fontLoaded = true;
    }
    
    if (!fontLoaded) {
        std::cerr << "Warning: Could not load any font. Menu text will not display." << std::endl;
        std::cerr << "Please add a font file to assets/fonts/ (e.g., arial.ttf)" << std::endl;
    }
    
    // Get font
    m_font = resourceManager.getFont("default");

    // --- אתחול מערכת הדיאלוגים (חדש!) ---
    if (m_font) {
        m_dialogueBox.init(m_font, getWindowSize());
    }
    
    // Initialize menu text elements
    if (m_font) {
        // Title: "ASHKELORD"
        titleText_.setFont(*m_font);
        titleText_.setString("ASHKELORD");
        titleText_.setCharacterSize(72); // Large, bold appearance
        titleText_.setFillColor(GOLD_COLOR);
        titleText_.setStyle(sf::Text::Bold);
        
        // Center title horizontally, position near top
        sf::FloatRect titleBounds = titleText_.getLocalBounds();
        titleText_.setOrigin(titleBounds.width / 2.0f, titleBounds.height / 2.0f);
        // Use dynamic window size for positioning
        sf::Vector2u windowSize = getWindowSize();
        titleText_.setPosition(windowSize.x / 2.0f, windowSize.y * 0.15f);
        
        // YALLA button
        yallaButton_.setFont(*m_font);
        yallaButton_.setString("YALLA (Start)");
        yallaButton_.setCharacterSize(40);
        yallaButton_.setFillColor(GOLD_COLOR);
        
        sf::FloatRect yallaBounds = yallaButton_.getLocalBounds();
        yallaButton_.setOrigin(yallaBounds.width / 2.0f, yallaBounds.height / 2.0f);
        sf::Vector2u windowSize = getWindowSize();
        yallaButton_.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f - 80.0f);
        
        // SETTINGS button
        settingsButton_.setFont(*m_font);
        settingsButton_.setString("SETTINGS");
        settingsButton_.setCharacterSize(40);
        settingsButton_.setFillColor(GOLD_COLOR);
        
        sf::FloatRect settingsBounds = settingsButton_.getLocalBounds();
        settingsButton_.setOrigin(settingsBounds.width / 2.0f, settingsBounds.height / 2.0f);
        settingsButton_.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
        
        // HABAYTA button
        habaytaButton_.setFont(*m_font);
        habaytaButton_.setString("HABAYTA (Exit)");
        habaytaButton_.setCharacterSize(40);
        habaytaButton_.setFillColor(GOLD_COLOR);
        
        sf::FloatRect habaytaBounds = habaytaButton_.getLocalBounds();
        habaytaButton_.setOrigin(habaytaBounds.width / 2.0f, habaytaBounds.height / 2.0f);
        habaytaButton_.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f + 80.0f);
        
        // Initialize settings menu elements
        initializeSettingsMenu();
    } else {
        std::cerr << "Error: Font not available. Menu may not display correctly." << std::endl;
    }
    
    // Initialize available resolutions
    initializeResolutions();
    
    // Load player texture (assuming it exists in assets/textures/)
    if (!resourceManager.loadTexture("ars_sprite", "assets/textures/ars_sprite.png")) {
        std::cerr << "Warning: Could not load player texture. Using fallback." << std::endl;
    }

    // Create player
    player_ = std::make_unique<Player>();
    
    // Center player on screen (use dynamic window size)
    sf::Vector2u windowSize = getWindowSize();
    if (player_) {
        player_->setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    }

    // Initialize NPCs at different positions (scaled for 1920x1080)
    // HealerNPC - Miri "Kapara" (Red)
    auto healer = std::make_unique<HealerNPC>();
    healer->setPosition(windowSize.x * 0.2f, windowSize.y * 0.3f);
    m_npcs.push_back(std::move(healer));

    // MerchantNPC - Zion "Scissors" (Green)
    auto merchant = std::make_unique<MerchantNPC>();
    merchant->setPosition(windowSize.x * 0.8f, windowSize.y * 0.3f);
    m_npcs.push_back(std::move(merchant));

    // EnemyNPC - Avi "Bicep" (Yellow)
    auto enemy = std::make_unique<EnemyNPC>();
    enemy->setPosition(windowSize.x * 0.2f, windowSize.y * 0.7f);
    m_npcs.push_back(std::move(enemy));

    // MysticNPC - Baba-Rentgen (Magenta)
    auto mystic = std::make_unique<MysticNPC>();
    mystic->setPosition(windowSize.x * 0.8f, windowSize.y * 0.7f);
    m_npcs.push_back(std::move(mystic));

    // QuestNPC - Yotam "The Tel-Avivian" (Cyan)
    auto quest = std::make_unique<QuestNPC>();
    quest->setPosition(windowSize.x * 0.5f, windowSize.y * 0.15f);
    m_npcs.push_back(std::move(quest));

    isRunning_ = true;
    std::cout << "Game initialized successfully!" << std::endl;
    std::cout << "NPCs spawned: " << m_npcs.size() << std::endl;
    return true;
}

void Game::run() {
    if (!isRunning_) {
        std::cerr << "Error: Game not initialized. Call initialize() first." << std::endl;
        return;
    }

    // Main game loop
    while (window_->isOpen() && isRunning_) {
        // Calculate delta time
        float deltaTime = clock_.restart().asSeconds();

        // Game loop: Handle Input -> Update -> Render
        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_->close();
            isRunning_ = false;
        }
        
        // Handle menu button clicks
        if (m_gameState == GameState::Menu && event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = window_->mapPixelToCoords(mousePos);
                
                // Check YALLA button click
                if (m_font && yallaButton_.getGlobalBounds().contains(worldPos)) {
                    m_gameState = GameState::Playing;
                }
                
                // Check SETTINGS button click
                if (m_font && settingsButton_.getGlobalBounds().contains(worldPos)) {
                    m_gameState = GameState::Settings;
                }
                
                // Check HABAYTA button click
                if (m_font && habaytaButton_.getGlobalBounds().contains(worldPos)) {
                    window_->close();
                    isRunning_ = false;
                }
            }
        }
        
        // Handle settings menu clicks
        if (m_gameState == GameState::Settings && event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = window_->mapPixelToCoords(mousePos);
                
                // Check BACK button click
                if (m_font && backButton_.getGlobalBounds().contains(worldPos)) {
                    m_gameState = GameState::Menu;
                }
                
                // Check fullscreen toggle click
                if (m_font && fullscreenToggle_.getGlobalBounds().contains(worldPos)) {
                    gameSettings_.fullscreen = !gameSettings_.fullscreen;
                    applySettings();
                }
                
                // Check resolution arrows
                if (m_font && resolutionLeftArrow_.getGlobalBounds().contains(worldPos)) {
                    if (gameSettings_.resolutionIndex > 0) {
                        gameSettings_.resolutionIndex--;
                        applySettings();
                    }
                }
                
                if (m_font && resolutionRightArrow_.getGlobalBounds().contains(worldPos)) {
                    if (gameSettings_.resolutionIndex < gameSettings_.availableResolutions.size() - 1) {
                        gameSettings_.resolutionIndex++;
                        applySettings();
                    }
                }
            }
        }
        
        // Handle NPC interaction (E or Enter key)
        if (m_gameState == GameState::Playing && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::E || event.key.code == sf::Keyboard::Return) {
                interactionKeyPressed_ = true;
            }
        }
        
        // Handle other events here (key presses, etc.)
    }
}

void Game::update(float deltaTime) {
    // Update based on current game state
    switch (m_gameState) {
        case GameState::Menu:
            updateMenu(deltaTime);
            break;
        case GameState::Settings:
            updateSettings(deltaTime);
            break;
        case GameState::Playing:
            updatePlaying(deltaTime);
            break;
    }
}

void Game::render() {
    // Clear the window
    window_->clear(sf::Color(135, 206, 235)); // Sky blue background (Ashkelon sky!)
    
    // Render based on current game state
    switch (m_gameState) {
        case GameState::Menu:
            drawMenu();
            break;
        case GameState::Settings:
            drawSettings();
            break;
        case GameState::Playing:
            drawPlaying();
            break;
    }
    
    // Display everything
    window_->display();
}

void Game::updateMenu(float deltaTime) {
    // Check for mouse hover effects
    if (m_font) {
        // YALLA button hover
        if (isMouseOverText(yallaButton_)) {
            yallaButton_.setFillColor(WHITE_COLOR);
        } else {
            yallaButton_.setFillColor(GOLD_COLOR);
        }
        
        // SETTINGS button hover
        if (isMouseOverText(settingsButton_)) {
            settingsButton_.setFillColor(WHITE_COLOR);
        } else {
            settingsButton_.setFillColor(GOLD_COLOR);
        }
        
        // HABAYTA button hover
        if (isMouseOverText(habaytaButton_)) {
            habaytaButton_.setFillColor(WHITE_COLOR);
        } else {
            habaytaButton_.setFillColor(GOLD_COLOR);
        }
    }
}

void Game::drawMenu() {
    if (!m_font) {
        // Fallback: draw simple text if font not available
        return;
    }
    
    // Draw title with white outline effect (draw multiple times with offsets)
    // Create outline by drawing the text in white at multiple offsets, then draw gold on top
    sf::Text titleOutline = titleText_;
    titleOutline.setFillColor(WHITE_COLOR);
    
    // Draw white outline (8 directions around the text)
    const float outlineOffset = 2.0f;
    titleOutline.setPosition(titleText_.getPosition().x - outlineOffset, titleText_.getPosition().y);
    window_->draw(titleOutline);
    titleOutline.setPosition(titleText_.getPosition().x + outlineOffset, titleText_.getPosition().y);
    window_->draw(titleOutline);
    titleOutline.setPosition(titleText_.getPosition().x, titleText_.getPosition().y - outlineOffset);
    window_->draw(titleOutline);
    titleOutline.setPosition(titleText_.getPosition().x, titleText_.getPosition().y + outlineOffset);
    window_->draw(titleOutline);
    titleOutline.setPosition(titleText_.getPosition().x - outlineOffset, titleText_.getPosition().y - outlineOffset);
    window_->draw(titleOutline);
    titleOutline.setPosition(titleText_.getPosition().x + outlineOffset, titleText_.getPosition().y - outlineOffset);
    window_->draw(titleOutline);
    titleOutline.setPosition(titleText_.getPosition().x - outlineOffset, titleText_.getPosition().y + outlineOffset);
    window_->draw(titleOutline);
    titleOutline.setPosition(titleText_.getPosition().x + outlineOffset, titleText_.getPosition().y + outlineOffset);
    window_->draw(titleOutline);
    
    // Then draw main title in gold on top
    window_->draw(titleText_);
    
    // Draw buttons
    window_->draw(yallaButton_);
    window_->draw(settingsButton_);
    window_->draw(habaytaButton_);
}

void Game::updatePlaying(float deltaTime) {
    // --- עדכון מערכת הדיאלוגים (חדש!) ---
    // אם הדיאלוג פתוח, אנחנו מעדכנים רק אותו ויוצאים (מקפיאים את השחקן)
    if (m_dialogueBox.isActive()) {
        m_dialogueBox.update(deltaTime);
        return; 
    }

    // Update player
    if (player_) {
        player_->update(deltaTime);
    }
    
    // Update NPCs
    for (auto& npc : m_npcs) {
        if (npc) {
            npc->update(deltaTime);
        }
    }
    
    // Handle NPC interaction
    if (interactionKeyPressed_) {
        handleNPCInteraction();
        interactionKeyPressed_ = false; // Reset flag
    }
}

void Game::drawPlaying() {
    // Render NPCs first (so player appears on top)
    for (const auto& npc : m_npcs) {
        if (npc) {
            npc->render(*window_);
        }
    }
    
    // Render player
    if (player_) {
        player_->render(*window_);
    }
    
    // --- ציור מערכת הדיאלוגים (חדש!) ---
    // מציירים בסוף כדי שיופיע מעל הכל
    m_dialogueBox.render(*window_);
}

bool Game::isMouseOverText(const sf::Text& text) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window_);
    sf::Vector2f worldPos = window_->mapPixelToCoords(mousePos);
    
    sf::FloatRect textBounds = text.getGlobalBounds();
    return textBounds.contains(worldPos);
}

void Game::handleNPCInteraction() {
    if (!player_) {
        return;
    }

    sf::Vector2f playerPos = player_->getPosition();
    
    // Check each NPC to see if player is nearby
    for (auto& npc : m_npcs) {
        if (npc && npc->isPlayerNearby(playerPos)) {
            // --- הפעלת הדיאלוג (חדש!) ---
            // פותח את הקופסה עם הטקסט של ה-NPC ושם הדובר
            m_dialogueBox.showText(npc->getDialogue(), npc->getName());
            
            // Interact with the first nearby NPC
            npc->interact(*player_);
            break; // Only interact with one NPC at a time
        }
    }
}

sf::Vector2u Game::getWindowSize() const {
    if (window_) {
        return window_->getSize();
    }
    return sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Game::initializeSettingsMenu() {
    if (!m_font) {
        return;
    }
    
    sf::Vector2u windowSize = getWindowSize();
    
    // Settings title
    settingsTitleText_.setFont(*m_font);
    settingsTitleText_.setString("SETTINGS");
    settingsTitleText_.setCharacterSize(60);
    settingsTitleText_.setFillColor(GOLD_COLOR);
    settingsTitleText_.setStyle(sf::Text::Bold);
    
    sf::FloatRect settingsTitleBounds = settingsTitleText_.getLocalBounds();
    settingsTitleText_.setOrigin(settingsTitleBounds.width / 2.0f, settingsTitleBounds.height / 2.0f);
    settingsTitleText_.setPosition(windowSize.x / 2.0f, windowSize.y * 0.15f);
    
    // Fullscreen label
    fullscreenLabel_.setFont(*m_font);
    fullscreenLabel_.setString("Fullscreen:");
    fullscreenLabel_.setCharacterSize(35);
    fullscreenLabel_.setFillColor(GOLD_COLOR);
    
    sf::FloatRect fullscreenLabelBounds = fullscreenLabel_.getLocalBounds();
    fullscreenLabel_.setOrigin(0, fullscreenLabelBounds.height / 2.0f);
    fullscreenLabel_.setPosition(windowSize.x * 0.3f, windowSize.y * 0.4f);
    
    // Fullscreen toggle
    fullscreenToggle_.setFont(*m_font);
    fullscreenToggle_.setString(gameSettings_.fullscreen ? "ON" : "OFF");
    fullscreenToggle_.setCharacterSize(35);
    fullscreenToggle_.setFillColor(WHITE_COLOR);
    
    sf::FloatRect fullscreenToggleBounds = fullscreenToggle_.getLocalBounds();
    fullscreenToggle_.setOrigin(0, fullscreenToggleBounds.height / 2.0f);
    fullscreenToggle_.setPosition(windowSize.x * 0.6f, windowSize.y * 0.4f);
    
    // Resolution label
    resolutionLabel_.setFont(*m_font);
    resolutionLabel_.setString("Resolution:");
    resolutionLabel_.setCharacterSize(35);
    resolutionLabel_.setFillColor(GOLD_COLOR);
    
    sf::FloatRect resolutionLabelBounds = resolutionLabel_.getLocalBounds();
    resolutionLabel_.setOrigin(0, resolutionLabelBounds.height / 2.0f);
    resolutionLabel_.setPosition(windowSize.x * 0.3f, windowSize.y * 0.5f);
    
    // Resolution value
    resolutionValue_.setFont(*m_font);
    resolutionValue_.setCharacterSize(35);
    resolutionValue_.setFillColor(WHITE_COLOR);
    updateResolutionDisplay();
    
    // Resolution arrows
    resolutionLeftArrow_.setFont(*m_font);
    resolutionLeftArrow_.setString("<");
    resolutionLeftArrow_.setCharacterSize(40);
    resolutionLeftArrow_.setFillColor(GOLD_COLOR);
    
    sf::FloatRect leftArrowBounds = resolutionLeftArrow_.getLocalBounds();
    resolutionLeftArrow_.setOrigin(leftArrowBounds.width / 2.0f, leftArrowBounds.height / 2.0f);
    resolutionLeftArrow_.setPosition(windowSize.x * 0.55f, windowSize.y * 0.5f);
    
    resolutionRightArrow_.setFont(*m_font);
    resolutionRightArrow_.setString(">");
    resolutionRightArrow_.setCharacterSize(40);
    resolutionRightArrow_.setFillColor(GOLD_COLOR);
    
    sf::FloatRect rightArrowBounds = resolutionRightArrow_.getLocalBounds();
    resolutionRightArrow_.setOrigin(rightArrowBounds.width / 2.0f, rightArrowBounds.height / 2.0f);
    resolutionRightArrow_.setPosition(windowSize.x * 0.75f, windowSize.y * 0.5f);
    
    // Back button
    backButton_.setFont(*m_font);
    backButton_.setString("BACK");
    backButton_.setCharacterSize(40);
    backButton_.setFillColor(GOLD_COLOR);
    
    sf::FloatRect backBounds = backButton_.getLocalBounds();
    backButton_.setOrigin(backBounds.width / 2.0f, backBounds.height / 2.0f);
    backButton_.setPosition(windowSize.x / 2.0f, windowSize.y * 0.75f);
}

void Game::updateSettings(float deltaTime) {
    if (!m_font) {
        return;
    }
    
    // Update hover effects
    if (isMouseOverText(backButton_)) {
        backButton_.setFillColor(WHITE_COLOR);
    } else {
        backButton_.setFillColor(GOLD_COLOR);
    }
    
    if (isMouseOverText(fullscreenToggle_)) {
        fullscreenToggle_.setFillColor(GOLD_COLOR);
    } else {
        fullscreenToggle_.setFillColor(WHITE_COLOR);
    }
    
    if (isMouseOverText(resolutionLeftArrow_)) {
        resolutionLeftArrow_.setFillColor(WHITE_COLOR);
    } else {
        resolutionLeftArrow_.setFillColor(GOLD_COLOR);
    }
    
    if (isMouseOverText(resolutionRightArrow_)) {
        resolutionRightArrow_.setFillColor(WHITE_COLOR);
    } else {
        resolutionRightArrow_.setFillColor(GOLD_COLOR);
    }
}

void Game::drawSettings() {
    if (!m_font) {
        return;
    }
    
    // Draw settings title with outline
    sf::Text titleOutline = settingsTitleText_;
    titleOutline.setFillColor(WHITE_COLOR);
    const float outlineOffset = 2.0f;
    
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i != 0 || j != 0) {
                titleOutline.setPosition(
                    settingsTitleText_.getPosition().x + i * outlineOffset,
                    settingsTitleText_.getPosition().y + j * outlineOffset
                );
                window_->draw(titleOutline);
            }
        }
    }
    window_->draw(settingsTitleText_);
    
    // Draw settings options
    window_->draw(fullscreenLabel_);
    window_->draw(fullscreenToggle_);
    window_->draw(resolutionLabel_);
    window_->draw(resolutionValue_);
    window_->draw(resolutionLeftArrow_);
    window_->draw(resolutionRightArrow_);
    window_->draw(backButton_);
}

void Game::initializeResolutions() {
    // Get all available fullscreen modes
    auto fullscreenModes = sf::VideoMode::getFullscreenModes();
    
    // Filter common resolutions (remove duplicates and sort)
    std::vector<sf::VideoMode> commonResolutions;
    
    // Add common resolutions
    commonResolutions.push_back(sf::VideoMode(1920, 1080));
    commonResolutions.push_back(sf::VideoMode(1680, 1050));
    commonResolutions.push_back(sf::VideoMode(1600, 900));
    commonResolutions.push_back(sf::VideoMode(1366, 768));
    commonResolutions.push_back(sf::VideoMode(1280, 720));
    commonResolutions.push_back(sf::VideoMode(1024, 768));
    commonResolutions.push_back(sf::VideoMode(800, 600));
    
    // Remove duplicates and invalid modes
    gameSettings_.availableResolutions.clear();
    for (const auto& mode : commonResolutions) {
        if (mode.isValid()) {
            // Check if not already added
            bool exists = false;
            for (const auto& existing : gameSettings_.availableResolutions) {
                if (existing.width == mode.width && existing.height == mode.height) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                gameSettings_.availableResolutions.push_back(mode);
            }
        }
    }
    
    // Sort by resolution (largest first)
    std::sort(gameSettings_.availableResolutions.begin(), gameSettings_.availableResolutions.end(),
        [](const sf::VideoMode& a, const sf::VideoMode& b) {
            return (a.width * a.height) > (b.width * b.height);
        });
    
    // Find current resolution index
    sf::Vector2u currentSize = getWindowSize();
    for (size_t i = 0; i < gameSettings_.availableResolutions.size(); ++i) {
        if (gameSettings_.availableResolutions[i].width == currentSize.x &&
            gameSettings_.availableResolutions[i].height == currentSize.y) {
            gameSettings_.resolutionIndex = i;
            break;
        }
    }
    
    // If no match found, default to first (largest) resolution
    if (gameSettings_.availableResolutions.empty()) {
        gameSettings_.availableResolutions.push_back(sf::VideoMode(1920, 1080));
        gameSettings_.resolutionIndex = 0;
    }
}

void Game::updateResolutionDisplay() {
    if (!m_font || gameSettings_.availableResolutions.empty()) {
        return;
    }
    
    const auto& currentRes = gameSettings_.availableResolutions[gameSettings_.resolutionIndex];
    std::string resString = std::to_string(currentRes.width) + "x" + std::to_string(currentRes.height);
    resolutionValue_.setString(resString);
    
    sf::Vector2u windowSize = getWindowSize();
    sf::FloatRect resolutionValueBounds = resolutionValue_.getLocalBounds();
    resolutionValue_.setOrigin(0, resolutionValueBounds.height / 2.0f);
    resolutionValue_.setPosition(windowSize.x * 0.65f, windowSize.y * 0.5f);
}

void Game::applySettings() {
    if (gameSettings_.availableResolutions.empty()) {
        return;
    }
    
    const auto& selectedRes = gameSettings_.availableResolutions[gameSettings_.resolutionIndex];
    
    // Recreate window with new settings
    if (gameSettings_.fullscreen) {
        // Try to find matching fullscreen mode
        auto fullscreenModes = sf::VideoMode::getFullscreenModes();
        bool found = false;
        
        for (const auto& mode : fullscreenModes) {
            if (mode.width == selectedRes.width && mode.height == selectedRes.height) {
                window_ = std::make_unique<sf::RenderWindow>(
                    mode,
                    WINDOW_TITLE,
                    sf::Style::Fullscreen
                );
                found = true;
                break;
            }
        }
        
        if (!found && !fullscreenModes.empty()) {
            // Use first available fullscreen mode
            window_ = std::make_unique<sf::RenderWindow>(
                fullscreenModes[0],
                WINDOW_TITLE,
                sf::Style::Fullscreen
            );
        }
    } else {
        // Windowed mode
        window_ = std::make_unique<sf::RenderWindow>(
            selectedRes,
            WINDOW_TITLE,
            sf::Style::Close | sf::Style::Titlebar
        );
    }
    
    if (window_) {
        window_->setFramerateLimit(60);
        
        // Update settings menu positions for new window size
        initializeSettingsMenu();
        
        // Update resolution display
        updateResolutionDisplay();
        
        // Update fullscreen toggle text
        fullscreenToggle_.setString(gameSettings_.fullscreen ? "ON" : "OFF");
        
        std::cout << "Settings applied: " 
                  << (gameSettings_.fullscreen ? "Fullscreen" : "Windowed") << " "
                  << selectedRes.width << "x" << selectedRes.height << std::endl;
        
        // --- עדכון גודל הדיאלוג אם הרזולוציה השתנתה (חדש!) ---
        if (m_font) {
            m_dialogueBox.init(m_font, getWindowSize());
        }
    }
}