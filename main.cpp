#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

const double Gravitation_const = 0.009;
const int way_const = 50001;
class corpuscle{
public:
    double size; 
    double mass;
    double charge;
    double x;
    double y;
    sf::Vector2f speed = sf::Vector2f(0, 0);

    void init(double s, double m, double c, double X, double Y, sf::Vector2f sp){
        size = s;
        mass = m;
        charge = c;
        x = X;
        y = Y;
        speed = sp;
    }
    
};

sf::Vector2f gravitation_power(corpuscle A, corpuscle B){
    double x = A.x - B.x;
    double y = A.y - B.y;

    double z_2 = (pow(x, 2) + pow(y, 2));   
    double F_x = x / pow(z_2, 0.5) * Gravitation_const * A.mass * B.mass / z_2; 
    double F_y = y / pow(z_2, 0.5) * Gravitation_const * A.mass * B.mass / z_2; 

    return sf::Vector2f(F_x, F_y);
}

sf::Vector2f multiply_vector_by_scalar(sf::Vector2f vector, double scalar){
    return sf::Vector2f(vector.x * scalar, vector.y * scalar);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");

    corpuscle e;
    e.init(5, 5, -1, 200, 400, sf::Vector2f(0, -0.02));
    
    sf::CircleShape e_circle(e.size);
     
    e_circle.setFillColor(sf::Color::Blue);
    e_circle.setPosition(sf::Vector2f(e.x, e.y));

    corpuscle p;
    p.init(20, 20, +1, 400, 400, sf::Vector2f(0, 0.005));
    
    sf::CircleShape p_circle(p.size);
    p_circle.setFillColor(sf::Color::Red);
    p_circle.setPosition(sf::Vector2f(p.x, p.y));

    sf::Vector2f e_way[way_const];
    for(int i = 0; i<way_const; i++){
        e_way[i] = sf::Vector2f(e.x + e.size/sqrt(2), e.y + e.size/sqrt(2));
    }

    sf::Vector2f p_way[way_const];
    for(int i = 0; i<way_const; i++){
        p_way[i] = sf::Vector2f(p.x + p.size/sqrt(2), p.y + p.size/sqrt(2));
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(e_circle.getGlobalBounds().intersects(p_circle.getGlobalBounds())){
            
            window.clear();
            window.draw(e_circle);
            window.draw(p_circle);
            window.display();

            continue;
        }

        sf::Vector2f F1 = gravitation_power(e, p);
        sf::Vector2f F2 = gravitation_power(p, e);

        e.speed = e.speed + multiply_vector_by_scalar(F2, 1/e.mass);
        e.x = e.x + e.speed.x;
        e.y = e.y + e.speed.y;  

        p.speed = p.speed + multiply_vector_by_scalar(F1, 1/p.mass);
        p.x = p.x + p.speed.x;
        p.y = p.y + p.speed.y; 

        e_circle.setPosition(sf::Vector2f(e.x, e.y));
        p_circle.setPosition(sf::Vector2f(p.x, p.y));
        
        std::cout << e.x << e.y << '\n';
        std::cout << p.x << p.y << '\n';

        window.clear();

        e_way[way_const - 1] = sf::Vector2f(e.x + e.size/sqrt(2), e.y + e.size/sqrt(2));
        p_way[way_const - 1] = sf::Vector2f(p.x + p.size/sqrt(2), p.y + p.size/sqrt(2));

        for(int i = 0; i<way_const - 1; i++){
           
            e_way[i] = e_way[i+1];
            p_way[i] = p_way[i+1];
            
            if(i % 1000 == 0){
                sf::CircleShape e_point = sf::CircleShape(1);
                e_point.setFillColor(sf::Color::White);
                e_point.setPosition(e_way[i]);

                window.draw(e_point);

                sf::CircleShape p_point = sf::CircleShape(1);
                p_point.setFillColor(sf::Color::Yellow);
                p_point.setPosition(p_way[i]);

                window.draw(p_point);
            }
            
        }

        window.draw(e_circle);
        window.draw(p_circle);

        window.display();
    }

    return 0;
}


