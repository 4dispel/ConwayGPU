__kernel void cycle(__global bool* current, __global bool* buffer, __global char* arrayoutput)
{
    int x = get_global_id(0)+1;
    int y = get_global_id(1)+1;
    int k = x+y*width;
    int l = x+y*(width+1);
    arrayoutput[l] = current[k] ? '1' : '0';
	int neighbours = 0;
    neighbours += current[k-1-width];
    neighbours += current[k-width];
    neighbours += current[k+1-width];
    neighbours += current[k-1];
    neighbours += current[k+1];
    neighbours += current[k-1+width];
    neighbours += current[k+width];
    neighbours += current[k+1+width];
    bool is_new =  (current[k] && neighbours >= 2 && neighbours <=3) || (!current[k] && neighbours == 3);
    buffer[k] = is_new;
}

__kernel void update(__global bool* current, __global bool* buffer){
    int i = get_global_id(0);
    current[i] = buffer[i];
}