// Comanda para executar: gcc lava.c -lglut -lGL -lGLU -lm -o lava && ./lava
#include "GL/glut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
# define M_PI 3.14159265358979323846


void createLittleWall(GLfloat x, GLfloat y, GLfloat z, GLfloat theta, GLfloat l, GLfloat height, 
    GLfloat depth);
void generateParticle();
double rad(GLfloat theta);
double rad(GLfloat theta){
    return ((double) fmod(theta, 360))*M_PI/180;
}


// Variáveis que iremos utilizar para rotação:
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 0.0f;

typedef struct Particle{
    GLfloat x, y, z, v_x, v_y, v_z;
    GLfloat a, b, c, p, h, t0;
    unsigned int lifetime;
} particle;

#define NUM_OF_PARTICLES 80000
#define LIFE_TIME 100

void ChangeSize(int w, int h){
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

void SpecialKeys(int key, int x, int y){
    // (1) O seguinte corpo da função é auto-explicativo:
    if (key == GLUT_KEY_LEFT)
        yRot -= 5.0f;
    if (key == GLUT_KEY_RIGHT)
        yRot += 5.0f;
    if (key == GLUT_KEY_UP)
        xRot += 5.0f;
    if (key == GLUT_KEY_DOWN)
        xRot += -5.0f;

    yRot = (GLfloat)((const int)yRot % 360);
    xRot = (GLfloat)((const int)xRot % 360);

    // (2) Marca a janela atual como precisando ser re-renderizada, para que na seguinte iteração da função...
    // ... glutMainLoop isto ocorra:
    glutPostRedisplay();
}

void SetupRC(){

    // (1) Este vetor define a intensidade RGBA da luz do ambiente:
    GLfloat whiteLight[] = {0.05f, 0.05f, 0.05f, 1.0f};
    // (2) Este vetor define a intensidade RGBA da luz de difusão:
    GLfloat sourceLight[] = {0.25f, 0.25f, 0.25f, 1.0f};
    // (3) Este vetor define as coordenadas na fonte de luz em coordenadas homogêneas (por isto com 4 coordenadas):
    GLfloat lightPos[] = {-10.f, 5.0f, 5.0f, 1.0f};

    // (4) Esta função ativa o buffer de teste de profundidade, que no momento da renderização verifica cada...
    // ... item a se renderizado, vendo se não há outro item (superfície) com menor profundidade (na frente):
    glEnable(GL_DEPTH_TEST);

    // (5) Esta função define de qual lado cada polígo será renderizado, de modo que estando os vértices...
    // ... ordenados no sentido horário ou anti-horário, dependendo se a direção sobre a qual a luz pode chegar...
    // ... está no sentido escolhido, a face será renderizada ou não:
    glFrontFace(GL_CCW);

    // (6) Esta função habilidade a não renderização das que estejam com os vértices sendo "visto" no sentido...
    // ... contrário ao escolhido:
    glEnable(GL_CULL_FACE);

    // (7) Esta função habilita a luz:
    glEnable(GL_LIGHTING);

    // (8) Esta função define os parâmetros do modelo de iluminação:
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
    // Nesse caso o  1º parâmetro indica que o 2º contém quatro valores de ponto flutuante que especificam a...
    // ... intensidade do RGBA ambiente de toda a cena.

    // (9) Esta função define parâmetros para uma determinada luz (fonte de luz):
    glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
    // Nesse caso a fonte de luz é a 0 (GL_LIGHT0), e o 2º parâmetro indica que o 3º contém quatro valores de...
    // ... ponto flutuante que especificam a intensidade do RGBA ambiente da luz.

    // (10) Esta função define parâmetros para uma determinada luz (fonte de luz):
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
    // Nesse caso a fonte de luz é a 0 (GL_LIGHT0), e o 2º parâmetro indica que o 3º contém quatro valores de...
    // ... ponto flutuante quatro valores que especificam a intensidade difusa do RGBA da luz.

    // (11) Esta função define parâmetros para uma determinada luz (fonte de luz):
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    // Nesse caso a fonte de luz é a 0 (GL_LIGHT0), e o 2º parâmetro indica que o 3º contém quatro valores de...
    // ... ponto flutuante quatro valores que especificam a posição da luz em coordenadas de objeto homogêneas.

    // (12) Esta função habilita a luz da fonte 0:
    glEnable(GL_LIGHT0);

    // (13) Esta função habilita para cada material ou materiais especificados por face, o parâmetro de...
    // ... material ou os parâmetros especificados pelo modo acompanham a cor atual o tempo todo.
    glEnable(GL_COLOR_MATERIAL);

    // (14) Define as propriedades que haverão junto a cor atual de cada objeto:
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // (15) Define a cor do fundo do espaço:
    glClearColor(0.25f, 0.25f, 0.50f, 1.0f);
  
}

particle lava[NUM_OF_PARTICLES];
GLfloat init_x = 0, init_y = 0, init_z = 0, t_delta = 0.01;
GLfloat max_p = 0.05, max_h = 1, max_coeff = 0.5, ground = -0.5;

// Iremos considerar que todas as particulas terão roda dada por um vetor da forma do...
// ... vetor paramétrico s = (at, -b(t-p)² + h, ct) assim teremos uma rota a qual podemos alterar facilmente...
// ... por meio das constantes a, b, c e h, em que a e c determinam a direção no plano xz, b o formato...
// ... da parábola percorrida, h a altura da parábola e p o pico da mesma.

// Usaremos como velocidade a derivada do vetor acima, aplicada ao ponto em que o objeto se encontra.

// Obs.: a ideia acima era boa porém também podemos fazer isto de maneira mais simples com vetores simples...
// ... decrementando a coordenada y (para que o objeto caia).

GLfloat maxf(GLfloat a, GLfloat b){
    if(a >= b) return a;
    else return b;
}

void parametricEq(particle *item){
    // A equação da trajetória é s(t) = (at, -b(t-p)² + h, ct), então ds/dt = (a, -2b(t-p), c*t), e portanto...
    // ... faremos t variar em 0.1 por segundo, e note que ao somarmos a taxa de variação as coordenadas teremos...
    // ... o vetor tangente (é o que estaremos "simulando"): v(t) = s(t0) + ds(t0)/dt * (t1-t0) 
    GLfloat t0 = (*item).t0, a = (*item).a, b = (*item).b, c = (*item).c, p = (*item).p, h = (*item).h;
    (*item).v_x = a*(t_delta);
    (*item).v_y = - 2*b*(t0-p)*(t_delta);
    (*item).v_z = c*(t_delta);
    (*item).t0 += t_delta;
    printf("a = %lf\n", (*item).a);
    printf("t_delta = %lf\n", t_delta);
    // printf("vx = %d\n", item->v_x);
}

void generateParticle(){

    for(int i = 0; i < NUM_OF_PARTICLES; i++){
        lava[i].x = init_x;
        lava[i].z = init_z;
        lava[i].y = init_y;

        lava[i].t0 = 0;
        lava[i].a = fmod((float) rand(), max_coeff);
        lava[i].b = fmod((float) rand(), max_coeff);
        lava[i].c = fmod((float) rand(), max_coeff);
        lava[i].p = fmod((float) rand(), max_coeff);
        lava[i].h = fmod((float) rand(), max_coeff);

        lava[i].lifetime = LIFE_TIME;   
   }
}

void regenerateParticle(int i){

    lava[i].x = init_x;
    lava[i].z = init_z;
    lava[i].y = init_y;
    
    lava[i].t0 = 0;
    lava[i].a = fmod((float) rand(), max_coeff);
    lava[i].b = fmod((float) rand(), max_coeff);
    lava[i].c = fmod((float) rand(), max_coeff);
    lava[i].p = fmod((float) rand(), max_coeff);
    lava[i].h = fmod((float) rand(), max_coeff);

    lava[i].lifetime = LIFE_TIME;      
}

void moveParticles(){
    glPointSize(5.0);
    glBegin(GL_POINTS);

        for(int i = 0; i < NUM_OF_PARTICLES; i++){

            if(lava[i].y > ground){
                parametricEq(&lava[i]);
                lava[i].x += lava[i].v_x; 
                lava[i].y += lava[i].v_y; 
                lava[i].z += lava[i].v_z; 
                // lava[i].x += 0.01; 
                // lava[i].y += -0.01; 
                // lava[i].z += 0.01; 
            }
            else{
                lava[i].v_x = 0; 
                lava[i].v_y = 0; 
                lava[i].v_z = 0;
                lava[i].y = ground;
                // lava[i].lifetime--;
            }

            // if(lava[i].lifetime <= 0) regenerateParticle(i);
            GLfloat dist_center = sqrt(powf(lava[i].x, 2) + powf(lava[i].z, 2));
            glColor3f(1,   0.1 + fmod(5*dist_center, 1.1), 0.1);
            glVertex3f(lava[i].x, lava[i].y, lava[i].z);
        }
    glEnd();
}


void RenderScene(void)
{
    GLUquadricObj *Ball;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();

    glTranslatef(0, 0, -6);
    
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);

    // lembrar vetor unitário (não lembro por que escrevi isso, mas tem haver com algo que foi dito na aula)
    // Aqui iremos colocar e/ou gerar as particulas:
    glPushMatrix();
    
        moveParticles();

    glPopMatrix();

    glPushMatrix();

        Ball = gluNewQuadric();

        glColor3f(0,0,0);

        glTranslatef(0, init_y, 0);

        glTranslatef(0, -0.5, 0);

        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        gluCylinder(Ball, 0.1, 0.025, 0.5, 26, 13);

    glPopMatrix();
    
    glPopMatrix();

    glutSwapBuffers();

    usleep(20000);
}

void createLittleWall(GLfloat x, GLfloat y, GLfloat z, GLfloat theta, GLfloat l, GLfloat height, 
    GLfloat depth){
    
    glPushMatrix();

    glTranslatef(x, y, z);

    glRotatef(theta, 0.0f, 1.0f, 0.0f);

    glScalef(1.0f, height/l, depth/l);

    glutSolidCube(l);

    glPopMatrix();

    return;
}

int main(int argc, char *argv[])
{
    // (1) Essa função inicial serve para que o OpenGL exercute os argumentos passados (argv) na hora da...
    // ... execução:
    glutInit(&argc, argv);

    // (2) Essa função define qual o modelo de janela a ser criado pelo OpenGL (note que são os bits do...
    // resultado da expressão que irão dar todas as definições para a janela):
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // (3) Essa função define as dimensões da janela a ser gerada pelo OpenGL:
    glutInitWindowSize(800, 600);

    // (4) Essa função cria a janela com o nome passado como argumento:
    glutCreateWindow("First OpenGL Code");

    // (5) Essa função recebe como paramêtro o endereço de uma função ("ChangeSize" nesse caso) que lida...
    // ... com o reajuste do tamanho da janela, assim sempre que a janela for reajustada tal função passada...
    // ... como argumento irá ser executada:
    glutReshapeFunc(ChangeSize);

    // (6) Essa função recebe como paramêtro o endereço de uma função ("Spec ialKeys" nesse caso) que lida...
    // ... com certas teclas assim sempre que tais teclas forem pressionadas tal função passada como argumento...
    // ... irá ser executada:
    glutSpecialFunc(SpecialKeys);

    // (7) Essa função se encarrega de colocar os objetos no espaço (desenhá-los) e fazer as operações de...
    // ... de translação, rotação e etc (manipulando a matriz de modelo):
    glutDisplayFunc(&RenderScene);

    glutIdleFunc(&RenderScene);
    // (8) Esta função define os parâmetros de iluminação e a cor do fundo do espaço:
    SetupRC();
    srand(time(NULL));
    generateParticle();

    glutMainLoop();
    return 0;
}
