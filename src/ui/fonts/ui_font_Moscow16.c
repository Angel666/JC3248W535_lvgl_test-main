/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --font D:/FT85BD/FT85D_DashBoard/assets/MOSCOW2024.otf -o D:/FT85BD/FT85D_DashBoard/assets\ui_font_Moscow16.c --format lvgl -r 0x20-0x3f -r 0x5b-0x7f --no-compress --no-prefilter
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_MOSCOW16
#define UI_FONT_MOSCOW16 1
#endif

#if UI_FONT_MOSCOW16

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xfc, 0x0, 0x30,

    /* U+0022 "\"" */
    0xcf, 0x3c, 0xf3, 0xcc, 0x0,

    /* U+0023 "#" */
    0x0, 0x1b, 0x1f, 0xcf, 0xe3, 0x63, 0xf9, 0xfc,
    0x6c, 0x0, 0x0,

    /* U+0024 "$" */
    0x10, 0x10, 0x3c, 0x3c, 0xd3, 0xd3, 0xd0, 0xd0,
    0x3c, 0x3c, 0x13, 0x13, 0xd3, 0x3c, 0x3c, 0x10,
    0x10,

    /* U+0025 "%" */
    0x1, 0x80, 0xb0, 0xd8, 0xcc, 0x66, 0x63, 0x30,
    0x0, 0x13, 0x19, 0x9c, 0xcc, 0x6c, 0x36, 0x2,
    0x0,

    /* U+0026 "&" */
    0x3c, 0x3c, 0xc3, 0xc1, 0xc0, 0xc0, 0xc0, 0x38,
    0x38, 0xc0, 0xc7, 0xc7, 0xc0, 0x3f, 0x3f,

    /* U+0027 "'" */
    0xff, 0xc0,

    /* U+0028 "(" */
    0x22, 0xcc, 0xcc, 0xc0, 0xcc, 0xcc, 0xc2, 0x20,

    /* U+0029 ")" */
    0x44, 0x33, 0x33, 0x30, 0x33, 0x33, 0x34, 0x40,

    /* U+002A "*" */
    0x0, 0x24, 0x3c, 0xff, 0xff, 0x1c, 0x34, 0x24,

    /* U+002B "+" */
    0x30, 0xcf, 0xff, 0x30, 0xc0,

    /* U+002C "," */
    0xf8,

    /* U+002D "-" */
    0xff, 0xff,

    /* U+002E "." */
    0x38,

    /* U+002F "/" */
    0x1, 0x80, 0xc0, 0xc0, 0x60, 0x60, 0x30, 0x30,
    0x0, 0x10, 0x18, 0xc, 0xc, 0x6, 0x6, 0x3,
    0x0,

    /* U+0030 "0" */
    0x3c, 0x3c, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x0,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x3c, 0x3c,

    /* U+0031 "1" */
    0xff, 0xfc, 0xff, 0xfc,

    /* U+0032 "2" */
    0xfc, 0x7c, 0x3, 0x3, 0x3, 0x3, 0x3c, 0x3c,
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x3e, 0x3e,

    /* U+0033 "3" */
    0xfc, 0xfc, 0x3, 0x3, 0x3, 0x3, 0x3c, 0x3c,
    0x3, 0x3, 0x3, 0x3, 0x3, 0xfc, 0xfc,

    /* U+0034 "4" */
    0x0, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x3c,
    0x3c, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x0,

    /* U+0035 "5" */
    0x3f, 0x3f, 0xc0, 0xc0, 0xc0, 0xc0, 0x3c, 0x3c,
    0x3, 0x3, 0x3, 0x3, 0x3, 0xfc, 0xfc,

    /* U+0036 "6" */
    0x3c, 0x3c, 0xc3, 0xc0, 0xc0, 0xc0, 0x3c, 0x3c,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x3c, 0x3c,

    /* U+0037 "7" */
    0x7e, 0xbd, 0xc3, 0xc3, 0x3, 0x3, 0x3, 0x0,
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,

    /* U+0038 "8" */
    0x3c, 0x3c, 0xc3, 0xc3, 0xc3, 0xc3, 0x3c, 0x3c,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x3c, 0x3c,

    /* U+0039 "9" */
    0x3c, 0x3c, 0xc3, 0xc3, 0xc3, 0xc3, 0x3c, 0x3c,
    0x3, 0x3, 0x3, 0xc3, 0xc3, 0x3c, 0x3c,

    /* U+003A ":" */
    0x38, 0x3, 0x80,

    /* U+003B ";" */
    0x38, 0x3e, 0x0,

    /* U+003C "<" */
    0xc7, 0x1c, 0x71, 0x8c, 0xee, 0xe6, 0x0,

    /* U+003D "=" */
    0xff, 0xff, 0x0, 0xff, 0xff,

    /* U+003E ">" */
    0x19, 0xdd, 0xcc, 0x63, 0x8e, 0x38, 0xc0,

    /* U+003F "?" */
    0x3c, 0x3c, 0xc3, 0xc3, 0xc3, 0x3, 0x4, 0x4,
    0x18, 0x18, 0x18, 0x0, 0x0, 0x18, 0x18,

    /* U+005B "[" */
    0x6a, 0xcc, 0xcc, 0xc0, 0xcc, 0xcc, 0xca, 0x60,

    /* U+005C "\\" */
    0xc0, 0x60, 0x38, 0xc, 0x7, 0x1, 0xc0, 0x40,
    0x0, 0xc, 0x7, 0x1, 0x80, 0xe0, 0x30, 0x1c,
    0x6,

    /* U+005D "]" */
    0x65, 0x33, 0x33, 0x30, 0x33, 0x33, 0x35, 0x60,

    /* U+005E "^" */
    0xf,

    /* U+005F "_" */
    0xff, 0xe0,

    /* U+0060 "`" */
    0x18,

    /* U+0061 "a" */
    0x3c, 0x3c, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd,
    0xbd, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x0,

    /* U+0062 "b" */
    0x7c, 0xbc, 0xc3, 0xc3, 0xc3, 0xc3, 0xbc, 0xbc,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbc, 0x7c,

    /* U+0063 "c" */
    0x3c, 0x3c, 0xc3, 0xc3, 0xc0, 0xc0, 0xc0, 0x0,
    0xc0, 0xc0, 0xc0, 0xc3, 0xc3, 0x3c, 0x3c,

    /* U+0064 "d" */
    0x7c, 0xbc, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x0,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbc, 0x7c,

    /* U+0065 "e" */
    0x3f, 0x3f, 0xc0, 0xc0, 0xc0, 0xc0, 0x3c, 0x3e,
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x3f, 0x3f,

    /* U+0066 "f" */
    0x3f, 0x3f, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x3c,
    0x3e, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x0,

    /* U+0067 "g" */
    0x3c, 0x3c, 0xc3, 0xc0, 0xc0, 0xc0, 0x9e, 0x9d,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x3c, 0x3c,

    /* U+0068 "h" */
    0x0, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd,
    0xbd, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x0,

    /* U+0069 "i" */
    0xff, 0xfc, 0xff, 0xfc,

    /* U+006A "j" */
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x0,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x3c, 0x3c,

    /* U+006B "k" */
    0xc3, 0xc7, 0xcc, 0xd8, 0xf0, 0xc0, 0xbc, 0xbd,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x0,

    /* U+006C "l" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x0,
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x3f, 0x3f,

    /* U+006D "m" */
    0x0, 0xc, 0x3, 0xf0, 0xff, 0x9f, 0xdf, 0xbc,
    0xf3, 0xc0, 0x38, 0x1, 0x80, 0x1c, 0x3, 0xc0,
    0x3c, 0x3, 0xc0, 0x3c, 0x3, 0xc0, 0x30,

    /* U+006E "n" */
    0x0, 0xc3, 0xf3, 0xfb, 0xdf, 0xcf, 0xc3, 0x81,
    0x81, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,

    /* U+006F "o" */
    0x3c, 0x3c, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x0,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x3c, 0x3c,

    /* U+0070 "p" */
    0x7c, 0xbc, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbc,
    0xbc, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x0,

    /* U+0071 "q" */
    0x3c, 0x3c, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x0,
    0xc3, 0xc3, 0xc3, 0xcd, 0xce, 0x3f, 0x3b,

    /* U+0072 "r" */
    0x7c, 0xbc, 0xc3, 0xc3, 0xc3, 0xc3, 0xbc, 0xbc,
    0xc0, 0xf0, 0xd8, 0xcc, 0xce, 0xc7, 0xc3,

    /* U+0073 "s" */
    0x3c, 0x3c, 0xc3, 0xc0, 0xc0, 0xc0, 0x3c, 0x3c,
    0x3, 0x3, 0x3, 0xc3, 0xc3, 0x3c, 0x3c,

    /* U+0074 "t" */
    0xff, 0xe7, 0x18, 0x18, 0x18, 0x18, 0x18, 0x0,
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,

    /* U+0075 "u" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x0,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x3c, 0x3c,

    /* U+0076 "v" */
    0x0, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x81,
    0xc3, 0x42, 0x66, 0x66, 0x24, 0x3c, 0x38,

    /* U+0077 "w" */
    0x0, 0x3, 0xc, 0x3c, 0x30, 0xf0, 0xc3, 0xc3,
    0xf, 0xc, 0x3c, 0x30, 0xe0, 0xc1, 0xc7, 0x8d,
    0x12, 0x26, 0x49, 0x9b, 0x36, 0x2c, 0xd0, 0xf3,
    0xc3, 0x87, 0x0,

    /* U+0078 "x" */
    0x81, 0xe0, 0x90, 0xcc, 0x42, 0x61, 0xa0, 0x70,
    0x0, 0x1c, 0x1a, 0x9, 0x8c, 0x44, 0x36, 0xa,
    0x6,

    /* U+0079 "y" */
    0x0, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x3c,
    0x3c, 0x3, 0x3, 0x3, 0x3, 0x3, 0xfc, 0x7c,

    /* U+007A "z" */
    0xfe, 0xfe, 0x0, 0x3, 0x6, 0xc, 0x18, 0x0,
    0x8, 0x18, 0x30, 0x60, 0x0, 0xfe, 0xfe,

    /* U+007B "{" */
    0x8, 0x23, 0xc, 0x30, 0xc3, 0x10, 0x40, 0xc3,
    0xc, 0x30, 0x20, 0x80,

    /* U+007C "|" */
    0x3f, 0xff, 0xcf, 0xff, 0xf0,

    /* U+007D "}" */
    0x41, 0x3, 0xc, 0x30, 0xc3, 0x2, 0x8, 0xc3,
    0xc, 0x31, 0x4, 0x0,

    /* U+007E "~" */
    0xfc
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 118, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 70, .box_w = 2, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 5, .adv_w = 123, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 10, .adv_w = 167, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 21, .adv_w = 152, .box_w = 8, .box_h = 17, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 38, .adv_w = 167, .box_w = 9, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 55, .adv_w = 167, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 70, .adv_w = 67, .box_w = 2, .box_h = 6, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 72, .adv_w = 90, .box_w = 4, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 80, .adv_w = 102, .box_w = 4, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 88, .adv_w = 167, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 96, .adv_w = 167, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 101, .adv_w = 70, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 102, .adv_w = 167, .box_w = 8, .box_h = 2, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 104, .adv_w = 70, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 105, .adv_w = 167, .box_w = 9, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 122, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 137, .adv_w = 167, .box_w = 2, .box_h = 15, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 141, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 156, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 171, .adv_w = 152, .box_w = 8, .box_h = 16, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 187, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 202, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 217, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 232, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 247, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 262, .adv_w = 70, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 265, .adv_w = 70, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 268, .adv_w = 167, .box_w = 5, .box_h = 10, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 275, .adv_w = 167, .box_w = 8, .box_h = 5, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 280, .adv_w = 167, .box_w = 5, .box_h = 10, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 287, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 302, .adv_w = 92, .box_w = 4, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 310, .adv_w = 167, .box_w = 9, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 327, .adv_w = 92, .box_w = 4, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 335, .adv_w = 114, .box_w = 4, .box_h = 2, .ofs_x = 2, .ofs_y = 14},
    {.bitmap_index = 336, .adv_w = 167, .box_w = 11, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 338, .adv_w = 114, .box_w = 3, .box_h = 2, .ofs_x = 3, .ofs_y = 14},
    {.bitmap_index = 339, .adv_w = 152, .box_w = 8, .box_h = 16, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 355, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 370, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 385, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 400, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 415, .adv_w = 152, .box_w = 8, .box_h = 16, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 431, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 446, .adv_w = 152, .box_w = 8, .box_h = 16, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 462, .adv_w = 54, .box_w = 2, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 466, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 481, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 496, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 511, .adv_w = 215, .box_w = 12, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 534, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 549, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 564, .adv_w = 152, .box_w = 8, .box_h = 16, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 580, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 595, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 610, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 625, .adv_w = 167, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 640, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 655, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 670, .adv_w = 247, .box_w = 14, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 697, .adv_w = 152, .box_w = 9, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 714, .adv_w = 152, .box_w = 8, .box_h = 16, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 730, .adv_w = 152, .box_w = 8, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 745, .adv_w = 114, .box_w = 6, .box_h = 15, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 757, .adv_w = 167, .box_w = 2, .box_h = 19, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 762, .adv_w = 114, .box_w = 6, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 774, .adv_w = 167, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = 14}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 32, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 91, .range_length = 36, .glyph_id_start = 33,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_Moscow16 = {
#else
lv_font_t ui_font_Moscow16 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 19,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_MOSCOW16*/

