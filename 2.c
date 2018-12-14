#include <stdlib.h>
#include <GL/glut.h>

/* Dimenzije prozora */
static int window_width, window_height;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);
float moveY = 0;

int main(int argc, char **argv)
{
    /* Ambijentalna boja svetla. */
    GLfloat light_ambient[] = { 0, 0, 0, 1 };

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = { 1, 1, 1, 1 };

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = { 1, 1, 1, 1 };

    /* Ambijentalno osvetljenje scene. */
    GLfloat model_ambient[] = { 0.4, 0.4, 0.4, 1 };
    
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
		if(moveY <= 4){
                    moveY += 0.9;
                    glutPostRedisplay();
                    glutTimerFunc(1, on_timer, 1);	

		}
                else {
                    glutTimerFunc(20, on_timer, 2);
                }
	}
	else if(value == 2){
		if(moveY >= -1){
                    moveY -= 0.9;
                    glutPostRedisplay();
                    glutTimerFunc(1, on_timer, 2);
		}
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
				glutTimerFunc(20, on_timer, 1);
				break;
			}
		case 'b':
		case 'B':
			{
				moveY -= 1;
				glutPostRedisplay();
				break;
			}
    }
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
            10, 15, 10,
            5, 0, 5,
            0, 1, 0
        );

    glBegin(GL_LINES);
    	/* X osa */
        glColor3f(1, 0, 0); //crvena
        glVertex3f(0,0,0);
        glVertex3f(8,0,0);
        
        /* Y osa */
        glColor3f(0, 1, 0); //plava
        glVertex3f(0,0,0);
        glVertex3f(0,8,0);
        
        /* Z osa */
        glColor3f(0, 0, 1); //zelena
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
	//9 rupa
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
//NOVA SFERA
	glPushMatrix();
		glTranslatef(2, -1 + moveY, 2);
		glColor3f(0.4, 0.6, 0.4);
		//glScalef(1, 0, 1);
		glutSolidSphere(1, 10000, 100);
	glPopMatrix();
    /* Nova slika se salje na ekran. */
    glutSwapBuffers();

	
}
 
