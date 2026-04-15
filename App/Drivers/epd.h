#ifndef __EPD_H
#define __EPD_H

#include "main.h"

/* EPD display size (1.54 inch, 200x200) */
#define EPD_WIDTH       200
#define EPD_HEIGHT      200

/* Color definitions for e-paper */
#define EPD_WHITE       0xFF
#define EPD_BLACK       0x00

/* Rotation */
#define ROTATE_0        0
#define ROTATE_90       90
#define ROTATE_180      180
#define ROTATE_270      270

/* Paint structure for frame buffer management */
typedef struct {
    uint8_t *Image;
    uint16_t Width;
    uint16_t Height;
    uint16_t WidthMemory;
    uint16_t HeightMemory;
    uint16_t Color;
    uint16_t Rotate;
    uint16_t WidthByte;
    uint16_t HeightByte;
} PAINT;

extern PAINT Paint;

/* EPD API */
void EPD_Init(void);
void EPD_Update(void);
void EPD_Clear(uint16_t Color);
void EPD_Display(const uint8_t *Image);
void Paint_NewImage(uint8_t *image, uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color);
void EPD_ShowString(uint16_t x, uint16_t y, const uint8_t *chr, uint16_t size1, uint16_t color);

#endif /* __EPD_H */
