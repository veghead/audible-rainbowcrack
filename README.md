# Audible Crack Tool
## Overview

An open source tool for cracking audible activation_bytes using rainbow tables.
It is designed to be built and run on u.&ast;x machines including Linux and MacOS.
This repository also contains the rainbow tables required.

With this tool you may use `ffmpeg` to convert AAX files to other formats so 
that you can play your Audible books on unsupported devices.

I did not write this tool, I just cobbled it together with 
code from these places:

- https://github.com/BlackArch/rainbowcrack.git
- https://github.com/inAudible-NG/tables

All of which is based on the original code from RainbowCrack:

- https://project-rainbowcrack.com/

## Building

Type `make`. If it doesn't build then try to make it build.

## Running

Run `ffprobe` (part of the `ffmpeg` tools) against your Audible `aax` file. One of the lines of output will
include a hash - it'll look something like:

```
ffprobe  ~/Desktop/1984_George_Orwell.aax
ffprobe version 4.1.4 Copyright (c) 2007-2019 the FFmpeg developers
  built with Apple LLVM version 10.0.0 (clang-1000.11.45.5)
  configuration: --prefix=/usr/local/Cellar/ffmpeg/4.1.4_1 --enable-shared --enable-pthreads --enable-version3 --enable-avresample --cc=clang --host-cflags='-I/Library/Java/JavaVirtualMachines/adoptopenjdk-12.0.1.jdk/Contents/Home/include -I/Library/Java/JavaVirtualMachines/adoptopenjdk-12.0.1.jdk/Contents/Home/include/darwin' --host-ldflags= --enable-ffplay --enable-gnutls --enable-gpl --enable-libaom --enable-libbluray --enable-libmp3lame --enable-libopus --enable-librubberband --enable-libsnappy --enable-libtesseract --enable-libtheora --enable-libvorbis --enable-libvpx --enable-libx264 --enable-libx265 --enable-libxvid --enable-lzma --enable-libfontconfig --enable-libfreetype --enable-frei0r --enable-libass --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libopenjpeg --enable-librtmp --enable-libspeex --enable-videotoolbox --disable-libjack --disable-indev=jack --enable-libaom --enable-libsoxr
  libavutil      56. 22.100 / 56. 22.100
  libavcodec     58. 35.100 / 58. 35.100
  libavformat    58. 20.100 / 58. 20.100
  libavdevice    58.  5.100 / 58.  5.100
  libavfilter     7. 40.101 /  7. 40.101
  libavresample   4.  0.  0 /  4.  0.  0
  libswscale      5.  3.100 /  5.  3.100
  libswresample   3.  3.100 /  3.  3.100
  libpostproc    55.  3.100 / 55.  3.100
[mov,mp4,m4a,3gp,3g2,mj2 @ 0x7fe44480a000] [aax] file checksum == b8f54c7aa84328b6d90fc3411d8389edf78f2550
```
That "file checksum" (`b8f54c7aa84328b6d90fc3411d8389edf78f2550` in this case) is the hash you need.

Next run audible-crack by supplied the path to the rainbow tables and the hash. E.g.

```
./audible-crack ./audible-tables/*.rt b8f54c7aa84328b6d90fc3411d8389edf78f2550
```

If all goes well you'll see something that looks like this:

```
result
-------------------------------------------------------
b8f54c7aa84328b6d90fc3411d8389edf78f2550  Rd6_  hex:763b4faa
```

The hex numbers (`763b4faa` in this case) are your `activation_bytes`. You can use them to convert the media.
E.g. with ffmpeg:

```
ffmpeg -activation_bytes 763b4faa -i ~/Desktop/1984_George_Orwell.aax 1984_George_Orwell.mp3
```
