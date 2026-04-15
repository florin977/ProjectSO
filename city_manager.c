#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef enum ROLE {
    MANAGER,
    INSPECTOR,
} ROLE;

typedef enum COMMAND_TYPE {
    ADD,
    LIST,
    VIEW,
    REMOVE_REPORT,
    ADD_REPORT,
    UPDATE_TRESHOLD,
    FILTER,
} COMMAND_TYPE;

typedef struct GPS_COORDS {
    float lat;
    float lng;
} GPS_COORDS;

typedef struct REPORT_DATA {
    GPS_COORDS coords;
    time_t timestamp;
    int severity_level;
    int report_id;
    // Inspector name is alredy in the command
    char issue_category[30];
    char description[200];
} REPORT_DATA;

typedef union COMMAND_ARGS {
    int treshold_value;
    char district_id[30];
    char report_id[30];
    char filter_condition[200];
    REPORT_DATA report_data;
} COMMAND_ARGS;

typedef struct COMMAND {
    ROLE role;
    COMMAND_TYPE type;
    COMMAND_ARGS args;
    char username[30];
} COMMAND;

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
        fprintf(stderr, "Invalid command type! Supported commands are: add;list;view;remove_report;add_report;update_treshold;filter\n");
        exit(-1);
    }
}

// TODO: Implement these
void execute_add(COMMAND *command, int argc, char **argv) {
    if(mkdir(argv[0], 0750)) {
        perror("Failed to create district directory");
    }
}

void execute_list(COMMAND *command, int argc, char **argv) {
    
}

void execute_view(COMMAND *command, int argc, char **argv) {
    
}

void execute_remove_report(COMMAND *command, int argc, char **argv) {
    
}

void execute_add_report(COMMAND *command, int argc, char **argv) {
    
}

void execute_update_treshold(COMMAND *command, int argc, char **argv) {
    
}

void execute_filter(COMMAND *command, int argc, char **argv) {
    
}

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
            // add_report <district_id> <report_id> then prints a newline and asks for data from the user.
            if (argc != 2) {
                fprintf(stderr, "Invalid argument count for the ADD_REPORT command\n");
                exit(-1);
            }
            execute_add_report(command, argc, argv);
        break;

        case UPDATE_TRESHOLD:
            if (argc != 2) {
                fprintf(stderr, "Invalid argument count for the UPDATE_TRESHOLD command\n");
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

int check_command_integrity(int argc, char **argv) {
    return (strcmp(argv[5], "add") ||
            strcmp(argv[5], "list") ||
            strcmp(argv[5], "view") ||
            strcmp(argv[5], "remove_report") ||
            strcmp(argv[5], "add_report") ||
            strcmp(argv[5], "update_treshold") ||
            strcmp(argv[5], "filter")
        );
}

int check_arg_integrity(int argc, char **argv) {
    int valid_command = check_command_integrity(argc, argv);

    return (!strcmp(argv[1], "--role") && !strcmp(argv[3], "--user") && valid_command);
}

int main(int argc, char **argv) {
    if (argc < 6) {
        fprintf(stderr, "Invalid argument count! Usage: city_manager --role <inspector|manager> --user <user name> --<command> <...command details..>\n");
        exit(-1);
    }

    if (!check_arg_integrity(argc, argv)) {
        fprintf(stderr, "Invalid argument usage! Usage: city_manager --role <inspector|manager> --user <user name> --<command> <...command details..>\n");
        exit(-1);
    }

    COMMAND command;
    get_role(&command, argv[2]);
    get_username(&command, argv[4]);
    get_type(&command, argv[5]);
    execute(&command, argc - 6, &argv[6]);

    printf("%d\n", command.role);
    printf("%s\n", command.username);
    printf("%d\n", command.type);

    return 0;
}