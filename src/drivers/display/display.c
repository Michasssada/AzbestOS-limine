#include "display.h"
#include "font8x8_basic.h"
// Framebuffer request structure
static volatile struct limine_framebuffer_request framebufferRequest = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Global framebuffer variables
static uint32_t *fbPtr = NULL;
static uint32_t width = 0;
static uint32_t height = 0;
static uint32_t pitch = 0;

void init_framebuffer(void) {
    if (framebufferRequest.response == NULL || framebufferRequest.response->framebuffer_count < 1) {
        for (;;); // Halt if no framebuffer
    }

    struct limine_framebuffer *framebuffer = framebufferRequest.response->framebuffers[0];
    fbPtr = (uint32_t *)framebuffer->address;
    width = framebuffer->width;
    height = framebuffer->height;
    pitch = framebuffer->pitch;
}

void put_pixel(int x, int y, uint32_t color) {
    if (x < 0 || y < 0 || x >= (int)width || y >= (int)height) return;
    fbPtr[y * (pitch / 4) + x] = color;
}

void clear_screen(uint32_t color) {
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            put_pixel(x, y, color);
        }
    }
}

void draw_char(char c, int x, int y, uint32_t color) {
    if (c < 0 || c > 127) return;

    const uint8_t *glyph = font8x8_basic[(int)c];
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (glyph[row] & (1 << col)) {
                put_pixel(x + col, y + row, color);
            }
        }
    }
}
void draw_string(const char *str, int x, int y, uint32_t color) {
    int startX = x;

    while (*str) {
        if (*str == '\n') {
            y += 8;      // Move to next line
            x = startX;  // Reset X to original start position
        } else if (*str == '\r') {
            x = startX;  // Carriage return
        } else if (*str == '\t') {
            x += 8 * 4;  // Move 4 characters forward for tab
        } else {
            draw_char(*str, x, y, color);
            x += 8;
        }
        str++;
    }
}
