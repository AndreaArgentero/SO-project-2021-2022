#include "headerLIB.h"

int randomValue(int Max_Value, int Min_Value){

    int Rand = rand() % ((Max_Value-Min_Value)+1)+Min_Value;
    
    return Rand;

}

int randomTransfer(int Max_Value, int Min_Value){

    int Rand = rand() % (((Max_Value/4)-Min_Value)+1)+Min_Value;
    
    return Rand;

}

float takeTime(struct timespec gettime){
    float Getting = ((float)gettime.tv_sec + ((float)gettime.tv_nsec)*pow(10, -9))*pow(10,-3);

    return Getting;
}

int DATA(char * path){
    FILE *in_file;
    int number;

    in_file = fopen(path, "r");

    if (in_file == NULL)
    {
        fclose(in_file);
        return 0;
    }
    else
    {
        fscanf(in_file, "%d", &number);
        fclose(in_file);
        return number;
    }
}