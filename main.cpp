#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <vector>


using namespace sf;

const float Gravitation_const = 0.1;
const int window_size = 900;
const int objects_quantity = 100;
const int splitting = 100;


class corpuscle{
private:
    Vector2f position; 

public:
    float size; 
    float mass;
    float charge;
    
    Vector2f speed = Vector2f(0, 0);
    CircleShape shape;

    void init(float Size, float Mass, float Charge, Vector2f Position, Vector2f Speed, Color color){
        size = Size;
        mass = Mass;
        charge = Charge;

        position = Position;
        speed = Speed;

        shape = CircleShape(size);
        shape.setFillColor(color);
        shape.setPosition(Position - Vector2f(size/sqrt(2), size/sqrt(2)));
    }
    void setPosition(Vector2f Position){
        position = Position;
        shape.setPosition(Position - Vector2f(size/sqrt(2), size/sqrt(2)));
    }
    
    Vector2f getPosition(){
        return position;
    }

};



int box(Vector2f vector){
    return int(vector.x / (window_size / pow(splitting, 0.5))) + 3 * int(vector.y / (window_size / pow(splitting, 0.5))); 
}

float RandomFloat(float min, float max){
    int precision = 4;
    
    float value;

    value = rand() % (int)pow(10, precision);
    value = min + (value / pow(10, precision)) * (max - min);

    return value;
}

float entropy(int box[splitting], double fact[objects_quantity + 1]){
    int nums = objects_quantity;
    float W = 0;

    for(int i = 0; i<splitting; i++){
        W = W + log(fact[nums] / fact[box[i]] / fact[nums - box[i]]);
        nums = nums - box[i];
    }

    return W;
}


Vector2f operator* (float scalar, Vector2f vector){
    return Vector2f(vector.x * scalar, vector.y * scalar);
}
Vector2f operator* (Vector2f vector, float scalar){
    return Vector2f(vector.x * scalar, vector.y * scalar);
}

Vector2f gravitation_power(corpuscle A, corpuscle B){
    float x = B.getPosition().x - A.getPosition().x;
    float y = B.getPosition().y - A.getPosition().y;

    float z = pow((pow(x, 2) + pow(y, 2)), 0.5);   
    
    float F_x = x * Gravitation_const * A.mass * B.mass / pow(z, 3); 
    float F_y = y * Gravitation_const * A.mass * B.mass / pow(z, 3); 

    return Vector2f(F_x, F_y);
}

int main()
{
    srand(time(0));
    int boxes[splitting];

    double factorial[objects_quantity + 1]; 
    factorial[0] = 1;
    for(int i=1;i<objects_quantity + 1; i++){
        factorial[i] = factorial[i-1] * i;
    }

    RenderWindow model_window(VideoMode(window_size, window_size), "Simple model");
    RenderWindow data_window(VideoMode(window_size, window_size), "Grahp model");

    corpuscle objects[objects_quantity];

    for(int i=0; i<objects_quantity; i++){
        
        if(i%10 == 0){
            corpuscle p; 
            p.init(10, 20, +1, Vector2f(RandomFloat(390, window_size - 390), RandomFloat(390, window_size - 390)), Vector2f(RandomFloat(-0.1, 0.1), RandomFloat(-0.1, 0.1)), Color::Red);
            objects[i] = p; 
        }
        else{
            corpuscle e; 
            e.init(5, 5, -1, Vector2f(RandomFloat(390, window_size - 390), RandomFloat(390, window_size - 390)), Vector2f(RandomFloat(-0.1, 0.1), RandomFloat(-0.1, 0.1)), Color::Blue);
            objects[i] = e; 
        }
        
    } 

    long long int simulation_time = 0;
    std::vector <CircleShape> data_massive = {};
    int data_len = 0;

    while (model_window.isOpen())
    {
        Event event;
        while (model_window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                model_window.close();
        }

        model_window.clear();

        simulation_time++;
        
        for(int i=0;i<splitting;i++){
            boxes[i] = 0;
        }

        Vector2f F;
        for(int i=0; i<objects_quantity; i++){
            F = Vector2f(0, 0);

            for(int j=0; j<objects_quantity; j++){
                if(!(i==j or objects[i].shape.getGlobalBounds().intersects(objects[j].shape.getGlobalBounds()))){
                    F = F + gravitation_power(objects[i], objects[j]);    
                }
                    
            }
            
            objects[i].speed = objects[i].speed + (F * (1/objects[i].mass));
            Vector2f new_Position = objects[i].getPosition() + objects[i].speed;

            if (new_Position.x >= window_size)
            {
                new_Position.x = window_size;
                objects[i].speed.x = objects[i].speed.x * -1;
            }
            if (new_Position.x <= 0)
            {
                new_Position.x = 0;
                objects[i].speed.x = objects[i].speed.x * -1;
            }
            if (new_Position.y >= window_size)
            {
                new_Position.y = window_size;
                objects[i].speed.y = objects[i].speed.y * -1;
            }
            if (new_Position.y <= 0)
            {
                new_Position.y = 0;
                objects[i].speed.y = objects[i].speed.y * -1;
            }
            
            boxes[box(new_Position)] ++;

            objects[i].setPosition(new_Position);

            model_window.draw(objects[i].shape);
        }
        
        float entropy_value = entropy(boxes, factorial);
        std::cout << entropy_value << '\n';
        model_window.display();

        if(data_window.isOpen()){
            Event event;
            while (data_window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    data_window.close();
            }

            data_window.clear();

            RectangleShape abscissa(Vector2f(2, window_size - 100));
            abscissa.rotate(-90);
            abscissa.setPosition(Vector2f(50, window_size - 47));

            RectangleShape ordinate(Vector2f(2, window_size - 100));
            ordinate.setPosition(Vector2f(50, 50));

            data_window.draw(abscissa);
            data_window.draw(ordinate);

            CircleShape point(1.5);
            point.setFillColor(Color::White);
            point.setPosition(Vector2f(simulation_time/50 + 50, window_size - (entropy_value / 300 * 800 + 50)));
            data_window.draw(point);
            data_massive.push_back(point);
            data_len++;

            for(int i=0; i<data_len; i++){
                data_window.draw(data_massive[i]);
            }
            
            data_window.display();
        }
        
    }

    return 0;
}


