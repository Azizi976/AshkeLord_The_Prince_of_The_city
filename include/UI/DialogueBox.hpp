#ifndef DIALOGUE_BOX_HPP
#define DIALOGUE_BOX_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

class DialogueBox {
public:
    DialogueBox();
    
    // אתחול הקופסה (גודל ופונט)
    void init(std::shared_ptr<sf::Font> font, const sf::Vector2u& windowSize);
    
    // הצגת טקסט חדש (פותח את הקופסה)
    void showText(const std::string& text, const std::string& speakerName = "");
    
    // עדכון הלוגיקה (אנימציית הקלדה, המתנה לקלט)
    void update(float deltaTime);
    
    // ציור הקופסה למסך
    void render(sf::RenderTarget& target);
    
    // בדיקה אם הקופסה כרגע פעילה (כדי לעצור את השחקן)
    bool isActive() const { return m_active; }
    
    // סגירה ידנית
    void close();

private:
    // עזרים לטיפול בטקסט ארוך
    std::string wrapText(const std::string& text, float maxWidth);

    bool m_active;
    std::shared_ptr<sf::Font> m_font;
    
    // אלמנטים גרפיים
    sf::RectangleShape m_background;
    sf::RectangleShape m_border;
    sf::Text m_content;      // הטקסט עצמו
    sf::Text m_speakerText;  // שם הדובר (למשל: "Miri")
    
    // לוגיקה
    float m_charTimer;       // טיימר לאפקט הקלדה (Typewriter effect)
    size_t m_visibleChars;   // כמה תווים רואים כרגע
    std::string m_fullText;  // הטקסט המלא שצריך להציג
};

#endif // DIALOGUE_BOX_HPP