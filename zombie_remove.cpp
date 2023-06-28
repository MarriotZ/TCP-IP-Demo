void handler_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))
    {
        printf("Removed process id: %d \n", id);
        printf("Message from child: %d \n", WEXITSTATUS(status));
    }
}
int main(int argc, char *argv[]){
    pid_t pid;
    struct sigaction act;
    act.sa_handler = handler_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);
    pid = fork();
    if (pid == 0){
        printf("I'm child process!\n");
        sleep(5);
        return 111;
    }
    else {
        printf("Child process id: %d \n", pid);
        pid = fork();
        if (pid == 0)        {
            printf("I'm child process!\n");
            sleep(5);
            exit(222);
        }
        else{
            printf("Child process id: %d \n", pid);
            for (size_t i = 0; i < 5; i++)
            {
                printf("Wait for an event to occur...\n");
                sleep(5);
            }
        }
    }
    return 0;}
