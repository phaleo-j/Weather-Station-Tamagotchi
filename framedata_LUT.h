
#ifndef FRAMEDATA_LUT_H
#define FRAMEDATA_LUT_H

#include <stdint.h>

 typedef struct {
     const uint32_t *data;    // might put back const, FRAMEDATA_LUT.C too
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;

  typedef struct {
	  uint32_t *frame_data;
	  uint16_t frame_width;
	  uint16_t frame_height;
	  uint8_t  frame_count;
	  uint32_t next_ptr_location;
  } tFrame;

    const extern tImage Image;
    const extern tImage Sprite;   //removed const
    void extract_frame(tImage sprite_sheet, tFrame *frame);
    const uint32_t *set_nextPointer(void);

#endif
