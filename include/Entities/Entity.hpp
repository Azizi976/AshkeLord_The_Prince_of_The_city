#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <memory>

/**
 * Entity - Base class for all game entities
 * Provides common functionality for position, sprite, and rendering
 */
class Entity {
public:
    Entity();
    virtual ~Entity() = default;

    /**
     * Update the entity (called every frame)
     * @param deltaTime Time elapsed since last frame
     */
    virtual void update(float deltaTime) = 0;

    /**
     * Render the entity to the target
     * @param target Render target (usually the window)
     */
    virtual void render(sf::RenderTarget& target) const;

    // Position getters and setters
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& position);
    void setPosition(float x, float y);

    // Sprite access
    sf::Sprite& getSprite();
    const sf::Sprite& getSprite() const;

protected:
    sf::Sprite sprite_;
    sf::Vector2f position_;
    float speed_; // Movement speed in pixels per second
};

#endif // ENTITY_HPP

