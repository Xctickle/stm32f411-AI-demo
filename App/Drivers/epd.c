/**
 * @file    epd.c
 * @brief   1.54" e-paper display driver (SSD1680/UC8151D)
 *          Ported to STM32F411 HAL with hardware SPI1
 */

#include "epd.h"
#include "epd_font.h"
#include "stm32f4xx_hal.h"

extern SPI_HandleTypeDef hspi1;

/* EPD control pins (defined in main.h) */
/* CS  = PB0 */
/* DC  = PB2 */
/* RST = PB1 */
/* BUSY= PA6 */

PAINT Paint;

/* ----- Low-level SPI communication ----- */

static void EPD_WriteByte(uint8_t data)
{
    HAL_GPIO_WritePin(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_SET);
}

static void EPD_WriteCmd(uint8_t cmd)
{
    HAL_GPIO_WritePin(EPD_DC_PORT, EPD_DC_PIN, GPIO_PIN_RESET);
    EPD_WriteByte(cmd);
    HAL_GPIO_WritePin(EPD_DC_PORT, EPD_DC_PIN, GPIO_PIN_SET);
}

static void EPD_WriteData(uint8_t data)
{
    EPD_WriteByte(data);
}

static void EPD_WriteDataBuf(const uint8_t *buf, uint32_t len)
{
    HAL_GPIO_WritePin(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)buf, len, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_SET);
}

static void EPD_WaitBusy(void)
{
    while (HAL_GPIO_ReadPin(EPD_BUSY_PORT, EPD_BUSY_PIN) != GPIO_PIN_RESET)
    {
    }
}

/* ----- Display initialization ----- */

void EPD_Init(void)
{
    /* Hardware reset */
    HAL_GPIO_WritePin(EPD_RST_PORT, EPD_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(EPD_RST_PORT, EPD_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(20);

    EPD_WaitBusy();

    EPD_WriteCmd(0x12);  /* SWRESET */
    EPD_WaitBusy();

    EPD_WriteCmd(0x01);  /* Driver output control */
    EPD_WriteData(0xC7);
    EPD_WriteData(0x00);
    EPD_WriteData(0x01);

    EPD_WriteCmd(0x11);  /* Data entry mode */
    EPD_WriteData(0x01);

    EPD_WriteCmd(0x44);  /* Set RAM-X address start/end */
    EPD_WriteData(0x00);
    EPD_WriteData(0x18); /* (24+1)*8 = 200 */

    EPD_WriteCmd(0x45);  /* Set RAM-Y address start/end */
    EPD_WriteData(0xC7);
    EPD_WriteData(0x00);
    EPD_WriteData(0x00);
    EPD_WriteData(0x00);

    EPD_WriteCmd(0x3C);  /* Border waveform */
    EPD_WriteData(0x05);

    EPD_WriteCmd(0x18);  /* Read built-in temperature sensor */
    EPD_WriteData(0x80);

    EPD_WriteCmd(0x4E);  /* Set RAM X address counter */
    EPD_WriteData(0x00);
    EPD_WriteCmd(0x4F);  /* Set RAM Y address counter */
    EPD_WriteData(0xC7);
    EPD_WriteData(0x00);

    EPD_WaitBusy();
}

void EPD_Update(void)
{
    EPD_WriteCmd(0x22);  /* Display Update Control */
    EPD_WriteData(0xF7);
    EPD_WriteCmd(0x20);  /* Activate Display Update Sequence */
    EPD_WaitBusy();
}

/* ----- Frame buffer operations ----- */

void Paint_NewImage(uint8_t *image, uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color)
{
    Paint.Image = image;
    Paint.WidthMemory = Width;
    Paint.HeightMemory = Height;
    Paint.Color = Color;
    Paint.WidthByte = (Width % 8 == 0) ? (Width / 8) : (Width / 8 + 1);
    Paint.HeightByte = Height;
    Paint.Rotate = Rotate;

    if (Rotate == ROTATE_0 || Rotate == ROTATE_180)
    {
        Paint.Width = Width;
        Paint.Height = Height;
    }
    else
    {
        Paint.Width = Height;
        Paint.Height = Width;
    }
}

static void Paint_SetPixel(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color)
{
    uint16_t X, Y;
    uint32_t Addr;
    uint8_t Rdata;

    switch (Paint.Rotate)
    {
    case 0:
        X = Xpoint;
        Y = Ypoint;
        break;
    case 90:
        X = Paint.WidthMemory - Ypoint - 1;
        Y = Xpoint;
        break;
    case 180:
        X = Paint.WidthMemory - Xpoint - 1;
        Y = Paint.HeightMemory - Ypoint - 1;
        break;
    case 270:
        X = Ypoint;
        Y = Paint.HeightMemory - Xpoint - 1;
        break;
    default:
        return;
    }

    Addr = X / 8 + Y * Paint.WidthByte;
    Rdata = Paint.Image[Addr];

    if (Color == EPD_BLACK)
        Paint.Image[Addr] = Rdata & ~(0x80 >> (X % 8));
    else
        Paint.Image[Addr] = Rdata | (0x80 >> (X % 8));
}

static void EPD_DrawPoint(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color)
{
    Paint_SetPixel(Xpoint - 1, Ypoint - 1, Color);
}

void EPD_Clear(uint16_t Color)
{
    uint16_t X, Y;
    uint32_t Addr;
    for (Y = 0; Y < Paint.HeightByte; Y++)
    {
        for (X = 0; X < Paint.WidthByte; X++)
        {
            Addr = X + Y * Paint.WidthByte;
            Paint.Image[Addr] = Color;
        }
    }
}

/* ----- Display refresh ----- */

void EPD_Display(const uint8_t *Image)
{
    EPD_WriteCmd(0x24);
    EPD_WriteDataBuf(Image, EPD_WIDTH * (EPD_HEIGHT / 8));
    EPD_Update();
}

/* ----- Text rendering ----- */

static void EPD_ShowChar(uint16_t x, uint16_t y, uint16_t chr, uint16_t size1, uint16_t color)
{
    uint16_t i, m, temp, size2, chr1;
    uint16_t x0, y0;

    x += 1; y += 1; x0 = x; y0 = y;
    size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2);
    chr1 = chr - ' ';

    for (i = 0; i < size2; i++)
    {
        temp = asc2_1608[chr1][i];
        for (m = 0; m < 8; m++)
        {
            if (temp & 0x01)
                EPD_DrawPoint(x, y, color);
            else
                EPD_DrawPoint(x, y, !color);
            temp >>= 1;
            y++;
        }
        x++;
        if ((x - x0) == size1 / 2)
        {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}

void EPD_ShowString(uint16_t x, uint16_t y, const uint8_t *chr, uint16_t size1, uint16_t color)
{
    while (*chr != '\0')
    {
        EPD_ShowChar(x, y, *chr, size1, color);
        chr++;
        x += size1 / 2;
    }
}
