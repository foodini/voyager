#include <stdio.h>
#include <stdlib.h>

typedef unsigned char u8;
typedef unsigned int u32;

u8 video_frame[720][1280][3];
u8 background_quote_frame[720][1280][3];
u8 background_blank_frame[720][1280][3];
u8 background_black_boxes_frame[720][1280][3];
u8 background_fadeout_frame[720][1280][3];
u8 background_about_frame[720][1280][3];

bool place(FILE * in, u32 width, u32 height, u32 x_left, u32 y_top) {
    const int num_bytes = width * height * 3;
    u8 input_buffer[num_bytes];

    if (fseek(in, -num_bytes, SEEK_END)) {
        printf("FSEEK FAIL!!!!!\n");
        return false;
    }
    if(fread(input_buffer, 1, num_bytes, in) == num_bytes) {
        for(u32 y=0; y<height; y++) {
            for(u32 x=0; x<width; x++) {
                video_frame[y + y_top][x + x_left][0] = input_buffer[(y*width + x) * 3];
                video_frame[y + y_top][x + x_left][1] = input_buffer[(y*width + x) * 3 + 1];
                video_frame[y + y_top][x + x_left][2] = input_buffer[(y*width + x) * 3 + 2];
            }
        }
    } else {
        printf("FREAD FAIL!!!!!\n");
        return false;
    }
    return true;
}

void dump_file(const char * filename) {
    FILE * out = fopen(filename, "wb");
    fprintf(out, "P6 1280 720 255\n");
    for (int y=0; y<720; y++) {
        for (int x=0; x<1280; x++) {
            fputc(video_frame[y][x][0], out);
            fputc(video_frame[y][x][1], out);
            fputc(video_frame[y][x][2], out);
        }
    }
    fclose(out);
}

void fade_black(u8 background[720][1280][3], u32 frame_start, u32 frame_count, int direction) {
    float t;
    char filename[256];
    for(u32 frame=0; frame<frame_count; frame++) {
        t = (float)frame/frame_count;
        if (direction == -1)
            t = 1.0-t;
        for(int y=0; y<720; y++) {
            for(int x=0; x<1280; x++) {
                video_frame[y][x][0] = (u8)(t*background[y][x][0]);
                video_frame[y][x][1] = (u8)(t*background[y][x][1]);
                video_frame[y][x][2] = (u8)(t*background[y][x][2]);
            }
        }
        sprintf(filename, "final_video/video_frame_%05d.ppm%c", frame_start+frame, '\0');
        printf("fade_black %s\n", filename);
        dump_file(filename);
    }
}

void fade_to(u8 background_to[720][1280][3], u8 background_from[720][1280][3], u32 frame_start, u32 frame_count) {
    float t;
    char filename[256];
    for(u32 frame=0; frame<frame_count; frame++) {
        t = (float)frame/frame_count;
        for(int y=0; y<720; y++) {
            for(int x=0; x<1280; x++) {
                video_frame[y][x][0] = (u8)(t*background_from[y][x][0] + (1-t)*background_to[y][x][0]);
                video_frame[y][x][1] = (u8)(t*background_from[y][x][1] + (1-t)*background_to[y][x][1]);
                video_frame[y][x][2] = (u8)(t*background_from[y][x][2] + (1-t)*background_to[y][x][2]);
            }
        }
        sprintf(filename, "final_video/video_frame_%05d.ppm%c", frame_start+frame, '\0');
        printf("fade_to %s\n", filename);
        dump_file(filename);
    }
}

int main(int argc, char * argv[]) {
    FILE * in;
    char filename[256];
    bool done = false;

    in = fopen("background_quote.pbm", "rb");
    fseek(in, -720*1280*3, SEEK_END);
    fread(background_quote_frame, 1, 720*1280*3, in);
    fclose(in);
    in = fopen("background_blank.pbm", "rb");
    fseek(in, -720*1280*3, SEEK_END);
    fread(background_blank_frame, 1, 720*1280*3, in);
    fclose(in);
    in = fopen("background_black_boxes.pbm", "rb");
    fseek(in, -720*1280*3, SEEK_END);
    fread(background_black_boxes_frame, 1, 720*1280*3, in);
    fclose(in);
    in = fopen("background_fadeout.pbm", "rb");
    fseek(in, -720*1280*3, SEEK_END);
    fread(background_fadeout_frame, 1, 720*1280*3, in);
    fclose(in);
    in = fopen("background_about.pbm", "rb");
    fseek(in, -720*1280*3, SEEK_END);
    fread(background_about_frame, 1, 720*1280*3, in);
    fclose(in);

    fade_black(background_quote_frame, 0, 30, 1);
    u32 frame_id;
    for(frame_id=0; frame_id<900; frame_id++) {
        if(frame_id == 0) {
            fade_black(background_quote_frame, frame_id, 30, 1);
            frame_id += 30;
        }
        if(frame_id == 420) {
            fade_to(background_quote_frame, background_blank_frame, frame_id, 30);
            frame_id += 30;
        }
        if(frame_id == 450) {
            fade_to(background_blank_frame, background_about_frame, frame_id, 30);
            frame_id += 30;
        }
        if(frame_id == 840) {
            fade_to(background_about_frame, background_blank_frame, frame_id, 30);
            frame_id += 30;
        }
        if(frame_id == 870) {
            fade_to(background_blank_frame, background_black_boxes_frame, frame_id, 30);
            frame_id += 30;
        }

        if (frame_id<900) {
            sprintf(filename, "final_video/video_frame_%05d.ppm%c", frame_id, '\0');
            printf("intro %s\n", filename);
            dump_file(filename);
        }
    }

    frame_id = 0;
    while(true) {
        done = true;
        sprintf(filename, "final_video/video_frame_%05d.ppm%c", frame_id+900, '\0');
        printf("%s\n", filename);

        sprintf(filename, "video_frames/0_%05d.ppm%c", frame_id, '\0');
        in = fopen(filename, "rb");
        printf("  %s", filename);
        if (in) {
            if(place(in, 540, 540, 67, 173)) {
                printf(" okay\n");
                done = false;
            } else {
                printf(" broken\n");
            }
        } else {
            printf(" missing\n");
        }
        fclose(in);

        sprintf(filename, "video_frames/1_%05d.ppm%c", frame_id, '\0');
        in = fopen(filename, "rb");
        printf("  %s", filename);
        if (in) {
            if(place(in, 540, 540, 673, 173)) {
                printf(" okay\n");
            } else {
                printf(" broken\n");
            }
        } else {
            printf(" missing\n");
        }
        fclose(in);

        sprintf(filename, "waveforms/0_%05d.ppm%c", frame_id, '\0');
        in = fopen(filename, "rb");
        printf("  %s", filename);
        if (in) {
            if(place(in, 610, 160, 20, 6)) {
                printf(" okay\n");
                done = false;
            } else {
                printf(" broken\n");
            }
        } else {
            printf(" missing\n");
        }
        fclose(in);

        sprintf(filename, "waveforms/1_%05d.ppm%c", frame_id, '\0');
        in = fopen(filename, "rb");
        printf("  %s", filename);
        if (in) {
            if(place(in, 610, 160, 650, 6)) {
                printf(" okay\n");
                done = false;
            } else {
                printf(" broken\n");
            }
        } else {
            printf(" missing\n");
        }
        fclose(in);

        if (done)
            break;

        sprintf(filename, "final_video/video_frame_%05d.ppm%c", frame_id+900, '\0');
        dump_file(filename);
        frame_id++;
    }
    fade_black(background_fadeout_frame, frame_id+900, 90, -1);
}
