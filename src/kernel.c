
#include "kernel.h"
#include "string.h"
#include "gdt.h"
#include "idt.h"
#include "vga.h"
#include "console.h"
//#include "mouse.h"
#include "keyboard.h"
#include "bitmap.h"

void kmain() {
    gdt_init();
    idt_init();
	console_init(WHITE, BLACK);
	keyboard_init();
	//mouse_init();
	vga_graphics_init();
	vga_graphics_clear_color(BRIGHT_CYAN);
	//draw_string(0, 0, WHITE, "LEFT CLICK TO START GUI");

}
