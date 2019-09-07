#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

void set_crs_in_window(sf::Vector2i& a){
    if (a.x <= 10 || a.x >= WINDOW_WIDTH - 10 ||
            a.y <= 10 || a.y >= WINDOW_HEIGHT - 10){
        a.x = std::max(10, a.x);
        a.x = std::min(a.x, WINDOW_WIDTH - 10);
        a.y = std::max(10, a.y);
        a.y = std::min(a.y, WINDOW_HEIGHT - 10);
    }
}

void create_line(sf::RectangleShape& line, const sf::Vector2i& p1, const sf::Vector2i& p2, const int& thickness){
    line.setRotation(0);

    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;

    line.setSize(sf::Vector2f(std::sqrt(dx * dx + dy * dy), thickness));
    line.setOrigin(0, thickness / 2);
    line.setPosition(p1.x, p1.y);
    line.setFillColor(sf::Color::Black);

    float angle;
    if (dx < 0)
        angle = std::atan((float)dy / (float)dx) * 180 / M_PI;
    if (dx == 0){
        if (dy < 0)
            angle = 90;
        else
            angle = -90;
    }
    if (dx > 0)
        angle = std::atan((float)dy / (float)dx) * 180 / M_PI + 180;

    line.rotate(angle);
}

void recalc_trig(std::vector<float>& sin_table, std::vector<float>& cos_table, const int& n_plane){
    sin_table.clear();
    cos_table.clear();

    float angle = 0;
    for (int i = 0; i < n_plane; i++){
        sin_table.push_back(std::sin(angle * M_PI / 180));
        cos_table.push_back(std::cos(angle * M_PI / 180));
        angle += (float)360 / n_plane;
    }
}

int main(){
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Symmetry");
    window.setFramerateLimit(60);
    srand(time(NULL));

    sf::RectangleShape rect_window;
    rect_window.setOutlineColor(sf::Color::Black);
    rect_window.setFillColor(sf::Color::White);
    rect_window.setOutlineThickness(10);
    rect_window.setSize(sf::Vector2f(WINDOW_WIDTH - 20, WINDOW_HEIGHT - 20));
    rect_window.setPosition(10, 10);


    sf::RectangleShape manage_rect;
    manage_rect.setSize(sf::Vector2f(105, 34));
    manage_rect.setOutlineColor(sf::Color::Black);
    manage_rect.setOutlineThickness(5);
    manage_rect.setFillColor(sf::Color::White);
    manage_rect.setPosition(10, 10);


    sf::Font font;
    font.loadFromFile("16852.otf");

    sf::Text minus_text("-", font, 40);
    minus_text.setFillColor(sf::Color::Black);
    minus_text.setOutlineColor(sf::Color::Blue);
    minus_text.setStyle(sf::Text::Bold);
    minus_text.setPosition(15, 4);

    sf::Text n_text("", font, 40);
    n_text.setString("2");
    n_text.setFillColor(sf::Color::Black);
    n_text.setStyle(sf::Text::Bold);
    n_text.setPosition(50, 4);

    sf::Text plus_text("+", font, 40);
    plus_text.setFillColor(sf::Color::Black);
    plus_text.setOutlineColor(sf::Color::Blue);
    plus_text.setStyle(sf::Text::Bold);
    plus_text.setPosition(85, 4);


    sf::Image palette_img;
    palette_img.loadFromFile("palette.png");
    palette_img.createMaskFromColor(sf::Color(255, 0, 0));
    sf::Texture palette_texture;
    palette_texture.loadFromImage(palette_img);
    sf::Sprite palette_sprite;
    palette_sprite.setTexture(palette_texture);
    palette_sprite.setOrigin(palette_img.getSize().x / 2, palette_img.getSize().y / 2);
    palette_sprite.setScale(0.2, 0.2);
    palette_sprite.setPosition(145, 30);


    sf::Vector2i pos_crsr;
    bool left_m_clicked = 0;

    std::vector<std::vector<sf::Vector2i> >point_list;
    point_list.push_back(std::vector<sf::Vector2i>());

    int n_plane = 2;
    
    sf::RectangleShape line;

    sf::RectangleShape sym_border_line(sf::Vector2f(WINDOW_HEIGHT, 1));
    sym_border_line.setOrigin(0, 0.5);
    sym_border_line.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    sym_border_line.setFillColor(sf::Color::Black);
    sym_border_line.setRotation(-90);

    std::vector<float> sin_table, cos_table;
    recalc_trig(sin_table, cos_table, n_plane);

    bool menu = 0;
    while (window.isOpen()){
        sf::Event event;

        pos_crsr = sf::Mouse::getPosition(window);

        if (manage_rect.getGlobalBounds().contains((sf::Vector2f)pos_crsr)){
            if (10 <= pos_crsr.x && pos_crsr.x <= 45)
                minus_text.setOutlineThickness(5);
            else
                minus_text.setOutlineThickness(0);

            if (pos_crsr.x >= 75)
                plus_text.setOutlineThickness(5);
            else
                plus_text.setOutlineThickness(0);
        }
        else{
            minus_text.setOutlineThickness(0);
            plus_text.setOutlineThickness(0);
        }

        if (palette_sprite.getGlobalBounds().contains((sf::Vector2f)pos_crsr))
            palette_sprite.setScale(0.3, 0.3);
        else
            palette_sprite.setScale(0.2, 0.2);

        // std::cout << pos_crsr.x << " " << pos_crsr.y << '\n';
            
        while(window.pollEvent(event)){
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed){
                if (event.mouseButton.button == sf::Mouse::Left){
                    left_m_clicked = 1;

                    pos_crsr = sf::Mouse::getPosition(window);

                    if (manage_rect.getGlobalBounds().contains((sf::Vector2f)pos_crsr)){
                        if (10 <= pos_crsr.x && pos_crsr.x <= 45 && n_plane >= 2){
                            n_plane--;
                            point_list.clear();
                            point_list.push_back(std::vector<sf::Vector2i>());
                            recalc_trig(sin_table, cos_table, n_plane);
                        }
                        if (pos_crsr.x >= 75){
                            n_plane++;
                            point_list.clear();
                            point_list.push_back(std::vector<sf::Vector2i>());
                            recalc_trig(sin_table, cos_table, n_plane);
                        }
                        n_text.setString(std::to_string(n_plane));
                    }

                    if (palette_sprite.getGlobalBounds().contains((sf::Vector2f)pos_crsr)){
                        menu = 1;
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased){
                if (event.mouseButton.button == sf::Mouse::Left){
                    left_m_clicked = 0;
                    point_list.push_back(std::vector<sf::Vector2i>());
                }
            }
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::R){
                    point_list.clear();
                    point_list.push_back(std::vector<sf::Vector2i>());
                }
            }
        }

        if (left_m_clicked){
            point_list.back().push_back({pos_crsr.x - WINDOW_WIDTH / 2, pos_crsr.y - WINDOW_HEIGHT / 2});
        }
        
        window.clear();

        window.draw(rect_window);

        for (int j = 0; j < point_list.size(); j++){
            for (int i = 0; i + 1 < point_list[j].size(); i++){
                sf::Vector2i p1 = point_list[j][i];
                sf::Vector2i p2 = point_list[j][i + 1];
                int x1 = p1.x, x2 = p2.x; 
                int y1 = p1.y, y2 = p2.y;

                float angle = 0;
                int i_s = 0, i_c = 0;
                for (int k = 0; k < n_plane; k++){
                    float s = sin_table[i_s++];
                    float c = cos_table[i_c++];
                    p1.x = x1 * c - y1 * s + WINDOW_WIDTH / 2;
                    p1.y = x1 * s + y1 * c + WINDOW_HEIGHT / 2;
                    p2.x = x2 * c - y2 * s + WINDOW_WIDTH / 2;
                    p2.y = x2 * s + y2 * c + WINDOW_HEIGHT / 2;
                    create_line(line, p1, p2, 5);
                    
                    window.draw(line);
                }
            }
        }

        if (n_plane >= 2){
            for (int i = 0; i < n_plane; i++){
                sym_border_line.rotate((float)360 / n_plane);
                window.draw(sym_border_line);
            }
        }
        
        window.draw(manage_rect);

        window.draw(minus_text);
        window.draw(plus_text);
        window.draw(n_text);

        window.draw(palette_sprite);
        window.display();
    }
    return 0;
}