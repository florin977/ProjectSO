#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t keep_alive = 1;

void execute(int *pipefd) {
  int pid = fork();

  if (pid == 0) { // Child process
    execlp("../../MonitorReports/monitor_reports",
           "../../MonitorReports/monitor_reports", NULL);

    char buffer[1024];
    char **message;
    message = malloc(sizeof(char *));

    if (message == NULL) {
      perror(NULL);
      exit(-1);
    }
    message[0] = malloc(1024 * sizeof(char));

    if (message[0] == NULL) {
      perror(NULL);
      exit(-1);
    }

    read(pipefd[1], buffer, 1024);
    char len[4];
    sscanf(len, "%4s", buffer);
    printf("%s\n", len);
    int message_len = strtol(len, NULL, 10);
    message[message_len] = 0;

    fprintf(stdout, "%s", message[0]);

  } else if (pid == -1) {
    fprintf(stderr, "Could not fork the monitor_reports process\n");
    exit(-1);
  } else {
    waitpid(pid, NULL, 0);
  }

  void start_monitor() {
    int pipefd[2] = {-1, -1};

    if (pipe(pipefd) == -1) {
      perror(NULL);
      exit(-1);
    }

    dup2(pipefd[1], STDOUT_FILENO);
  }
}

int main(int argc, char **argv) {

  while (keep_alive) {
  }

  return 0;
}
