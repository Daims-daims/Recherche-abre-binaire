#include <stdio.h>
#include <stdlib.h>

int nb_exemple,nb_features,nb_noeuds;
int deb_Pjn,deb_Vkn,deb_Fkn,deb_Cik, deb_CTerm, deb_Tjk;


int getPjk(int j, int k) {
    return (k-1)*nb_features + j;
}

int getVjk(int j, int k) {
    if ( k <= nb_noeuds ) {
        return deb_Vkn + nb_noeuds * ( j-1) - j*(j-1)/2 + k-j;
    }
    else{
        return deb_Vkn + nb_noeuds*(nb_noeuds-1)/2 + j + ( nb_noeuds * ( k - (nb_noeuds + 1)));
    }
}

int getFjk(int j, int k){
    if(k<=nb_noeuds){
        return deb_Fkn + nb_noeuds * ( j-1) - j*(j-1)/2 + k-j;
    }else{
        return deb_Fkn + nb_noeuds*(nb_noeuds-1)/2 + j + ( nb_noeuds * ( k - (nb_noeuds + 1)));
    }
}

int getCik(int i, int k){
    if ( k <= nb_noeuds ) {
        return deb_Cik + (nb_noeuds-1)*(i-1) + (k-1);
    }
    return deb_CTerm  + i + ( nb_exemple * ( k - (nb_noeuds + 1)));
}

int getTjk(int j, int k) {
    if ( k <= nb_noeuds ) 
        return deb_Tjk + nb_noeuds * ( j-1) - j*(j-1)/2 + k-j;
    
    else
        return deb_Tjk + nb_noeuds*(nb_noeuds-1)/2 + j + ( nb_noeuds * ( k - (nb_noeuds + 1)));
}

int nb_clause_unicite_Vkn(int nb_noeuds) {
    if ( nb_noeuds = 0 )
        return 0;
    return ( (nb_noeuds+1) * nb_noeuds ) + nb_clause_unicite_Vkn(nb_noeuds-1);
}

int main (int argc, char *argv[]) {
    
    if (argc != 5) {
        printf("Usage : %s <Nombre exemple> <Nb ex positif> <nb_features> <Nb noeuds>\n", argv[0]);
        exit(1);
    }
    
    nb_exemple = atoi(argv[1]);
    int nb_exemple_pos = atoi(argv[2]);
    nb_features = atoi(argv[3]);
    nb_noeuds = atoi(argv[4]);
    
    int val_Eij[nb_exemple][nb_features];
    int val_Ei[nb_exemple];
    
    FILE* fichier = NULL;
    fichier = fopen("test3.txt","r");
    
    // remplissage de val_Ei et val_Eij
    
    if ( fichier != NULL ) {
        
        int i = 0;
        int carac_courant = fgetc(fichier);
        
        while ( carac_courant != 10 ) { // 10 = saut de ligne en ascii; 32 = espace en ascii; je fais - 48 après car en ascii 0 = 48 et 1 = 49
            if ( carac_courant != 32 ) { 
                val_Ei[i] = carac_courant-48;
                i++;
            }
            carac_courant = fgetc(fichier);
        }
        
        i=0;
        int j;
        carac_courant = fgetc(fichier);
        
        while ( carac_courant != EOF ) {
            j=0;
            while ( carac_courant != 10 ) {
                if ( carac_courant != 32 ) {
                    val_Eij[i][j]=carac_courant-48;
                    j++;
                }
                carac_courant = fgetc(fichier);
            }
            carac_courant = fgetc(fichier);
            i++;
        }
    }
    
    // affichage des tableaux de bool
    
    /*
    for ( int i=0; i<nb_exemple; i++) {
        printf("%d",val_Ei[i]);
    }
    printf("\n");
    for ( int i=0; i<nb_exemple; i++) {
        for ( int j=0; j<nb_features; j++) {
            printf("%d",val_Eij[i][j]);
        }
        printf("\n");
    }
    */
    
    else {
        perror("problème lecture fichier");
        exit(3);
    }
    
    int nb_Vkn = ( nb_noeuds * (nb_noeuds - 1 ) )/2;
    int nb_variable = nb_features*nb_noeuds + 3 * nb_Vkn + 6 * nb_noeuds + nb_exemple * ( nb_noeuds - 1) + nb_exemple * 2;
    int nb_clause = 10000; //nb_noeuds + (nb_features * (nb_features-1))/2 * nb_noeuds + nb_noeuds*4 + nb_clause_unicite_Vkn(nb_noeuds)*2 + nb_noeuds-1 + ;
    
    // Création du fichier qui sera rempli au format sat
    
    char NomFichier[30];
    printf("\nVeuillez maintenant rentrer le nom du fichier qui sera rempli au format sat ( max 30 caractères ) : ");
    scanf("%s",NomFichier);
    FILE* f;
    f = fopen(NomFichier, "w" );
    if ( f == NULL ) {
        perror(NomFichier);
        exit(2);
    }
    
    fprintf(f,"p cnf %d %d \n",nb_variable,nb_clause); // on remplit la première ligne au format sat
    
    // Les variables
    
    /*
     * Les Pjn si le j-ième feature est dans le n-ième noeud ( de 1 à nb_features * nb_noeuds )
     * P11 -> P21
     * Les Vkn si avec 1 on va du k-ième noeud dans le n-ième noeud
     * V12 -> V13
     * Vkp si avec 1 on va du k-ième noeud dans une feuille positive
     * Vkm si avec 1 on va du k-ième noeud dans une feuille négative
     * les Fkn si avec 0 on va du k-ième noeud dans le n-ième noeud
     * Fkp si avec 0 on va du k-ième noeud dans une feuille positive
     * Fkm si avec 0 on va du k-ième noeud dans une feuille négative
     * Cik si l'exemple i s'est frayé un chemin jusqu'au noeud k
     * C11 -> C12
     * Cip si i renvoie vers une feuille positive
     * Cim si i renvoie vers une feuille négative
    */
    
    // Existence Pij

    for(int j=1;j<=nb_noeuds;j++){
        for(int i =1;i<=nb_features;i++){
            fprintf(f,"%d ",getPjk(i,j));
        }
        fprintf(f,"0\n");
    }

    // Unicité Pij

    for(int i = 1 ; i<=nb_noeuds ; i++){
        for(int j=2 ; j<=nb_features-1;j++){
            for(int j_p=j+1 ; j_p<=nb_features;j_p++){
                fprintf(f,"%d %d 0\n",-getPjk(i,j),-getPjk(i,j_p));
            }
        }
    }

    // Existence Vij 

    for(int i=1 ; i<=nb_noeuds ; i++){
        for(int j = i+1 ; j<=nb_noeuds+2 ; j++ ) {
            fprintf(f,"%d ",getVjk(i,j));
        }
        fprintf(f,"0\n");
    }

    // Unicité Vij

    for(int i = 1 ; i<=nb_noeuds ; i++){
        for(int j=i+1; j<=nb_noeuds+1;j++){
            for(int j_p=j+1; j_p<=nb_noeuds+2;j_p++){
                fprintf(f,"%d %d 0\n",-getVjk(i,j),-getVjk(i,j_p));
            }
        }
    }

    // Existence Fij 

    for(int i=1 ; i<=nb_noeuds ; i++){
        for(int j = i+1 ; j<=nb_noeuds+2 ; j++ ) {
            fprintf(f,"%d ",getFjk(i,j));
        }
        fprintf(f,"0\n");
    }

    // Unicité Fij

    for(int i = 1 ; i<=nb_noeuds ; i++){
        for(int j=i+1; j<=nb_noeuds+1;j++){
            for(int j_p=j+1; j_p<=nb_noeuds+2;j_p++){
                fprintf(f,"%d %d 0\n",-getFjk(i,j),-getFjk(i,j_p));
            }
        }
    }

    //Existence du père

    for(int j=2 ; j<=nb_noeuds+2;j++){
        for(int i =1;i<=j-1;i++){
            fprintf(f,"%d %d ",-getVjk(i,j),-getFjk(i,j));
        }
        fprintf(f,"0\n");
    }
     
     //Unicité du père

     for(int j=3;j<=nb_noeuds;j++){
         for(int i =1;i<=j-1;i++){
             for(int i_p = 1 ;i_p<=j-1;i_p++){
                 fprintf(f,"%d %d 0\n",-getVjk(i_p,j),-getVjk(i,j));
                 fprintf(f,"%d %d 0\n",-getVjk(i_p,j),-getFjk(i,j));
                 fprintf(f,"%d %d 0\n",-getFjk(i_p,j),-getVjk(i,j));
                 fprintf(f,"%d %d 0\n",-getFjk(i_p,j),-getFjk(i,j));
             }
         }
     }


     //--------------------- Cik -------------------------

    // si de j on peut passer à k, Cij => Cik (Cas j!=1)

    for(int i=1;i<=nb_exemple;i++){
        for(int j=2;j<=nb_noeuds;j++){
            for(int k=j+1;k<=nb_noeuds+2;k++){
                for(int m=1 ; m<=nb_features;m++){
                    if(val_Eij[i-1][m-1]==1){
                        fprintf(f,"%d %d %d %d 0\n",-getCik(i,j),-getVjk(j,k),-getPjk(m,j),getCik(i,k));
                    }
                    else{
                        fprintf(f,"%d %d %d %d 0\n",-getCik(i,j),-getFjk(j,k),-getPjk(m,j),getCik(i,k));
                    }
                }
            }
        }
    }

    // Cas j=1

    for(int i=1;i<=nb_exemple;i++){
        for(int k=2;k<=nb_noeuds+2;k++){
            for(int m=1 ; m<=nb_features;m++){
                if(val_Eij[i-1][m-1]==1){
                    fprintf(f,"%d %d %d 0\n",-getVjk(1,k),-getPjk(m,1),getCik(i,k));
                }
                else{
                    fprintf(f,"%d %d %d 0\n",-getFjk(1,k),-getPjk(m,1),getCik(i,k));
                }
            }
        }
    }


    // Tout exemple positif renvoie vrai et inversement

    for(int i = 1 ;i<=nb_exemple;i++){
        if(val_Ei[i-1]==1){
            fprintf(f,"%d 0\n",getCik(i,nb_noeuds+1));
            fprintf(f,"%d 0\n",-getCik(i,nb_noeuds+2));
        }
        else{
            fprintf(f,"%d 0\n",-getCik(i,nb_noeuds+1));
            fprintf(f,"%d 0\n",getCik(i,nb_noeuds+2));
        }
    } 



    // --------------  Tjk ----------------------
    
    // Tjk => Vjk V Fjk

    for(int j=1; j<=nb_noeuds;j++){
        for(int k=j+1;k<=nb_noeuds+2;k++){
            fprintf(f,"%d %d %d 0\n",-getTjk(j,k),getVjk(j,k),getFjk(j,k));
        }
    }

    // -Vjk V -Fjk => -Tjk

    for(int j=1 ; j<=nb_noeuds ; j++){
        for(int k=j+1 ; k<=nb_noeuds+2; k++){
            fprintf(f,"%d %d 0\n",getTjk(j,k),-getVjk(j,k));
            fprintf(f,"%d %d 0\n",getTjk(j,k),-getFjk(j,k));
        }
    }

    // on ne peut avoir un chemin menant à 2 noeuds voisins ( Cas j!= 1 )

    for(int i=1; i<=nb_exemple ;i++){
        for(int j=2; j<=nb_noeuds ; j++){
            for(int k=j+1;k<=nb_noeuds+1;k++){
                for(int k_p=k+1;k_p<=nb_noeuds+2;k_p++){
                    fprintf(f,"%d %d %d %d %d 0\n",-getCik(i,j),-getTjk(j,k),-getCik(i,k),-getTjk(j,k_p),-getCik(i,k_p));
                }
            }
        }
    }
    
    // Cas j=1

    for(int i=1; i<=nb_exemple ;i++){
        for(int k=2;k<=nb_noeuds+1;k++){
            for(int k_p=k+1;k_p<=nb_noeuds+2;k_p++){
                fprintf(f," %d %d %d %d 0\n",-getTjk(1,k),-getCik(i,k),-getTjk(1,k_p),-getCik(i,k_p));
            }
        }
    }
    

    // Chemin jusqu'au fils => Chemin jusqu'au pere ( Cas j!=1 (inutile) )

    for(int i =1; i<=nb_exemple ;i++){
        for(int j=2;j<=nb_noeuds;j++){
            for(int k=j+1;k<=nb_noeuds+2;k++){
                fprintf(f,"%d %d %d 0\n",-getCik(i,j),-getTjk(k,j),getCik(i,k));
            }
        }
    }

}






















/* Ordre:

    Cij : C12 C13 C14 C22 C23 C24 C 1+ C2+ C1- C2- */















