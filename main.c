/*
 * sttyl - A simple implementation of stty
 */
#include "main.h"

int main(int argc, char *argv[]) {
    struct termios tty_info;

    if (tcgetattr(STDIN_FILENO, &tty_info) == -1) {
        perror("Cannot get params about stdin");
        exit(EXIT_FAILURE);
    }

    if (argc == 1) {
        show_baud(cfgetospeed(&tty_info));
        show_keys(&tty_info);
        show_flags(&tty_info);
    } else {
        for (int i = 1; i < argc; i++) {
            int table = -1;
            int match = -1;
            char argument[30];
            int value = 1;

            if (*argv[i] == '-') {
                value = 0;                      // set value to deactivate
                strcpy(argument, argv[i] + 1);  // copy without '-'
            } else {
                strcpy(argument, argv[i]);      // keep value as 1 and copy wholesale
            }

            for (int ii = 0; ii < (sizeof(iflags)/sizeof(struct flaginfo))-1; ii++) {
                if (strcmp(argument, iflags[ii].fl_name) == 0) {
                    match = iflags[ii].fl_value;
                    table = 0;
                }
            }
            for (int ii = 0; ii < (sizeof(oflags)/sizeof(struct flaginfo))-1; ii++) {
                if (strcmp(argument, oflags[ii].fl_name) == 0) {
                    match = oflags[ii].fl_value;
                    table = 1;
                }
            }
            for (int ii = 0; ii < (sizeof(lflags)/sizeof(struct flaginfo))-1; ii++) {
                if (strcmp(argument, lflags[ii].fl_name) == 0) {
                    match = lflags[ii].fl_value;
                    table = 2;
                }
            }
            for (int ii = 0; ii < (sizeof(cflags)/sizeof(struct flaginfo))-1; ii++) {
                if (strcmp(argument, cflags[ii].fl_name) == 0) {
                    match = cflags[ii].fl_value;
                    table = 3;
                }
            }
            for (int ii = 0; ii < (sizeof(c_cc)/sizeof(struct flaginfo))-1; ii++) {
                if (strcmp(argument, c_cc[ii].fl_name) == 0) {
                    if (++i < argc) {
                        match = c_cc[ii].fl_value;
                        table = 4;
                        value = (match == VMIN || match == VTIME ? atoi((const char *) argv[i]) : normalize_key(*argv[i]));//*argv[i]);
                        printf("%d\n", value);
                    } else {
                        match = -1;
                    }
                }
            }

            if (match == -1) {
                perror("unknown mode");
            } else {
                set_flag(value, table, match, &tty_info);
            }
        }
    }

    return 0;
}

/*
 * prints the speed in english
 */
void show_baud(int speed) {
  printf("speed ");
  switch (speed) {
    case B110:    printf("110\n");    break;
    case B300:    printf("300\n");    break;
    case B600:    printf("600\n");    break;
    case B1200:   printf("1200\n");   break;
    case B1800:   printf("1800\n");   break;
    case B2400:   printf("2400\n");   break;
    case B4800:   printf("4800\n");   break;
    case B9600:   printf("9600\n");   break;
    case B19200:  printf("19200\n");  break;
    case B38400:  printf("38400\n");  break;
    case B57600:  printf("57600\n");  break;
    case B115200: printf("115200\n"); break;
    default:      printf("fast\n");   break;                // Can't be bothered past this point
  }
}

/*
 * shows values of c_cc
 */
void show_keys(struct termios *ttyp) {
    for (int i = 0; i < 4; i++) {
        printf("%s = ", c_cc[i].fl_name);
        printf("^%c; ", normalize_key(ttyp->c_cc[c_cc[i].fl_value]));
    }
    printf("min = %d; time = %d;\n", ttyp->c_cc[VMIN], ttyp->c_cc[VTIME]);
}

/*
 * shows output, input, local, and control flags
 */
void show_flags(struct termios *ttyp) {
    show_flagset(ttyp->c_iflag, iflags);
    show_flagset(ttyp->c_oflag, oflags);
    show_flagset(ttyp->c_lflag, lflags);
    show_flagset(ttyp->c_cflag, cflags);
}

/*
 * print flagset in human readable format
 */
void show_flagset(int value, struct flaginfo bitnames[]) {
    for (int i = 0; bitnames[i].fl_value; i++) {
        printf((value & bitnames[i].fl_value) ? "" : "-");      // print '-' if flag is deactivated
        printf("%s ", bitnames[i].fl_name);
    }
    printf("\n");
}

/*
 * normalizes c_cc values to ascii as C understands it
 */
int normalize_key(int ascii) {
    ascii = ascii - 1 + 'A';                                    // weird math to make C happy
    return ((ascii > 127) ? ascii - 128 : ascii);               // avoid overflows before returning
}

/*
 * set flag based on table passed to function, then commit change
 */
void set_flag(int value, int table, int flag, struct termios *ttyp) {
    switch (table) {
        case 0: // c_iflag
            if (value == 0) {
                ttyp->c_iflag &= ~flag; // deactivate
            } else {
                ttyp->c_iflag |= flag;  // activate
            }
            break;
        case 1: // c_oflag
            if (value == 0) {
                ttyp->c_oflag &= ~flag; // deactivate
            } else {
                ttyp->c_oflag |= flag;  // activate
            }
            break;
        case 2: // c_lflag
            if (value == 0) {
                ttyp->c_lflag &= ~flag; // deactivate
            } else {
                ttyp->c_lflag |= flag;  // activate
            }
            break;
        case 3: // c_cflag
            if (value == 0) {
                ttyp->c_cflag &= ~flag; // deactivate
            } else {
                ttyp->c_cflag |= flag;  // activate
            }
            break;
        case 4: // c_cc
            ttyp->c_cc[flag] = (cc_t) value;                        // assign value to matched flag
            break;
        default:
            perror("Got to 'default' in set_flag table selection"); // this shouldn't happen ever
    }

    if (tcsetattr(STDIN_FILENO, TCSANOW, ttyp) == -1) {             // committ changes
        perror("tcsetattr failed");
        exit(EXIT_FAILURE);
    }
}