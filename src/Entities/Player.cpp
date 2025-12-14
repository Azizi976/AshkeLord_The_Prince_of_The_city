#include "Entities/Player.hpp"
#include "ResourceManager.hpp"
#include <iostream>
#include <cmath>

Player::Player() : streetCred_(0), nervimLevel_(100), maxNervim_(100), damage_(10) {
    // Set initial speed
    speed_ = 150.0f; // pixels per second
    
    // Try to load the player texture
    auto& resourceManager = ResourceManager::getInstance();
    auto texture = resourceManager.getTexture("ars_sprite");
    
    if (texture) {
        sprite_.setTexture(*texture);
        // Center the sprite origin for better rotation/positioning
        auto bounds = sprite_.getLocalBounds();
        sprite_.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    } else {
        std::cerr << "Warning: Player texture 'ars_sprite' not found. "
                  << "Please load it in ResourceManager before creating Player." << std::endl;
        // Create a simple colored rectangle as fallback
        static sf::Texture fallbackTexture;
        if (fallbackTexture.getSize().x == 0) {
            fallbackTexture.create(32, 32);
            sf::Image img;
            img.create(32, 32, sf::Color::Blue);
            fallbackTexture.update(img);
        }
        sprite_.setTexture(fallbackTexture);
        auto bounds = sprite_.getLocalBounds();
        sprite_.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    }
    
    // Set initial position (center of screen, will be adjusted by Game)
    setPosition(400.0f, 300.0f);
    velocity_ = sf::Vector2f(0.0f, 0.0f);
}

void Player::update(float deltaTime) {
    // Handle input
    handleInput();
    
    // Update position based on velocity
    position_ += velocity_ * deltaTime;
    sprite_.setPosition(position_);
    
    // Reset velocity (will be set again by handleInput next frame)
    velocity_ = sf::Vector2f(0.0f, 0.0f);
}

void Player::handleInput() {
    // Reset velocity
    velocity_ = sf::Vector2f(0.0f, 0.0f);
    
    // Check for movement input (WASD or Arrow keys)
    bool movingUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool movingDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S) || 
                      sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    bool movingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || 
                      sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D) || 
                       sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    
    // Calculate velocity based on input
    if (movingUp) {
        velocity_.y -= speed_;
    }
    if (movingDown) {
        velocity_.y += speed_;
    }
    if (movingLeft) {
        velocity_.x -= speed_;
    }
    if (movingRight) {
        velocity_.x += speed_;
    }
    
    // Normalize diagonal movement to maintain consistent speed
    if (velocity_.x != 0.0f && velocity_.y != 0.0f) {
        float length = std::sqrt(velocity_.x * velocity_.x + velocity_.y * velocity_.y);
        velocity_.x = (velocity_.x / length) * speed_;
        velocity_.y = (velocity_.y / length) * speed_;
    }
}

void Player::heal(int amount) {
    nervimLevel_ += amount;
    if (nervimLevel_ > maxNervim_) {
        nervimLevel_ = maxNervim_;
    }
}

void Player::increaseDamage(int amount) {
    damage_ += amount;
}

