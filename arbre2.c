#include <stdio.h>
#include <stdlib.h>

int nb_exemple,nb_features,nb_noeuds;
int deb_Pjn,deb_Vkn,deb_Fkn,deb_Cik, deb_CTerm;


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
        return deb_Cik + nb_noeuds * ( k-2) + i;
    }
    return deb_CTerm + i + ( nb_exemple * ( k - (nb_noeuds + 1)));
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
    int nb_variable = nb_features*nb_noeuds + 2 * nb_Vkn + 4 * nb_noeuds + nb_exemple * ( nb_noeuds - 1) + nb_exemple * 2;
    int nb_clause = 0; //à faire
    
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

    deb_Pjn= 1;
    deb_Vkn = nb_features*nb_noeuds ;
    deb_Fkn = nb_features*nb_noeuds + nb_Vkn + nb_noeuds*2 ;
    deb_Cik = nb_features*nb_noeuds + nb_Vkn*2 + nb_noeuds*4;
    deb_CTerm = deb_Cik + (nb_noeuds - 1 ) * nb_exemple;

    // Chaque features dans au plus un noeud 
    
    for ( int j = 1; j <= nb_features; j++ ) {
        for ( int n = 1; n <= nb_noeuds; n++ ) {
            for (int k = n+1; k <= nb_noeuds; k++) {
                fprintf(f,"%d %d 0\n",-getPjk(j,n),-getPjk(j,k));
            }
        }
    }
    
    // Un seul feature par noeud
    
    /* Dans chaque noeud au moins un feature */
    
    for ( int n = 1; n <= nb_noeuds; n++ ) {
        for ( int j = 1; j<= nb_features; j++ ) {
            fprintf(f,"%d ",getPjk(j,n));
        }
        fprintf(f,"0\n");
    }
    
    /* Dans chaque noeud au plus un feature */
    
    for ( int n = 1; n <= nb_noeuds; n++ ) {
        for ( int j = 1; j <= nb_features; j++ ) {
            for ( int i = j+1; i <= nb_features; i++ ) {
                fprintf(f,"%d %d 0\n",-getPjk(j,n),-getPjk(i,n));
            }
        }
    }
    
    // Unicité des fils d'un noeud k en fonction de la valeur booléenne du feature de ce noeud ( un fils pour 0 et un fils pour 1 )
    
    /* Chaque Noeud a au moins un fils avec 1 */
    
    for ( int n = 1; n <= nb_noeuds; n++ ) {
        for ( int k = n+1; k<= nb_noeuds; k++ ) {
            fprintf(f,"%d ",getVjk(n,k));
            
        }
        fprintf(f,"%d %d 0\n",getVjk(n,nb_noeuds+1),getVjk(n,nb_noeuds+2));
    }
    
    /* Chaque Noeud a au moins un fils avec 0 */
    
    for ( int n = 1; n <= nb_noeuds; n++ ) {
        for ( int k = n+1; k<= nb_noeuds; k++ ) {
            fprintf(f,"%d ",getFjk(n,k));
        }
        fprintf(f,"%d %d 0\n",getFjk(n,nb_noeuds+1),getFjk(n,nb_noeuds+2));
    }
    
    /* Max un fils avec 1 par noeud */
    
    for ( int n = 1; n <= nb_noeuds; n++ ) {
        fprintf(f,"%d %d 0\n",-getVjk(n,nb_noeuds+1),-getVjk(n,nb_noeuds+2));
        for ( int j = n+1; j <= nb_noeuds; j++ ) {
            for ( int i = j+1; i <= nb_noeuds; i++ ) {
                fprintf(f,"%d %d 0\n",-getVjk(n,j),-getVjk(n,i));
            }
            fprintf(f,"%d %d 0\n",-getVjk(n,j),-getVjk(n,nb_noeuds+1));
            fprintf(f,"%d %d 0\n",-getVjk(n,j),-getVjk(n,nb_noeuds+2));
        }
    }
    
    /* Max un fils avec 0 par noeud */
    
    for ( int n = 1; n <= nb_noeuds; n++ ) {
        fprintf(f,"%d %d 0\n",-getFjk(n,nb_noeuds+1),-getFjk(n,nb_noeuds+2));
        for ( int j = n+1; j <= nb_noeuds; j++ ) {
            for ( int i = j+1; i <= nb_noeuds; i++ ) {
                fprintf(f,"%d %d 0\n",-getFjk(n,j),-getFjk(n,i));
            }
            fprintf(f,"%d %d 0\n",-getFjk(n,j),-getFjk(n,nb_noeuds+1));
            fprintf(f,"%d %d 0\n",-getFjk(n,j),-getFjk(n,nb_noeuds+2));
        }
    }
    
    // on ne peut pas aller vers le même noeuds avec vrai ou faux 
    
    for (int n = 1; n <= nb_noeuds; n++ ) {
        for (int k = n+1; k <= nb_noeuds + 2; k++) {
            fprintf(f,"%d %d 0\n",-getVjk(n,k),-getFjk(n,k));
        }
    }
    
    /* contraintes pour être sûr qu'on est un arbre ( être sûr que les noeuds > 1 ont un père ) */
    
    for ( int n = 2; n <= nb_noeuds; n++ ) {
        for ( int k = 1; k < n; k++ ) {
            fprintf(f,"%d %d ",getVjk(k,n),getFjk(k,n));
        }
        fprintf(f,"0\n");
    }
    
    // + unicité du père
    
    for ( int j = 3; j<= nb_noeuds; j++ ) {
        for ( int i = 1; i < j; i++ ) {
            for ( int k = 1; k < i; k++ ) {
                fprintf(f,"%d %d 0\n",-getVjk(i,j),-getVjk(k,j));
                fprintf(f,"%d %d 0\n",-getFjk(i,j),-getVjk(k,j));
                fprintf(f,"%d %d 0\n",-getVjk(i,j),-getFjk(k,j));
                fprintf(f,"%d %d 0\n",-getFjk(i,j),-getFjk(k,j));
            }
        }
    }
    
    /* contraintes sur les cik */
    
    // cas où on part du noeud 1
    
    // On évite les faux positif et faux négatif

    for (int i = 1; i <= nb_exemple; i++) {
        for (int j = 1; j <= nb_features; j++) {
            if ( val_Ei[i-1] == 0 ) {
                if ( val_Eij[i-1][j-1] == 1 )
                    fprintf(f,"%d %d 0\n",-getVjk(1,nb_noeuds+1),-getPjk(j,1));
                else
                    fprintf(f,"%d %d 0\n",-getFjk(1,nb_noeuds+1),-getPjk(j,1));
            }
            else
                if ( val_Eij[i-1][j-1] == 1 )
                    fprintf(f,"%d %d 0\n",-getVjk(1,nb_noeuds+2),-getPjk(j,1));
                else
                    fprintf(f,"%d %d 0\n",-getFjk(1,nb_noeuds+2),-getPjk(j,1));
        }
    }
    
    // si Eij == 1 et Pj1 et V12 alors Ci2 ; Si Eij == 0 et Pj1 et F12 alors Ci2
    
    for ( int i = 1; i <= nb_exemple; i++ ) {
        for( int m = 1 ; m <= nb_features ; m++ ) {
            if ( val_Eij[i-1][m-1] == 1 ) 
                fprintf(f,"%d %d %d 0\n",-getVjk(1,2),getCik(i,2),-getPjk(m,1));
            else
                fprintf(f,"%d %d %d 0\n",-getFjk(1,2),getCik(i,2),-getPjk(m,1));
        }
    }
    
    // Cas général
    
    // Cik et ( Vjk ou Fjk ) => Cij
    
    for ( int i = 1; i <= nb_exemple; i++ ) {
        for ( int j = 2; j <= nb_noeuds; j++ ) {
            for ( int k = j+1; k <= nb_noeuds; k++ ) {
                fprintf(f,"%d %d %d 0\n",-getCik(i,k),getCik(i,j),-getVjk(j,k));
                fprintf(f,"%d %d %d 0\n",-getCik(i,k),getCik(i,j),-getFjk(j,k));
            }
        }
    }
    
    // si Eij == 1 et Pmj et Vjk et Cij alors Cik ; Si Eij == 0 et Pmj et Fjk alors Cik

    for(int i =1 ; i<=nb_exemple; i++){
        for ( int j= 2 ; j<= nb_noeuds; j++){
            for(int k=j+1 ; k<=nb_noeuds+2 ; k++){
                for(int m=1 ; m<=nb_features ; m++){
                    if ( val_Eij[i-1][m-1] == 1 ) {
                        fprintf(f,"%d %d %d %d 0\n",-getCik(i,j),getCik(i,k),-getVjk(j,k),-getPjk(m,j));
                    }
                    else{
                        fprintf(f,"%d %d %d %d 0\n",-getCik(i,j),getCik(i,k),-getFjk(j,k),-getPjk(m,j));
                    }
                }
            }
        }
    }
    
    // Tout exemple positif renvoie vrai tout exemples négatif renvoie faux
    
    for ( int i = 1; i <= nb_exemple; i++ ) {
        if ( val_Ei[i-1] == 1 )
            fprintf(f,"%d 0\n",getCik(i,nb_noeuds+1));
        else
            fprintf(f,"%d 0\n",getCik(i,nb_noeuds+2));
    }
    
    // Aucun exemple positif renvoie faux et aucun négatif renvoie vrai
    
    for ( int i = 1; i <= nb_exemple; i++ ) {
        if ( val_Ei[i-1] == 1 )
            fprintf(f,"%d 0\n",-getCik(i,nb_noeuds+2));
        else
            fprintf(f,"%d 0\n",-getCik(i,nb_noeuds+1));
    }
}






















