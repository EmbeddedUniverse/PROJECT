
#include "Menu.h"



void runMenu(void){
       unsigned short commandMenu = 0;
       bool endMenu = false;
       while(!endMenu) {

           printf("\n------------------------------------------------------------------------------------------------------------------------------\n");
           printf("\t\t\t\t   Bienvenue dans le clone trainer de EmbededUniverse ");
           printf("\n------------------------------------------------------------------------------------------------------------------------------\n");
           printf("\t1 - Choisir le nombre de balles | 2 - Choisir le temps de la partie | 3 - Instruction | 4 - Commencer la partie!");
           printf("\n------------------------------------------------------------------------------------------------------------------------------\n");
           printf("\n\n->Command : ");
           scanf("%u", &commandMenu);

           if(commandMenu == 1)
               choisirNbrBalle();

           else if(commandMenu == 2)
               choisrTemps();

           else if(commandMenu == 3)
               instruction();

           else if(commandMenu == 4)
               endMenu = true;


           else
               printf("**************INVALID REQUEST**************\n");

       }

}

void choisirNbrBalle(){
    unsigned short command = 0;
    printf("\n------------------------------------------------------\n");
    printf("\t   Combien de munitions par chargeur voulez-vous? ");
    printf("\n------------------------------------------------------\n");
    printf("\t1 - 6 balles | 2 - 12 balles | 3 - 24 balles");
    printf("\n------------------------------------------------------\n");
    printf("\n\n->Command : ");
    scanf("%d", &command);

    switch(command){
        case 1:
            printf("\n\n-------6 munitions selectionnées------\n\n");
            COM_send(_6Ammo,PIC);
            break;

        case 2:
            printf("\n\n-------12 munitions selectionnées------\n\n");
            COM_send(_12Ammo,PIC);
            break;

        case 3:
            printf("\n\n-------24 munitions selectionnées------\n\n");
            COM_send(_24Ammo,PIC);
            break;

    }
}
void choisrTemps(){
    unsigned short command = 0;
        printf("\n------------------------------------------------------\n");
        printf("\t   Combien de temps par partie voulez-vous? ");
        printf("\n------------------------------------------------------\n");
        printf("\t1 - 30 sec | 2 - 60 sec | 3 - 120 sec");
        printf("\n------------------------------------------------------\n");
        printf("\n\n->Command : ");
        scanf("%d", &command);

        switch(command){
            case 1:
                printf("\n\n-------30 secondes selectionnées------\n\n");
                COM_send(_30secTimer,PIC);
                break;

            case 2:
                printf("\n\n-------60 secondes selectionnées------\n\n");
                COM_send(_60secTimer,PIC);
                break;

            case 3:
                printf("\n\n-------120 secondes selectionnées------\n\n");
                COM_send(_120secTimer,PIC);
                break;

        }

}
void instruction(){
          printf("\n------------------------------------------------------------------------------------------------------------------------------\n");
          printf("\t\t\t\t\t   Le but du jeu est simple : ");
          printf("\n------------------------------------------------------------------------------------------------------------------------------\n");
          DSK6713_waitusec(10000);
          printf("\n\t\t\t\t   Tirer les cible pour accumuler des points : \n");
          DSK6713_waitusec(20000);
          printf("\n\t\t\t\t   Bon doit être le mode de tir pour que le tir soit valide \n");
          DSK6713_waitusec(20000);
          printf("\n\t\t\t\t   De gauche à droite le mouvement doit être pour un changement de mode \n");
          DSK6713_waitusec(20000);
          printf("\n\t\t\t\t   De Haut en bas le movement doit être pour recharger \n");
          DSK6713_waitusec(20000);
          printf("\n\t\t\t\t   Pew doit être crier pour tirer \n");
          DSK6713_waitusec(20000);
          printf("\n\t\t\t\t   Bonne Partie \n");
          DSK6713_waitusec(30000);
}

