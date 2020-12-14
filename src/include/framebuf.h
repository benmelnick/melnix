#ifndef INCLUDE_FRAMEBUF_h_
#define INCLUDE_FRAMEBUF_h_

// Framebuffer is a memory-mapped I/O device for displaying data to the screen
// Has 80 columns and 25 rows
// Framebuffer is just an array of 16-bit words:
// Bit:     | 15 14 13 12 | 11 10 9 8 | 7 6 5 4 3 2 1 0 |
// Content: | BG color    | FG color  | ASCII character |

#define FB_ADDRESS  0x000B8000    // starting address of frame buffer
#define FB_COLUMNS  80
#define FB_ROWS     25

// Framebuffer has two I/O ports: one to accept data data, and one to
// describe the data being received
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT    0x3D5

// I/O port commands
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

// Available frame buffer colors
#define FB_BLACK      0
#define FB_BLUE       1
#define FB_GREEN      2
#define FB_CYAN       3
#define FB_RED        4
#define FB_MAGENTA    5
#define FB_BROWN      6
#define FB_LGRAY      7
#define FB_DGRAY      8
#define FB_LBLUE      9
#define FB_LGREEN     10
#define FB_LCYAN      11
#define FB_LRED       12
#define FB_LMAGENTA   13
#define FB_LBROWN     14
#define FB_WHITE      15

// API available to kernel
void init_fb(void);
void fb_clear(void);
void fb_put(char);
void fb_write(char*);

#endif //INCLUDE_FRAMEBUF_h_