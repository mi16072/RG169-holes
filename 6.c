#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define PI 3.1415926535
#define EPSILON 0.01
//#define FILENAME0 "wall.bmp" /* Imena fajlova sa teksturama. */
/* Pozicije kugli */
#define DNO -1
#define VRH 4
/* Parametri tajmera */
#define TIMER_INTERVAL 20
#define TIMER_ID 2

#define DRAW_CYLINDER

/* Globalne promenljive */
static float animation_angle = 0;
static int animation_ongoing = 0;
static int tikovi = 0;
static int indikator_za_angle = 1;
static float angle_cekica = 0;
static int animation_cekica = 0;
static int mouse_x, mouse_y; /* Koordinate pokazivaca misa. */
static float matrix[16]; /* Kumulativana matrica rotacije. */
static int window_width, window_height; /* Dimenzije prozora */
    /* Pozicija cekica */
int cekicX = 10;
int cekicY = 5;
int cekicZ = 10;
int cekic_hit = 0;
    /* Opis kugle */
typedef struct 
{ 
    float x;
    float moveY;
    float z;
    int treba_da_se_krece; /* Fleg za 3 kugle koje izlaze */
    int za_crtanje; /* Fleg za kugle koje prezive udarac */
}KUGLA;

KUGLA niz_kugli[9]; /* Sve kugle */
int tri_kugle[3]; /* Odabrane random kugle */

/* Deklaracije callback funkcija */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);
//static void on_idle(void);
static void on_mouse(int button, int state, int x, int y);
static void on_motion(int x, int y);

/* Implementirane funkcije */
static void on_timer(int value);
static void iscrtaj_krug(double x, double y, double z, double r);
static void distance();
static void nacrtaj_cilindar();
static void nacrtaj_drsku();
static void random_kugle();

int main(int argc, char **argv)
{
    animation_ongoing = 0;
    /* Izaberemo kugle */
    random_kugle(); 
    
    /* Inicijalizujemo GLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreiramo prozor */
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("6.");

    /* Registrujemo callback funkcije */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    //glutIdleFunc(on_idle);
    glutMouseFunc(on_mouse);
    glutMotionFunc(on_motion);

    /* Incijalizuju se koordinate misa */
    mouse_x = 0;
    mouse_y = 0;

    /* Inicijalizujemo matricu rotacije */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

    /* Obavljamo inicijalizaciju  OpenGL-a */
    glClearColor(0.627, 0.627, 0.627, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    glLineWidth(1.5);

    /* Program ulazi u glavnu petlju */
    glutMainLoop();

    return 0;
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
                /* Na taster A pokreni animaciju */
                if(!animation_ongoing) {
                    animation_ongoing = 1;
                    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
                }
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
        case 32 :
            /* Pokrece se cekic */
            if(!animation_cekica) {
                animation_cekica = 1;
            }
                

            break;
    }/* Kraj switch-a */
    cekic_hit = 1;
}

static void on_reshape(int width, int height)
{
    /* Pamte se sirina i visina prozora */
    window_width = width;
    window_height = height;
}

static void on_display(void)
{
    /* Pozicija svetla */
    GLfloat light_position[] = {1, 1, 1, 0};

    /* Ambijentalno osvetljenje scene. */
    GLfloat model_ambient[] = { 0.4, 0.4, 0.4, 1 };
//Svetla
     /* Ambijentalna boja svetla */
    GLfloat light_ambient[] = { 1, 1, 1, 1 };

    /* Difuzna boja svetla */
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };

    /* Spekularna boja svetla */
    GLfloat light_specular[] = { 0, 0, 0, 1 };

//Materijali
    /* Koeficijenti ambijentalne refleksije materijala */
    GLfloat material_ambient[] = { 0, 0, 0, 1 };

    /* Koeficijenti difuzne refleksije materijala */
    GLfloat material_diffuse[] = { 0, 0, 0.4, 1 };

    /* Koeficijenti spekularne refleksije materijala */
    GLfloat material_specular[] =  { 0.3, 0.3, 0.3, 1 };

    /* Koeficijent glatkosti materijala */
    GLfloat shininess = 20;

    /* Brise se prethodni sadrzaj prozora */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Ukljucuje se osvjetljenje i podesavaju parametri svetla */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    /* Podesava se viewport */
    glViewport(0, 0, window_width, window_height);

    /* Podesavamo projekciju */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
            60,
            window_width/(float)window_height,
            1, 25);

    /* Podesavamo tacku pogleda */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            5, 15, 15,
            5, 0, 5,
            0, 1, 0
        );

    /* Primenjuje se matrica rotacije */
    glMultMatrixf(matrix);

    glDisable(GL_LIGHTING);
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
    glEnable(GL_LIGHTING);

    //glDisable(GL_LIGHTING);
    /* Iscrtavanje 9 rupa (krugovi) */
	for(int i=2; i<=8; i+=3){
		for(int j=2; j<=8; j+=3){
			glPushMatrix();
				glScalef(1, 0, 1);
                glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
                iscrtaj_krug(i, 0.1, j, 1);
			glPopMatrix();
	    }
    }
    glEnable(GL_LIGHTING);
    /* Izlazece kugle */
    for(int k=0; k<9; k++)
    {
        int i = (k/3)*3 + 2;
        int j = (k%3)*3 + 2;
	glPushMatrix();
        niz_kugli[k].x = i;
        niz_kugli[k].z = j;
        glTranslatef(niz_kugli[k].x, niz_kugli[k].moveY, niz_kugli[k].z);
        if(niz_kugli[k].za_crtanje) /* Ako su nisu dobile fleg ne iscrtavaju se */
            glutSolidSphere(1, 50, 50);
    glPopMatrix();
    }
    
     /* Crtanje cekica */
    glPushMatrix();
        /* Cilindar */
        //glColor3f(0, 0.298, 0.6);
        glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
        glTranslatef(cekicX, cekicY, cekicZ); /*Na visini 6*/
        glTranslatef(0, 0, 1);
        glRotatef(-angle_cekica, 1, 0, 0); /* Udarac cekica */
        glTranslatef(0, 0, -1);
        nacrtaj_cilindar();
        /* Gornja baza valjka */
        glPushMatrix();
            glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
            glTranslatef(0, 3, 0);
            iscrtaj_krug(0, 0, 0, 1);
        glPopMatrix();
        /* Donja baza valjka */
        glPushMatrix();
            glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
            glTranslatef(0, 1, 0);
            iscrtaj_krug(0, 0, 0, 1);
        glPopMatrix();
        /* Drska cekica */
        glPushMatrix();
            glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
            glRotatef(90, 1, 0, 0);
            glScalef(0.5, 1, 0.5);
            glTranslatef(0, 1, -4);
            nacrtaj_drsku();
        glPopMatrix();
        /* Baza drske */
        glPushMatrix();
            glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
            glScalef(0.5, 0.5, 0.5);
            glTranslatef(0, 4, 8.3);
            glRotatef(90, 1, 0, 0);
            iscrtaj_krug(0, 0, 0, 1);
        glPopMatrix();
    glPopMatrix();
    //glEnable(GL_LIGHTING);

    glDisable(GL_LIGHTING);
    /*Postavljanje ravni*/
	glPushMatrix();
		glTranslatef(5, 0, 5);
		glColor3f(0.4, 0.698, 1); //TODO:menjaj
		glScalef(10, 0, 10);
		glutSolidCube(1);
	glPopMatrix();

    glutSwapBuffers();
}

static void on_mouse(int button, int state, int x, int y)
{
    /* Pamti se pozicija pokazivaca misa. */
    mouse_x = x;
    mouse_y = y;
}

static void on_motion(int x, int y)
{
    /* Promene pozicije pokazivaca misa */
    int delta_x, delta_y;

    /* Izracunavaju se promene pozicije pokazivaca misa */
    delta_x = x - mouse_x;
    delta_y = y - mouse_y;

    /* Pamti se nova pozicija pokazivaca misa */
    mouse_x = x;
    mouse_y = y;

    /* Izracunava se nova matrica rotacije */
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glLoadIdentity();
        glRotatef(180 * (float) delta_x / window_width, 0, 1, 0);
        glRotatef(180 * (float) delta_y / window_height, 1, 0, 0);
        glMultMatrixf(matrix);

        glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();

    /* Ponovno iscrtavanje prozora */
    glutPostRedisplay();
}

static void on_timer(int value) {

    if(value != TIMER_ID) 
        return;
    /* Animacija kugli */
    if(indikator_za_angle)
        animation_angle += 2;

    /* Dok je u toku animacija cekica, proveravaj rastojanje od kugle i menjaj ugao rotacije */
    if(animation_cekica) {
        distance();
        angle_cekica += 4;
    }
    /* Cekic se rotira do 30 stepeni a onda se vraca na pocetni ugao, zaustavlja se animacija cekica */
    if(angle_cekica >= 30) {
        angle_cekica = 0;
        animation_cekica = 0;
    }

    /* Animacija kugli, kretanje gore-dole */
    for(int i=0;i<9;i++) {
        if(niz_kugli[i].treba_da_se_krece)
            niz_kugli[i].moveY = 5 * sin(animation_angle * PI/180); /* Ide do visine 4 (nalazi se na -1 po Y koordinati) */
    }

    /* Zaustavi kugle na vrhu, da igrac moze da udari */
    if(animation_angle >= 90) {
        indikator_za_angle = 0;
        tikovi += 1;
        if(tikovi >= 50) {
            indikator_za_angle = 1;
        }
    }

    /* Ponovno biranje random kugli */
    if(animation_angle >= 180) {
        //animation_ongoing = 0;
        animation_angle = 0;
        random_kugle();
        tikovi = 0;
    }

    glutPostRedisplay();

    /* Zovi tajmer dok je animacija u toku */
    if(animation_ongoing)
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
 }

static void iscrtaj_krug(double x, double y, double z, double r) {

    glPushMatrix();
        glTranslatef(x, y, z);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0, 0, 0);

            float angle = 0;
            while(angle <= 360) {
                float x1 = r * cos(angle * PI/180);
                float z1 = r * sin(angle * PI/180);

                glVertex3f(x1, y, z1);
                angle += 0.01;
            }
        glEnd();
    glPopMatrix();
}

/* Racunamo rastojanje kugle i valjka da bismo utvrdili da li je doslo do kolizije */
static void distance() {

    for(int i=0;i<9;i++) {
        KUGLA k = niz_kugli[i];
        float distanca = sqrt((k.x - cekicX)*(k.x - cekicX) + (k.moveY - cekicY - 1)*(k.moveY - cekicY - 1) + (k.z - cekicZ)*(k.z - cekicZ));
        if(distanca <= 1) {
                niz_kugli[i].za_crtanje = 0;
        }
    }
}

/* Parametrizacija cilindra */
#ifdef DRAW_CYLINDER
/*
 * Postavlja koordinate i normale temena cilindra,
 * u zavisnosti od parametara u i v
 */
void set_normal_and_vertex(float u, float v)
{
    glNormal3f(
            sin(v),
            0,
            cos(v)
            );
    glVertex3f(
            sin(v),
            u,
            cos(v)
            );
}
#endif

/* Crtamo valjak */
void nacrtaj_cilindar()
{
    glPushMatrix();
        glTranslatef(0, 1, 0);
        glRotatef(-90, 1, 0, 0);
        GLUquadric *valjak = gluNewQuadric();
        gluCylinder(valjak, 1, 1, 2, 30, 30);
    glPopMatrix();
}

/* Crtamo drsku cekica */
void nacrtaj_drsku()
{
    float u, v;
    glPushMatrix();
    /* Crtamo objekat strip po strip */
    for (u = 0; u < PI; u += PI / 20) {
        glBegin(GL_QUAD_STRIP);
        for (v = 0; v <= PI*2 + EPSILON; v += PI / 20) {
            set_normal_and_vertex(u, v);
            set_normal_and_vertex(u + PI / 20, v);
        }
        glEnd();
    }
    glPopMatrix();
}

/* Odabir tri nasumicne kugle */
void random_kugle()
{
    for(int i=0; i<9; i++)
    {
        niz_kugli[i].moveY = DNO; /* Sve na dnu */
        niz_kugli[i].treba_da_se_krece = 0; /* Sve zaustavljene */
        niz_kugli[i].za_crtanje = 1; /* Sve imaju fleg za iscrtavanje */
    }
    
    int prva, druga, treca;

    srand(time(NULL)); //seme

    prva = rand()%9;
    druga = rand()%9;
    treca = rand()%9;
    
    /* Obezbedjujemo da su sve kugle razlicite */
    while(prva == druga)
        prva = rand()%9;
    while(treca == prva || treca == druga)
        treca = rand()%9;

    /* Samo one dobijaju fleg za kretanje */
    niz_kugli[prva].treba_da_se_krece = 1;
    niz_kugli[druga].treba_da_se_krece = 1;
    niz_kugli[treca].treba_da_se_krece = 1;
}
