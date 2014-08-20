VideoFeedback-FractalLight
==========================

Stash for code written during my MSci (05-06) and since, on Video Feedback and Fractal Light

# What it does

Using GL for high performance screen rendering, it takes an image & does
a lossy ROTATE + MAGNIFY operation on successive frames. This simulates video
feedback; the resulting images are often fractal in nature.

The code randomly changes the ROTATE and MAGNIFY parameters, and probably some
others as well (certainly a JITTER parameter which generates colour noise at
the core of the image).

It's pretty psychedlic, and you get to choose the image used as the (repeated)
starting configuration.

![Screenshot](/screenshot.png)

# Youtube Demo

http://youtu.be/9VXTFCwtSz0

# How to get it working

You'll need something like this to get it all working:
Install GLUT development libraries, on Ubuntu/Deb something like this should work:
`#sudo apt-get install libglut3-dev`

#make a starter picture with something like:
`# convert FOO.jpg -resize 800x800 output.pnm`

Compile with something like:
`gcc -o glVF -lglut glVF.c  -funroll-all-loops -funsafe-math-optimizations -O999 -finline`
In 2014 on Debian this becomes:
`gcc -o glVF glVF.c  -funroll-all-loops -funsafe-math-optimizations -O999 -finline -lm -lGL -lglut`
