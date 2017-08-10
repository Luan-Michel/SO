//
// DIRETORIOS.C
//
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

static int pesquisa(const char *arg, const char *pasta)
{
    DIR *dirpont;
    struct dirent *dp;
    int c,i;

    if ((dirpont = opendir( pasta)) == NULL) {
        printf( "nao pode abrir %s\n\n",pasta );
        return 0;
    }

    do {
        errno = 0;
        if ((dp = readdir(dirpont)) != NULL) {
	    //printf("%s %d\n\n",dp->d_name, dp->d_type);
	    if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && dp->d_type == 4){
		if((c = pesquisa(arg, dp->d_name))){
			for(i=0;i<3*c;i++){
				printf(" ");
			}
			printf("%s\n",dp->d_name);
			c++;
			return c;
		}
	    }
            if (strcmp(dp->d_name, arg) != 0)
                continue;
            (void) printf("achou: %s(%d)\n", dp->d_name, dp->d_type);
            (void) closedir(dirpont);
            return 1;
        }
    } while (dp != NULL);

    if (errno != 0)
        perror("Erro lendo o diretorio\n\n");
    else
        (void) printf("Falha ao procurar: %s\n", arg);
    (void) closedir(dirpont);
    return 0;
}

int main(int argc, char *argv[])
{
    int i;

    if( argc < 2 ) {
       printf( "USO: %s nome_arquivo ...\n\n", argv[ 0 ] );
       return( -1 );
    }    
    for (i = 1; i < argc; i++)
        pesquisa( argv[i], "." );
    return (0);
}
