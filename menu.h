#include <SFML/Graphics.hpp>

#define MENU_WIDTH 500
#define MENU_HEIGHT 350

sf::RectangleShape blind_rect(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
sf::RectangleShape menu_rect(sf::Vector2f(MENU_WIDTH, MENU_HEIGHT));

sf::RectangleShape r_rect, g_rect, b_rect, th_rect;
sf::RectangleShape r_slider, g_slider, b_slider, th_slider;

sf::RectangleShape cbox_1_rect;
sf::Text cbox_1_text;
sf::CircleShape rgb_circle;
sf::Color rgb;

void build_menu(const sf::Font& font, const sf::Color& c = sf::Color::Black){
    blind_rect.setPosition(0, 0);
    blind_rect.setFillColor(sf::Color(160, 160, 160, 150));

    int x = (WINDOW_WIDTH - MENU_WIDTH) / 2;
    int y = (WINDOW_HEIGHT - MENU_HEIGHT) / 2;

    menu_rect.setPosition(x, y);
    menu_rect.setOutlineColor(sf::Color::Black);
    menu_rect.setOutlineThickness(5);
    menu_rect.setFillColor(sf::Color::White);

    r_rect.setSize(sf::Vector2f(50, MENU_HEIGHT - 40));
    r_rect.setFillColor(sf::Color(255, 0, 0));
    r_rect.setPosition(x + 20, y + 20);

    g_rect.setSize(sf::Vector2f(50, MENU_HEIGHT - 40));
    g_rect.setFillColor(sf::Color(0, 255, 0));
    g_rect.setPosition(x + 90, y + 20);

    b_rect.setSize(sf::Vector2f(50, MENU_HEIGHT - 40));
    b_rect.setFillColor(sf::Color(0, 0, 255));
    b_rect.setPosition(x + 160, y + 20);

    r_slider.setSize(sf::Vector2f(60, 20));
    r_slider.setFillColor(sf::Color::Black);
    r_slider.setOutlineColor(sf::Color(160, 160, 160));
    r_slider.setOutlineThickness(2);
    r_slider.setPosition(x + 15, - c.r * (r_rect.getSize().y - 20) / (float)255 + 
        (r_rect.getPosition().y + r_rect.getSize().y  - 20));

    g_slider = b_slider = r_slider;
    
    g_slider.setPosition(x + 85, - c.g * (g_rect.getSize().y - 20) / (float)255 + 
        (g_rect.getPosition().y + g_rect.getSize().y  - 20));

    b_slider.setPosition(x + 155, - c.b * (b_rect.getSize().y - 20) / (float)255 + 
        (b_rect.getPosition().y + b_rect.getSize().y  - 20));

    th_rect.setSize(sf::Vector2f(240, 20));
    th_rect.setFillColor(sf::Color::White);
    th_rect.setOutlineColor(sf::Color::Black);
    th_rect.setOutlineThickness(3);
    th_rect.setPosition(x + 240, y + 30);

    th_slider.setSize(sf::Vector2f(10, 40));
    th_slider.setFillColor(sf::Color::White);
    th_slider.setOutlineColor(sf::Color::Black);
    th_slider.setOutlineThickness(3);
    th_slider.setPosition(x + 270, y + 20);

    rgb_circle.setRadius(5);
    rgb_circle.setOrigin(5, 5);
    rgb_circle.setFillColor(c);
    rgb_circle.setOutlineColor(sf::Color(160, 160, 160));
    rgb_circle.setOutlineThickness(3);
    rgb_circle.setPosition(x + 360, y + 160);

    cbox_1_rect.setSize(sf::Vector2f(30, 30));
    cbox_1_rect.setFillColor(sf::Color::Green);
    cbox_1_rect.setOutlineColor(sf::Color::Black);
    cbox_1_rect.setOutlineThickness(5);
    cbox_1_rect.setPosition(x + 240, y + 295);

    cbox_1_text.setFont(font);
    cbox_1_text.setFillColor(sf::Color::Black);
    cbox_1_text.setCharacterSize(23);
    cbox_1_text.setString("Enable line \nof symmetry");
    cbox_1_text.setPosition(x + 285, y + 285);
}

void draw_menu_items(sf::RenderWindow& window){
    window.draw(blind_rect);
    window.draw(menu_rect);


    window.draw(r_rect);
    window.draw(r_slider);

    window.draw(g_rect);
    window.draw(g_slider);
    
    window.draw(b_rect);
    window.draw(b_slider);


    window.draw(th_rect);
    window.draw(th_slider);

    window.draw(rgb_circle);

    window.draw(cbox_1_rect);
    window.draw(cbox_1_text);
}