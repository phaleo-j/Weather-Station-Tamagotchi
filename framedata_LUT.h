
#ifndef FRAMEDATA_LUT_H
#define FRAMEDATA_LUT_H

#include <stdint.h>

 typedef struct {
     const uint32_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;

    const extern tImage Image;

#endif

