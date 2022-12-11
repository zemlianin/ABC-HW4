#include <iostream>
#include <pthread.h>
#include <vector>
#include "stdlib.h"
#include <semaphore.h>
#include <fstream>

#include<chrono>

#ifdef _WIN32

#include <windows.h>

void sleep(unsigned milliseconds) {
    Sleep(milliseconds);
}

#else
#include <unistd.h>

void sleep(unsigned milliseconds)
{
    usleep(milliseconds * 1000); // takes microseconds
}
#endif

sem_t semaphore0;
sem_t semaphore1;
sem_t semaphore2;
sem_t semaphore3;
sem_t semaphore4;
sem_t semaphore5;
bool flagInOut;
std::ofstream outf;
std::ifstream inf;

// 5 картин, на которых, посетитель за счет семафоров ожидает снижения числа зрителей до 10 ,
// задерживается на некоторое время и затем отходит от картины
void Review1() {
    sem_wait(&semaphore1);
    sleep(100);
    sem_post(&semaphore1);
}

void Review2() {
    sem_wait(&semaphore2);
    sleep(100);
    sem_post(&semaphore2);
}

void Review3() {
    sem_wait(&semaphore3);
    sleep(100);
    sem_post(&semaphore3);
}

void Review4() {
    sem_wait(&semaphore4);
    sleep(100);
    sem_post(&semaphore4);
}

void Review5() {
    sem_wait(&semaphore5);
    sleep(100);
    sem_post(&semaphore5);
}

void out(std::string s) {
    if (flagInOut) {
        std::cout << s;
    } else {
        if (outf.is_open()) {
            outf << s;
        }
    }
}

std::string in() {
    std::string s;
    if (flagInOut) {
        std::cin >> s;
    } else {

        if (inf.is_open()) {
            inf >> s;
        }

    }
    return s;
}

class Visitor {
public:
    int id;
    pthread_t thread;

    Visitor(int id) {
        this->id = id;
        // У каждого посетителя есть суой поток, он будет действоать независимо
        thread = pthread_t();
    }

    void Action(void *(*func)(void *)) {
        pthread_create(&thread, NULL, func, &id);
    }
};

void *Gallery(void *arg) {
    int idVis = *((int *) (arg));
    srand(idVis);
    // Посетители пытаются попасть в зал с картинами, но стоит симофор с лиитом в 50 человек
    sem_wait(&semaphore0);
    out(std::to_string(idVis) + " in Gallery\n");
    bool isFinish = false;
    while (!isFinish) {
        // Посетитель принимает решение, пройти к какой либо картине или уйти из галлереи
        int choice = (rand()) % 6;
        switch (choice) {
            case 0:
                isFinish = true;
                break;
            case 1:
                out(std::to_string(idVis) + " around 1 Picture\n");
                Review1();
                break;
            case 2:
                out(std::to_string(idVis) + " around 2 Picture\n");
                Review2();
                break;
            case 3:
                out(std::to_string(idVis) + " around 3 Picture\n");
                Review3();
                break;
            case 4:
                out(std::to_string(idVis) + " around 4 Picture\n");
                Review4();
                break;
            case 5:
                out(std::to_string(idVis) + " around 5 Picture\n");
                Review5();
                break;
        }
    }
    sem_post(&semaphore0);
    // Выход из галлереи
    out(std::to_string(idVis) + " exit from Gallery\n");
}

int main(int argc, char *argv[]) {
    /*Условие задачи:
    17. Задача о картинной галерее.
    Вахтер следит за тем, чтобы в картинной
    галерее одновременно было не более 50 посетителей. Для обозрения представлены
    5 картин. Посетитель ходит от картины к картине, и если на картину любуются более
    чем десять посетителей, он стоит в стороне и ждет, пока
    число желающих увидеть картину не станет меньше. Посетитель может покинуть галерею. В
    галерею также пытаются постоянно зайти новые посетители, которые ожидают своей очереди,
    если та заполнена. Создать многопоточное приложение,
    моделирующее однодневную работу картинной галереи.*/

    std::vector<Visitor> visitors;
    outf.open("output.txt");
    inf.open("input.txt");
    // Дефолтное значение числа зрителей
    int number_of_vesitors = 70;
    int command;
    std::cout << "Input case of input and output\n1.From/to File\n2.From/to Console\n";
    std::cin >> command;
    if (command == 1) {
        flagInOut = false;
    } else {
        flagInOut = true;
    }
    std::cout << "Select numbers of vesitors\n1. Input numbers of vesitors\n2. Use arguments of command line"
                 "\n3. Use Random\n ";
    std::cin >> command;
    if (command == 1) {
        number_of_vesitors = std::stoi(in());
    } else if (command == 2) {
        if (argc == 2) {
            number_of_vesitors = std::stoi(argv[1]);
        }
    } else {
        srand(std::chrono::system_clock::now().time_since_epoch().count());
        number_of_vesitors = rand() % 100+200;
        std::cout << std::to_string(number_of_vesitors) + "\n";
    }
    // Инициализация симофоров
    sem_init(&semaphore0, 0, 50);
    sem_init(&semaphore1, 0, 10);
    sem_init(&semaphore2, 0, 10);
    sem_init(&semaphore3, 0, 10);
    sem_init(&semaphore4, 0, 10);
    sem_init(&semaphore5, 0, 10);
    for (int i = 0; i < number_of_vesitors; ++i) {
        visitors.push_back(Visitor(i));
        // У галлереи появляются посетители
        visitors[i].Action(Gallery);
    }
    for (int i = 0; i < number_of_vesitors; ++i) {
        // Ожидание завершения просмотра для всех посетителей
        pthread_join(visitors[i].thread, NULL);
    }

    sem_destroy(&semaphore0);
    sem_destroy(&semaphore1);
    sem_destroy(&semaphore2);
    sem_destroy(&semaphore3);
    sem_destroy(&semaphore4);
    sem_destroy(&semaphore5);
    outf.close();
    inf.close();
    return 0;
}
