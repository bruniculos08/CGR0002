#include <GL/glut.h>
#include <math.h>
#define PI 3.14159265358979323846
static double rotacaoBraco = 0;
static double rotacaoCotovelo = 0;
static double rotacaoCoxa = 0;
static double rotacaoPerna = 0;
// Rotation
static GLfloat yRot = 0.0f;
static GLfloat xRot = 0.0f;
static GLfloat zRot = 0.0f;
static GLfloat zoom = -5.0;
// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(int w, int h)
{
    GLfloat fAspect;

    // Prevent a divide by zero
    if (h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    fAspect = (GLfloat)w / (GLfloat)h;

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Produce the perspective projection
    gluPerspective(35.0, fAspect, 1.0, 40.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// This function does any needed initialization on the rendering context.  Here it sets up and initializes the lighting for the scene.
void SetupRC()
{

    // Light values and coordinates
    GLfloat whiteLight[] = {0.05, 0.05, 0.05, 1.0f};
    GLfloat sourceLight[] = {0.25, 0.25, 0.25, 0.25f};
    GLfloat lightPos[] = {-1.0, 5.0, 5.0, 1.0f};

    glEnable(GL_DEPTH_TEST); // Hidden surface removal
    glFrontFace(GL_CCW);     // Counter clock-wise polygons face out
    glEnable(GL_CULL_FACE);  // Do not calculate inside

    // Enable lighting
    glEnable(GL_LIGHTING);

    // Setup and enable light 0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Black blue background
    glClearColor(0.7, 0.8, 1.0, 0.0);
}

void NormalKeys(unsigned char key, int x, int y)
{
    switch (key)

    {
    case 'z':
        zoom += .3;
        break;
    case 'x':
        zoom -= .3;
        break;
    case 'w':
        xRot -= 5.0f;
        break;
    case 's':
        xRot += 5.0f;
        break;
    case 'a':
        yRot -= 5.0f;
        break;
    case 'd':
        yRot += 5.0f;
        break;
    case 'q':
        zRot += 5.0f;
        break;
    case 'e':
        zRot -= 5.0f;
        break;
    case 'r':
        rotacaoBraco = ((int)rotacaoBraco + 5) % 360;
        glutPostRedisplay();
        break;
    case 't':
        rotacaoBraco = ((int)rotacaoBraco - 5) % 360;
        glutPostRedisplay();
        break;
    case 'f':
        rotacaoCotovelo = ((int)rotacaoCotovelo + 5) % 360;
        glutPostRedisplay();
        break;
    case 'g':
        rotacaoCotovelo = ((int)rotacaoCotovelo - 5) % 360;
        glutPostRedisplay();
        break;
    case 'c':
        rotacaoCoxa = ((int)rotacaoCoxa + 5) % 360;
        glutPostRedisplay();
        break;
    case 'v':
        rotacaoCoxa = ((int)rotacaoCoxa - 5) % 360;
        glutPostRedisplay();
        break;
    case 'b':
        rotacaoPerna = ((int)rotacaoPerna + 5) % 360;
        glutPostRedisplay();
        break;
    case 'n':
        rotacaoPerna = ((int)rotacaoPerna - 5) % 360;
        glutPostRedisplay();
        break;
    }
    yRot = (GLfloat)((const int)yRot % 360);
    xRot = (GLfloat)((const int)xRot % 360);
    zRot = (GLfloat)((const int)zRot % 360);
    glutPostRedisplay();
    // Nao quebra o aspect ratio nem nada :)
    ChangeSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}
void DrawHead()
{
    GLUquadricObj *pObj;
    pObj = gluNewQuadric();
    glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix(); // save transform matrix state
    glTranslatef(0.0f, 1.45f, 0.0f);
    glScalef(.8, 1.05, 1);
    glutSolidCube(0.4);
    glPopMatrix(); // restore transform matrix state
    glPushMatrix();
    glTranslatef(0.0f, 1.77, 0.0f);
    glRotatef(90.0f, 1.0f, .0f, 0.0f);
    gluCylinder(pObj, 0.02f, 0.02f, 0.15f, 26, 13);
    glPopMatrix();
    glPushMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 1.77f, 0.0f);
    gluSphere(pObj, 0.05f, 26, 13);
    glPopMatrix();

    // eyes
    glColor3f(.0f, 1.0f, .3f);
    glPushMatrix();
    glTranslatef(-0.08f, 1.56f, 0.22f);
    gluSphere(pObj, 0.045f, 26, 13);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.08f, 1.56f, 0.22f);
    gluSphere(pObj, 0.045f, 26, 13);
    glPopMatrix();

    // mouth
    glColor3f(.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0, 1.45, 0.2f);
    glScalef(0.4f, 0.08f, 0.02f);
    glutSolidCube(0.5);
    glPopMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.05, 1.41, 0.2f);
    glScalef(0.2f, 0.08f, 0.02f);
    glutSolidCube(0.5);
    glPopMatrix();
    glColor3f(.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-.02, 1.41, 0.2f);
    glScalef(0.08f, 0.08f, 0.02f);
    glutSolidCube(0.5);
    glPopMatrix();
}
void DrawBody()
{
    glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix();
    glTranslatef(0.0, 0.9645, 0.0f);
    glScalef(0.8f, 1.2f, 0.8f);
    glutSolidCube(0.5);
    glPopMatrix();
}
void DrawArms()
{
    GLUquadricObj *ombroDireito, *ombroEsquerdo, *bracoDireito, *bracoEsquerdo, *cotoveloDireito, *cotoveloEsquerdo,*anteEsquerdo,*anteDireito,*maoEsquerda,*maoDireita;
    ombroEsquerdo = gluNewQuadric();
    bracoEsquerdo = gluNewQuadric();
    cotoveloEsquerdo = gluNewQuadric();
    anteEsquerdo = gluNewQuadric();
    maoEsquerda = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0.22f, 1.22f, 0.0f);
    gluSphere(ombroEsquerdo, 0.08f, 26, 13);
    glPushMatrix();
    glTranslatef(0.0, 0, 0.0f);
    glRotatef(90 + rotacaoBraco, 1.0f, 0.0f, 0.0f);
    gluCylinder(bracoEsquerdo, 0.05f, 0.05f, 0.2f, 26, 13);
    glPushMatrix();
    glTranslatef(0.0, .0, 0.2);
    gluSphere(cotoveloEsquerdo, 0.08f, 26, 13);
    glPushMatrix();
    glTranslatef(0,0, 0);
    glRotatef(rotacaoCotovelo-90, 1, 0, 0);
    gluCylinder(anteEsquerdo, 0.05f, 0.05f, 0.28f, 26, 13);
    glPushMatrix();
    glTranslatef(0.0,.0,0.28);
    gluSphere(maoEsquerda, 0.08f, 26, 13);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
	ombroDireito = gluNewQuadric();
    bracoDireito = gluNewQuadric();
    cotoveloDireito = gluNewQuadric();
    anteDireito = gluNewQuadric();
    maoDireita = gluNewQuadric();
    glPushMatrix();
    glTranslatef(-0.22f, 1.22f, 0.0f);
    gluSphere(ombroDireito, 0.08f, 26, 13);
    glPushMatrix();
    glTranslatef(0.0, 0, 0.0f);
    glRotatef(90 + rotacaoBraco, 1.0f, 0.0f, 0.0f);
    gluCylinder(bracoDireito, 0.05f, 0.05f, 0.2f, 26, 13);
    glPushMatrix();
    glTranslatef(0.0, .0, 0.2);
    gluSphere(cotoveloDireito, 0.08f, 26, 13);
    glPushMatrix();
    glTranslatef(0,0, 0);
    glRotatef(rotacaoCotovelo-90, 1, 0, 0);
    gluCylinder(anteDireito, 0.05f, 0.05f, 0.28f, 26, 13);
    glPushMatrix();
    glTranslatef(0.0,.0,0.28);
    gluSphere(maoDireita, 0.08f, 26, 13);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}
void DrawLegs()
{
    GLUquadricObj *coxaEsquerda, *coxaDireita, *cusetEsquerdo, *cusetDireito, *pernaEsquerda, *pernaDireita, *calcanharDireito, *calcanharEsquerdo, *joelhoEsquerdo, *joelhoDireito, *peDireito, *peEsquerdo;
    GLUquadricObj *tampaoDireito, *tampaoEsquerdo;
    coxaEsquerda = gluNewQuadric();
    cusetEsquerdo = gluNewQuadric();
    pernaEsquerda = gluNewQuadric();
    calcanharEsquerdo = gluNewQuadric();
    joelhoEsquerdo = gluNewQuadric();
    peEsquerdo = gluNewQuadric();
    tampaoEsquerdo = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0.15f, 0.63f, 0.0f);
    gluSphere(cusetEsquerdo, 0.08f, 26, 13);
    glPushMatrix();
    glRotatef(90.0 + rotacaoCoxa, 1.0f, 0.0f, 0.0f);
    gluCylinder(coxaEsquerda, 0.05f, 0.05f, 0.28f, 26, 13);
    glPushMatrix();
    glTranslatef(0.0,0.0,0.28);
    gluSphere(joelhoEsquerdo, 0.08f, 26, 13);
    glPushMatrix();
    glRotatef(rotacaoPerna, 1.0f, 0.0f, 0.0f);
    gluCylinder(pernaEsquerda, 0.05f, 0.05f, 0.28f, 26, 13);
    glPushMatrix();
    glTranslatef(0.0,0.0,0.28);
    gluSphere(calcanharEsquerdo, 0.05f, 26, 13);
    glPushMatrix();
    glRotatef(-90,1.0,0.0,0.0);
    gluCylinder(peEsquerdo, 0.05f, 0.03f, 0.2f, 26, 13);
    glPushMatrix();
    glTranslatef(0.0,0.0,.2);
    gluSphere(tampaoEsquerdo, 0.03f, 26, 13);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    
    coxaDireita = gluNewQuadric();
    cusetDireito = gluNewQuadric();
    pernaDireita = gluNewQuadric();
    calcanharDireito = gluNewQuadric();
    joelhoDireito = gluNewQuadric();
    peDireito = gluNewQuadric();
    tampaoDireito = gluNewQuadric();
    glPushMatrix();
    glTranslatef(-0.15f, 0.63f, 0.0f);
    gluSphere(cusetDireito, 0.08f, 26, 13);
    glPushMatrix();
    glRotatef(90.0 + rotacaoCoxa, 1.0f, 0.0f, 0.0f);
    gluCylinder(coxaDireita, 0.05f, 0.05f, 0.28f, 26, 13);
    glPushMatrix();
    glTranslatef(0.0,0.0,0.28);
    gluSphere(joelhoDireito, 0.08f, 26, 13);
    glPushMatrix();
    glRotatef(rotacaoPerna, 1.0f, 0.0f, 0.0f);
    gluCylinder(pernaDireita, 0.05f, 0.05f, 0.28f, 26, 13);
    glPushMatrix();
    glTranslatef(0.0,0.0,0.28);
    gluSphere(calcanharDireito, 0.05f, 26, 13);
    glPushMatrix();
    glRotatef(-90,1.0,0.0,0.0);
    gluCylinder(peDireito, 0.05f, 0.03f, 0.2f, 26, 13);
    glPushMatrix();
    glTranslatef(0.0,0.0,.2);
    gluSphere(tampaoDireito, 0.03f, 26, 13);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    
}
// Called to draw scene
void RenderScene(void)
{

    GLUquadricObj *pObj; // Quadric Object

    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Save the matrix state and do the rotations
    glPushMatrix();

    // Move object back and do in place rotation
    glTranslatef(0.0, -1.0, zoom);
    glRotatef(yRot, 0.0, 1.0, 0.0);
    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(zRot, 0.0, 0.0, 1.0);

    // Draw something
    pObj = gluNewQuadric();
    gluQuadricNormals(pObj, GLU_SMOOTH);

    DrawHead();
    DrawBody();
    DrawArms();
    DrawLegs();

    // Restore the matrix state
    glPopMatrix();

    // Buffer swap
    glutSwapBuffers();
}

int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("RoboArticulado");
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(NormalKeys);
    glutDisplayFunc(RenderScene);
    SetupRC();
    glutMainLoop();

    return 0;
}