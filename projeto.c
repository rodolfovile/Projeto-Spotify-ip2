#include <stdio.h>
#include <stdlib.h>
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


typedef struct Playlist {
	int codigo; // autoincremento
	int codigoUsuario; // o código do usuário que criou a playlist
	char titulo[TAM_NOME]; // título desta playlist
	int qtdMusicas; // quantidade atual de músicas incluídas nesta playlist
	int musicas[QTD_MUSICAS]; // vetor contendo os códigos das músicas
} TPlaylist;


int geradorId() {

	int codigo = 1;
	int i = 0;

	++codigo;
	i = codigo;

	return i;

}



// [1] - Cadastrar Usuarios -- BEGIN --

void cadastrarUsuario(char *arquivo) {
	struct Usuario user;
	int i;
	FILE *arq;

	if(!(arq = fopen(arquivo, "ab"))) {
		printf("Erro ao abrir o arquivo");

	} else {
		printf("Digite seu nome: \n");
		fgets(user.nome, sizeof(user.nome), stdin);

		printf("Digite o login: \n");
		fgets(user.login, sizeof(user.login), stdin);

		printf("seu codigo de acesso sera gerado..");



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
	if (!(arq = fopen(arquivo, "rb"))) {//if(!arq)
		printf("erro ao abrir o arquivo. \n");
		return 1; //falha ao abrir o arquivo
	}

	fseek(arq, 0, SEEK_SET);//move o ponteiro de registro p/ o início
	fread(&user, sizeof(struct Usuario), 1, arq);
	while(!feof(arq)) {//enquanto for diferente do final do arquivo
		if ((strcmp(user.nome, user.nome) == 0) || (strcmp(user.login, user.login) == 0)) {//compara se os nomes são iguais
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
int buscarCodigo(int codigo) {
	struct Usuario user;
	int i = 0;

	FILE *arq;
	if((arq = fopen(ARQUIVO, "rb"))==NULL) {//abertura p/ leitura
		printf("erro ao tentar abrir o arquivo");
		return 1;
	}

	fseek(arq, 0, SEEK_SET);
	fread(&user, sizeof(struct Usuario), 1, arq);
	while (!feof(arq)) {
		if (user.codigo == codigo) {
			fclose(arq);
			return i;
		}
		else {
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
		fseek(arq, 0, SEEK_SET)
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

	//verifica se o usuario existe
	int verificauser(int ver, char nome[]){ //ver = verifica
		FILE *arq;
		printf("Digite o nome do usuario: \n");
		gets(usu.nome);
		arq=fopen(ARQUIVO, "r");//abertura em modo leitura
		while (!feof(arq)
		{
			fread(&user,sizeof(usu),1,arq);
			if(ver == user.codigo){//verifica se o codigo existe p/ o usuário
				fclose(arq);
				return 1;//falha ao fechar o arquivo.
			}
		}
		fclose(arq);
		return 0; //sucesso ao fechar.
	}

	// Verificar se o arquivo está vazio.
	int arqverifica(char *ARQUIVO){
		FILE *arq;
		long int tamanho;
		arq=fopen(ARQUIVO, "r");
		if(arq != NULL) {			
			fseek(arq,0,SEEK_END);//movimenta a posição de leitura p/ o fim.
			tamanho  = ftell(arq);//pega a posição corrente de leitura.
			printf("O arquivo possui: %d bytes", tamanho);//imprime o tamanho do arquivo.
		}
		else{
			printf("Arquivo inexistente.\n");
		}
		fclose(arq);
		return 0;
	}


	//[4] - Listar dados USUARIO. --BEGIN-- 
	void listarUsuarios () {
		struct Usuario user;

		FILE *arq;

		if((arq = fopen(ARQUIVO, "rb")) == NULL) {
			printf("Nao foi possivel abrir o arquivo!\n");//erro ao abrir arquivo.

		}

		printf("Os dados do usuario sao: \n");//mostra na tela os dados do usuário.
		fread(&user, sizeof(struct Usuario), 1, arq);

		while(!feof(arq)) {

			printf("Codigo: %d \n", user.codigo);
			printf("Nome do usuario: %s \n ", user.nome);
			printf("Login do usuario: %s \n ", user.login);
			fread(&user,sizeof(struct Usuario), 1, arq);    
		}
		fclose(arq);//fecha o arquivo.

	}//fim do procedimento.

	// Listar nome do USUARIO. --END--

	//Alterar o nome, login e senha do usuario.
	void alterar (char nome[]){
		FILE *arq1;
		FILE *arq2;

		int codigo, alt;
		char login, senha, nome;
		printf("Informe o nome do usuario: \n");
		gets(usu.nome);
		fflush((stdin);
		printf("Informe o login do usuario: \n");
		gets(usu.login);
		fflush(stdin);
		printf("Informe sua senha: \n");
		for(i=0; i<15; ++i){
			fgets(senha, sizeof(senha), stdin);
			senha[i] = getch();
			putchar("*");
		}
		arq1=fopen(ARQUIVO, "r");
		arq2=fopen(arquivo_temporario.bin,"w");
		while(fread(&usu,sizeof(usu),1,arq1)){
			senha = usu.codigo;
			if(senha!=codigo){
				fwrite(&usu,sizeof(usu),1,arq2);
			}
			else{
				printf("\n\t1. Alterar o nome do usuario %d:", cod);
				printf("\n\t2. Alterar o login do usuario %d:",cod);
				printf("\n\t3. Alterar a senha do usurario %d:",cod);
				printf("\n\n Escolha a opcao desejada.");
				scanf("%d", & alt);
				switch (alt)
				{
				case 1:
					printf("Informe o novo nome: ");
					scanf("%s", &usu.nome);
					break;
				case 2:
					printf("Informe o novo login: ");
					scanf("%s", &usu.login);
					break;
				case 3:
					printf("Informe a nova senha: ");
					scanf("%s", &usu.senha);
					break;
				default:
					printf("Opcao invalida");
					break;
				}
				fwrite(&usu,sizeof(usu),1,arq1);
			}
		}

		fclose(arq1);
		fclose(arq2);
		arq1=fopen(ARQUIVO, "w");
		arq2=fopen("arquivo_temporario.bin","r");
		while (fread(&usu,sizeof(usu),1,arq1)
		{
			fwrite(&usu,sizeof(usu),1,arq1);
		}
		fclose(arq1);
		fclose(arq2);
		printf("Arquivo alterado.");
	}
}



int main () {

	char nome[50], login[15], senha[15];
	int op, remover, codigo, i;

	printf("Digite seu login: \n");
	fflush(stdin);
	fgets(login, sizeof(login), stdin);
	printf("Digite sua senha: \n");
	for (i = 0 ; i <15 ; ++i) {
		fgets(senha, sizeof(senha), stdin);
		senha[i] = getch();
		putchar("*");
	}








	do {



	} while (op != 0);

	return 0;
}
