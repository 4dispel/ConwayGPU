# ConwayGPU

wanted to try GPU compute using OpenCL

made conways game of life with OpenCL 

main performance problems are the speed at which cout writes to the console and reading the buffer from the GPU

you can run this headless by changing Life.PassPrint(); to Life.Pass();

btw this checks for "NVIDIA Corporation" as the GPU vendor to find the dedicated GPU, obviously change this if you don't use an nvidia gpu

the life.cl file needs to be one directory up from the executable
