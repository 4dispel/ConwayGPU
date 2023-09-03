#pragma once
#include <iostream>
#include "OpenClBlackbox.h"

class Game
{
    private:
    std::string widthstring;
    std::string heightstring;
    bool * currentData;
    bool * bufferData;
    size_t cycleglobalworksize[2];
    size_t updateglobalworksize[1];
    void MakeProgram();
    void SetupCurrent();
    void SetupOutput();
    void SetupBlackbox();
    public:
    const size_t width;
    const size_t height;
    CLbox BlackBox;
    char * arrayOutput;
    Game(const int w, const int h);
    ~Game();
    void PassPrint();
    void Pass();
    void Print();
};