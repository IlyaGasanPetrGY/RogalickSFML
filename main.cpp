#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "map.h"
#include "mapresourses.h"
#include "mapwalls.h"

using namespace sf;



class Player { // класс Игрока
public:
    int playerScore = 0;
    float x, y, w, h, dx, dy, speed; //координаты игрока х и у, высота и ширина,
    //ускорение (по х и по у), сама скорость
    int dir; //направление (direction) движения игрока
    std::string File; //файл с расширением
    Image image;//сфмл изображение
    Texture texture;//сфмл текстура
    Sprite sprite;//сфмл спрайт
    //Конструктор с параметрами для класса Player. При создании объекта класса мы будем задавать
    //имя файла, координату Х и У, ширину и высоту
    Player(std::string F, float X, float Y, float W, float H){

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
         dir= 0;
        sprite.setTextureRect(IntRect(0, 0, w, h)); //Задаем спрайту один прямоугольник для
    //вывода одного льва. IntRect – для приведения типов
    }

    void update(float time) //функция "оживления/обновления" объекта класса. Принимает в себя
        //время SFML, вследствие чего работает бесконечно, давая персонажу движение.
    {
        switch (dir)//реализуем поведение в зависимости от направления. Каждая цифра
        //соответствует направлению.
        {
            case 0:
                dx = speed;
                dy = 0;
                break; //по “X” задаем положительную скорость, по “Y” - 0.
            //Персонаж идет только вправо.
            case 1:
                dx = -speed;
                dy = 0;


                break;//по “X” задаем отрицательную скорость, по и “Y” – 0.
            //Персонаж идет только влево.
            case 2:
                dx = 0;
                dy = speed;
                break;//по “X” задаем нулевое значение, по “Y”
            //положительное. Получается, что персонаж идет только вниз
            case 3:
                dx = 0;
                dy = -speed;
                break;//по “X” задаем нулевое значение, по “Y”
            //отрицательное. Получается, что персонаж идет только вверх
        }
        x += dx*time; //движение по “X”
        y += dy*time; //движение по “Y”
        speed = 0; //обнуляем скорость, чтобы персонаж остановился.
        sprite.setPosition(x, y); //выводим спрайт в позицию (x, y).
        interactionWithMap();
        //бесконечно выводим в этой функции, иначе бы наш спрайт стоял на месте.
    }
    void interactionWithMap()//ф-ция взаимодействия с картой
    {
        //Проходим только по тем тайлам (квадратикам размера 32*32), которые контактируют с игроком.
        //Частично или полностью находятся под изображением игрока!
        for (int i = y / 32; i < (y + h) / 32; i++)
            for (int j = x / 32; j<(x + w) / 32; j++){
                //”x” делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается.
                //Он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких тайлах
                //Кроме того, j<(x + w)/32 - условие ограничения координат по “x”, т.е. координата самого
                //правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева
                // направо по иксу, проходя от левого квадрата (соприкасающегося с героем), до правого
                // квадрата (соприкасающегося с героем)
                if (TileMap_resourses[i][j] == '0')//если наш квадратик соответствует символу “0”
                //(стена), то проверяем "направление скорости" персонажа:
                {
                    if (dy>0) {//если мы шли вниз,
                        y = i * 32 - h;//то стопорим (-h) координату “y” персонажа.
                    //сначала получаем координату “i” нашего квадратика на карте и
                    //затем вычитаем из высоты спрайта персонажа.
                    }
                    if (dy<0){
                        y = i * 32 + 32;//аналогично с движением вверх.
                    }
                    if (dx>0){
                        x = j * 32 - w;//если идем вправо, то координата “x” равна
                    //стена (символ 0) минус ширина персонажа
                    }
                    if (dx < 0){
                        x = j * 32 + 32; //аналогично идем влево
                    }

                }
                if (TileMap_resoursesForEat[i][j] == 'p'){
                    playerScore++;
                    TileMap_resoursesForEat[i][j] = ' ';
                }
        }
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

    while (window.isOpen())
    {

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

        //"-0,1" - будем считать скоростью, умножаем её на наше время “time” и получаем
        //пройденное расстояние
        if ((Keyboard::isKeyPressed(Keyboard::Left)||(Keyboard::isKeyPressed(Keyboard::A)))) {

            CurrentFrame += time*0.005;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.dir = 1;
            p.speed = 0.1;
            p.sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 160, 32, 32));
            p.sprite.setScale({-1,1});
        }
        if ((Keyboard::isKeyPressed(Keyboard::Right)||(Keyboard::isKeyPressed(Keyboard::D)))) {
            CurrentFrame += time*0.005;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.dir = 0;
            p.speed = 0.1;
            p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 160, 32, 32));
            p.sprite.setScale({1,1});
        }
        if ((Keyboard::isKeyPressed(Keyboard::Up)||(Keyboard::isKeyPressed(Keyboard::W)))) {

            CurrentFrame += time*0.005;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.dir = 3;
            p.speed = 0.1;
            p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 32, 32));

        }
        if ((Keyboard::isKeyPressed(Keyboard::Down)||(Keyboard::isKeyPressed(Keyboard::S)))) {

            CurrentFrame += time*0.005;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.dir = 2;
            p.speed = 0.1;
            p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 132, 32, 32));

        }





        p.update(time);
        window.clear();


        DrawingFloore(s_map, &window);
        DrawingWalls(s_map, &window);
        DrawingResoursesForEat(s_map, &window);

        std::ostringstream playerScoreString; // объявили переменную
        playerScoreString << p.playerScore;

        text.setString("Player score: " + playerScoreString.str());//задает строку тексту
        text.setPosition(50, 50);//задаем позицию текста
        window.draw(text);


        window.draw(p.sprite);

        window.display();
    }
    return 0; }


