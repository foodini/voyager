# voyager
Uglier things have been spotted on github, but not by reliable witnesses.

I make no apologies for this disaster. Design was not a priority, as every line 
of code written was an experiment. Unless I decide to tackle the jitter or
color offset problems, I don't expect to go back to this thing - ever again.

The point of the project was to see if it could be done. I figure, if I can get
this far, The Kerbals will get at least this far, probably much further.

The LeftChannel.raw and RightChannel.raw can be found at:
https://drive.google.com/drive/folders/0B0Swx_1rwA6XcFFLc29ncFJSZmM

If you're interested in the attack on this problem:
https://boingboing.net/2017/09/05/how-to-decode-the-images-on-th.html

* voyager.cpp processes LeftChannel.raw and RightChannel.raw into output images.
* waveforms.cpp draws LeftChannel.raw and RightChannel.raw waveform images.
* composite.cpp creates the final frames of the video from previous products.
* ffmpeg.sh requires that you have ffmpeg installed and creates the video.
