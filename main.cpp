/* Rotating cube with color interpolation */

/* Demonstration of use of homogeneous coordinate
transformations and simple data structure for representing
cube from Chapter 4 */

/*Both normals and colors are assigned to the vertices */
/*Cube is centered at origin so (unnormalized) normals
are the same as the vertex values */

#define GLEW_STATIC
#include <Windows.h>    // for solving the Code::Blocks errors

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <vector>
#include "glm.c"
#include "gencube.h"
#include "shader.h"

static GLfloat theta =180.0;
static GLfloat eyeX=0,eyeY=0,eyeZ=50;

static GLMmodel *pipes[2]= {0};
static int count=0;
static int delay=0;
static std::vector<std::pair<int,std::vector<int>>> list;

const static int rotate[16][3]=
{
    {0,0,0}, //no use
    {90,0,0},{90,0,90},{90,0,180},{0,0,90}, //+y
    {180,0,0},{180,0,180},{0,0,180},{0,0,0}, //horizon
    {270,0,0},{270,0,270},{270,0,180},{0,0,270}, //-y
    {0,0,0},{0,0,90},{0,90,0} //straight
};

static int colorMode=0;
static const int allColorMode=4;
static std::vector<std::vector<GLfloat>> colorList;

void randomColor()
{
    GLfloat color[]=
    {
        static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
        static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
        static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
    };
    for(int i=0; i<2; i++)
    {
        GLMmodel *obj=pipes[i];
        GLMgroup *group=obj->groups;
        GLMmaterial *material=&obj->materials[group->material];
        for(int k=0; k<3; k++)
        {
            obj->materials[obj->groups->material].diffuse[k]=color[k];
        }
    }
}
void ChangeColor(int idx)
{
    std::vector<GLfloat> &color=colorList[idx];
    for(int i=0; i<2; i++)
    {
        GLMmodel *obj=pipes[i];
        GLMgroup *group=obj->groups;
        GLMmaterial *material=&obj->materials[group->material];
        for(int k=0; k<3; k++)
        {
            obj->materials[obj->groups->material].diffuse[k]=color[k];
        }
    }
}
void genColor()
{
    colorList.clear();
    GLfloat ca[3]=
    {
        static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
        static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
        static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
    },cb[3]=
    {
        static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
        static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
        static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
    };
    int size=list.size();
    for(int i=0; i<size; i++)
    {
        std::vector<GLfloat> cc(3);
        for(int j=0; j<3; j++)
        {
            cc[j]=ca[j]+(cb[j]-ca[j])*i/size;
        }
        colorList.push_back(cc);
    }
}
void drawOBJ()
{
    for(int j=0; j<count; j++)
    {
        glPushMatrix();
        glLoadIdentity();

        int tube=list[j].first;
        GLMmodel* myObj;
        if(tube<13&&tube>0)
        {
            myObj=pipes[/*curColor*2*/0];
        }
        else if(tube>=13&&tube<=15)
        {
            myObj=pipes[/*curColor*2+*/1];
        }
        else
        {
            exit(2);
        }

        int *position=&list[j].second[0];
        glTranslatef(position[0]*2,position[1]*2,position[2]*2);
        glRotatef(rotate[tube][0],1,0,0);
        glRotatef(rotate[tube][1],0,1,0);
        glRotatef(rotate[tube][2],0,0,1);

        GLMgroup* group = myObj->groups;
        if(colorMode==1)
        {
            randomColor();
        }
        if(colorMode==3)
        {
            ChangeColor(j);
        }
        for (int g=0; g<myObj->numgroups; g++)
        {
            GLMmaterial*  material = &myObj->materials[group->material];
            for (int i=0; i<group->numtriangles; i++)
            {
                glBegin(GL_TRIANGLES);
                for (int v=0; v<3; v++)
                {
                    float *p = &myObj->vertices[ myObj->triangles
                                                 [group->triangles[i]].vindices[v]*3 ];
                    glColor3fv( material->diffuse );
                    glNormal3fv( & myObj->normals[ myObj->triangles[i].nindices[v]*3 ] );
                    glVertex3fv( p );
                }
                glEnd();
            }

            group = group->next;
        }
        glPopMatrix();
    }
}

void display(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat Light0_pos[]= {0,0.0,100.0,1.0};
    glLightfv(GL_LIGHT0,GL_POSITION, Light0_pos);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 35, 1.0,10,1000);

    GLfloat degree=theta*M_PI/180.0;
    gluLookAt(eyeX,eyeY,eyeZ,eyeX+sin(degree),eyeY,eyeZ+cos(degree),0.0,1.0,0.0);
    glMatrixMode(GL_MODELVIEW);
    if(count<list.size())
    {
        if(colorMode==2)
        {
            randomColor();
        }
        count++;
    }
    else
    {

        if(delay<30)
        {
            delay++;
        }
        else
        {
            delay=0;
            if(colorMode==0)
            {
                randomColor();
            }
            list=genCube();
            genColor();
            count=0;
        }
    }
    drawOBJ();
    glFlush();
    glutSwapBuffers();
}

void spinCube()
{
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    const GLfloat step=1;
    const GLfloat angle=0.5;
    GLfloat degree=theta*M_PI/180.0;

    static unsigned char lastKey='\0';
    static GLfloat amplify=1;

    //if the same action happenned , amplify the action

    if(lastKey==key)
    {
        amplify+=0.2;
    }
    else
    {
        amplify=1;
    }
    lastKey=key;

    switch(key)
    {
    //exit
    case '~':
    case '`':
        exit(0);
        break;
    case 'r':
    case 'R':
        if(delay==0)
        {
            colorMode++;
            colorMode%=allColorMode;
        }
        break;
    //forward move
    case 'w':
    case 'W':
        eyeX+=step*sin(degree)*amplify;
        eyeZ+=step*cos(degree)*amplify;
        break;
    //backward move
    case 's':
    case 'S':
        eyeX-=step*sin(degree)*amplify;
        eyeZ-=step*cos(degree)*amplify;
        break;
    }
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
                2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
                2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    glutInit(&argc, argv);

    /* need both double buffering and z buffer */

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("PipeGenerator");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutIdleFunc(spinCube);
    glutKeyboardFunc(keyboard);

    glEnable(GL_DEPTH_TEST); /* Enable hidden--surface--removal */

    glewInit();
    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
        printf("Ready for GLSL\n");
    else
    {
        printf("No GLSL support\n");
        exit(1);
    }

    pipes[0]=glmReadOBJ("tubeO1.obj");
    glmUnitize(pipes[0]);

    pipes[1]=glmReadOBJ("tubeO13.obj");
    glmUnitize(pipes[1]);

    randomColor();
    list=genCube();
    genColor();

    setShaders();
    glutMainLoop();
    return 0;
}
