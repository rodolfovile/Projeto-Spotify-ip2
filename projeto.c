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



//buscar usuario pelo nome
int buscarNome(char *ARQUIVO, char nome[]) {
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


//buscar pelo codigo 
int buscarCodigo(char *ARQUIVO, int codigo) {
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

//buscar senha

int buscarSenha(char *ARQUIVO, char senha[]) {
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

//retorna o codigo do login

int buscarLogin(char *ARQUIVO, char login[]) {
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




int main () {

char nome[50], login[15], senha[15];
int op, remover, codigo;


printf("Digite seu login: \n");
fgets(login, sizeof(login), stdin);
printf("Digite sua senha: \n");
fgets(senha, sizeof(senha), stdin);

do {
    
}





    return 0;
}
