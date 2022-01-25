#ifndef TFUNCS_H
#define TFUNCS_H
double sleeps[100];

int tsleep(int index, int num){
        sleeps[index]+=0.00075;
        if(sleeps[index] < (double)num)
                return 0;
        else
                return (sleeps[index]=0)+1;
}
#endif
