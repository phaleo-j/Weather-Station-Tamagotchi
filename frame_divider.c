
#include "framedata_LUT.h"
#include <stdio.h>
#include <stdlib.h>

static uint32_t frame_data_sprite_buffer[8192]; // declare in SRAM, NOT flash
//



void extract_frame(tImage sprite_sheet, tFrame *frame){


	uint8_t counter = 0;   //to go next row
	uint8_t next_row = 0;
	frame->frame_height = sprite_sheet.height;
	frame->frame_width = sprite_sheet.width / frame->frame_count; //frames might be rectangular

	uint16_t skip_value = ((sprite_sheet.width - frame->frame_width) / 2);

	frame->frame_data = frame_data_sprite_buffer; // point data pointer to start of sprite frame

	sprite_sheet.data = &sprite_sheet.data[((frame->next_ptr_location))] ;  // might do - 1 ?

    // frame width divided by 2 since we are going 2 pixels at a time.
	//save a pointer?

 for (int x = 0; x < (frame->frame_height * frame->frame_width) / 2 ;  x++){  //total elements in frame

	 counter++;
	     if (counter > (frame->frame_width / 2)) {   //time to switch to next row, correct, greater than 24
			 counter = 1;
			 next_row++;
		 }

	  frame->frame_data[x] = sprite_sheet.data[x + ((skip_value * next_row))];
 }



//after loop is over:
 // don't save pointer at the last location, so you need to go back to beginning and skip over the first 24 elements
 frame->next_ptr_location = frame->next_ptr_location + ((frame->frame_width)/2);  // start at next frame offset
 if (frame->next_ptr_location >= ((frame->frame_width / 2) * frame->frame_count)) {
        frame->next_ptr_location = 0;
    }

}




