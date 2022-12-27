#ifndef POISONSMAP
#define POISONSMAP
#include <SFML/Graphics.hpp>
#include <iostream>
const int HEIGHT_MAP_poison = 15;//размер карты по высоте
const int WIDTH_MAP_poison = 20;//размер карты по ширине

std::string TileMap_poison[HEIGHT_MAP] = {
    "                    ",
    "                    ",
    "                    ",
    "                    ",
    "                    ",
    "                    ",
    "                    ",
    "                    ",
    "                    ",
    "                    ",
    "                    ",
    "                    ",
    "                    ",
    "                    ",
    "                    ",
};
void randomMapGenerate(){//рандомно расставляем камни
    int randomElementX = 0;//переменная для хранения случайного элемента по горизонтали
    int randomElementY = 0;//переменная для хранения случайного элемента по вертикали
    srand(time(0));//Инициализация генератора случайных чисел
    int countStone = 1;//количество камней

    while (countStone>0){

        randomElementX = 1 + rand() % (WIDTH_MAP - 2);//псевдослучайное значение по “x” от 1 до
        //ширина карты-1. Ограничение введено чтобы не получать числа бордюра карты
        randomElementY = 1 + rand() % (HEIGHT_MAP - 2);//по “y”
        if (TileMap_poison[randomElementY][randomElementX] == ' ') {//если встретили символ пробел,
            TileMap_poison[randomElementY][randomElementX] = 'd'; //то ставим туда камень.
            countStone--;
        }
    }
}

#endif // POISONSMAP

