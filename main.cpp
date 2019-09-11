#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#include "menu.h"

struct point_s{
    sf::Vector2i coord;
    sf::Color color;
    int thickness;
};

//Создать линию между 2 точками
void create_line(sf::RectangleShape& line, const point_s& p1, const point_s& p2){
    int dx = p1.coord.x - p2.coord.x;
    int dy = p1.coord.y - p2.coord.y;

    line.setSize(sf::Vector2f(std::sqrt(dx * dx + dy * dy), p2.thickness));
    line.setOrigin(0, p2.thickness / 2);
    line.setPosition(p1.coord.x, p1.coord.y);
    line.setFillColor(p2.color);

    float angle;
    if (dx < 0)
        angle = std::atan((float)dy / (float)dx) * 180 / M_PI + 180 * (dx > 0);
    else if (dx == 0){
        if (dy < 0)
            angle = 90;
        else
            angle = -90;
    }
    else
        angle = std::atan((float)dy / (float)dx) * 180 / M_PI + 180;

    line.setRotation(angle);
}

//Расчитать таблицу синусов и косинусов для поворотов линий
void recalc_trig(std::vector<float>& sin_table, std::vector<float>& cos_table, const int& n_plane){
    float angle = 0;
    for (int i = 0; i < n_plane; i++){
        sin_table[i] = std::sin(angle * M_PI / 180);
        cos_table[i] = std::cos(angle * M_PI / 180);
        angle += (float)360 / n_plane;
    }
}

//Получить rgb с ползунка 
float calc_slider_rgb(const sf::RectangleShape& r, const sf::RectangleShape& sl){
    return ((255 / (r.getSize().y - 20)) * (r.getPosition().y + r.getSize().y - sl.getPosition().y - 20));
} 

//Получить толщину линий с ползунка
float calc_slider_th(const sf::RectangleShape& r, const sf::RectangleShape& sl){
    return (1 + (19 / (r.getSize().x - 10)) * (sl.getPosition().x - r.getPosition().x));
}

int main(){
    //init
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
    palette_sprite.setPosition(87, 77);

    sf::Text clear_text("R", font, 55);
    clear_text.setFillColor(sf::Color::White);
    clear_text.setOutlineColor(sf::Color::Black);
    clear_text.setOutlineThickness(4);
    clear_text.setPosition(15, 45);

    sf::Vector2i pos_crsr;
    bool left_m_clicked = 0;

    std::vector<std::vector<point_s> >point_list;//Храним вектора векторов, чтобы прорисовывать линии
    point_list.push_back(std::vector<point_s>());

    int n_plane = 2;
    
    sf::RectangleShape line;

    sf::RectangleShape sym_border_line(sf::Vector2f(WINDOW_HEIGHT, 1));
    sym_border_line.setOrigin(0, 0.5);
    sym_border_line.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    sym_border_line.setFillColor(sf::Color::Black);
    sym_border_line.setRotation(-90);

    std::vector<float> sin_table(500), cos_table(500); // таблица предрасчитанных косинусов и синусов
    recalc_trig(sin_table, cos_table, n_plane);

    sf::Color line_color = sf::Color::Black;
    float line_th = 5;

    build_menu(font);//Расставляем кнопки по меню 
    bool show_menu = 0;

    bool th_slider_move = 0;
    bool r_slider_move = 0, g_slider_move = 0, b_slider_move = 0;
    bool show_line_of_sym = 1;

    while (window.isOpen()){
        sf::Event event;

        pos_crsr = sf::Mouse::getPosition(window);

        if (!show_menu && manage_rect.getGlobalBounds().contains((sf::Vector2f)pos_crsr)){
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

        if (show_menu)
            palette_sprite.rotate(0.5);
        else{
            palette_sprite.setScale(0.25, 0.25);
            clear_text.setOutlineColor(sf::Color::Black);
            
            if (palette_sprite.getGlobalBounds().contains((sf::Vector2f)pos_crsr))
                palette_sprite.setScale(0.35, 0.35);
            else if (clear_text.getGlobalBounds().contains((sf::Vector2f)pos_crsr))
                clear_text.setOutlineColor(sf::Color::Blue);
        }

        if (show_menu){ // подсветить ползунок, если навели на него
            if (r_rect.getGlobalBounds().contains((sf::Vector2f)pos_crsr)){
                r_slider.setOutlineColor(sf::Color(128, 170, 255));
                r_slider.setOutlineThickness(4);
            }
            else{
                r_slider.setOutlineColor(sf::Color(160, 160, 160));
                r_slider.setOutlineThickness(2);
            }

            if (g_rect.getGlobalBounds().contains((sf::Vector2f)pos_crsr)){
                g_slider.setOutlineColor(sf::Color(128, 170, 255));
                g_slider.setOutlineThickness(4);
            }
            else{
                g_slider.setOutlineColor(sf::Color(160, 160, 160));
                g_slider.setOutlineThickness(2);
            }

            if (b_rect.getGlobalBounds().contains((sf::Vector2f)pos_crsr)){
                b_slider.setOutlineColor(sf::Color(128, 170, 255));
                b_slider.setOutlineThickness(4);
            }
            else{
                b_slider.setOutlineColor(sf::Color(160, 160, 160));
                b_slider.setOutlineThickness(2);
            }

            if (th_rect.getGlobalBounds().contains((sf::Vector2f)pos_crsr)){
                th_slider.setOutlineColor(sf::Color(128, 170, 255));
                th_slider.setOutlineThickness(4);
            }
            else{
                th_slider.setOutlineColor(sf::Color::Black);
                th_slider.setOutlineThickness(3);
            }
        }

        while(window.pollEvent(event)){
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed){
                if (event.mouseButton.button == sf::Mouse::Left){
                    left_m_clicked = 1;

                    pos_crsr = sf::Mouse::getPosition(window);
                    sf::Vector2f pos_t = (sf::Vector2f)pos_crsr;

                    if (!show_menu && manage_rect.getGlobalBounds().contains(pos_t)){
                        if (10 <= pos_crsr.x && pos_crsr.x <= 45 && n_plane >= 2){ //Если уменьшаем количество полуплоскостей
                            n_plane--;
                            point_list.clear();
                            point_list.push_back(std::vector<point_s>());
                            recalc_trig(sin_table, cos_table, n_plane);
                            if (n_plane == 9){ //Уменьшаем рамку до нужного размера
                                manage_rect.setSize(sf::Vector2f(manage_rect.getSize().x - 20, 
                                    manage_rect.getSize().y));
                                plus_text.move(-20, 0);
                                n_text.move(-5, 0);
                            }
                            if (n_plane == 19)//Увеличиваем рамку 
                                n_text.move(5, 0);
                        }
                        if (pos_crsr.x >= 75){ //Если увеличиваем количество полуплоскостей
                            n_plane++;
                            point_list.clear();
                            point_list.push_back(std::vector<point_s>());
                            recalc_trig(sin_table, cos_table, n_plane);
                            if (n_plane == 10){ //Увеличиваем рамку до нужного размера
                                manage_rect.setSize(sf::Vector2f(manage_rect.getSize().x + 20, 
                                    manage_rect.getSize().y));
                                plus_text.move(20, 0);
                                n_text.move(5, 0);
                            }
                            if (n_plane == 20) //Уменьшаем рамку
                                n_text.move(-5, 0);
                        }
                        n_text.setString(std::to_string(n_plane));
                    }

                    if (!show_menu && clear_text.getGlobalBounds().contains(pos_t)){
                        point_list.clear();
                        point_list.push_back(std::vector<point_s>());
                    }

                    if (show_menu && menu_rect.getGlobalBounds().contains(pos_t)){ 
                        // Если кликаем на ползунок/жмём на чекбокс
                        if (r_rect.getGlobalBounds().contains(pos_t))
                            r_slider_move = 1;
                        else if (g_rect.getGlobalBounds().contains(pos_t))
                            g_slider_move = 1;
                        else if (b_rect.getGlobalBounds().contains(pos_t))
                            b_slider_move = 1;
                        else if (th_rect.getGlobalBounds().contains(pos_t))
                            th_slider_move = 1;
                        else if (cbox_1_rect.getGlobalBounds().contains(pos_t)){
                            show_line_of_sym ^= 1;
                            if (show_line_of_sym)
                                cbox_1_rect.setFillColor(sf::Color::Green);
                            else
                                cbox_1_rect.setFillColor(sf::Color::White);
                        }
                    }
                    else //Если кликнули вне меню
                        show_menu = 0;

                    if (palette_sprite.getGlobalBounds().contains(pos_t))
                        show_menu = 1;
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
                if (event.mouseButton.button == sf::Mouse::Left){
                    left_m_clicked = 0;
                    point_list.push_back(std::vector<point_s>());
                    th_slider_move = r_slider_move = g_slider_move = b_slider_move = 0;
                }

            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::R){
                    point_list.clear();
                    point_list.push_back(std::vector<point_s>());

                    show_menu = 0;
                }
            }
        }

        if (left_m_clicked){
            if (!show_menu){
                point_s point;
                point.coord.x = pos_crsr.x - WINDOW_WIDTH / 2; //Ставим начало системы координат в центр экрана
                point.coord.y = pos_crsr.y - WINDOW_HEIGHT / 2;
                point.color = line_color;
                point.thickness = line_th;
                point_list.back().push_back(point);           // и закидываем в вектор
            }
            else{
                // Не даём ползункам выйти за свои границы
                int y_rgb = std::max(pos_crsr.y, (int)menu_rect.getPosition().y + 20);
                y_rgb = std::min(y_rgb, (int)menu_rect.getPosition().y + MENU_HEIGHT - 40);
                // Обработка движения ползунков
                if (r_slider_move){
                    r_slider.setPosition(r_slider.getPosition().x, y_rgb);
                    line_color.r = calc_slider_rgb(r_rect, r_slider);
                }
                else if (g_slider_move){
                    g_slider.setPosition(g_slider.getPosition().x, y_rgb);
                    line_color.g = calc_slider_rgb(g_rect, g_slider);
                }
                else if (b_slider_move){
                    b_slider.setPosition(b_slider.getPosition().x, y_rgb);
                    line_color.b = calc_slider_rgb(b_rect, b_slider);
                }
                else if (th_slider_move){
                    int x_th = std::max(pos_crsr.x, (int)menu_rect.getPosition().x + 240);
                    x_th = std::min(x_th, (int)menu_rect.getPosition().x + 470);
                    th_slider.setPosition(x_th, th_slider.getPosition().y);
                    line_th = calc_slider_th(th_rect, th_slider);
                }

                // Отрисовываем образованную линию для наглядности
                rgb_circle.setRadius(2 * line_th);
                rgb_circle.setOrigin(2 * line_th, 2 * line_th);
                rgb_circle.setFillColor(line_color);
            }
        }
        
        window.clear();

        window.draw(rect_window);

        for (int j = 0; j < point_list.size(); j++){
            for (int i = 0; i + 1 < point_list[j].size(); i++){
                point_s p1 = point_list[j][i];
                point_s p2 = point_list[j][i + 1];
                int x1 = p1.coord.x, x2 = p2.coord.x; 
                int y1 = p1.coord.y, y2 = p2.coord.y;

                //Поворачиваем каждую точку n_plane раз под нужным углом и возвращаем
                //начало системы координат в точку (0; 0)
                for (int k = 0; k < n_plane; k++){
                    float s = sin_table[k];
                    float c = cos_table[k];
                    p1.coord.x = x1 * c - y1 * s + WINDOW_WIDTH / 2;
                    p1.coord.y = x1 * s + y1 * c + WINDOW_HEIGHT / 2;
                    p2.coord.x = x2 * c - y2 * s + WINDOW_WIDTH / 2;
                    p2.coord.y = x2 * s + y2 * c + WINDOW_HEIGHT / 2;
                    create_line(line, p1, p2);
                    
                    window.draw(line);
                }
            }
        }

        if (show_line_of_sym && n_plane >= 2) // Отрисовка линий симметрии
            for (int i = 0; i < n_plane; i++){
                sym_border_line.rotate((float)360 / n_plane);
                window.draw(sym_border_line);
            }
        
        //Отрисовка
        window.draw(manage_rect);

        window.draw(minus_text);
        window.draw(plus_text);
        window.draw(n_text);

        window.draw(palette_sprite);
        window.draw(clear_text);

        if (show_menu)
            draw_menu_items(window);
        
        window.display();
    }
    return 0;
}