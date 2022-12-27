#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "map.h"
#include "mapresourses.h"
#include "mapwalls.h"
#include "poisonsmap.h"

using namespace sf;



class Player { // класс Игрока
public:
    enum {left, right, up, down, stay} state;
    int playerScore = 0;
    float x, y, w, h, dx, dy, speed; //координаты игрока х и у, высота и ширина,
    //ускорение (по х и по у), сама скорость
    int health; //направление (direction) движения игрока
    bool life;
    std::string File; //файл с расширением
    Image image;//сфмл изображение
    Texture texture;//сфмл текстура
    Sprite sprite;//сфмл спрайт
    float CurrentFrame = 0;//хранит текущий кадр
    //Конструктор с параметрами для класса Player. При создании объекта класса мы будем задавать
    //имя файла, координату Х и У, ширину и высоту
    Player(std::string F, float X, float Y, float W, float H){
        life = true;
        state = stay;
        File = F; //имя файла+расширение
        w = W; h = H; //высота и ширина
        image.loadFromFile("images/" + File);//загружаем в image изображение, вместо File
        //передадим то, что пропишем при создании объекта. В нашем случае это "hero.png". Получится
        //запись, идентичная image.loadFromFile("images/hero/png");
        image.createMaskFromColor(Color(41, 33, 59)); //убираем ненужный темно-синий цвет
        texture.loadFromImage(image); //заносим наше изображение в текстуру
        sprite.setTexture(texture); //заливаем спрайт текстурой
        x = X; y = Y; //координата появления спрайта
         speed= 0;
         health = 100;
        sprite.setTextureRect(IntRect(0, 0, w, h)); //Задаем спрайту один прямоугольник для
    //вывода одного льва. IntRect – для приведения типов
    }
    void control(){
        if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) {
            state = left;
            speed = 0.1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) {
            state = right;
            speed = 0.1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) {
            state = up;
            speed = 0.1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
            state = down;
            speed = 0.1;
        }
    }

    void update(float time) //функция "оживления/обновления" объекта класса. Принимает в себя
    //время SFML, вследствие чего работает бесконечно, давая персонажу движение.
    {
    if (life) {//проверяем, жив ли герой
        control();//функция управления персонажем
        switch (state)//тут делаются различные действия в зависимости от состояния
        {
            case right:{//состояние идти вправо
                dx = speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 160, 32, 32));
                sprite.setScale({1,1});
                break;
            }
            case left:{//состояние идти влево
                dx = -speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 160, 32, 32));
                sprite.setScale({-1,1});
                break;
            }
            case up:{//идти вверх
                dy = -speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 32, 32));
                break;
            }
            case down:{//идти вниз
                dy = speed;
                CurrentFrame += 0.005*time;
                if (CurrentFrame > 3) CurrentFrame -= 3;
                sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 132, 32, 32));
                break;
            }
            case stay:{//стоим
                dy = speed;
                dx = speed;
                break;
            }
        }
        x += dx*time; //движение по “X”
        checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
        y += dy*time; //движение по “Y”
        checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
        speed = 0; //обнуляем скорость, чтобы персонаж остановился.
        //state = stay; //состояние - стоит
        sprite.setPosition(x, y); //переставляем спрайт в новую позицию (x, y).
        if (health <= 0)
        {
            life = false;
        }//если жизней меньше 0, либо равно 0, то умираем
    }
    }


    void checkCollisionWithMap(float Dx, float Dy) {
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
            for (int j = x / 32; j<(x + w) / 32; j++)
            {
                if (TileMap[i][j] == '0')//если элемент тайлик земли
                {
                    if (Dy > 0) { y = i * 32 - h; dy = 0; }//по Y
                    if (Dy < 0) { y = i * 32 + 32; dy = 0; }//столкновение с верхними краями карты
                    if (Dx > 0) { x = j * 32 - w; dx = 0; }//с правым краем карты
                    if (Dx < 0) { x = j * 32 + 32; dx = 0; }// с левым краем карты
                }
                if (TileMap_resoursesForEat[i][j] == 'p') {
                    playerScore++; //если взяли камень, переменная playerScore=playerScore+1;
                    TileMap_resoursesForEat[i][j] = ' ';
                }
                if (TileMap_poison[i][j] == 'd') {
                    health -= 40;//если взяли ядовитый цветок,то переменная health=health-40;
                    TileMap_poison[i][j] = ' ';//убрали цветок
                }
                if (TileMap[i][j] == 'h') {
                    health += 20;//если взяли сердечко,то переменная health=health+20;
                    TileMap[i][j] = ' ';//убрали сердечко
                }
            }
    }

    FloatRect getRect(){//метод получения прямоугольника. его коорд, размеры (шир,высот).
        FloatRect FR(x, y, w, h); // переменная FR типа FloatRect
        return FR;
    }
};
void DrawingFloore(Sprite s_map, RenderWindow* window){
    for (int i = 0; i < HEIGHT_MAP; i++)
        for (int j = 0; j < WIDTH_MAP; j++)
        {
            if (TileMap[i][j] == 'f')
            {
                s_map.setTextureRect(IntRect(5*16, 3*16, 16, 16));
            }
            //встретили символ s, то рисуем 2й квадратик



            //если
                //встретили символ 0, то рисуем 3й квадратик

                s_map.setScale({2,2});
                s_map.setPosition(j * 32, i * 32);//раскладываем квадратики в карту.

                window->draw(s_map);//рисуем квадратики на экран
        }
}


void DrawingWalls(Sprite s_map, RenderWindow* window){
    for (int i = 0; i < HEIGHT_MAP; i++)
        for (int j = 0; j < WIDTH_MAP; j++)
        {
            if (TileMap_resourses[i][j] == '0')
            {
                s_map.setTextureRect(IntRect(2*16, 16*6, 16, 16));

                s_map.setScale({2,2});
                s_map.setPosition(j * 32, i * 32);//раскладываем квадратики в карту.

                window->draw(s_map);
            }
            //встретили символ s, то рисуем 2й квадратик



            //если
                //встретили символ 0, то рисуем 3й квадратик

                //рисуем квадратики на экран
        }
}
void DrawingResoursesForEat(Sprite s_map, RenderWindow* window){
    for (int i = 0; i < HEIGHT_MAP; i++)
        for (int j = 0; j < WIDTH_MAP; j++)
        {
            if (TileMap_resoursesForEat[i][j] == 'p')
            {
                s_map.setTextureRect(IntRect(0, 0, 16, 16));

                s_map.setScale({2,2});
                s_map.setPosition(j * 32, i * 32);//раскладываем квадратики в карту.

                window->draw(s_map);
            }
            //встретили символ s, то рисуем 2й квадратик



            //если
                //встретили символ 0, то рисуем 3й квадратик

                //рисуем квадратики на экран
        }
}

void DrawingPoisonMap(Sprite s_map, RenderWindow* window){
    for (int i = 0; i < HEIGHT_MAP_poison; i++)
        for (int j = 0; j < WIDTH_MAP_poison; j++)
        {
            if (TileMap_poison[i][j] == 'd')
            {
                s_map.setTextureRect(IntRect(0, 123, 16, 16));

                s_map.setScale({2,2});
                s_map.setPosition(j * 32, i * 32);//раскладываем квадратики в карту.

                window->draw(s_map);

            }

        }
}

int main()
{
    Font font;
    font.loadFromFile("D:/sprites/Fonts/Montserrat-Italic-VariableFont_wght.ttf");
    Text text("", font, 20);

    text.setColor(Color::Red);

    text.setStyle(sf::Text::Bold | sf::Text::Underlined);



    Image map_image;//объект изображения для карты
    map_image.loadFromFile("images/mappurple.png");//загружаем файл для карты
    Texture map;//текстура карты
    map.loadFromImage(map_image);//заряжаем текстуру картинкой
    Sprite s_map;//создаём спрайт для карты
    s_map.setTexture(map);


    Image apples_image;//объект изображения для карты
    apples_image.loadFromFile("images/Crops.png");//загружаем файл для карты
    Texture apple;//текстура карты
    apple.loadFromImage(map_image);//заряжаем текстуру картинкой
    Sprite s_apple;//создаём спрайт для карты
    s_apple.setTexture(apple);



    Player p("hero1.png", 100, 100, 32, 32);

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(640, 480, desktop.bitsPerPixel), "Lesson 5");

    Clock clock; //создаем переменную времени и одновременно запускаем часы!

    float CurrentFrame = 0;

    bool flag = false;

    float createObject = 0;

    Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры
    int gameTime = 0;

    Clock timerforspawn;
    while (window.isOpen())
    {
        std::cout << p.speed;
        float time2 = timerforspawn.getElapsedTime().asSeconds();
        // дать время с последнего перезапуска часов, в данном случае время, прошедшее с
        //предыдущей итерации и вызова restart();
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart(); //перезапуск часов
        time = time / 800; //скорость игры



        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
        }

        createObject += time;
        //"-0,1" - будем считать скоростью, умножаем её на наше время “time” и получаем
        //пройденное расстояние
        if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();

        if (time2 > 10){
            timerforspawn.restart();
            randomMapGenerate();
        }



        p.update(time);
        window.clear();
        //оживляем объект “p” класса “Player” с помощью времени sfml,
        // передавая время в качестве параметра функции update.

        DrawingFloore(s_map, &window);
        DrawingWalls(s_map, &window);
        DrawingResoursesForEat(s_map, &window);
        DrawingPoisonMap(s_map, &window);
        std::ostringstream playerScoreString; // объявили переменную
        playerScoreString << p.playerScore;

        text.setString("Player score: " + playerScoreString.str());//задает строку тексту
        text.setPosition(50, 50);//задаем позицию текста
        window.draw(text);


        window.draw(p.sprite);

        window.display();
    }
    return 0; }


