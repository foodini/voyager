#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Video output.
// Fix aspect ratio.

typedef unsigned short u8;
typedef unsigned int u32;

const u32 frame_height = 364;
const u32 traces_per_frame = 540;
const u32 video_frame_border_width = (traces_per_frame - frame_height)/2;
const u32 audio_sample_rate = 384000;
const u32 max_samples_per_trace = 3400;
const u32 min_samples_per_trace = 3000;
const u32 num_images_per_channel = 78;

u8 frame[traces_per_frame][frame_height][3];
u8 video_frame[traces_per_frame][traces_per_frame][3];
const u32 video_frame_rate = 30;

u32 start_points[2][num_images_per_channel] = {
    {
         15*audio_sample_rate + 240351,         //0
         21*audio_sample_rate + 245777,
         27*audio_sample_rate + 296562 + 3151,
         33*audio_sample_rate + 267909,
         39*audio_sample_rate + 206400 + 3151,
         45*audio_sample_rate + 144927 + 3151,
         51*audio_sample_rate +  23594 + 3151,
         56*audio_sample_rate + 208044 + 3151,
         62*audio_sample_rate +  13271,
         67*audio_sample_rate + 213105 + 3151,
         74*audio_sample_rate + 256650,         //10
         80*audio_sample_rate + 170066 + 3151,
         86*audio_sample_rate + 148759,
         92*audio_sample_rate + 113755 + 3151,
         97*audio_sample_rate + 366248,
        103*audio_sample_rate + 269951,
        109*audio_sample_rate + 116120,
        114*audio_sample_rate + 266391 + 3151,
        120*audio_sample_rate + 154943,
        126*audio_sample_rate +  91019,
        131*audio_sample_rate + 265735 + 3151,  //20
        137*audio_sample_rate + 142217,
        143*audio_sample_rate +  37743 + 3151,
        149*audio_sample_rate +  76845 + 3151,
        155*audio_sample_rate +   1231 + 3151,
        160*audio_sample_rate + 265923,
        166*audio_sample_rate + 148792,
        172*audio_sample_rate +  84628,
        177*audio_sample_rate + 311218,
        183*audio_sample_rate + 231998 + 3151,
        189*audio_sample_rate + 172027 + 3151,  //30
        194*audio_sample_rate + 355015 + 3151,
        201*audio_sample_rate +  60411,
        207*audio_sample_rate +  12999 + 3151,
        212*audio_sample_rate + 308295,
        218*audio_sample_rate + 233638 + 3151,
        224*audio_sample_rate + 152102 + 3151,
        230*audio_sample_rate +  39363 + 3151,
        236*audio_sample_rate + 113295,
        241*audio_sample_rate + 366821,
        247*audio_sample_rate + 314987,         //40
        253*audio_sample_rate + 243240 - 3151,
        259*audio_sample_rate + 173965,
        265*audio_sample_rate + 121353,
        271*audio_sample_rate +  70064,
        277*audio_sample_rate +  23299,
        282*audio_sample_rate + 365023,
        288*audio_sample_rate + 193849,
        294*audio_sample_rate +  93673,
        300*audio_sample_rate +  14155,
        305*audio_sample_rate + 346213,         //50
        311*audio_sample_rate + 307560,
        317*audio_sample_rate + 147218,
        323*audio_sample_rate +  27443,
        328*audio_sample_rate + 251100 + 3151,
        334*audio_sample_rate + 126710,
        340*audio_sample_rate +  33693 + 3151,
        345*audio_sample_rate + 283171 + 3151,
        351*audio_sample_rate + 160924 + 3151,
        357*audio_sample_rate +  78195,
        362*audio_sample_rate + 263903,         //60
        368*audio_sample_rate + 143670 + 3151*2,
        373*audio_sample_rate + 361377 + 3151*2,
        379*audio_sample_rate + 284588 + 3151,
        385*audio_sample_rate + 227413 + 3151,
        391*audio_sample_rate + 176842,
        397*audio_sample_rate + 16062,
        402*audio_sample_rate + 368972,
        408*audio_sample_rate + 176484 + 3151,
        414*audio_sample_rate + 159821,
        420*audio_sample_rate + 155064 + 3151,  //70
        427*audio_sample_rate +  51185,
        432*audio_sample_rate + 247060 + 3151,
        438*audio_sample_rate + 174216,
        443*audio_sample_rate + 378736,
        449*audio_sample_rate + 233795 + 3151,
        455*audio_sample_rate +  94877,
        461*audio_sample_rate + 198461 + 3151,
    },
    {
         16*audio_sample_rate + 359378 + 3151,  //0
         22*audio_sample_rate + 209325,
         28*audio_sample_rate + 170121 + 3151,
         34*audio_sample_rate + 161714 + 3151,
         40*audio_sample_rate +  94992 + 3151,
         45*audio_sample_rate + 376141,
         51*audio_sample_rate + 215446 + 3151,
         57*audio_sample_rate +  52853,
         62*audio_sample_rate + 337528 + 3151,
         68*audio_sample_rate + 200092 + 3151,
         74*audio_sample_rate + 205555,
         80*audio_sample_rate +  66584 + 3151,  //11
         85*audio_sample_rate + 346872,
         91*audio_sample_rate + 277026 + 3151,
         97*audio_sample_rate + 172925,
        103*audio_sample_rate +  34621,
        108*audio_sample_rate + 341194,
        114*audio_sample_rate + 223624 + 3151,
        120*audio_sample_rate + 139779,
        126*audio_sample_rate +  65050,
        131*audio_sample_rate + 348141 + 3151,
        137*audio_sample_rate + 224467 + 3151,         //21
        143*audio_sample_rate + 146940,
        149*audio_sample_rate +  85823 + 3151,
        154*audio_sample_rate + 371241 + 3151,
        160*audio_sample_rate + 305053,
        166*audio_sample_rate + 245569,
        172*audio_sample_rate + 113060,
        177*audio_sample_rate + 367922 + 3151,
        183*audio_sample_rate + 378191 + 3151,
        189*audio_sample_rate + 253823,
        195*audio_sample_rate + 200249,         //31
        201*audio_sample_rate +  98237,
        206*audio_sample_rate + 366832,
        212*audio_sample_rate + 224703,
        218*audio_sample_rate +  85423,
        223*audio_sample_rate + 349160,
        229*audio_sample_rate + 247995,
        235*audio_sample_rate + 128678 + 3151,
        241*audio_sample_rate +  81391,
        246*audio_sample_rate + 336806,
        252*audio_sample_rate + 213356,         //41
        258*audio_sample_rate + 139425,
        263*audio_sample_rate + 352645,
        269*audio_sample_rate + 306314,
        275*audio_sample_rate + 124880 + 3151,
        281*audio_sample_rate +  31592,
        286*audio_sample_rate + 343591,
        292*audio_sample_rate + 278818 + 3151,
        298*audio_sample_rate + 232050,
        304*audio_sample_rate + 166472,
        310*audio_sample_rate + 166081 + 3151,         //51
        316*audio_sample_rate +  76140,
        322*audio_sample_rate +  32683 + 3151,
        328*audio_sample_rate +  14942 + 3151,
        333*audio_sample_rate + 280296,
        339*audio_sample_rate +  63618 + 3151,
        344*audio_sample_rate + 360468 + 3151,
        350*audio_sample_rate + 278844 + 3151,
        356*audio_sample_rate + 189398,
        362*audio_sample_rate +  64876 + 3151,
        367*audio_sample_rate + 322827 + 3151,         //61
        373*audio_sample_rate + 216440 + 3151,
        379*audio_sample_rate + 193843 + 3151,
        385*audio_sample_rate + 133714,
        391*audio_sample_rate +  34728 + 3151,
        396*audio_sample_rate + 331129,
        402*audio_sample_rate + 293135,
        408*audio_sample_rate + 247545 + 3151,
        414*audio_sample_rate + 306950 + 3151,
        420*audio_sample_rate + 230194 + 3151,
        426*audio_sample_rate + 366703,         //71
        432*audio_sample_rate + 226258 + 3151,
        438*audio_sample_rate + 117409 + 3151,
        444*audio_sample_rate +  86063,
        449*audio_sample_rate + 316364 + 3151,
        455*audio_sample_rate + 187808,
        461*audio_sample_rate +  60886,        // try 64075?
    },
};

const u8 red = 0;
const u8 grn = 1;
const u8 blu = 2;
const u8 bnw = 255;

u8 color[2][num_images_per_channel] = {
    {
        bnw, bnw, bnw, bnw, bnw, bnw, bnw, red, grn, blu,   //0
        bnw, bnw, bnw, red, grn, blu, red, grn, blu, bnw,   //10
        bnw, bnw, bnw, bnw, bnw, bnw, bnw, bnw, red, grn,   //20
        blu, bnw, bnw, bnw, bnw, bnw, bnw, bnw, bnw, bnw,   //30
        bnw, red, grn, blu, red, grn, blu, red, grn, blu,   //40
        bnw, bnw, bnw, bnw, bnw, bnw, bnw, bnw, red, grn,   //50
        blu, red, grn, blu, bnw, red, grn, blu, red, grn,   //60
        blu, red, grn, blu, bnw, bnw, bnw, bnw,             //70
    },
    {
        red, grn, blu, bnw, bnw, bnw, bnw, red, grn, blu,   //0
        bnw, bnw, bnw, bnw, bnw, bnw, bnw, bnw, bnw, bnw,   //10
        bnw, bnw, bnw, bnw, bnw, bnw, bnw, red, grn, blu,   //20
        bnw, bnw, bnw, bnw, bnw, bnw, bnw, bnw, bnw, bnw,   //30
        red, grn, blu, bnw, bnw, bnw, bnw, red, grn, blu,   //40
        bnw, bnw, red, grn, blu, bnw, bnw, bnw, bnw, bnw,   //50
        bnw, bnw, bnw, bnw, bnw, bnw, bnw, bnw, bnw, red,   //60
        grn, blu, bnw, red, grn, blu, bnw, bnw,             //70
    },
};

u8 orientation[2][num_images_per_channel] = {
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //0
        0, 0, 2, 2, 2, 2, 0, 0, 0, 0,   //10
        0, 0, 0, 1, 1, 1, 1, 1, 1, 1,   //20
        1, 1, 1, 1, 1, 1, 0, 0, 1, 1,   //30
        1, 1, 1, 1, 1, 1, 1, 0, 0, 0,   //40
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,   //50
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //60
        1, 1, 1, 1, 2, 1, 1, 1,         //70
    },
    {
        1, 1, 1, 0, 0, 0, 0, 1, 1, 1,   //0
        0, 0, 1, 1, 0, 1, 0, 1, 1, 0,   //10
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0,   //20
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0,   //30
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1,   //40
        0, 1, 0, 0, 0, 1, 0, 0, 0, 1,   //50
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //60
        0, 0, 1, 0, 0, 0, 0, 2,         //70
    },
};

// * 3/2 to make sure I have more than I could possibly need.
const u32 input_buffer_sample_count = traces_per_frame * max_samples_per_trace * 3 / 2;
float input_buffer[input_buffer_sample_count];

u8 cos_lut[1000];

u8 float_to_intensity(float f) {
    static bool initted = false;
    if (!initted) {
        initted = true;
        for (int i=0; i<1000; i++) {
            float cos_term = cos(i*M_PI/1000.0);
            cos_lut[i] = 255 - (u8)((cos_term+1.0) * 255.0/2.0);
        }
    }
    const float lower_bound = -0.26;
    const float upper_bound = 0.18;

    if (f <= lower_bound)
        return 255;
    if (f >= upper_bound)
        return 0;
    float t = (upper_bound - f) / (upper_bound - lower_bound);
    return cos_lut[(u32)(t*1000)];
}

u8 intensity(u32 pixel_start, u32 pixel_end) {
    float max = -1.0;
    for (u32 offset = pixel_start; offset <= pixel_end; offset++) {
        if (input_buffer[offset] > max)
            max = input_buffer[offset];
    }
    return float_to_intensity(max);
}

u32 histogram[100];

u8 average(u32 pixel_start, u32 pixel_end) {
    float sum = 0.0;
    for (u32 offset = pixel_start; offset <= pixel_end; offset++) {
        histogram[(u32)((input_buffer[offset] + 1.0) * 50)]++;
        sum += input_buffer[offset];
    }

    float avg = sum/(pixel_end + 1 - pixel_start);
    return float_to_intensity(avg);
}


void set_video_pixel(u8 audio_channel, u32 sample_offset, u32 x, u32 y, u8 image_channel, u8 pixel_intensity) {
    static u32 previous_sample_offset = 0; 
    static bool initted = false;
    static u32 next_video_frame_boundary;
    static bool video_frame_dirty; 
    static char video_frame_filename[256];
    static char previous_video_frame_filename[256];
    static u32 video_frame_id;

    if (sample_offset < previous_sample_offset)
        initted = false;

    previous_sample_offset = sample_offset;

    if (!initted) {
        printf("Clearing frame buffer...\n");
        next_video_frame_boundary = 0;
        video_frame_dirty = false;
        video_frame_id = 0;
        for (u32 _y=0; _y<traces_per_frame; _y++) {
            for (u32 _x=0; _x<traces_per_frame; _x++) {
                for (u32 c=0; c<3; c++) {
                    video_frame[_y][_x][c] = 0;
                }
            }
        }
        initted = true;
    }

    while (sample_offset > next_video_frame_boundary) {
        sprintf(video_frame_filename, "video_frames/%d_%05d.ppm%c", audio_channel, video_frame_id, '\0');
        unlink(video_frame_filename);
        if (video_frame_dirty || video_frame_id == 0) {
            FILE * out = fopen(video_frame_filename, "wb");
            printf("writing %s...", video_frame_filename);
            fflush(stdout);
            fprintf(out, "P6 %d %d 255\n", traces_per_frame, traces_per_frame);
            for (u32 _y=0; _y<traces_per_frame; _y++) {
                for (u32 _x=0; _x<traces_per_frame; _x++) {
                    for (u32 c=0; c<3; c++) {
                        fputc(video_frame[_y][_x][c], out);
                    }
                }
            }
            fclose(out);
            printf("done\n");
            strcpy(previous_video_frame_filename, video_frame_filename+13);
            video_frame_dirty = false;
        } else {
            printf("linking %s to %s...", video_frame_filename, previous_video_frame_filename);
            fflush(stdout);
            symlink(previous_video_frame_filename, video_frame_filename);
            printf("done\n");
        }

        video_frame_id += 1;
        next_video_frame_boundary += audio_sample_rate / video_frame_rate;
    }

    video_frame[y][x][image_channel] = pixel_intensity;
    video_frame_dirty = true;
}

u32 find_trace_start(u32 start_offset) {
    u32 preamble_max_point = start_offset - 190;
    float max_signal = -1.0;
    //Find that huge spike in the sync data in between traces. These things happen 3100, then 3300,
    //then 3100, 3300, ...., samples apart, so we can only use them as a landmark.
    for (u32 sample_offset = start_offset; sample_offset < start_offset + 190; sample_offset++) {
        if (input_buffer[sample_offset] > max_signal) {
            preamble_max_point = sample_offset;
            max_signal = input_buffer[sample_offset];
        }
    }
    //There's a falling edge, <190 pixels later, that is the start of the actual frame. We're
    //looking for the bottom of that edge:
    u32 preamble_min_point = preamble_max_point;
    float min_signal = 1.0;
    for (u32 sample_offset = preamble_max_point; sample_offset < preamble_max_point + 190; sample_offset++) {
        if (input_buffer[sample_offset] < min_signal) {
            preamble_min_point = sample_offset;
            min_signal = input_buffer[sample_offset];
        }
    }

    return preamble_min_point;
}

void find_image_bounds(u32 start_offset, u32 trace_id, u32 * image_start, u32 * image_end) {
    if (trace_id < 164) {
        *image_start = start_offset + 220;
        *image_end = start_offset + 2900;
        if (trace_id % 2 == 0) {
            *image_start -= 12;
            *image_end -= 12;
        }
    } else {
        *image_start = start_offset + 220;
        *image_end = start_offset + 2897;
        if (trace_id % 2 == 0) {
            *image_start -= 12;
            *image_end -= 12;
        }
    }
}

u32 decode(const char * filename, u32 num_frames, u8 audio_channel) {
    char input_filename[256];
    char bnw_output_filename[256];
    char color_output_filename[256];
    sprintf(input_filename, "%s.raw%c", filename, '\0');
    FILE * in = fopen(input_filename, "rb");
    u8 current_color_channel;
    bool end_of_image = false;
    u32 video_x, video_y;

    u32 image_start, image_end;

    for (u32 frame_id=0; frame_id<num_frames; frame_id++) {
        u32 start_point = start_points[audio_channel][frame_id];
        if (color[audio_channel][frame_id] == bnw) {
            current_color_channel = 0;
            end_of_image = true;
        } else { 
            current_color_channel = color[audio_channel][frame_id];
            if (current_color_channel == 2)
                end_of_image = true;
        }
        fseek(in, start_point*4, 0);
        fread(input_buffer, 4, input_buffer_sample_count, in);
        //The start of our frame is going to be wherever the signal hits maximum in the next short
        //span of time:
        u32 trace_start = find_trace_start(0);

        for (u32 trace_id = 0; trace_id < traces_per_frame; trace_id++) {
            u32 trace_end = find_trace_start(trace_start + min_samples_per_trace);

            find_image_bounds(trace_start, trace_id, &image_start, &image_end);

            float image_width = image_end - image_start - 1;
            float pixel_width = image_width / frame_height;
            float pixel_start = image_start;
            float pixel_end;
            if (current_color_channel == 0) {
                switch (orientation[audio_channel][frame_id]) {
                    case 0:
                        //No Correction
                        for (u32 i=0; i<traces_per_frame; i++) {
                            video_frame[i][trace_id][0] = 0;
                            video_frame[i][trace_id][1] = 0;
                            video_frame[i][trace_id][2] = 0;
                        }
                    break;
                    case 1:
                        //Rotate Left
                        for (u32 i=0; i<traces_per_frame; i++) {
                            video_frame[trace_id][i][0] = 0;
                            video_frame[trace_id][i][1] = 0;
                            video_frame[trace_id][i][2] = 0;
                        }
                    break;
                    case 2:
                        //Rotate Left
                        for (u32 i=0; i<traces_per_frame; i++) {
                            video_frame[traces_per_frame - trace_id - 1][i][0] = 0;
                            video_frame[traces_per_frame - trace_id - 1][i][1] = 0;
                            video_frame[traces_per_frame - trace_id - 1][i][2] = 0;
                        }
                    break;
                }
            }
            for (u32 pixel_id = 0; pixel_id < frame_height; pixel_id++) {
                pixel_end = pixel_start + pixel_width;
                //frame[trace_id][pixel_id] = intensity((u32)pixel_start, (u32)pixel_end - 1);
                u8 pixel_intensity = average((u32)pixel_start, (u32)pixel_end - 1);
                frame[trace_id][pixel_id][current_color_channel] = pixel_intensity;
                if (orientation[audio_channel][frame_id] == 0) {
                    //No correction
                    video_x = trace_id;
                    video_y = pixel_id + video_frame_border_width;
                } else {
                    if (orientation[audio_channel][frame_id] == 1) {
                        //TODO clear row if pixel_id == 0
                        //Rotate right
                        video_x = traces_per_frame - pixel_id - video_frame_border_width - 1;
                        video_y = trace_id;
                    } else {
                        //TODO clear row if pixel_id == 0
                        //Rotate left
                        video_x = video_frame_border_width + pixel_id;
                        video_y = traces_per_frame - trace_id - 1;
                    }
                }
                set_video_pixel(audio_channel, start_point + pixel_end, video_x, video_y,
                                current_color_channel, pixel_intensity);
                if (color[audio_channel][frame_id] == bnw) {
                    //Hack. 
                    video_frame[video_y][video_x][1] = video_frame[video_y][video_x][2] = pixel_intensity;
                }
                pixel_start = pixel_end;
            }
            trace_start = trace_end;
        }

        sprintf(bnw_output_filename, "%s_%03d.pgm%c", filename, frame_id, '\0');
        sprintf(color_output_filename, "%s_%03d.ppm%c", filename, frame_id, '\0');
        printf("%s\n", bnw_output_filename);
        FILE * bnw_out = fopen(bnw_output_filename, "wb");
        FILE * color_out = NULL;

        if (current_color_channel == blu)
            color_out = fopen(color_output_filename, "wb");
        switch (orientation[audio_channel][frame_id]) {
            case 0:  // No correction
                fprintf(bnw_out, "P5 %d %d 255\n", traces_per_frame, frame_height);
                if (color_out)
                    fprintf(color_out, "P6 %d %d 255\n", traces_per_frame, frame_height);
                for (u32 y=0; y<frame_height; y++)
                    for (u32 x=0; x<traces_per_frame; x++) {
                        fputc(frame[x][y][current_color_channel], bnw_out);
                        if (color_out) {
                            fputc(frame[x][y][0], color_out);
                            fputc(frame[x][y][1], color_out);
                            fputc(frame[x][y][2], color_out);
                        }
                    }
                break;
            case 1:  // Rotate left
                fprintf(bnw_out, "P5 %d %d 255\n", frame_height, traces_per_frame);
                if (color_out)
                    fprintf(color_out, "P6 %d %d 255\n", frame_height, traces_per_frame);
                for (u32 x=0; x<traces_per_frame; x++)
                    for (u32 y=frame_height-1; y<frame_height; y--) {
                        fputc(frame[x][y][current_color_channel], bnw_out);
                        if (color_out) {
                            fputc(frame[x][y][0], color_out);
                            fputc(frame[x][y][1], color_out);
                            fputc(frame[x][y][2], color_out);
                        }
                    }
                break;
            case 2:  //rotate right
                fprintf(bnw_out, "P5 %d %d 255\n", frame_height, traces_per_frame);
                if (color_out)
                    fprintf(color_out, "P6 %d %d 255\n", frame_height, traces_per_frame);
                for (u32 x=traces_per_frame-1; x<traces_per_frame; x--)
                    for (u32 y=0; y<frame_height; y++) {
                        fputc(frame[x][y][current_color_channel], bnw_out);
                        if (color_out) {
                            fputc(frame[x][y][0], color_out);
                            fputc(frame[x][y][1], color_out);
                            fputc(frame[x][y][2], color_out);
                        }
                    }
                break;
        }
        fclose(bnw_out);
        if (color_out)
            fclose(color_out);
    }

    return 0;
}

int main() {
    for (u32 i=0; i< 100; i++)
        histogram[i] = 0;

    decode("LeftChannel", num_images_per_channel, 0);
    decode("RightChannel", num_images_per_channel, 1);
    for (u32 i=0; i< 100; i++)
        printf ("%f %d\n", i/50.0 - 1.0, histogram[i]);

    return 0;
}
