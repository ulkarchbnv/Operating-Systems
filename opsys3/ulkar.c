#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <mqueue.h>
#include <sys/wait.h>
#include <fcntl.h>

#define SIZE 256
#define QUEUE_NAME "/chainsaw_info"

void handler(int sig) {
    printf("Customer service got signal: %d\n", sig);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s \"Model\" Price\n", argv[0]);
        return 1;
    }
    
    char *model = argv[1];
    int price = atoi(argv[2]);
    
    signal(SIGUSR1, handler);
    
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe error");
        return 1;
    }
    
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = SIZE,
        .mq_curmsgs = 0
    };
    
    mq_unlink(QUEUE_NAME);
    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
    if (mq == -1) {
        perror("mq open error");
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }
    
    pid_t pid = fork();
    
    if (pid > 0) {
        close(pipefd[1]);
        
        pause();
        
        char msg[SIZE];
        read(pipefd[0], msg, SIZE);
        printf("Customer service gets a question: %s", msg);
        
        mq_send(mq, model, strlen(model) + 1, 0);
        
        char priceText[32];
        sprintf(priceText, "%d", price);
        mq_send(mq, priceText, strlen(priceText) + 1, 0);
        
        wait(NULL);
        
        close(pipefd[0]);
        mq_close(mq);
        mq_unlink(QUEUE_NAME);
        printf("Customer service done.\n");
    }
    else if (pid == 0) {
        close(pipefd[0]);
        
        sleep(2);
        
        kill(getppid(), SIGUSR1);
        char question[] = "What is the best deal right now?\n";
        write(pipefd[1], question, strlen(question) + 1);
        printf("Buyer sent question.\n");
        
        char modelRecv[SIZE];
        char priceRecv[SIZE];
        mq_receive(mq, modelRecv, SIZE, NULL);
        mq_receive(mq, priceRecv, SIZE, NULL);
        
        printf("Buyer now has info:\n");
        printf("  Model: %s\n", modelRecv);
        printf("  Price: %s Ft\n", priceRecv);
        
        close(pipefd[1]);
        mq_close(mq);
        printf("Buyer done.\n");
        exit(0);
    }
    else {
        perror("fork error");
        close(pipefd[0]);
        close(pipefd[1]);
        mq_close(mq);
        mq_unlink(QUEUE_NAME);
        return 1;
    }
    
    return 0;
}