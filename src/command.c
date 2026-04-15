#include "../include/command.h"
#include <asm-generic/errno-base.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

void get_role(COMMAND *command, char *s) {
  if (!strcmp(s, "inspector")) {
    command->role = INSPECTOR;
  } else if (!strcmp(s, "manager")) {
    command->role = MANAGER;
  } else {
    fprintf(stderr, "Invalid user role! Valid roles are: inspector; manager\n");
    exit(-1);
  }
}

void get_username(COMMAND *command, char *s) {
  int valid_length = 0;
  // Check that the string is at most 30 characters long
  for (int i = 0; i < 30; i++) {
    if (s[i] == 0) {
      valid_length = 1;
      break;
    }
  }

  if (!valid_length) {
    fprintf(stderr, "Invalid username length. Limit is 30\n");
    exit(-1);
  }

  strcpy(command->username, s);
}

void get_type(COMMAND *command, char *s) {
  if (!strcmp(s, "--add")) {
    command->type = ADD;
  } else if (!strcmp(s, "--list")) {
    command->type = LIST;
  } else if (!strcmp(s, "--view")) {
    command->type = VIEW;
  } else if (!strcmp(s, "--remove_report")) {
    command->type = REMOVE_REPORT;
  } else if (!strcmp(s, "--add_report")) {
    command->type = ADD_REPORT;
  } else if (!strcmp(s, "--update_treshold")) {
    command->type = UPDATE_TRESHOLD;
  } else if (!strcmp(s, "--filter")) {
    command->type = FILTER;
  } else {
    fprintf(stderr,
            "Invalid command type! Supported commands are: "
            "add;list;view;remove_report;add_report;update_treshold;filter\n");
    exit(-1);
  }
}

void create_file(const char *pathname, mode_t mode) {
  if (open(pathname, O_CREAT | O_EXCL | O_RDWR, mode) == -1) {
    // File already exists, just make sure to have the correct permissions and
    // continue
    if (errno == EEXIST) {
      chmod(pathname, mode);

    } else {
      // Some other error, just exit and print the error
      perror("Failed to create file reports.dat");
    }
  }

  // Fix a bug with umask preventing the correct permissions
  chmod(pathname, mode);
}

// TODO: Implement these
void execute_add(COMMAND *command, int argc, char **argv) {
  // Create district directory and change the mdoe if it exists
  if (mkdir(argv[0], 0750)) {
    if (errno == EEXIST) {
      chmod(argv[0], 0750);
    } else {
      perror("Failed to create district directory");
    }
  }

  char path[256];
  snprintf(path, 255, "%s/reports.dat", argv[0]);
  create_file(path, 0664);

  snprintf(path, 255, "%s/district.cfg", argv[0]);
  create_file(path, 0640);

  snprintf(path, 255, "%s/logged_district", argv[0]);
  create_file(path, 0644);
}

void execute_list(COMMAND *command, int argc, char **argv) {}

void execute_view(COMMAND *command, int argc, char **argv) {}

void execute_remove_report(COMMAND *command, int argc, char **argv) {}

void execute_add_report(COMMAND *command, int argc, char **argv) {}

void execute_update_treshold(COMMAND *command, int argc, char **argv) {}

void execute_filter(COMMAND *command, int argc, char **argv) {}

// these argv start right after the "--command"; argc is smaller as well.
void execute(COMMAND *command, int argc, char **argv) {
  // TODO: Check role using chmod in each case.
  switch (command->type) {
  case ADD:
    if (argc != 1) {
      fprintf(stderr, "Invalid argument count for the ADD command\n");
      exit(-1);
    }
    execute_add(command, argc, argv);
    break;

  case LIST:
    if (argc != 1) {
      fprintf(stderr, "Invalid argument count for the LIST command\n");
      exit(-1);
    }
    execute_list(command, argc, argv);
    break;

  case VIEW:
    if (argc != 2) {
      fprintf(stderr, "Invalid argument count for the VIEW command\n");
      exit(-1);
    }
    execute_view(command, argc, argv);
    break;

  case REMOVE_REPORT:
    if (argc != 2) {
      fprintf(stderr, "Invalid argument count for the REMOVE_REPORT command\n");
      exit(-1);
    }
    execute_remove_report(command, argc, argv);
    break;

  case ADD_REPORT:
    // add_report <district_id> <report_id> then prints a newline and asks for
    // data from the user.
    if (argc != 2) {
      fprintf(stderr, "Invalid argument count for the ADD_REPORT command\n");
      exit(-1);
    }
    execute_add_report(command, argc, argv);
    break;

  case UPDATE_TRESHOLD:
    if (argc != 2) {
      fprintf(stderr,
              "Invalid argument count for the UPDATE_TRESHOLD command\n");
      exit(-1);
    }
    execute_update_treshold(command, argc, argv);
    break;

  case FILTER:
    if (argc != 2) {
      fprintf(stderr, "Invalid argument count for the FILTER command\n");
      exit(-1);
    }
    execute_filter(command, argc, argv);
    break;
  }
}
