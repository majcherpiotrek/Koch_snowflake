#include <GL/glut.h>
#include <math.h>
#include <iostream>



/*Struktura reprezentująca punkt w dwuwymiarowym układzie współrzędnych*/
struct point2d
{
    GLfloat X;
    GLfloat Y;
};

/*********DANE WEJŚCIOWE***********/
const GLfloat RADIUS = 70.0f;
const GLint NUM_STEPS = 4;
const point2d CENTER = {0.0f, 0.0f};
/**********************************/

/*Funkcja wykonująca obrót punktu "p" wokół środka układu współrzędnych o kąt "deg" podany w radianach*/
void rotate(point2d& p, double deg)
{   
    double x = (double)p.X;
    double y = (double)p.Y;
    p.X = (GLfloat)(x*cos(deg) - y*sin(deg)); // x' = x*cos(deg) - y*sin(deg)
    p.Y = (GLfloat)(y*cos(deg) + x*sin(deg)); // y' = y*cos(deg) + x*sin(deg)
}

/*Funkcja tworząca łamaną Koch'a*/
void breakLine(GLint steps, point2d A, point2d B)
{

    if(steps > 0)
    {
    /*Wyliczenie długości odcinka*/
    GLfloat xd = B.X - A.X;
    GLfloat yd = B.Y - A.Y;

    GLfloat len = (GLfloat)sqrt((float)(xd*xd + yd*yd));
    
    /*Znalezienie punktów w 1/3 i 2/3 */
    
    point2d vec = {xd/len, yd/len}; //wektor jednostkowy
    
    point2d p13 = {A.X+vec.X*len/3, A.Y+vec.Y*len/3};
    point2d p23 = {A.X+vec.X*2*len/3, A.Y+vec.Y*2*len/3};
    
    /*Szukamy nowego punktu - trzeciego wierzchołka trójkąta*/

    // 1. Przesuwamy tak, aby wykonać obrót wookół (0,0)
    point2d w = {p23.X - p13.X, p23.Y - p13.Y}; 

    double deg = M_PI/3; //kąt 60 stopni
  
    // 2. Wykonujemy obrót o 60 stopni wokół (0.0)
    rotate(w, deg);
    
    // 3. Przesuwamy z powrotem
    w.X += p13.X; 
    w.Y += p13.Y;
  
        /*Wypełniamy kolorem trójkąt, powstały na łamanej*/
        glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex2f(p13.X, p13.Y);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex2f(p23.X, p23.Y);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex2f(w.X, w.Y);
        glEnd();

        /*Wywołujemy rekurencyjnie funkcję tworzącą łamaną dla każdego z powstałych odcinków*/
        breakLine(steps-1, A, p13);
        breakLine(steps-1, p13, w);
        breakLine(steps-1, w, p23);
        breakLine(steps-1, p23, B);
    }
    return;  
}

/*Funkcja rysująca wypełniony płatek śniegu Koch'a w zadanej liczbie kroków 
oraz dla trójkąta równobocznego o zadanym środku i dł. boku*/
void drawKochSnowflake( GLint steps, point2d ctr, GLfloat a )
{
    /*Najpierw tworzymy równoramienny trójkąt prostokątny.
    Punkt ctr oznacza miejsce, gdzie wysokość z wierzchołka top_v
    opada na przeciwprostokątną*/
    point2d top_v = {ctr.X, ctr.Y + a}; //Wierzchołek trójkąta powyżej środka
    point2d right_v = {ctr.X + a, ctr.Y}; // po prawej
    point2d left_v = {ctr.X - a, ctr.Y}; // po lewej

    /*Teraz obracamy lewy i prawy wierzchołek wokół punktu ctr,
    aby otrzymać trójkąt równoboczny.*/
    rotate(right_v, -M_PI/6);
    rotate(left_v, M_PI/6);

    /*Wypełniamy kolorem trójkąt bazowy*/
    glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex2f(left_v.X, left_v.Y);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex2f(right_v.X, right_v.Y);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex2f(top_v.X, top_v.Y);
    glEnd();

    breakLine(steps, left_v, top_v);
    breakLine(steps, top_v, right_v);
    breakLine(steps, right_v, left_v);
}
void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawKochSnowflake(NUM_STEPS, CENTER, RADIUS);

    glFlush();
    // Przekazanie poleceń rysujących do wykonania
}

void MyInit(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Kolor okna wnętrza okna - ustawiono na czarny
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



    glutCreateWindow("Płatek śniegu Koch'a - Piotr Majcher");
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