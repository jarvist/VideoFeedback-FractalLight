/* (gl) Video Feedback by Jarvist Moore Frost
   This code written 2004-2010.
   GLUT skeleton found at http://stackoverflow.com/questions/503816/linux-fastest-way-to-draw
*/

#include <GL/glut.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#define X_RES 800
#define Y_RES 800

#define BACKGROUND 127

/*
You'll need something like this to get it all working:
Install GLUT development libraries, on Ubuntu/Deb something like this should work:
#sudo apt-get install libglut3-dev

#make a starter picture with something like:
# convert FOO.jpg -resize 800x800 output.pnm 

Compile with something like:
gcc -o glVF -lglut glVF.c  -funroll-all-loops -funsafe-math-optimizations -O999 -finline
In 2014 on Debian this becomes:
gcc -o glVF glVF.c  -funroll-all-loops -funsafe-math-optimizations -O999 -finline -lm -lGL -lglut
*/

unsigned char segfault_buffer1[X_RES][Y_RES][3];
unsigned char curpic[X_RES][Y_RES][3];     
unsigned char segfault_buffer2[X_RES][Y_RES][3]; //I should be arrested for this :) 
						// BUT IT MAKES IT RUN FAST SO I DON'T CARE (Skips bounds checking vo fn)     
unsigned char newpic[X_RES][Y_RES][3];

float MAG=1.05;
int X_OFF=0; //offset of newcenter in pixels
int Y_OFF=0;
int jitter=2;
float TWIST=3.14/4;
float PIXW=3.2;

int fcount=0;

float vo(int x, int y,int colour) //value of a particular pixel; with bounds checking
{
   if (x<0||x>=X_RES) return(BACKGROUND);
   if (y<0||y>=Y_RES) return(BACKGROUND);

   return ((float)curpic[x][y][colour]);
}

void inputpic(char * filename)
{
  int x,y,colour;
   unsigned char tmp;
   int pmax;

   FILE * f;
   f=fopen(filename,"r");

   fscanf(f,"P6\n800 800\n255\n"); //.pgm filetype

  fread(&curpic[0][0][0],1,X_RES*Y_RES*3,f);

   fclose(f);
}

void outputpic(char * filename)
{
  int x,y,colour;
   unsigned char tmp;
   int pmax;

   FILE * f;
   f=fopen(filename,"w");

   fprintf(f,"P6\n800 800\n255\n"); //.pgm filetype

   fwrite(&curpic[0][0][0],1,X_RES*Y_RES*3,f);

   fclose(f);
}


swap()
{
  int x,y,colour;

   for (x=0;x<X_RES;x++)
     for(y=0;y<Y_RES;y++)
        for (colour=0;colour<3;colour++)        // NB: Use memcopy!
          curpic[x][y][colour]=newpic[x][y][colour];
}

zoom()
{
 int x,y,pcount,colour;
   float nx,ny,np,dx,dy,ddx=0.0,ddy=0.0;
   float C,S; //holds Cos and Sin of Twist for the whole Fn
        float colouroffset;

   C=cos(TWIST);
   S=sin(TWIST);

   colouroffset=(float)colour*(float)PIXW/3.0;     //make the r,g,b pixels on detector on a line along x
   ddx=-PIXW/2+colouroffset; //sampling appears pointless - surpresses fractal character?

   for (x=0;x<X_RES;x++)
     for(y=0;y<Y_RES;y++)
       {
        for (colour=0;colour<3;colour++)
        {
        pcount=0; np=0.0;


//      for     (ddx=-PIXW/2+colouroffset;ddx<-PIXW/2+colouroffset+PIXW/3.0;ddx+=0.1)    //discretisation scan across the pixel to sample
            //for (ddy=-PIXW/2;ddy<PIXW/2;ddy+=0.3)
//          {
//              pcount+=1; //used to conserve brightness / set sensitivity of pixel
                dx=(float)(X_OFF+x-(X_RES/2)) + ddx;
                dy=(float)(Y_OFF+y-(Y_RES/2)) + ddy;    //set dx and dy relative to origin in centre of frame
                dx/=MAG; dy/=MAG;
                nx=(float)X_RES/2 + dx*C + dy*S     ;   //matrix multiplication on combined Scale Trace matrix (MAG) and standard rotation matrix
                ny=(float)Y_RES/2 + -dx*S    + dy*C ;
		np+=curpic[(int)(nx+0.5)][(int)(ny+0.5)][colour]; // bounds checking only slows you down
        //        np+=vo((int)(nx+0.5),(int)(ny+0.5),colour);
//          }
//      np/=(float)pcount;
               // np=127.0+(1.0*(np-127.0));
//                if (abs(x-X_RES/2)<5 && abs(y-Y_RES/2)<5 && jitter>0) np+=rand()%(2*jitter)-jitter; //otherwise hits floating point exp with jitter=0 when it tries %0
//          if (np>255.0) np=255.0; if (np<0.0) np=0.0;
          newpic[x][y][colour]=(unsigned char)np;
        }
    }

//ADD JITTER
  if (jitter>0){
   for (x=X_RES/2-5;x<X_RES/2+5;x++)
     for(y=Y_RES/2-5;y<Y_RES/2+5;y++)
	for (colour=0;colour<3;colour++)
	    newpic[x][y][colour]+=(unsigned char)(rand()%(2*jitter)-jitter);

}

  swap();
}


//original on: http://stackoverflow.com/questions/503816/linux-fastest-way-to-draw
void renderScene() {    
    char filename[50];
    // limit FPS
	glutTimerFunc(100, renderScene, 0);

    // render the texture here
    fcount++;
    jitter=8; //catch all to set jitter low (enough to get a few gradients in colours)
    if (fcount%17==0) { if(rand()%5==0) jitter=60; } //occ, on a different period to the usual changes, add a burst of colour
    if (fcount%51==0) //randomise the settings slightly
    {
	if (rand()%10==0) { MAG=1+(0.1*(float)rand()/RAND_MAX)-0.08; inputpic("output.pnm"); }//refresh picture occasionally
	MAG+=(0.1*(float)rand()/RAND_MAX)-0.05;
	TWIST+=(0.1*(float)rand()/RAND_MAX)-0.05;
	if (MAG<0.95) MAG=1.4; //stop shrinking away for ever
	if (rand()%2==0) TWIST=0.0-TWIST; //start rotating other way
	if (rand()%10==0) TWIST=(1.0*(float)rand()/RAND_MAX)-0.5; //head spinning too much!
    }
    zoom();

    // save picture frame for posterity
    sprintf(filename,"VF_%08d.ppm",fcount);
    outputpic(filename);
    //outputpic("sausages.png");

    glEnable (GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D (
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        X_RES,
        Y_RES,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        &curpic[0][0][0]
    );

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0, -1.0);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0, -1.0);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0,  1.0);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0,  1.0);
    glEnd();

    glFlush();
    glutSwapBuffers();


}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(X_RES,Y_RES);
    glutCreateWindow("VIDEO FRACTALS");

    glutDisplayFunc(renderScene);

    glutTimerFunc(100, renderScene, 0);
	// max FPS
    //glutIdleFunc(renderScene);

    inputpic("output.pnm");

    glutMainLoop();

    return 0;
}
