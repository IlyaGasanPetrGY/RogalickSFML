//#include "entity.h"
//Entity::Entity(sf::Image &image, float X, float Y, int W, int H, std::string Name){
//    x = X; y = Y; //координата появления спрайта
//    w = W; h = H;
//    name = Name;
//    moveTimer = 0;
//    dx = 0; dy = 0;
//    speed = 0;
//    CurrentFrame = 0;
//    health = 100;
//    life = true; //инициализировали логическую переменную жизни, герой жив
//    texture.loadFromImage(image); //заносим наше изображение в текстуру
//    sprite.setTexture(texture);
//}

//sf::FloatRect Entity::getRect(){
//    sf::FloatRect FR(x, y, w, h); // переменная FR типа FloatRect
//    return FR;
//    //return FloatRect(x, y, w, h);
//    //Тип данных (класс) "sf::FloatRect" позволяет хранить четыре координаты прямоугольника
//    //в нашей игре это координаты текущего расположения тайла на карте
//    //далее это позволит спросить, есть ли ещё какой-либо тайл на этом месте
//    //эта ф-ция нужна для проверки пересечений
//}
