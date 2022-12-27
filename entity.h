#ifndef ENTITY
#define ENTITY
class entity{
    enum {left, right, up, down, stay} state;
    float x, y, w, h, dx, dy, speed;
    std::string File; //файл с расширением
    Image image;//сфмл изображение
    Texture texture;//сфмл текстура
    Sprite sprite;//сфмл спрайт

    virtual void update(float time);
    virtual void interactionWithMap();
    virtual void getdamge();
    virtual void spawnbullet();
};

#endif // ENTITY

