#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
char *textFileRead(char *fn)
{
    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fn != NULL)
    {
        fp = fopen(fn,"rt");

        if (fp != NULL)
        {

            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);
            if (count > 0)
            {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

#define printOpenGLError() printOglError(__FILE__, __LINE__)
int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

void printInfoLog(GLhandleARB obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

void setShaders()
{
    static int inited = 0;
    char *vs = NULL,*fs = NULL;
    GLhandleARB v,f,f2,p;
    if (! inited)
    {

        v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
        f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
        f2 = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    }

    vs = textFileRead("myshader.vert");
    fs = textFileRead("myshader.frag");

    const char * vv = vs;
    const char * ff = fs;

    glShaderSourceARB(v, 1, &vv,NULL);
    glShaderSourceARB(f, 1, &ff,NULL);

    free(vs);
    free(fs);

    glCompileShaderARB(v);
    glCompileShaderARB(f);

    printInfoLog(v);
    printInfoLog(f);
    printInfoLog(f2);

    if (! inited)
    {
        p = glCreateProgramObjectARB();
    }
    glAttachObjectARB(p,v);
    glAttachObjectARB(p,f);

    glLinkProgramARB(p);
    printInfoLog(p);

    glUseProgramObjectARB(p);
}
