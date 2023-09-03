#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <CL/cl.h>
#include <ctime>
#include <chrono>
#include <thread>
#include "OpenClBlackbox.h"
#include "Game.h"

int main(int, char**) {
    Game Life = Game(20, 20);

    auto startTime = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000; i++){
        Life.PassPrint();
        
        //std::string userInput;
        //std::getline(std::cin, userInput);
        
        //std::chrono::seconds sleepDuration(1);
        //std::this_thread::sleep_for(sleepDuration);
        //std::system("cls");
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Computation took " << duration.count() << " milliseconds." << std::endl;
    return 0;
}