#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#define BUFMAX 1005

typedef struct RESUMEGAME{// structura salveaza datele pentru resume game 
	int ir;
	int scorer;
	int folosire505r;
	int nextquestionr;
    int nrgresiter;
    int nrcorecter;

}RESUMEGAME;

int scormaxim (int max,char playername){ // functia stocheaza si afiseaza scorul maxim 
		initscr();
        clear();
        noecho();
        mvprintw(0, 0, "%s", "Scor Maxim");

        mvprintw(4, 2, "%s", "Scorul maxim obtinut :");
        mvprintw(4, 25,"%d", max);
        mvprintw(12, 0, "%s", "q - reintoarcere in meniu ");
        while(getch() != 'q');
        refresh();
        clear();
        return 0;
}


int doHelp(){ //functia afiseaza o fereastra pentru optiunea help
        initscr();
        clear();
        noecho();
        mvprintw(0, 0, "%s", "MENIU DE HELP");
        
        mvprintw(2, 0, "%s", "Controls:");
        mvprintw(4, 2, "%s", "a - selecteaza varianta A");
        mvprintw(5, 2, "%s", "b - selecteaza varianta B");
        mvprintw(6, 2, "%s", "c - selecteaza varianta C");
        mvprintw(7, 2, "%s", "d - selecteaza varianta D");
        mvprintw(8, 2, "%s", "f - selecteaza varianta ajutatoare 50/50 care scapa de 2 rasounsuri gresite");
        mvprintw(9, 2, "%s", "n - selecteaza varianta ajutatoare Skip the question care sare la intrebarea urmatoare");
        mvprintw(11, 2, "%s", "q - reintoarcere in meniu ");
        

        while(getch() != 'q');
        refresh();
        clear();
        return 0;
}   

int printScore(int firstRun, int *score){ //functia printeaza scorul
      if(firstRun == 1) //daca jocul este nou(nu Resume) reseteaza scorul
                *score = 0;
        
        mvprintw(11, 0, "%s", "Scor: ");//actualizeaza scorul
        mvprintw(11, 10, "%s", "$");
        mvprintw(11, 8, "%d", *score); //printeaza scorul 
  return 0;
}

int printTime(int firstRun, int *timeSpentPlaying) {//functia calculeaza si printeaza timpul si intervalul de timp petrecut in joc
        time_t currentTime;
        static time_t startTime; //folosit ca sa evitam variabilele globale
        if(firstRun == 1){//daca jocul este nou(nu Resume) recalculeaza timpul la care s-a intrat in joc
              time(&startTime);
        }
        time(&currentTime);
        mvprintw(0, 0, "%s", ctime(&currentTime));// timpul este intotdeauna printat la coordonatele x=0 si y=0
        *timeSpentPlaying = (int) difftime(currentTime, startTime); //calculeaza timpul petrecut in joc   
        mvprintw(1, 0, "%s", "Timp petrecut jucand: ");
        mvprintw(1, 15, "%s", " secunde ");
        mvprintw(1, 11, "%d", *timeSpentPlaying); // printeaza timpul petrecut in joc
    return 0;
}

int meniu(const char **array,const int row, const int col, const int lungimesir, const int width, int lungimemeniu, int selection)
        {
        int counter,offset=0,ky=0;
        char formatstring[7];
        curs_set(0);
        WINDOW *win = newwin(24,80,0,0); // creez fereastra 
        box(win, '$', '$'); // creez conturul 
        start_color();
        init_pair(2, COLOR_YELLOW, COLOR_MAGENTA);
        if (lungimesir < lungimemeniu)
                lungimemeniu=lungimesir;

        if (selection > lungimemeniu)
                offset=selection-lungimemeniu+1;

        sprintf(formatstring,"%%-%ds",width); // aliniaza liniile 
       
        while(ky != 27)
            {
                    
            for (counter=0; counter < lungimemeniu; counter++)
                    {
                            
                    if (counter+offset==selection)
                            attron(A_REVERSE);
                    mvprintw(row+counter,col,formatstring,array[counter+offset]);
                    attroff(A_REVERSE);
                            touchwin(win);
                            wrefresh(win);
                    }
            
            ky=getch();
            switch(ky)
                    {
                    case KEY_UP:
                            if (selection)
                                    {
                                    selection--;
                                    if (selection < offset)
                                            offset--;
                                    }
                            break;
                    case KEY_DOWN:
                            if (selection < lungimesir-1)
                                    {
                                    selection++;
                                    if (selection > offset+lungimemeniu-1)
                                            offset++;
                                    }
                            break;
                    case KEY_HOME:
                            selection=0;
                            offset=0;
                            break;
                    case KEY_END:
                            selection=lungimesir-1;
                            offset=lungimesir-lungimemeniu;
                            break;
                    case KEY_PPAGE:
                            selection-=lungimemeniu;
                            if (selection < 0)
                                    selection=0;
                            offset-=lungimemeniu;
                            if (offset < 0)
                                    offset=0;
                            break;
                    case KEY_NPAGE:
                            selection+=lungimemeniu;
                            if (selection > lungimesir-1)
                                    selection=lungimesir-1;
                            offset+=lungimemeniu;
                            if (offset > lungimesir-lungimemeniu)
                                    offset=lungimesir-lungimemeniu;
                            break;
                    case 10: 
                            return selection;
                            break;
                    case KEY_F(1): // functia  KEY 1
                            return -1;
                    case 27: 
                            ky=getch();
                            if (ky == 27)
                                    {
                                    curs_set(0);
                                    mvaddstr(9,77,"   ");
                                    return -1;
                                    }
                            else
                                    if (ky=='[')
                                            {
                                            getch();
                                            getch();
                                            }
                                    else 
                                            ungetch(ky);
                    }
            }
        return -1;
    }

int newgame(int *a,int *b,int *c,int *d,int *e,int *f,int i,int score,int folosire505,int nextquestion,int nrgresite,int nrcorecte,int *firstRun,int argc,char **argv,int *max) // functia incepe un joc nou 
{

                                FILE *pFile;   
                                char buffer[BUFMAX] ,ch ,random ,s;
                                int altcaracter,variante2,timeSpentPlaying,linie;                          
                                 WINDOW *w;
                                    initscr(); // initializeaza Ncurses
                                    clear();
                                    w = newwin(24, 80, 0, 0); // creeaza o fereastra
                                    start_color();
                                    init_pair(1, COLOR_GREEN, COLOR_RED);
                                    noecho();
                                    ch=getch();
                                    timeSpentPlaying=0;
                                    
                                                while((i<argc)&&(ch!='q'))
                                                {
                                                    if (i==-1)
                                                    {
                                                        clear();
                                                        refresh();
                                                        mvprintw(4,4,"%s","Scorul obtinut:");
                                                        mvprintw(4,39,"%d",score);
                                                        mvprintw(5,4,"%s","Nr gresite:");
                                                        mvprintw(5,39,"%d",nrgresite);
                                                        mvprintw(6,4,"%s","Nr corecte:");
                                                        mvprintw(6,39,"%d",nrcorecte);
                                                        mvprintw(9,4,"%s","Apasa 'q' pentru a iesi");
                                                        wrefresh(w);
                                                        if (score > *max)
                                                        {
                                                            *max=score;
                                                        }
                                                        
                                                            
                                                        ch=getch();

                                                       
                                                    }
                                                    else
                                                        {pFile = fopen(argv[i],"r");
                                                            if (pFile == NULL)
                                                                fprintf(stderr, "Eroare! Nu am putut deschide fisierul!");
                                                            else
                                                                {
                                                                    linie=0;
                                                                    while(fgets(buffer, BUFMAX, pFile) != NULL)
                                                                        {
                                                                            linie+=1;
                                                                            switch(linie){ // printeaza intrebarile pe ecran cu variantele de raspuns 
                                                                            case 1:
                                                                                    mvprintw(4,4,"%s",buffer);
                                                                                    break;
                                                                            case 2:
                                                                                    s=buffer[0];
                                                                                    break;
                                                                           case 3:
                                                                                    mvprintw(6,2,"%s",buffer);
                                                                                    
                                                                                    break;
                                                                            case 4:
                                                                                    mvprintw(6,39,"%s",buffer);
                                                                                    
                                                                                    break;
                                                                            case 5:
                                                                                    mvprintw(8,2,"%s",buffer);
                                                                                    
                                                                                    break;
                                                                            case 6:
                                                                                    mvprintw(8,39,"%s",buffer);
                                                                                    
                                                                                    break;        
                                                                            default:
                                                                                break;
                                                                            }

                                                                        }
                                                                   if(i==(argc-1))
                                                                        i=-1;
                                                                    else
                                                                        i++;
                                                                    
                                                                    printTime(*firstRun,&timeSpentPlaying);
                                                                    printScore(*firstRun,&score);
                                                                        ch=getch();
                                                                        altcaracter=0;
                                                                    if(ch!='q')
                                                                    {   
                                                                        do{
                                                                              
                                                                                switch(ch){
                                                                                case 'a': 
                                                                                        if ((score>0)&&(ch!=s))
                                                                                            {
                                                                                                score-=5;
                                                                                                nrgresite++;
                                                                                            }else
                                                                                                if(ch==s)
                                                                                                    {
                                                                                                        score+=10;
                                                                                                        nrcorecte++;
                                                                                                    }
                                                                                        if((score==0)&&(ch!=s))
                                                                                        { 
                                                                                            nrgresite++;
                                                                                        }
                                                                                        clear();
                                                                                        printScore(*firstRun,&score);
                                                                                        printTime(*firstRun,&timeSpentPlaying);
                                                                                        altcaracter=0;
                                                                                        break;
                                                                                case 'b':
                                                                                        if ((score>0)&&(ch!=s))
                                                                                        {
                                                                                            score-=5;
                                                                                            nrgresite++;
                                                                                        }
                                                                                        else
                                                                                            if (ch==s)
                                                                                            {
                                                                                                score+=10;
                                                                                                nrcorecte++;
                                                                                            }
                                                                                        if((score==0)&&(ch!=s))
                                                                                        { 
                                                                                            nrgresite++;
                                                                                        }
                                                                                        clear();
                                                                                        printScore(*firstRun,&score);
                                                                                        printTime(*firstRun,&timeSpentPlaying);
                                                                                        altcaracter=0;
                                                                                        break;
                                                                                case 'c':
                                                                                        if ((score>0)&&(ch!=s))
                                                                                        {
                                                                                            score-=5;
                                                                                            nrgresite++;
                                                                                        }
                                                                                        else
                                                                                            if (ch==s)
                                                                                            {
                                                                                                score+=10;
                                                                                                nrcorecte++;
                                                                                            }
                                                                                        if((score==0)&&(ch!=s))
                                                                                        { 
                                                                                            nrgresite++;
                                                                                        }
                                                                                        clear();
                                                                                        printScore(*firstRun,&score);
                                                                                        printTime(*firstRun,&timeSpentPlaying);
                                                                                        altcaracter=0;
                                                                                        break;
                                                                                case 'd':
                                                                                        if ((score>0)&&(ch!=s))
                                                                                        {
                                                                                            score-=5;
                                                                                            nrgresite++;
                                                                                        }
                                                                                        else
                                                                                            if (ch==s)
                                                                                            {
                                                                                                score+=10;
                                                                                                nrcorecte++;
                                                                                            }
                                                                                        if((score==0)&&(ch!=s))
                                                                                        { 
                                                                                            nrgresite++;
                                                                                        }
                                                                                        clear();
                                                                                        printScore(*firstRun,&score);
                                                                                        printTime(*firstRun,&timeSpentPlaying);
                                                                                        altcaracter=0;
                                                                                        break;
                                                                                case 'f':// in caz ca apasa f activeaza 50/50
                                                                                        if(folosire505==0)
                                                                                        {
                                                                                                variante2=0;  
                                                                                                
                                                                                                do
                                                                                                    {
                                                                                                        srand(time(NULL));
                                                                                                        random=rand()%(101-97)+97;
                                                                                                        if (random!=s)
                                                                                                        {
                                                                                                            variante2++;
                                                                                                            switch(random){
                                                                                                            case 'a':
                                                                                                                    clear();
                                                                                                                    mvprintw(6,2,"%s",'-');
                                                                                                                    break;
                                                                                                            case 'b':
                                                                                                                    clear();
                                                                                                                    mvprintw(6,39,"%s",'-');
                                                                                                                    break;
                                                                                                            case 'c':
                                                                                                                    clear();
                                                                                                                    mvprintw(8,2,"%s",'-');
                                                                                                                    break;
                                                                                                            case 'd':
                                                                                                                    clear();
                                                                                                                    mvprintw(8,39,"%s",'-');
                                                                                                                    break;

                                                                                                            }

                                                                                                        }
                                                                                                    }while (variante2!=2);
                                                                                                  folosire505=1;
                                                                                        }else
                                                                                            {
                                                                                                ch=getch();
                                                                                                altcaracter=1;
                                                                                            }
                                                                                        break;
                                                                                case 'n'://activeaza n 
                                                                                        if ((nextquestion==0)&&(i!=(argc-1)))
                                                                                            {
                                                                                                nextquestion=1;
                                                                                            }else
                                                                                                {
                                                                                                    ch=getch();
                                                                                                    altcaracter=1;
                                                                                                }
                                                                                        break;
                                                                                case 'q':
                                                                                        *a=i;
                                                                                        *b=score;
                                                                                        *c=folosire505;
                                                                                        *d=nextquestion;
                                                                                        *e=nrcorecte;
                                                                                        *f=nrgresite;
                                                                                        *firstRun=1;
                                                                                        altcaracter=0;
                                                                                        break;
                                                                                             
                                                                                default:
                                                                                        altcaracter=1;
                                                                                        ch=getch();
                                                                                }
                                                                            
                                                                        }while(altcaracter==1);
                                                                    }
                                                                    wrefresh(w);
                                                                    fclose (pFile);
                                                                   


                                                                }
                                                            }
                                                        
                                                }
                                            
                                             keypad(w, TRUE); //permite ca tastatura sa fie folosita in fereastra
                                                curs_set(0); 
                                                refresh();
                                                getch();
                                                endwin();

    return 0;
}

int main(int argc,char *argv[])
        {
            FILE *pFile;
        char buffer[BUFMAX] ,ch ,random ,s;    
        int selection ,row=10 ,max=0, col=34, lungimesir=5, width=5, lungimemeniu=5, firstRun=0; 
        int timeSpentPlaying=0 ,score=0 ,altcaracter=0 ,folosire505=0 ,variante2=0 ,linie=0 ,i=1,nrgresite=0,nrcorecte=0,nextquestion=0,a,b,c,d,e,f;
        const char *elementemeniu[]={"New Game","Resume Game","Score","Help","Quit"};
        RESUMEGAME r;
        initscr();
        noecho();
        keypad(stdscr,TRUE);
            selection=meniu(elementemeniu,row,col,lungimesir,width,lungimemeniu,3);

        while(selection!=4)// cat timp nu apasa pe quit se executa 
                {
                        switch(selection){
                        case 0: // selecteaza new game se seteaza datele pentru resume si apeleaza functia newgame 
                                r.ir=1;
                                r.scorer=0;
                                r.folosire505r=0;
                                r.nextquestionr=0;
                                r.nrcorecter=0;
                                r.nrgresiter=0;
                                firstRun=0;
                                newgame(&r.ir,&r.scorer,&r.folosire505r,&r.nextquestionr,&r.nrcorecter,&r.nrgresiter,i,score,folosire505,nextquestion,nrcorecte,nrgresite,&firstRun,argc,argv,&max); 
                                firstRun=1;
                                break;
                        case 1: // selecteaza resume game si sa jucat jocul da resume cu datele salvate anterior 
                                if (firstRun==1)
                                    {  
                                       newgame(&i,&score,&folosire505,&nextquestion,&nrcorecte,&nrgresite,r.ir,r.scorer,r.folosire505r,r.nextquestionr,r.nrcorecter,r.nrgresiter,&firstRun,argc,argv,&max); 
                                    }  
                                break;
                        case 2:// selecteza score si afiseaza o fereastra cu scorul maxim inregistrat 
                            	scormaxim(max,'a');  
                                break;
                        case 3:// selecteaza help deschide o fereastra cu informati despre modul de jucare 
                                doHelp();
                }

                        
                        selection=meniu(elementemeniu,row,col,lungimesir,width,lungimemeniu,3);// printeaza meniul 
                        refresh();
        }
        endwin();// inchide fereastra 
        return 1;
        }
