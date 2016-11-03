#include <GL/glut.h>
#include <math.h>
#include <iostream>

struct point2d
{
    GLfloat X;
    GLfloat Y;
};

void rotate(point2d& p, double deg)
{   
    double x = (double)p.X;
    double y = (double)p.Y;
    p.X = (GLfloat)(x*cos(deg) - y*sin(deg));
    p.Y = (GLfloat)(y*cos(deg) + x*sin(deg));
}

void breakLine(GLint steps, point2d A, point2d B, bool upside_down)
{
    if(steps > 0)
    {
    /*Wyliczenie długości odcinka*/
    GLfloat xd = B.X - A.X;
    GLfloat yd = B.Y - A.Y;

    GLfloat len = (GLfloat)sqrt((float)(xd*xd + yd*yd));
    /*Znalezienie punktów w 1/3 i 2/3 */
    point2d vec = {xd/len, yd/len};
    point2d p13 = {A.X+vec.X*len/3, A.Y+vec.Y*len/3};
    point2d p23 = {A.X+vec.X*2*len/3, A.Y+vec.Y*2*len/3};
    
    /*Szukamy nowego punktu - trzeciego wierzchołka trójkąta*/
    point2d w = {p23.X - p13.X, p23.Y - p13.Y}; //przesuwamy tak, aby wykonać obrót wookół (0,0)
    double deg = M_PI/3;
    if(upside_down)
        deg *= -1;
    rotate(w, deg); //obrót o 60 stopni wokół (0.0)
    //przesunięcie z powrotem
    w.X += p13.X; 
    w.Y += p13.Y;
  
    
        breakLine(steps-1, A, p13, upside_down);
        breakLine(steps-1, p13, w, upside_down);
        breakLine(steps-1, w, p23, upside_down);
        breakLine(steps-1, p23, B, upside_down);
    }
    else
    {
        glBegin(GL_LINE_STRIP);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex2f(A.X, A.Y);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex2f(B.X, B.Y);
        glEnd();
    }
    
}
void drawKochSnowflake( GLint steps, point2d A, point2d B, point2d C )
{
    breakLine(steps, A, C, false);
    breakLine(steps, C, B, false);
    breakLine(steps, A, B, true);
}
void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszczącym
    point2d A = {-50.0f, -50.0f};
    point2d B = {50.0f, -50.0f};
    point2d C = {B.X - A.X, B.Y - A.Y};
    rotate(C, M_PI/3);
    C.X += A.X;
    C.Y += A.Y;

    drawKochSnowflake(7, A, B, C);
    //breakLine(3,-45.0f, 0.0f, 45.0f, 0.0f, true);

    glFlush();
    // Przekazanie poleceń rysujących do wykonania
}

void MyInit(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Kolor okna wnętrza okna - ustawiono na szary
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
    GLfloat AspectRatio;

    // Deklaracja zmiennej AspectRatio określającej proporcję wymiarów okna



    if (vertical == 0)
        // Zabezpieczenie pzred dzieleniem przez 0

        vertical = 1;

    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielkościokna okna urządzenia (Viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)

    glMatrixMode(GL_PROJECTION);
    // Określenie układu współrzędnych obserwatora

    glLoadIdentity();
    // Określenie przestrzeni ograniczającej

    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    // Wyznaczenie współczynnika proporcji okna

    // Gdy okno na ekranie nie jest kwadratem wymagane jest
    // określenie okna obserwatora.
    // Pozwala to zachować właściwe proporcje rysowanego obiektu
    // Do określenia okna obserwatora służy funkcja glOrtho(...)



    if (horizontal <= vertical)

        glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

    else

        glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    // Określenie układu współrzędnych     

    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    // Ustawienie trybu wyświetlania
    // GLUT_SINGLE - pojedynczy bufor wyświetlania
    // GLUT_RGBA - model kolorów RGB



    glutCreateWindow("Dywan Sierpińskiego - Piotr Majcher");
    // Utworzenie okna i określenie treści napisu w nagłówku okna

    glutDisplayFunc(RenderScene);
    // Określenie, że funkcja RenderScene będzie funkcją zwrotną (callback)
    // Biblioteka GLUT będzie wywoływała tą funkcję za każdym razem, gdy
    // trzeba będzie przerysować okno


    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną za
    // zmiany rozmiaru okna

    MyInit();
    // Funkcja MyInit (zdefiniowana powyżej) wykonuje wszelkie
    // inicjalizacje konieczneprzed przystąpieniem do renderowania


    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT
    return 0;
} 