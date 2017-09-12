#include <stdio.h>

#define MARK do{printf("%d\n", __LINE__);fflush(stdout);}while(0)

typedef unsigned short u8;
typedef unsigned int u32;

const u32 frame_width = 610;
const u32 frame_height = 160;
const u32 video_frame_rate = 30;
const u32 audio_sample_rate = 384000;
const u32 samples_per_file = audio_sample_rate / video_frame_rate;

float input_buffer[samples_per_file];
u8 video_frame[frame_height][frame_width];

const char * input_filenames[2] = {
    "LeftChannel.raw",
    "RightChannel.raw"
};

int main(int argc, char *argv[]) {
    char filename[256];
    for (int input_channel=0; input_channel<2; input_channel++) {
        u32 frame_id = 0;
        FILE * in = fopen(input_filenames[input_channel], "rb");
        while (true) {
            sprintf(filename, "waveforms/%d_%05d.ppm%c", input_channel, frame_id, '\0');
            printf("%s\n", filename);
            for (int y=0; y<frame_height; y++) {
                for (int x=0; x<frame_width; x++) {
                    video_frame[y][x] = 0;
                }
            }
            FILE * out = fopen(filename, "wb");
            fprintf(out, "P6 %d %d 255\n", frame_width, frame_height);
            float sample;
            u32 samples_read = fread(input_buffer, 4, samples_per_file, in);
            float x=0;
            for (u32 sample_id=0; sample_id<samples_per_file; sample_id++) {
                sample = (input_buffer[sample_id] * frame_height / 0.34) + (frame_height/2);
                if (sample < 0.0)
                    sample = 0.0;
                else if (sample >= frame_height)
                    sample = frame_height - 1;
                video_frame[(u32)sample][(u32)x] = 255;
                x += (float)frame_width / samples_per_file;
            }
            for (int i=0; i<frame_height; i++) {
                for (int j=0; j<frame_width; j++) {
                    fputc(video_frame[i][j], out);
                    fputc(video_frame[i][j], out);
                    fputc(video_frame[i][j], out);
                }
            }
            fclose(out);
            if (samples_read < samples_per_file) {
                printf("short on samples\n");
                fclose(in);
                break;
            }
            frame_id++;
        }
    }
}
