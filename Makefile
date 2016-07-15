all: glVF.c
	gcc -o glVF glVF.c -funroll-all-loops -funsafe-math-optimizations -O4-finline -lm -lGL -lglut

mac: glVF.c 
	gcc -o glVF glVF.c -funroll-all-loops -funsafe-math-optimizations -O4 -finline -lm -framework GLUT -framework OpenGL

youtube-render:
	ffmpeg -framerate 10 -i VF_%08d.ppm -s:v 800x800 -c:v libx264 -profile:v high -crf 23 -pix_fmt yuv420p -r 10 VF_youtube.mp4
