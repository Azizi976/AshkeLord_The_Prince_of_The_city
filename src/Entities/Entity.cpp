#include "Entities/Entity.hpp"

Entity::Entity() : position_(0.0f, 0.0f), speed_(100.0f) {
    // Default constructor
}

void Entity::render(sf::RenderTarget& target) const {
    target.draw(sprite_);
}

sf::Vector2f Entity::getPosition() const {
    return position_;
}

void Entity::setPosition(const sf::Vector2f& position) {
    position_ = position;
    sprite_.setPosition(position_);
}

void Entity::setPosition(float x, float y) {
    setPosition(sf::Vector2f(x, y));
}

sf::Sprite& Entity::getSprite() {
    return sprite_;
}

const sf::Sprite& Entity::getSprite() const {
    return sprite_;
}

