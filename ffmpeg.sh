ffmpeg -v info -y -f image2 -thread_queue_size 32 -r 30 -i ./final_video/video_frame_%05d.ppm -i 384kHzStereo.mp3 -threads 8 -shortest -s:v 1280x720 -r 30 -b:v 6M -preset veryslow voyager_6m.mkv
