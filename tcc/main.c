#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void show_ur_fct_depend(char **A, int Nb_fct_depend) {
    int i;
    for (i = 0 ; i < Nb_fct_depend ; i++) {
        printf(" %c --> %c\n", A[i][0], A[i][1]);
    }
}


char **alloc_2D_Tab(int Nb_fct_depend) {
    char **A;
    int i;
    A = (char **) calloc(Nb_fct_depend,sizeof(char *));
    if(A==NULL) {
        printf("\n Error !! unable to allocate memory ");
        exit(22);
    }
    for (i = 0; i < Nb_fct_depend ; i++) {
        A[i] = (char *) calloc(2,sizeof(char));
        if(A[i]==NULL) {
            printf("\n Error !! unable to allocate memory %d ",i);
            exit(22+i);
        }
    }
    return A;
}
char **init_fct_depend(char **Table_2D,int Nb_fct_depend, char attributs[10]) {
    int i, j, z ;
    Table_2D = alloc_2D_Tab(Nb_fct_depend);
    printf("\n --> Enter Attributes of Functional Dependencies : \n\n");
    for (i = 0; i < Nb_fct_depend ; i++) {
        for (j = 0; j < 2; j++) {
            int found = 0;
            do {
                printf("[%d][%d] = ", i, j);
                scanf(" %c", &Table_2D[i][j]);
                for (z = 0; z < 10; z++) {
                    if (attributs[z] == Table_2D[i][j]) {
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    printf("\nInvalid input. Please enter a character from the attributs array.\n");

            } while (!found);

        }
    }
    return Table_2D;
}


char *calcul_right_side(char attribut,char **fct_dependances,int N) {
    char *resulat_final = (char) malloc(30 * sizeof(char));
    int i,y,z=0;
    for (y = 0; y < N; y++) {
        if (attribut==fct_dependances[y][0]) {
        resulat_final[z]=fct_dependances[y][1];
            z++;
            char *resulat= calcul_right_side(fct_dependances[y][1],fct_dependances,N);
            for(i=0; i<strlen(resulat); i++) {
                if (strchr(resulat_final, resulat[i]) == NULL) {
                    resulat_final[z] = resulat[i];
                    z++;
                }
            }
        }
    }
    resulat_final[z] = '\0';
    return resulat_final;
}

char *calculer_closure(char set[15], char **fct_dependances, int Nb_fct_depend) {
    char *closure;
    char *closure_final=(char)malloc(100*sizeof(char));
    int i,j,z,k=0;
    for (j = 0; set[j] != '\0'; j++) {
        for ( i = 0; i < Nb_fct_depend; i++) {
            if(fct_dependances[i][0] == set[j]) {
                if (strchr(set, fct_dependances[i][1]) == NULL) {
                    closure_final[k] = fct_dependances[i][1];
                    k++;
                    closure = calcul_right_side(fct_dependances[i][1],fct_dependances, Nb_fct_depend);
                    for(z=0; z<strlen(closure) ; z++) {
                        if (strchr(closure_final,closure[z]) == NULL) {
                            closure_final[k] = closure[z];
                            k++;
                        }
                    }
                }
            }
        }
    }
    closure_final[k] = '\0';
    return closure_final;
}



char **transfer_relation_fct(char Relation[], char **fct_dependances, int Nb_fct_depend , int Taille) {
    int i, j;
    char **array_2D = alloc_2D_Tab(Taille);
    int index = 0;
    for (i = 0; i < strlen(Relation); i++) {
        for (j = 0; j < Nb_fct_depend; j++) {
            if (Relation[i] == fct_dependances[j][0] && fct_dependances[j][1] && strchr(Relation, fct_dependances[j][1])) {
                array_2D[index][0] = fct_dependances[j][0];
                array_2D[index][1] = fct_dependances[j][1];
                index++;
                break;
            }
        }
    }
    return array_2D;
}





char found_Key(char Relation[], char **fct_dependances, int Nb_fct_depend) {
    int i, j;
    int NB=strlen(Relation);
    char *closure = (char *) malloc(10 * sizeof(char));
    char key ='\0';
    for (j=0 ; Relation[j] != '\0'; j++) {
        key = Relation[j];
        closure = calculer_closure(&key, fct_dependances, Nb_fct_depend);
       int N = strlen(closure);
       N=N+1;
        if ( N == NB) {
            char result = key;
            free(closure);
            return result;
            break;
        }
    }
    return '\0';
}




char test_left_H_S(char Relation[], char **fct_dependances, int Nb_fct_depend) {
    int i;
    char KEY;
    KEY = found_Key(Relation, fct_dependances, Nb_fct_depend);
    for (i = 0; i < Nb_fct_depend; i++) {
        if (fct_dependances[i][0] != KEY) {
            return fct_dependances[i][0];
        }
    }
    return '\0';
}



int check_result(char result1[], char result2[], char **fct_dependances, int Nb_fct_depend){
    int i,j;
    char X,Y;
char **Table_2D=NULL;
    int Taille = 0;
    for (i = 0; i < strlen(result1); i++) {
        for (j = 0; j < Nb_fct_depend; j++) {
            if (result1[i] == fct_dependances[j][0] && strchr(result1, fct_dependances[j][1])) {
                Taille++;
                break;
            }
        }
    }
    Table_2D=transfer_relation_fct(result1, fct_dependances, Nb_fct_depend, Taille);
    X = test_left_H_S(result1, Table_2D, Taille);
    Table_2D=NULL;
    Taille = 0;
    for (i = 0; i < strlen(result2); i++) {
        for (j = 0; j < Nb_fct_depend; j++) {
            if (result2[i] == fct_dependances[j][0] && strchr(result2, fct_dependances[j][1])) {
                Taille++;
                break;
            }
        }
    }
    Table_2D=transfer_relation_fct(result2, fct_dependances, Nb_fct_depend, Taille);
    Y = test_left_H_S(result2, Table_2D, Taille);
    if (X == NULL && Y==NULL) {
        printf("The Key is on the left-hand side in your functions dependencies!\n");
        printf("All Relations are on 'BCNF'\n");
        return 1;
    }
}


void test_the_result(char **fct_dependances, int Nb_fct_depend, char result1[], char result2[] ) {
    int i, y, j, z = 0, k = 0;
    char **Table_2D=NULL;
    int Taille = 0;
    for (i = 0; i < strlen(result1); i++) {
        for (j = 0; j < Nb_fct_depend; j++) {
            if (result1[i] == fct_dependances[j][0] && strchr(result1, fct_dependances[j][1])) {
                Taille++;
                break;
            }
        }
    }
    Table_2D=transfer_relation_fct(result1, fct_dependances, Nb_fct_depend, Taille);
    char X;
    char *result_R1 , *result_R2;
    result_R1 = (char *) malloc(10 * sizeof(char));
    result_R2 = (char *) malloc(10 * sizeof(char));
    X = test_left_H_S(result1, Table_2D, Taille);
    if(X!=NULL){
    result_R1 = calculer_closure(&X, Table_2D, Taille);
    for (i = 0; i < strlen(result1) ; i++) {
        int is_present_in_R1 = 0;
        for (j = 0; j < strlen(result_R1) ; j++) {
            if (result1[i] == result_R1[j]) {
                is_present_in_R1 = 1;
                break;
            }
        }
        if (!is_present_in_R1) {
            result_R2[z++] = result1[i];
        }
    }
    strncat(result_R1, &X, 1);
    result_R2[z++]='\0';
    printf("R1(%c",result_R1[0]);
    k=1;
    while(result_R1[k] != '\0') {
        printf(", %c",result_R1[k]);
        k++;
    }
    printf(")\n");
    printf("R2(%c",result_R2[0]);
    for (k = 1; k < z && result_R2[k] != '\0' ; k++) {
        printf(", %c",result_R2[k]);
    }
    printf(")");
    printf("\n%s\n",result_R1);
    printf("%s\n",result_R2);

    free(result_R1);
    free(result_R2);
    }else{
    printf("The Key is on the left-hand side in your functions dependencies11\n");
    }
    Table_2D=NULL;
    Taille = 0;
    for (i = 0; i < strlen(result2); i++) {
        for (j = 0; j < Nb_fct_depend; j++) {
            if (result2[i] == fct_dependances[j][0] && strchr(result2, fct_dependances[j][1])) {
                Taille++;
                break;
            }
        }
    }
    Table_2D=transfer_relation_fct(result2, fct_dependances, Nb_fct_depend, Taille);
    show_ur_fct_depend(Table_2D,Taille);

    X = test_left_H_S(result2, Table_2D, Taille);
    if(X==NULL){
    printf("The Key is on the left-hand side in your functions dependencies!\n");
    exit(22);
    }
    result_R1 = calculer_closure(&X, Table_2D, Taille);
    for (i = 0; i < strlen(result2) ; i++) {
        int is_present_in_R1 = 0;
        for (j = 0; j < strlen(result_R1) ; j++) {
            if (result2[i] == result_R1[j]) {
                is_present_in_R1 = 1;
                break;
            }
        }
        if (!is_present_in_R1) {
            result_R2[z++] = result2[i];
        }
    }
    strncat(result_R1, &X, 1);
    result_R2[z++]='\0';
    printf("R1(%c",result_R1[0]);
    k=1;
    while(result_R1[k] != '\0') {
        printf(", %c",result_R1[k]);
        k++;
    }
    printf(")\n");
    printf("R2(%c",result_R2[0]);
    for (k = 1; k < z && result_R2[k] != '\0' ; k++) {
        printf(", %c",result_R2[k]);
    }
    printf(")");
    printf("\n%s\n",result_R1);
    printf("%s\n",result_R2);
    free(result_R1);
    free(result_R2);
}




void after_left_H_S(char **fct_dependances, int Nb_fct_depend, char Attributs[10]) {
    int i, y, j, z = 0, k = 0 , test;
    char X;
    char *result_R1 , *result_R2;
    result_R1 = (char *) malloc(10 * sizeof(char));
    result_R2 = (char *) malloc(10 * sizeof(char));
    X = test_left_H_S(Attributs, fct_dependances, Nb_fct_depend);
    if (X==NULL){
    printf("The Key is on the left-hand side in your functions dependencies!\n");
    printf("the Relation is on 'BCNF'\n");
    exit(22);
    }
    result_R1 = calculer_closure(&X, fct_dependances, Nb_fct_depend);
    for (i = 0; i < strlen(Attributs) ; i++) {
        int is_present_in_R1 = 0;
        for (j = 0; j < strlen(result_R1) ; j++) {
            if (Attributs[i] == result_R1[j]) {
                is_present_in_R1 = 1;
                break;
            }
        }
        if (!is_present_in_R1) {
            result_R2[z++] = Attributs[i];
        }
    }
    strncat(result_R1, &X, 1);
    result_R2[z++]='\0';
    printf("R1(%c",result_R1[0]);
    k=1;
    while(result_R1[k] != '\0') {
        printf(", %c",result_R1[k]);
        k++;
    }
    printf(")\n");
    printf("R2(%c",result_R2[0]);
    for (k = 1; k < z && result_R2[k] != '\0' ; k++) {
        printf(", %c",result_R2[k]);
    }
    printf(")");
    printf("\n%s\n",result_R1);
    printf("%s\n",result_R2);
  int Test;
    Test=check_result(result_R1, result_R2, fct_dependances, Nb_fct_depend);
    if (Test!=1) {
     test_the_result(fct_dependances, Nb_fct_depend, result_R1, result_R2);
}
}

int main() {
    char **M = NULL;
    char *result = (char *) malloc(20 * sizeof(char));
    char Name_relation[10], Attributes[10], VAL, Key;
    int NB, K=0, y=0, z=0;
    int answer;
    int choix;
    printf("\t\t\t\t ####FORME NORMALE DE BOYCE-CODD #### \n\n");
    printf("\t\t\t\t CREATOR ARE : 'ZADO KHALID' ON 27/04/2023\n\n");
    printf("\n\t\t\t\t\t   _________ \n");
    printf("\n\t\t\t\t\t           WELCOME  \n");
    printf("\n\t\t\t\t\t             -_-  \n");
    printf("\n\t\t\t\t\t   _________ ");
    printf("\n\n\t\t\t\t       ");
    system("pause");
    system("cls");
    do {
        printf("\n\t\t\t ===================================================== \n");
        printf("\n\t\t\t ---> 1.  To Treat Your Relation. \n");
        printf("\n\t\t\t ---> 2.  To Exit the program. \n");
        printf("\n\t\t\t =======================================================>  ");
        printf("Your choice : ");
        scanf("%d", &choix);
        switch (choix) {
        case 1:
            system("cls");
            //printf(" --> Please Enter Your Relation Name: ");
            //scanf("%s", Name_relation);
            printf("\n --> Please Enter Your Relation Attributs : \n");
            printf("\t\t TO STOP TYPE '0' ! \n");
            while(1) {
                printf("\n\t ---> ");
                scanf(" %c",&VAL);
                if (VAL=='0') break;
                Attributes[K]=VAL;
                K++;
            }
            Attributes[K]='\0';

            printf("\n\n\t");
            system("pause");
            system("cls");
FONCT:
            printf(" --> Your Relation : %s('%s') ",Name_relation,Attributes);

            do {
                printf("\n\n --> Enter Number of Functional Dependencies : ");
                scanf("%d", &NB);
            } while (NB < 0);
            M = init_fct_depend(M,NB,Attributes);
            printf("\n    ");
            system("pause");
            system("cls");
ATTRI :
            printf(" --> Your Relation : %s(%s)\n",Name_relation,Attributes);
            printf("\n --> Your Functional Dependencies : \n\n");
            show_ur_fct_depend(M,NB);
            printf("\n\n  ");
            system("pause");
            system("cls");
            printf("\n --> The  Relation : \n");
            after_left_H_S(M,NB,Attributes);
            printf("\n  Precedent (tape 1)     ||     Change the Fct_depend (tape 2)     ||     Menu Pricipal (tape 3) \n");
            printf("\n\t\t\t         Your choice :");
            scanf(" %d",&answer);
            if(answer == 1) {
                system("cls");
                goto ATTRI ;
            } else if (answer==2) {
                system("cls");
                goto FONCT;
            } else
                system("cls");
            break;
        case 2:
            exit(0);
        default:
            printf("\t\t\tInvalid choice !! Try Again.\n");
            printf("\t\t\t");
            system("pause");
            system("cls");
        }
    } while (choix != 2);
    return 0;
}
