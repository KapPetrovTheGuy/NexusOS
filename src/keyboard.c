#include "keyboard.h"

#include "console.h"
#include "idt.h"
#include "io_ports.h"
#include "isr.h"
#include "types.h"
#include "string.h"
#include "bitmap.h"
#include "vga.h"
#include "mouse.h"

static BOOL g_caps_lock = FALSE;
static BOOL g_shift_pressed = FALSE;
static BOOL program = FALSE;
static BOOL nexver = FALSE;
char g_ch = 0, g_scan_code = 0;

// see scan codes defined in keyboard.h for index
char g_scan_code_chars[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};


static int get_scancode() {
    int i, scancode = 0;

    // get scancode until status is on(key pressed)
    for (i = 1000; i > 0; i++) {
        // Check if scan code is ready
        if ((inportb(KEYBOARD_STATUS_PORT) & 1) == 0) continue;
        // Get the scan code
        scancode = inportb(KEYBOARD_DATA_PORT);
        break;
    }
    if (i > 0)
        return scancode;
    return 0;
}

char alternate_chars(char ch) {
    switch(ch) {
        case '`': return '~';
        case '1': return '!';
        case '2': return '@';
        case '3': return '#';
        case '4': return '$';
        case '5': return '%';
        case '6': return '^';
        case '7': return '&';
        case '8': return '*';
        case '9': return '(';
        case '0': return ')';
        case '-': return '_';
        case '=': return '+';
        case '[': return '{';
        case ']': return '}';
        case '\\': return '|';
        case ';': return ':';
        case '\'': return '\"';
        case ',': return '<';
        case '.': return '>';
        case '/': return '?';
        default: return ch;
    }
}

void keyboard_handler(REGISTERS *r) {
    int scancode;

    g_ch = 0;
    scancode = get_scancode();
    g_scan_code = scancode;
    if (scancode & 0x80) {
        // key release
    } else {
        // key down
        switch(scancode) {
            case SCAN_CODE_KEY_CAPS_LOCK:
                vga_graphics_fill_rect(50, 22, 200, 175, BLUE);
                break;

            case SCAN_CODE_KEY_ENTER:
                g_ch = '\n';
                break;

            case SCAN_CODE_KEY_TAB:
                g_ch = '\t';
                break;

            case SCAN_CODE_KEY_LEFT_SHIFT:
                g_shift_pressed = TRUE;
                break;
            case SCAN_CODE_KEY_G:
                //refresh = FALSE;
                program = TRUE;
                checkprogram();
                break;

            case SCAN_CODE_KEY_N:
                nexver = TRUE;
                checknexver();
                break;

            case SCAN_CODE_KEY_C:
                program = FALSE;
                nexver = FALSE;
                checkprogram();
                checknexver();
                break;

            default:
                break;
        }
    }
}

void checkprogram() {

    if(program == TRUE)
    {
        vga_graphics_fill_rect(50, 22, 200, 175, WHITE);
        draw_string(50, 23, BLACK, "GUIDE");
        draw_string(50, 110, BLACK, "Press C to close an app");
    }

    else if (program == FALSE)
    {
        vga_graphics_fill_rect(50, 22, 200, 175, BRIGHT_CYAN);
    }
}

void checknexver()
{

    if(nexver == TRUE)
    {
        vga_graphics_fill_rect(50, 22, 200, 200, WHITE);
        draw_string(50, 23, BLACK, "NEXVER");
        draw_string(50, 38, BLACK, "VERSION: NEXUS OS 2.0.2");
    }

    else if (nexver == FALSE)
    {
        vga_graphics_fill_rect(50, 22, 200, 175, BRIGHT_CYAN);
    }
}

void check()
{
    checkprogram();
    checknexver();
}

void keyboard_init() {
    isr_register_interrupt_handler(IRQ_BASE + 1, keyboard_handler);
}

// a blocking character read
char kb_getchar() {
    char c;

    while(g_ch <= 0);
    c = g_ch;
    g_ch = 0;
    g_scan_code = 0;
    return c;
}

char kb_get_scancode() {
    char code;

    while(g_scan_code <= 0);
    code = g_scan_code;
    g_ch = 0;
    g_scan_code = 0;
    return code;
}



