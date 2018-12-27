#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <time.h>

/* Dimenzije prozora */
static int window_width, window_height;
#define DNO -1
#define VRH 4
/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);

/*Stanja*/
#define DOLE 0
#define GORE 1
#define STOP 2

/*Pomeraj cekica*/
int cekicX = 2;
int cekicZ = 2;
int cekic_hit = 0;

/*Parametri tajmera*/
#define SPEED 0.9
#define TIMER_INTERVAL 20

/*Opis kugle*/
typedef struct 
{
    float moveY;
    int smer;
}KUGLA;

KUGLA niz_kugli[9];

int tri_kugle[3];

static void on_idle(void)
{
    glutPostRedisplay();
}

/*Odabir tri nasumicne kugle*/
void random_kugle()
{
    for(int i=0; i<9; i++)
    {
        niz_kugli[i].moveY = DNO; //sve na dnu
        niz_kugli[i].smer = STOP; //sve zaustavljene
    }
    
    /*Inicijalne vrednosti, da bismo ponistili prethodne vrednosti*/
    tri_kugle[0] = -1;
    tri_kugle[1] = -1;
    tri_kugle[2] = -1;
    
    srand(time(NULL)); //seme

    /*Tri razlicite kugle*/
    tri_kugle[0] = rand()%9;
    tri_kugle[1] = rand()%9;
    tri_kugle[2] = rand()%9;
    while(tri_kugle[1] == tri_kugle[0])
        tri_kugle[1] = rand()%9;
    while(tri_kugle[2] == tri_kugle[0] || tri_kugle[2] == tri_kugle[1])
        tri_kugle[2] = rand()%9;

    /*Sve tri treba da idu na gore*/
    niz_kugli[tri_kugle[0]].smer = 1;
    niz_kugli[tri_kugle[1]].smer = 1;    
    niz_kugli[tri_kugle[2]].smer = 1;
}

int main(int argc, char **argv)
{

    /*Izaberemo kugle.*/
    random_kugle(); 
    
    /* Inicijalizujemo GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreiramo prozor. */
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2.");

    /* Registrujemo callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    glutIdleFunc(on_idle);

    /* Obavljamo inicijalizaciju  OpenGL-a. */
    glClearColor(0.8, 0.8, 0.8, 0);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(1.5);

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void on_timer(int value){
    if(value == 1){
        bool opet = false;
        /*Prolazimo kroz sve kugle*/
        for(int i=0; i<9; i++){
            /*Ako je smer na gore, pomeramo je po Y koordinati navise.*/
            if(niz_kugli[i].smer == GORE){
                niz_kugli[i].moveY += SPEED;
               
                /*Ako je stigla do vrha, promenimo joj smer kretanja.*/
                if(niz_kugli[i].moveY >= VRH)
                {
                    niz_kugli[i].smer = !niz_kugli[i].smer;
                }
            }
            /*Inace, ako je smer na dole, pomeramo je po Y koordinati nanize.*/
            else if(niz_kugli[i].smer == DOLE){
                if(niz_kugli[i].moveY >= DNO){
                    niz_kugli[i].moveY -= SPEED;
                    /*Ako je stigla do dna, promenimo joj smer kretanja.*/ 
                    if(niz_kugli[i].moveY <= DNO)
                    {
                        niz_kugli[i].smer = !niz_kugli[i].smer;
                        opet = true;
                    }
                }
                   
            }
        }
        glutTimerFunc(TIMER_INTERVAL, on_timer, 1);
        if(opet)
            random_kugle();
        }
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
                case 27:
                /* Program se zavrsava. */
                exit(0);
                break;
		case 'a': 
		case 'A':
			{
				glutTimerFunc(TIMER_INTERVAL, on_timer, 1);
				break;
			}
		case '9':{
            cekicX = 8;
            cekicZ = 2;
            glutPostRedisplay();
            break;
        }

        case '6':{
            cekicX = 8;
            cekicZ = 5;
            glutPostRedisplay();
            break;
        }

        case '3':{
            cekicX = 8;
            cekicZ = 8;
            glutPostRedisplay();
            break;
        }

        case '8':{
            cekicX = 5;
            cekicZ = 2;
            glutPostRedisplay();
            break;
        }

        case '5':{
            cekicX = 5;
            cekicZ = 5;
            glutPostRedisplay();
            break;
        }

        case '2':{
            cekicX = 5;
            cekicZ = 8;
            glutPostRedisplay();
            break;
        }

        case '7':{
            cekicX = 2;
            cekicZ = 2;
            glutPostRedisplay();
            break;
        }

        case '4':{
            cekicX = 2;
            cekicZ = 5;
            glutPostRedisplay();
            break;
        }

        case '1':{
            cekicX = 2;
            cekicZ = 8;
            glutPostRedisplay();
            break;
        }
    }/*Kraj switch-a*/
    cekic_hit = 1;
}



static void on_reshape(int width, int height)
{
    /* Pamte se sirina i visina prozora. */
    window_width = width;
    window_height = height;
}

static void on_display(void)
{
    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     /* Ambijentalna boja svetla. */
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1 };

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

    /* Ambijentalno osvetljenje scene. */
    //GLfloat model_ambient[] = { 0.4, 0.4, 0.4, 1 };
    
    /*Pozicija svetla.*/
    GLfloat light_position[] = {1, 1, 1, 0};

    /* Podesava se viewport. */
    glViewport(0, 0, window_width, window_height);

    /* Podesavamo projekciju. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
            60,
            window_width/(float)window_height,
            1, 25);

    /* Podesavamo tacku pogleda. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            5, 15, 15,
            5, 0, 5,
            0, 1, 0
        );

    glBegin(GL_LINES);
    	/* X osa */
        glColor3f(1, 0, 0); /*crvena*/
        glVertex3f(0,0,0);
        glVertex3f(8,0,0);
        
        /* Y osa */
        glColor3f(0, 1, 0); /*plava*/
        glVertex3f(0,0,0);
        glVertex3f(0,8,0);
        
        /* Z osa */
        glColor3f(0, 0, 1); /*zelena*/
        glVertex3f(0,0,0);
        glVertex3f(0,0,8);
    glEnd();

    /*Postavljanje ravni*/
	glPushMatrix();
		glTranslatef(5, 0, 5);
		glColor3f(0.1565, 0.24, 0.1488);
		glScalef(10, 0, 10);
		glutSolidCube(1);
	glPopMatrix();

    /*9 rupa*/
	for(int i=2; i<=8; i+=3){
		for(int j=2; j<=8; j+=3){
			glPushMatrix();
				glTranslatef(i, 0.1, j);
				glColor3f(0, 0, 0);
				glScalef(1, 0, 1);
			glutSolidSphere(1, 10000, 100);
			glPopMatrix();
	}
}
    /*Izlazece kugle*/
    for(int k=0; k<9; k++)
    {
        int i = (k/3)*3 + 2;
        int j = (k%3)*3 + 2;
	glPushMatrix();
        glTranslatef(i, niz_kugli[k].moveY, j);
        glColor3f(1, 0, 0);
        //glScalef(1, 0, 1);
        glutSolidSphere(1, 10000, 100);
        glPopMatrix();
    }

     /*Cekic*/
    glPushMatrix();
        glColor3f(0.5, 0.7, 0);
        glTranslatef(cekicX, 6, cekicZ); /*Na visini 6*/
        glutSolidCube(2);
    glPopMatrix();

    glutSwapBuffers();

    
}