#include "neonate.h"
struct termios orig_termios;
void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}
void die(const char *s)
{
    perror(s);
    exit(1);
}

void neonate(char **command)
{
    char c;
    struct termios original_termios;
    tcgetattr(STDIN_FILENO, &original_termios);
    struct termios raw_termios = original_termios;
    raw_termios.c_lflag &= ~(ICANON | ECHO);
    raw_termios.c_cc[VMIN] = 0;
    raw_termios.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios);
    time_t start_time, end_time;
    time(&start_time);
    int ct = 0;
    while (1)
    {
        time(&end_time);
        double elapsed_time = difftime(end_time, start_time);
        if (elapsed_time > (ct * atoi(command[2])))
        {
            ct++;
            FILE *f = fopen("/proc/loadavg", "r");
            if (f == NULL)
            {
                fprintf(stderr, "Can't open /proc/loadavg file\n");
                break;
            }
            char data[1024];
            fgets(data, 1024, f);
            char *val = strtok(data, " ");
            for (int i = 0; i < 4; i++)
                val = strtok(NULL, " ");

            printf("%s", val);
        }
        if (read(STDIN_FILENO, &c, 1) == -1)
        {
            perror("read");
            break;
        }
        if (c == 'x')
        {
            c = ' ';
            break;
        }
    }
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
    return;
}