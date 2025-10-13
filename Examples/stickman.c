#include "ftascii.h"
#include <signal.h>
#include <locale.h>
#include <time.h>
#include <sys/stat.h>

struct stickman {
    int x;
    int y;
};

struct stickman stickman = {
    .x = 8,
    .y = 8,
};

static term_t *term_pointer;

static void handlectrl_c(int sig) {
    (void)sig;
    systemExit(term_pointer);
}

static void setup_signal_handlers() {
    signal(SIGINT, handlectrl_c);
    signal(SIGTERM, handlectrl_c);
}

static void initializeTerm(term_t *term)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    srand(time(NULL));

    setlocale(LC_ALL, "en_US.UTF-8");
    system("stty -echo -icanon -icrnl time 0 min 0");

    write(1, NOMOUSE, 6);  // Hide cursor
    write(1, CLEAR, 4); // Clear screen
    write(1, CURSOR, 6);

    *term = (term_t){   w.ws_col, w.ws_row, w.ws_col * w.ws_row,
                        NULL, NULL, NULL,
                        1, 1, 1e6,0};

    term_pointer = term;

    init_term(term);
    setup_signal_handlers();
}

void KeyPress(char *key, term_t *term) {
    // Escape sequence for arroy keys
    if (key[0] == '\033' && key[1] == '[') {
        int x = stickman.x;
        int y = stickman.y;
        if (key[2] == ARROW_UP
            && !check_terminal_border(term, x, y - 1)) {
            stickman.y--;
        } else if (key[2] == ARROW_DOWN
                   && !check_terminal_border(term, x, y + 3)) {
            stickman.y++;
        } else if (key[2] == ARROW_RIGHT
                   && !check_terminal_border(term, x + 2, y)) {
            stickman.x++;
        } else if (key[2] == ARROW_LEFT
                   && !check_terminal_border(term, x - 2, y)) {
            stickman.x--;
        }

    } else {    // Regular keypress for single characters
        switch (key[0]) {
            case 27: // Dec value for escape
                systemExit(term);
                break;
            case 'q':
                systemExit(term);
                break;
            case 'p':
                systemExit(term);
                break;
            default:
                break;
        }
    }
}

void animated_border(term_t *t, int y)
{
    char *_all_colors[3] = {RED, ORANGE, YELLOW};

    float frequency = 0.1;
    float phase_shift = t->frame * 0.1;
    int color_index = (int)((sin(frequency * y + phase_shift) + 1) * 2) % 3;
    t->pixels[y].color = _all_colors[color_index];
    t->pixels[y].uni = "█";
}

void background(term_t *t, int y)
{
    t->pixels[y].color = CYAN;
    t->pixels[y].uni = "@";
}

static void draw_callback(term_t *term)
{

    set_background_pixel(term, stickman.x + 1, stickman.y, BLACK, " ");
    set_background_pixel(term, stickman.x - 1, stickman.y, BLACK, " ");

    map_pix(term, stickman.x, stickman.y, RED, "⬤");
    map_pix(term, stickman.x, stickman.y + 1, RED, "|");
    map_pix(term, stickman.x, stickman.y + 2, RED, "|");
    map_pix(term, stickman.x + 1, stickman.y + 1, RED, "-");
    map_pix(term, stickman.x -1, stickman.y + 1, RED, "-");
}

int main() {
    term_t *term = malloc(sizeof(term_t));

    if (term == NULL) {
        perror("Failed to allocate memory for term");
        exit(EXIT_FAILURE);
    }

    initializeTerm(term);

    while (term->draw)
    {
        ft_keyhook(term);
        draw(term, &draw_callback);
        usleep(term->delay);
    }

    systemExit(term);
    return 0;
}
