#include <stdio.h>
#include <stdlib.>
#include <string.h>
#include <time.h>
#include <conio.h>




#define TAM_NOME 50
#define TAM_LOGIN 15
#define TAM_SENHA 15
#define TAM_PLYLST 200
#define QTD_MUSICAS 100



#define ARQUIVO "usuarios.bin"

//structs 

typedef struct Usuario {
	int codigo; // autoincremento
	char nome[TAM_NOME];
	char login[15];
	char senha[15];
	int qtdPlaylists; // quantidade de playlists criadas pelo usuário
	int playlists[TAM_PLYLST]; // vetor com os códigos das playlists criadas
	int qtdPlaylistsFav; // IDEM: favoritadas pelo usuário
	int playlistsFav[TAM_PLYLST]; // IDEM: favoritadas pelo usuário
} TUsuario;

typedef struct Musica {
	int codigo; // autoincremento
	char titulo[TAM_NOME]; // título da música
	char artista[TAM_NOME]; // nome do artista que está cantando esta música
} TMusica;


typedef playlist {
	int codigo; // autoincremento
	int codigoUsuario; // o código do usuário que criou a playlist
	char titulo[TAM_NOME]; // título desta playlist
	int qtdMusicas; // quantidade atual de músicas incluídas nesta playlist
	int musicas[QTD_MUSICAS]; // vetor contendo os códigos das músicas
} TPlaylist;

// [1] - Cadastrar Usuarios -- BEGIN --

void cadastrarUsuario(char *arquivo) {
        struct Usuario user;
        int i;
        FILE *arq;

        if(!(arq = fopen(arquivo, "ab"))) {
            printf("Erro ao abrir o arquivo");
            return;
        } else {
            printf("Digite seu nome: \n");
            fgets(user.nome, sizeof(user.nome), stdin);

            printf("Digite o login: \n");
            fgets(user.login, sizeof(user.login), stdin);

            printf("seu codigo de acesso sera gerado..");

            //funcao gerador codigo acesso

            if (fwrite(&user, sizeof(struct Usuario),1, arq)!= 1) {
                printf("Error ao gravar arquivo..");
            }

            if (fclose(arq)) {
                printf("Erro ao fechar arquivo..");
            }

        }
    }





// [1] - Buscar dados do USUARIO. --BEGIN--
 
//buscar usuario pelo NOME
int buscarNome(char *arquivo, char nome[]) {
    struct Usuario user;
    int i = 0;
        FILE *arq;
            if (!(arq = fopen(arquivo, "rb"))) {
                printf("erro ao abrir o arquivo. \n");
                return;
            }

        fseek(arq, 0, SEEK_SET);
        fread(&user, sizeof(struct Usuario), 1, arq);
        while(!feof(arq)) {
            if ((strcmp(user.nome, nome) == 0) || (strcmp(user.login, login) == 0)) {
                fclose(arq);
                return i;
            } else {
                ++i;
                fread(&user, sizeof(struct Usuario), 1, arq);
            }
        }
        fclose(arq);

        return -1;
}


//buscar pelo CODIGO 
int buscarCodigoUser(char *arquivo, int codigo) {
    struct Usuario user;
    int i = 0;

    FILE *arq
    if(!(arq = fopen(arquivo, "rb"))) {
        printf("erro ao tentar abrir o arquivo");
        return;
    }
    
    fseek(arq, 0, SEEK_SET);
	fread(&user, sizeof(struct Usuario), 1, arq);
	while (!feof(arq)) {
		if (u.codigo == codigo) {
			fclose(arq);
			return i;
        }
        } else {
         ++i;
         fread(&user, sizeof(struct Usuario), 1, arq);
    }
    fclose(arq);
    return -1;
}

//buscar SENHA

int buscarSenhaUser(char *arquivo, char senha[]) {
    struct Usuario user;
    int i = 0;

    FILE *arq;
    if(!(arq = fopen(arquivo, "rb"))) {
        printf("Erro ao tentar abrir o arquivo..");
        return;
    }
    fseek(arq, 0, SEEK_SET) {
    fread(&user, sizeof(struct Usuario), 1, arq);
    while(!(feof(arq))) {
        if ((strcmp(user.senha, senha) == 0)) {
            fclose(arq);
            return i;
        } else {
            ++i;
            fread(&user, sizeof(struct Usuario),1 ,arq);
        }
        fclose(arq);

        return -1;
    }
    }
}

//retorna o codigo do LOGIN

int buscarLoginUser(char *arquivo, char login[]) {
    struct Usuario user;

    FILE *arq;
    if(!(arq = fopen(arquivo, "rb"))) {
        printf("Erro ao tentar abrir o arquivo");
        return;
    } 

    fseek(arq, 0, SEEK_SET);
    fread(&user, sizeof(struct Usuario), 1, arq);
    while(!feof(arq)) {
        if (strcmp(user.login, usuario) == 0) {
            fclose(arq);
            return user.codigo;
        }
        fread(&user, sizeof(struct Usuario), 1, arq);
    }
    fclose(arq);
}

// [1] - Buscar dados do USUARIO. --END--


// [2] - Alterar dados do USUARIO. --BEGIN--







int main () {

char nome[50], login[15], senha[15];
int op, remover, codigo, i;

printf("Digite seu login: \n");
fflush(stdin);
fgets(login, sizeof(login), stdin);
printf("Digite sua senha: \n");
for (i = 0 ; i < 15 ; ++i) {
    fgets(senha, sizeof(senha), stdin);
    senha[i] = getch();
    putchar("*");
}








do {


    
} while (op != 0);

    return 0;
}
