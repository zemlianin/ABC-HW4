#include <iostream>
#include <pthread.h>
#include <vector>
#include "stdlib.h"
#include <semaphore.h>
#include <fstream>
#include<windows.h>
#include<unistd.h>
sem_t semaphore0;
sem_t semaphore1;
sem_t semaphore2;
sem_t semaphore3;
sem_t semaphore4;
sem_t semaphore5;
bool flagInOut;
std::ofstream outf;
std::ifstream inf;
pthread_mutex_t mutex;

void Review1() {
    sem_wait(&semaphore1);
    sleep(1000);
    sem_post(&semaphore1);
}

void Review2() {
    sem_wait(&semaphore2);
    sleep(1000);
    sem_post(&semaphore2);
}

void Review3() {
    sem_wait(&semaphore3);
    sleep(1000);
    sem_post(&semaphore3);
}

void Review4() {
    sem_wait(&semaphore4);
    sleep(1000);
    sem_post(&semaphore4);
}

void Review5() {
    sem_wait(&semaphore5);
    sleep(1000);
    sem_post(&semaphore5);
}

void out(std::string s) {
    if (flagInOut) {
        std::cout << s;
    } else {
       // std::ofstream outf;
      //  pthread_mutex_lock(&mutex);

        if (outf.is_open()) {
            outf << s;
        }
      //  pthread_mutex_unlock(&mutex);
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
        thread = pthread_t();
    }

    void Action(void *(*func)(void *)) {
        pthread_create(&thread, NULL, func, &id);
    }
};

void *Gallery(void *arg) {
    int idVis = *((int *) (arg));
    srand(idVis);
    sem_wait(&semaphore0);
    out(std::to_string(idVis) + " in Gallery\n");
    bool isFinish = false;
    while (!isFinish) {
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
    out(std::to_string(idVis) + " exit from Gallery\n");
}

int main(int argc, char *argv[]) {
    std::vector<Visitor> visitors;
    outf.open("output.txt");
    inf.open("input.txt");
    int number_of_vesitors = 70;
    int command;
    std::cout << "Input case of input and output\n1.From/to File\n2.From/to Console\n";
    std::cin >> command;
    if (command == 1) {
        flagInOut = false;
    } else {
        flagInOut = true;
    }
    std::cout << "Select numbers of vesitors\n1. Input numbers of vesitors\n2. Use arguments of command line";
    std::cin >> command;
    if (command == 1) {
        number_of_vesitors = std::stoi(in());
    } else {
        if (argc == 2) {
            number_of_vesitors = std::stoi(argv[1]);
        }
    }
    sem_init(&semaphore0, 0, 50);
    sem_init(&semaphore1, 0, 10);
    sem_init(&semaphore2, 0, 10);
    sem_init(&semaphore3, 0, 10);
    sem_init(&semaphore4, 0, 10);
    sem_init(&semaphore5, 0, 10);
    for (int i = 0; i < number_of_vesitors; ++i) {
        visitors.push_back(Visitor(i));
        visitors[i].Action(Gallery);
    }
    for (int i = 0; i < number_of_vesitors; ++i) {
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
