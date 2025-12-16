#include "UI/DialogueBox.hpp"
#include <sstream>

DialogueBox::DialogueBox() : m_active(false), m_charTimer(0.0f), m_visibleChars(0) {
    // הגדרות עיצוב בסיסיות לקופסה
    m_background.setFillColor(sf::Color::White);
    m_background.setOutlineThickness(4.0f);
    m_background.setOutlineColor(sf::Color::Black);
}

void DialogueBox::init(std::shared_ptr<sf::Font> font, const sf::Vector2u& windowSize) {
    m_font = font;
    
    // גודל הקופסה: רוחב המסך פחות שוליים, גובה 25% מהמסך
    float width = windowSize.x * 0.9f;
    float height = windowSize.y * 0.25f;
    float x = (windowSize.x - width) / 2.0f;
    float y = windowSize.y - height - 30.0f; // קצת מעל התחתית

    m_background.setSize(sf::Vector2f(width, height));
    m_background.setPosition(x, y);

    // הגדרת הטקסט
    m_content.setFont(*m_font);
    m_content.setCharacterSize(30);
    m_content.setFillColor(sf::Color::Black);
    m_content.setPosition(x + 20.0f, y + 50.0f); // פדינג פנימי

    // הגדרת שם הדובר (בקטן מעל הטקסט)
    m_speakerText.setFont(*m_font);
    m_speakerText.setCharacterSize(24);
    m_speakerText.setFillColor(sf::Color(100, 100, 100)); // אפור כהה
    m_speakerText.setStyle(sf::Text::Bold);
    m_speakerText.setPosition(x + 20.0f, y + 15.0f);
}

void DialogueBox::showText(const std::string& text, const std::string& speakerName) {
    m_active = true;
    m_speakerText.setString(speakerName);
    
    // חישוב רוחב מקסימלי לטקסט (רוחב קופסה פחות שוליים)
    float maxWidth = m_background.getSize().x - 40.0f;
    m_fullText = wrapText(text, maxWidth);
    
    m_visibleChars = 0;
    m_content.setString("");
}

void DialogueBox::update(float deltaTime) {
    if (!m_active) return;

    // אפקט מכונת כתיבה (Typewriter Effect)
    // מציג אות נוספת כל כמה מילי-שניות
    if (m_visibleChars < m_fullText.size()) {
        m_charTimer += deltaTime;
        if (m_charTimer > 0.03f) { // מהירות ההקלדה
            m_charTimer = 0.0f;
            m_visibleChars++;
            m_content.setString(m_fullText.substr(0, m_visibleChars));
        }
    }

    // בדיקת לחיצה לסגירה או האצה
    // הערה: עדיף לנהל אינפוט ב-Game.cpp, אבל לצורך פשטות נבדוק כאן
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        if (m_visibleChars < m_fullText.size()) {
            // אם הטקסט עדיין נכתב, הלחיצה משלימה אותו מיד
            m_visibleChars = m_fullText.size();
            m_content.setString(m_fullText);
        } else {
            // אם הטקסט סיים להיכתב, הלחיצה סוגרת את הקופסה
            // (במשחק אמיתי זה יעבור לשורה הבאה בדיאלוג)
            // כאן נוסיף השהיה קטנה כדי לא לסגור מיד באותו פריים שנפתח
            if (m_charTimer > 0.2f) { 
               close();
            }
        }
    }
}

void DialogueBox::render(sf::RenderTarget& target) {
    if (!m_active) return;
    
    target.draw(m_background);
    target.draw(m_speakerText);
    target.draw(m_content);
}

void DialogueBox::close() {
    m_active = false;
}

// פונקציית עזר לשבירת שורות
std::string DialogueBox::wrapText(const std::string& text, float maxWidth) {
    std::stringstream ss(text);
    std::string word;
    std::string currentLine;
    std::string result;

    while (ss >> word) {
        std::string testLine = currentLine + (currentLine.empty() ? "" : " ") + word;
        sf::Text testText = m_content;
        testText.setString(testLine);
        
        if (testText.getLocalBounds().width > maxWidth) {
            result += currentLine + "\n";
            currentLine = word;
        } else {
            currentLine = testLine;
        }
    }
    result += currentLine;
    return result;
}