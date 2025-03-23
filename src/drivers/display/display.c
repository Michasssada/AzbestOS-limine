#include "display.h"

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
