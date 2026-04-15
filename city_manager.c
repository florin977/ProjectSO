#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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

// argv start from the first argument (right after the "--command"); argc is offset as well
void execute(COMMAND *command, int argc, char **argv) {
    printf("%d, %s\n", argc, argv[0]);

    switch (command->type) {
        case ADD:
            if (argc != 1) {
                fprintf(stderr, "Invalid argument count for the ADD command\n");
                exit(-1);
            }
            // TO DO: implement the logic
        break;

        case LIST:
            if (argc != 1) {
                fprintf(stderr, "Invalid argument count for the LIST command\n");
                exit(-1);
            }
            // TO DO: implement the logic
        break;

        case VIEW:
            if (argc != 2) {
                fprintf(stderr, "Invalid argument count for the VIEW command\n");
                exit(-1);
            }
            // TO DO: implement the logic
        break;

        case REMOVE_REPORT:
            if (argc != 2) {
                fprintf(stderr, "Invalid argument count for the REMOVE_REPORT command\n");
                exit(-1);
            }
            // TO DO: implement the logic
        break;

        case ADD_REPORT:
            // add_report <district_id> <report_id> then prints a newline and asks for data from the user.
            if (argc != 2) {
                fprintf(stderr, "Invalid argument count for the ADD_REPORT command\n");
                exit(-1);
            }
            // TO DO: implement the logic
        break;

        case UPDATE_TRESHOLD:
            if (argc != 2) {
                fprintf(stderr, "Invalid argument count for the UPDATE_TRESHOLD command\n");
                exit(-1);
            }
            // TO DO: implement the logic
        break;

        case FILTER:
            if (argc != 2) {
                fprintf(stderr, "Invalid argument count for the FILTER command\n");
                exit(-1);
            }
            // TO DO: implement the logic
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