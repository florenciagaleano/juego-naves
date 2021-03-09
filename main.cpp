#include <stdio.h>
#include <windows.h>//permite dar posiciion
#include <conio.h>
#include <list>
using namespace std;
#define ARRIBA 72
#define ABAJO 80
#define IZQ 75
#define  DER 77

//COORDENADA Y FUNCIONA INVERTIDA
void gotoxy(int x,int y)
{
    HANDLE hCon;
    COORD dwPos;//define coordenadas cursor

    hCon=GetStdHandle(STD_OUTPUT_HANDLE);

    dwPos.X=x;
    dwPos.Y=y;

    SetConsoleCursorPosition(hCon,dwPos);
}

void ocultarCursor()
{
    HANDLE hCon;
    hCon=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize=0;//VALORES DE 1 A 100 definen el estilo delcursor
    cci.bVisible=FALSE;//si queremos que se muestre o no en pantalla

    SetConsoleCursorInfo(hCon,&cci);
}

void pintar_limites()
{
    for(int i=2;i<78;i++)
    {
        gotoxy(i,3);
        printf("%c",205);
        gotoxy(i,33);
        printf("%c",205);
    }

    for(int i=4;i<33;i++)
    {
        gotoxy(2,i);
        printf("%c",186);
        gotoxy(77,i);;
        printf("%c",186);
    }

    gotoxy(2,3);
    printf("%c",201);
    gotoxy(2,33);
    printf("%c",200);
    gotoxy(77,3);
    printf("%c",187);
    gotoxy(77,33);
    printf("%c",188);
}

class NAVE
{
    int x,y;//priv: solo sirve para esta clase
    int corazones;
    int vidas;
public://se puede usar en cualquier parte del programa
    NAVE(int _x,int _y,int _corazones,int _vidas);//constructor
    //NAVE(int _x,int _y,int _corazones,int _vidas):x(_x),y(_y),corazones(_corazones),vidas(_vidas){}
    int X(){return x;}
    int Y(){return y;}
    void pintar();
    void borrar();
    void mover();
    void pintarCorazones();//vidas
    void morir();
    void coras(){corazones--;}
};

NAVE::NAVE(int _x,int _y,int _corazones,int _vidas)//inicializa los atributos de nave
{
    x=_x;
    y=_y;
    corazones=_corazones;
    vidas=_vidas;
    //TAMBIEN SE PUEDE HACER DIRECTO EN LA CLASE
}

void NAVE::pintar()
{
    gotoxy(x,y);
    printf("  %c",30);                /*▲*/
    gotoxy(x,y+1);                 /*(¤)*/
    printf(" %c%c%c",'(',207,')');   /*▲¥ ¥▲*/
    gotoxy(x,y+2);
    printf("%c%c %c%c",30,190,190,30);
}

void NAVE::borrar()
{
    gotoxy(x,y);
    printf("             ");
    gotoxy(x,y+1);
    printf("     ");
    gotoxy(x,y+2);
    printf("             ");
}

void NAVE::mover()
{
    char tecla;
    //kbhit: funcion para cuando se presiona una tecla
       if(kbhit())
       {
           tecla=getch();//guardo la letra que presiono el jugador
           borrar();
           //printf(" ");//imprimo expacio en blanco para que el asterisco se mueva en vez de dejar un rastro

           if(tecla==IZQ&&x>3)//no se puede chocar con los bordes
           {
               x--;
           }else if(tecla==DER&&x+6<77)
           {
               x++;
           }else if(tecla==ARRIBA&&y>4)
           {
               y--;
           }else if(tecla==ABAJO&&y+3<33)
           {
               y++;
           }
           pintar();//printf("*");
           pintarCorazones();
       }
}

void NAVE:: pintarCorazones()
{
    gotoxy(50,2);
    printf("Vidas: %d",vidas);
    gotoxy(64,2);
    printf("Salud");
    gotoxy(70,2);
    for(int i=0;i<corazones;i++)
    {
        gotoxy(70,i+2);
        printf("%c",3);//corazoncitos
    }
}

void NAVE::morir()
{
    if(corazones==0)
    {
        borrar();
        gotoxy(x,y);    printf("  **  ");
        gotoxy(x,y+1);  printf(" ****");
        gotoxy(x,y+2);  printf("  **");
        Sleep(200);

        borrar();
        gotoxy(x,y);    printf(" * ** * ");
        gotoxy(x,y+1);  printf("  ****  ");
        gotoxy(x,y+2);  printf(" * ** * ");//explosion
        Sleep(200);

        borrar();
        vidas--;
        corazones=3;
        pintarCorazones();
        pintar();//hay que pintar la nave again
    }
}


class ASTEROIDE
{
    int x,y;
public:
    ASTEROIDE(int _x,int _y);//:x(_x),(_y){}
    void pintar();
    void mover();
    void choque(class NAVE &N);//paso por referencia pq a partir de esto se van a modificar las vidas, salud,etc
};

ASTEROIDE::ASTEROIDE(int _x,int _y)
{
    x=_x;
    y=_y;
}

void ASTEROIDE:: pintar()
{
    gotoxy(x,y);
    printf("%c",184);
}

void ASTEROIDE:: mover()
{
    gotoxy(x,y);
    printf("   ");//borro rastro asteroide
    y++;//van cayendo
    if(y>32)//casi pasa el limite
    {
        x=rand()%71+4;//num entre 4 y 71
        y=4;
    }

    pintar();
}

void ASTEROIDE::choque(class NAVE &N)
{
    if( x>=N.X() && x<N.X()+6 && y>=N.Y() && y<=N.Y()+2 )//tomo el intervalo que ocupa la nave
    {
        N.coras();//corazones-1
        N.borrar();
        N.pintar();
        N.pintarCorazones();//actualizo el nom de coras
        x=rand()%71+4;
        y=4;
    }
}

class BALA{
    int x,y;
public:
    BALA(int _x,int _y);
    void mover();
    bool fuera();
    int X(){return x;}
    int Y(){return y;}
};

BALA::BALA(int _x,int _y)
{
    x=_x;
    y=_y;
}

void BALA::mover()
{
    gotoxy(x,y);
    printf("    ");//borro
    y--;

    gotoxy(x,y);
    printf("*");
}

bool BALA ::fuera()
{
    if(y==4)
    {
        return true;
    }

    return false;
}

int main()
{
    ocultarCursor();//funciona maso
    pintar_limites();
    NAVE Navecita(7,7,3,3);

    ASTEROIDE Ast(10,4),Ast2(4,8),Ast3(15,10);
    list <BALA*> B;
    list <BALA*>::iterator it;

    Navecita.pintar();
    bool game_over=false;
    int x=0 ,y=0;

    gotoxy(x,y);
    printf("*");

    while(!game_over)
    {
        if(kbhit())
        {
            char tecla=getch();
            if(tecla='a')
            {
                B.push_back(new BALA(Navecita.X()+2,Navecita.Y()-1));
            }


            for(it=B.begin();it!=B.end();it++)
            {
                (*it)->mover();
                if((*it)->fuera())
                {
                    gotoxy((*it)->X(),(*it)->Y());
                    printf("   ");
                    delete(*it);
                    it=B.erase(it);
                }
            }
        }

        Ast.mover();
        Ast2.mover();
        Ast3.mover();
        Navecita.mover();
        Sleep(20);//menos iteraciones por segundo
    }

    return 0;
}
