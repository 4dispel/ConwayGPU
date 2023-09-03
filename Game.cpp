#include <iostream>
#include <cassert>
#include "OpenClBlackbox.h"
#include "Game.h"
#include <ctime>

static std::string read_file( const char* fileName ) {
	std::fstream f;
	f.open( fileName, std::ios_base::in );
	assert( f.is_open() );

	std::string res;
	while( !f.eof() ) {
		char c;
		f.get( c );
		res += c;
	}
	f.close();
	return std::move(res);
}

static std::string searchreplace(std::string plainstring, std::string wrongstring, std::string newstring){
    int position = plainstring.find(wrongstring);
    int wronglength = wrongstring.length();
    while(position != -1) {
        plainstring.erase(position, wronglength);
        plainstring.insert(position, newstring);
        position = plainstring.find(wrongstring);
    }
    return plainstring;
}

void Game::MakeProgram()
{
    std::string s = read_file( "../life.cl" );
    s.pop_back();
    s = searchreplace(s, "width", widthstring);
	const char* programSource = s.c_str();
    BlackBox.CreateProgram(programSource);
}

void Game::SetupCurrent()
{
    std::srand(std::time(0));

    for(int i = 0; i< width; i++){
        currentData[i] = 0;
    }
    for(int i = 0; i< height; i++){
        currentData[i*width] = 0;
    }
    for(int i = 0; i< width; i++){
        currentData[i+(height-1)*width] = 0;
    }
    for(int i = 0; i< height; i++){
        currentData[width-1+width*i] = 0;
    }
    for(int i = 1; i < width -1; i++){
        for(int j = 1; j < height -1; j++){
            currentData[i+width*j] = std::rand() % 2 == 1;
        }
    }
}

void Game::SetupOutput()
{
    for (int i = 0; i < height; i++)
    {
        arrayOutput[(width+1)*i + width] = '\n';
    }
    arrayOutput[((width+1)*height)] = '\0';
    for (int j = 0; j < height; j++)
    {
        for(int k = 0; k < width; k++)
        {
            arrayOutput[j*(width+1) + k] = currentData[j * width + k] ? '1' : '0';
        }
    }
    for (int j = 0; j < height; j++)
    {
        arrayOutput[j*(width+1)] = '.';
        arrayOutput[j*(width+1) + width - 1] = '.';
    }
    for (int k = 0; k < width; k++)
    {
        arrayOutput[k] = '.';
        arrayOutput[(width+1)*(height-1)+k] = '.';
    }
}

void Game::SetupBlackbox()
{
    BlackBox.CreateKernel(0, "cycle");
    BlackBox.CreateKernel(1, "update");
    BlackBox.CreateBuffer(0, width * height * sizeof(bool));
    BlackBox.CreateBuffer(1, width * height * sizeof( bool ));
    BlackBox.CreateBuffer(2, ((width + 1) * height + 1 )* sizeof( char ));
    BlackBox.EnqueueWrite(0, width * height * sizeof( bool ), currentData);
    BlackBox.EnqueueWrite(2, ((width + 1) * height + 1 )* sizeof( char ), arrayOutput);
    BlackBox.SetArg(0, 0, 0);
    BlackBox.SetArg(0, 1, 1);
    BlackBox.SetArg(0, 2, 2);
    cycleglobalworksize[0] = width-2;
    cycleglobalworksize[1] = height-2;
    updateglobalworksize[0] = width*height;
    BlackBox.EnqueKernel(0, 2, cycleglobalworksize);
    BlackBox.SetArg(1, 0, 0);
    BlackBox.SetArg(1, 1, 1);
    BlackBox.EnqueKernel(1, 1, updateglobalworksize);
    BlackBox.await();
}


Game::Game(const int w, const int h) : width(w + 2), height(h + 2)
{
    widthstring = std::to_string(width);
    heightstring = std::to_string(height);
    currentData = new bool[width*height];
    bufferData = new bool[width*height];
    SetupCurrent();
    CLbox BlackBox = CLbox();
    MakeProgram();
    arrayOutput = new char[((width+1)*height)+1];
    SetupOutput();
    SetupBlackbox();
}

Game::~Game()
{
    delete[] currentData;
    delete[] bufferData;
    delete[] arrayOutput;
}

void Game::PassPrint()
{
    //std::cout << "PassPrint" << std::endl;
    BlackBox.EnqueKernel(0, 2, cycleglobalworksize);
    //std::cout << "PassPrint2" << std::endl;
    BlackBox.EnqueKernel(1, 1, updateglobalworksize);
    BlackBox.EnqueueRead(2, ((width + 1) * height + 1 )* sizeof( char ), arrayOutput);
    BlackBox.await();
    std::cout << arrayOutput << std::endl;
}

void Game::Pass()
{
    BlackBox.EnqueKernel(0, 2, cycleglobalworksize);
    //std::cout << "PassPrint2" << std::endl;
    BlackBox.EnqueKernel(1, 1, updateglobalworksize);
    BlackBox.await();
}

void Game::Print()
{
    BlackBox.EnqueueRead(2, ((width + 1) * height + 1 )* sizeof( char ), arrayOutput);
    BlackBox.await();
    std::cout << arrayOutput << std::endl;
}