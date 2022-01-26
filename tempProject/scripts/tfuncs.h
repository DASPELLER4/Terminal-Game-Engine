#ifndef TFUNCS_H
#define TFUNCS_H
double sleeps[100];

void sigintHandler(int sig_num){
        signal(SIGINT, sigintHandler);
}

int tsleep(int index, double num){
        sleeps[index]+=0.00075;
        if(sleeps[index] < (double)num)
                return 0;
        else
                return (sleeps[index]=0)+1;
}

int kbhit(void){
        int ch = getch();
        if (ch != ERR) {
                ungetch(ch);
                return 1;
        } else {
                return 0;
        }
}
#endif
