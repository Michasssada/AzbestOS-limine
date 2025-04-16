//
// Created by Tangerine on 21.03.2025.
//
#include <stdint.h>
#include <limine.h>
#include <stddef.h>

// Function declarations
void init_framebuffer(void);
void put_pixel(int x, int y, uint32_t color);
void clear_screen(uint32_t color);
void draw_char(char c, int x, int y, uint32_t color);
void draw_string(const char *str, int x, int y, uint32_t color);

