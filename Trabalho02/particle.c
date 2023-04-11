// Comanda para executar: gcc particle.c -lglut -lGL -lGLU -lm -o particle && ./particle
#include "GL/glut.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

#define _USE_MATH_DEFINES
#define NUM_OF_PARTICLES 500
# define M_PI 3.14159265358979323846
int window;

void generateParticle();
double rad(GLfloat theta);
GLfloat random_float(GLfloat max_value);
GLfloat random_direction();

// Variáveis que iremos utilizar para rotação:
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 0.0f;

typedef struct Particle{
    GLfloat x, y, v_x, v_y;
    unsigned int lifetime_seconds;
} particle;

GLfloat random_direction(){
    int r = rand()%2;
    if(r == 0) return 1.0f;
    return -1.0f;
}

GLfloat random_float(GLfloat max_value){
    return ((GLfloat)rand()/(GLfloat)RAND_MAX) * max_value;
}

void ChangeSize(int w, int h)
{
    // (1) Essa é a variável que será utlizada para armazer o aspect ratio (razão largura/altura) da...
    // ... tela:
    GLfloat fAspect;

    // Obs.: esta condição evita que se h for passado como um valor muito próximo de 0 (pois ele então...
    // ... seria arrendondado para 0) ocorra divisão por 0:
    if (h == 0)
        h = 1;

    // (2) Esta função serve para ajustar a transformação afim das cordenadas x e y para as cordenadas...
    // ... x e y normalizadas (cada eixo em um intervalo de -1 à 1, ou de 0 à 1):
    glViewport(0, 0, w, h);
    // Os dois primeiros parâmetro indicam respectivamente as coordenadas x e y do canto inferior esquerdo...
    // ... da janela, enquanto w é a largura e h é a altura da mesma.

    fAspect = (GLfloat)w / (GLfloat)h;

    // (3) Esta função especifica qual é a que será alterada pelos comandos de translação, rotação e etc,...
    // ... na pilha:
    glMatrixMode(GL_PROJECTION);
    // Neste caso iremos alterar a matriz de projeção (a última a ser aplicada nos vértices na hora da rende...
    // ...rização e é a a matriz relacionada à câmera e transformação do 3D para o 2D da tela).

    // (4) Esta função troca a matriz atual (na pilha) pela matriz identidade ("reseta"):
    glLoadIdentity();
    // Note que isto é bem importante visto que sem isso as transformações se acomulariam cada vez que a cena...
    // ... é renderizada.

    // (5) Esta função configura a matriz de projeção de perspectiva (joga ela na própria pilha):
    gluPerspective(35.0f, fAspect, 1.0, 40.0);
    // Note que o primeiro argumento é o fovy (campo de ângulo de exibição, em graus, na direção y), o segundo...
    // é o aspect ratio, o terceiro é o zNear (A distância do visualizador até o plano de recorte próximo, sem-
    // ...pre positivo;  é como se fosse a distância m[inima para renderização) e o último é o zFar (A distância...
    // ... do visualizador para o plano de recorte distante, sempre positivo; é como se fosse a distância máxi...
    // ...ma de renderização).

    // (6) Função já descrita antes:
    glMatrixMode(GL_MODELVIEW);
    // Neste caso iremos alterar a matriz de modelo (a primeira a ser aplicada nos vértices na hora da rende...
    // ...rização, ou seja, a matriz que é realmente utilizada para se mexer nos objetos).

    // (7) Função já descrita antes:
    glLoadIdentity();
}

void SpecialKeys(int key, int x, int y)
{
    // (1) O seguinte corpo da função é auto-explicativo:
    if (key == GLUT_KEY_LEFT)
        yRot -= 5.0f;
    if (key == GLUT_KEY_RIGHT)
        yRot += 5.0f;
    if (key == GLUT_KEY_UP)
        xRot += 5.0f;
    if (key == GLUT_KEY_DOWN)
        generateParticle();
        // xRot += -5.0f;
    
    yRot = (GLfloat)((const int)yRot % 360);
    xRot = (GLfloat)((const int)xRot % 360);

    // (2) Marca a janela atual como precisando ser re-renderizada, para que na seguinte iteração da função...
    // ... glutMainLoop isto ocorra:
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
    // if (key == 27){ 
    //     glutDestroyWindow(window); 
    //     exit(0);                   
    // }
}

void InitGL(int Width, int Height)
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
  glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
  glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
  glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();				// Reset The Projection Matrix

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

  glMatrixMode(GL_MODELVIEW);

}

particle followers[NUM_OF_PARTICLES];
particle first_point;
float velocity_module, velocity_random_x, velocity_random_y, first_point_delta = 0.1;
float init_coords_width = 1, init_coords_height= 1, coords_width = 5, coords_height = 4;


void generateParticle(){
    srand(time(NULL));
    first_point.x = random_float(coords_width)*random_direction();
    first_point.y = random_float(coords_height)*random_direction();
    first_point.v_x = random_direction();
    first_point.v_y = random_direction();
    
    for(int i = 0; i < NUM_OF_PARTICLES; i++){
        followers[i].x = random_float(init_coords_width)*random_direction();
        followers[i].y = random_float(init_coords_height)*random_direction();
        followers[i].v_x = 0;
        followers[i].v_y = 0;
    }
}

double dist(particle p1, particle p2){
    double d = sqrt(pow((p1.x - p2.x),2) + pow((p1.y - p2.y),2));
    return d;
}

GLfloat theta = 0;

void RenderScene(void)
{
    srand(time(NULL));
    GLUquadricObj *Ball;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -6.0f);


    glRotatef(xRot, 1, 0, 0);
    glRotatef(yRot, 0, 1, 0);

    glPointSize(3.0);
    glBegin(GL_POINTS);

    glColor3f(1, 1, 1);
    glVertex3f(first_point.x, first_point.y, -6.0f);
    glColor3f(0.2, 0.8, 1); 

    first_point.x = first_point.x + first_point_delta*first_point.v_x;
    first_point.y = first_point.y + first_point_delta*first_point.v_y; 

    if(first_point.x > coords_width || first_point.x < -coords_width){
        first_point.v_x = - (first_point.v_x);
        // first_point.v_y = - first_point.v_y;
    }
    
    if(first_point.y > coords_height || first_point.y < -coords_height){
        // first_point.v_x = - first_point.v_x;
        first_point.v_y = - (first_point.v_y);
    }

    for(int i = 0; i < NUM_OF_PARTICLES; i++){
        if((followers[i].x > coords_width || followers[i].x < -coords_width) || (followers[i].y > coords_height || followers[i].y < -coords_height)){
            
            if(followers[i].x > coords_width || followers[i].x < -coords_width){
                // Por que não está dando para desacelerar as particulas?
                followers[i].v_x = - (followers[i].v_x - random_float(0));
            }
            
            if(followers[i].y > coords_height || followers[i].y < -coords_height){
                followers[i].v_y = - (followers[i].v_y - random_float(0));
            }
        }
        else if(dist(first_point, followers[i]) <= 0.5){
            // vetor de velocidade unitário:
            velocity_module = sqrt(powf((first_point.x - followers[i].x), 2) + powf((first_point.y - followers[i].y), 2));
            velocity_random_x = 0.01;
            velocity_random_y = 0.01;
            followers[i].v_x = -velocity_random_x*(first_point.x - followers[i].x)/velocity_module;
            followers[i].v_y = -velocity_random_y*(first_point.y - followers[i].y)/velocity_module;

        }

        followers[i].x = followers[i].x + 1*followers[i].v_x;
        followers[i].y = followers[i].y + 1*followers[i].v_y;

        glVertex3f(followers[i].x, followers[i].y, -6.0f);
    }
    
    glEnd();
    
    glPopMatrix();

    glutSwapBuffers();
    usleep(20000);

}

double rad(GLfloat theta){
    return ((double) fmod(theta, 360))*M_PI/180;
}

int main(int argc, char *argv[])
{
    // (1) Essa função inicial serve para que o OpenGL exercute os argumentos passados (argv) na hora da...
    // ... execução:
    glutInit(&argc, argv);

    // (2) Essa função define qual o modelo de janela a ser criado pelo OpenGL (note que são os bits do...
    // resultado da expressão que irão dar todas as definições para a janela):
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

    // (3) Essa função define as dimensões da janela a ser gerada pelo OpenGL:
    glutInitWindowSize(800, 600);

    glutInitWindowPosition(0, 0);  

    // (4) Essa função cria a janela com o nome passado como argumento:
    window = glutCreateWindow("First Particle");

    // (5) Essa função recebe como paramêtro o endereço de uma função ("ChangeSize" nesse caso) que lida...
    // ... com o reajuste do tamanho da janela, assim sempre que a janela for reajustada tal função passada...
    // ... como argumento irá ser executada:
    glutReshapeFunc(ChangeSize);

    // (6) Essa função recebe como parâmetro um função que recebe cada entrada do teclado com um char e lida...
    // ... com esta entrada:
    glutKeyboardFunc(keyboard);

    // (7) Essa função recebe como paramêtro o endereço de uma função ("Spec ialKeys" nesse caso) que lida...
    // ... com certas teclas assim sempre que tais teclas forem pressionadas tal função passada como argumento...
    // ... irá ser executada:
    glutSpecialFunc(SpecialKeys);

    // (8) Essa função se encarrega de colocar os objetos no espaço (desenhá-los) e fazer as operações de...
    // ... de translação, rotação e etc (manipulando a matriz de modelo):
    glutDisplayFunc(&RenderScene);

    // (9)
    glutIdleFunc(&RenderScene);

    // (10) Esta função define os parâmetros de iluminação e a cor do fundo do espaço:
    InitGL(640, 480);

    generateParticle();

    glutMainLoop();
    return 0;
}
