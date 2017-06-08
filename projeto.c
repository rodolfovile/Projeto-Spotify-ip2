#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>

#define TAM_NOME 50
#define TAM_LOGIN 15
#define TAM_SENHA 15
#define TAM_PLYLIST 200
#define QTD_MUSICAS 100
#define ARQUIVO_USUARIOS "usuarios.bin"
#define ARQUIVO_MUSICAS "musicas.bin"
#define ARQUIVO_TEMP "temp.bin"
#define ARQUIVO_PLAYLISTS "playlists.bin"

// DEFININDO TODAS AS STRUCTS DO CÓDIGO

//USUÁRIO
typedef struct Usuario {
	int codigo; // autoincremento
	char nome[TAM_NOME];
	char login[15];
	char senha[15];
	int qtdPlaylists; // quantidade de playlists criadas pelo usuário
	int playlists[TAM_PLYLIST]; // vetor com os códigos das playlists criadas
	int qtdPlaylistsFav; // IDEM: favoritadas pelo usuário
	int playlistsFav[TAM_PLYLIST]; // IDEM: favoritadas pelo usuário
} TUsuario;

//MÚSICA
typedef struct Musica {
	int codigo; // autoincremento
	char titulo[TAM_NOME]; // título da música
	char artista[TAM_NOME]; // nome do artista que está cantando esta música
} TMusica;

//PLAYLIST
typedef struct playlist {
	int codigo; // autoincremento
	int codigoUsuario; // o código do usuário que criou a playlist
	char titulo[TAM_NOME]; // título desta playlist
	int qtdMusicas; // quantidade atual de músicas incluídas nesta playlist
	int musicas[QTD_MUSICAS]; // vetor contendo os códigos das músicas
} TPlaylist;

//ASSINATURAS
void mascararSenha (char senha[]);
int validaCadastroUsuario(char login[], char senha[]);
int menuLogin(char login[], char senha[]);
int cadastroNovoUsuario();
void menuPrincipal(char login[]);
int subGrupoUsuario(char login[]);
int subGrupoMusica();
int subGrupoPlaylist(char login[]);
int listarDadosUsuario(char login[]);
int possivelCadastrarLogin(char login[], FILE* arq);
int alterarDadosUsuario(char login[]);
int alterarDadosUsuarioLogin(char login[]);
int alterarDadosUsuarioSenha(char login[]);
int alterarDadosUsuarioNome(char login[]);
int listarUsuarios();
int consultarUsuarios();
int listarMusica();
int consultarMusica();
int listarMinhaPlaylist(char login[]);
int listarOutrasPlaylists(char login[]);
int consultarPlaylist();
int cadastrarPlaylist(char login[]);
int inserirMusicaPlaylist(char login[]);
int excluirMusicaPlaylist(char login[]);
int excluirPlaylist();
int favoritarPlaylist(char login[]);
int autoIncrementoCodigo(FILE* arq, int flag);

//MAIN
int main() {
	char login[TAM_LOGIN], senha[TAM_SENHA];

    menuLogin(login, senha);
	menuPrincipal(login);

	getchar();
	return 0;
}

//CODIGO PARA PREENCHER A SENHA ATRAVÉS DE REFERENCIA E COLOCAR " * "
void mascararSenha (char senha[]) {
	char letraTemp = ' ';
	int contador = 0;

	while (letraTemp != '\r') {
		letraTemp = getch();

		if (letraTemp != '\r' && letraTemp != '\b' && contador < TAM_SENHA - 1) {
			putchar('*');
			senha[contador] = letraTemp;
			contador++;
		} else if (letraTemp == '\b') {

			if (contador > 0) {
				putchar('\b');
				putchar(' ');
				putchar('\b');
				contador--;
			}
		}
	}
	senha[contador] = '\0';
}

//MENU LOGIN
int menuLogin(char login[], char senha[]) {

	int opcao = 0, retorno = 0;

	do {
        do {
            printf("\n");
            printf("|--------------MENU LOGIN--------------|\n\n");
            printf("\t1 - Login\n");
            printf("\t2 - Cadastre-se\n");
            printf("\t3 - Finalizar programa\n");
            printf("\n");
            printf("|--------------------------------------|\n\n");

            printf("Qual opcao deseja acessar? ");
            scanf("%d", &opcao);
			fflush(stdin);

            if (opcao < 1 || opcao > 3)
            {
                printf("Opcao incorreta. Digite novamente!");
				scanf("%d", &opcao);
				fflush(stdin);
                system("pause>>NULL");
                system("cls");
            }

        }
        while (opcao < 1 || opcao > 3);
        system("cls");

        switch (opcao)
        {
        case 1:
            retorno = validaCadastroUsuario(login, senha);
            break;
        case 2:
            retorno = cadastroNovoUsuario();
            break;
        case 3:
            exit(0);
            system("pause>>NULL");
            system("cls");
            break;
        }
    }
    while(retorno != 0);

    system("pause>>NULL");
    system("cls");

    return retorno;
}

//VALIDAÇÃO DO CADASTRO DO USUÁRIO
int validaCadastroUsuario(char login[], char senha[]) {

	FILE *arq = NULL;
	TUsuario usuario;
 	int retorno = 1;

	if(!(arq = fopen(ARQUIVO_USUARIOS, "rb"))) {
		printf("\nAinda nao existem usuarios cadastrados!");
		system("pause>>NULL");
		system("cls");
		retorno = 1;
		return retorno;
	}

	if(fread(&usuario, sizeof(usuario), 1, arq) != 1) {
		printf("\nErro ao ler arquivo!\n");
		retorno = 1;
		fclose(arq);
		return retorno;
	}

	do {
		printf("\n");
		printf("|-------------USUARIO-------------|\n\n");
		printf("\tLogin: ");
		scanf("%s", login);
		fflush(stdin);

		printf("\tSenha: ");
		mascararSenha(senha);
		printf("\n\n");
		printf("|-----------------------------------|\n\n");

        while(!feof(arq)) {
            if (strcmp(usuario.login,login) == 0 && strcmp(usuario.senha,senha) == 0) {
                printf("\nSeja bem-vindo(a), %s!\n", usuario.nome);
                retorno = 0;
                break;
            }

			// Caso o usuario for correto, informamos que a senha está errada e pediremos para tentar novamente.
			else if (strcmp(usuario.login, login) == 0 && strcmp(usuario.senha, senha) != 0) {
				printf("\nLogin ou senha invalidos! Tente novamente!\n", usuario.nome);
				system("pause>>NULL");
				system("cls");
				retorno = 1;
				break;
			}

			// Desta forma saberemos que o login não está no arquivo
            if(fread(&usuario, sizeof(usuario), 1, arq) != 1) {
				if (feof(arq)) {
					printf("\nLogin inexistente! Favor tentar novamente ou fazer um novo cadastro!\n");
					system("pause>>NULL");
					system("cls");
					retorno = 1;
					break;
				}
				else {
					printf("\nErro ao ler arquivo!\n");
					retorno = 1;
					break;
				}
            }
        }

		// Saimos do loop que valida a senha caso der problemas.
		if (retorno == 1) {
			break;
		}

	// Status que deve repetir o loop caso o usuario acertar o login e errar a senha. (Verificar se vale a pena)
	} while (retorno == 2);

    fclose(arq);

	return retorno;
}

//NOVO CADASTRO
int cadastroNovoUsuario() {

	TUsuario novoCadastro;
	FILE *arq = NULL;
	int proximoCodigo = 0, usuarioInserido = 0, retorno = 0;
	char confirmacaoSenha[TAM_SENHA];

	arq = fopen(ARQUIVO_USUARIOS, "rb");

	if (arq != NULL) {
		proximoCodigo = autoIncrementoCodigo(arq, 1);
		fclose(arq);
	} else {
		proximoCodigo++;
	}

	if (!(arq = fopen(ARQUIVO_USUARIOS, "ab"))) {
		printf("\tErro ao acessar o arquivo!\n");
	}

	novoCadastro.codigo = proximoCodigo;

	printf("\nDigite seu nome: ");
	fflush(stdin);
	fgets(novoCadastro.nome, TAM_NOME, stdin);

	novoCadastro.nome[strlen(novoCadastro.nome) - 1] = '\0';

	printf("\nDigite seu login: ");
	fflush(stdin);
	fgets(novoCadastro.login, TAM_LOGIN, stdin);

	novoCadastro.login[strlen(novoCadastro.login) - 1] = '\0';

	printf("\nDigite sua senha: ");
	mascararSenha(novoCadastro.senha);
	fflush(stdin);

	printf("\n\nConfirme sua senha: ");
	mascararSenha(confirmacaoSenha); 
	fflush(stdin);

	if(strcmp(novoCadastro.senha, confirmacaoSenha) == 0) {
		printf("\n\nSEJA BEM-VINDO(A)!\n");
        system("pause>>NULL");
        system("cls");
	} else {
		while(strcmp(novoCadastro.senha, confirmacaoSenha) != 0) {
			printf("\nAs senhas sao diferentes! Favor digitar novamente ");
			printf("\nConfirme sua senha: ");
			mascararSenha(confirmacaoSenha);
			fflush(stdin);
		}
	}

	// Inicializar as qtdPlaylists para evitar problemas
	novoCadastro.qtdPlaylists = 0;
	novoCadastro.qtdPlaylistsFav = 0;
	usuarioInserido = fwrite(&novoCadastro, sizeof(TUsuario), 1, arq);

	if (usuarioInserido <= 0) {
		printf("\nErro ao fazer o cadastro!\n");
		retorno = 1;
		return retorno;
	}

	if (fclose(arq) == EOF) {
		printf("\tErro ao fechar o arquivo!\n");
		retorno = 1;
		return retorno;
	}
	retorno = 1;
	return retorno;
}

//MENU GERAL
void menuPrincipal(char login[]) {

	int opcao = 0, retorno = 0;

	do {
		do {
			printf("\n");
			printf("|---------------MENU----------------|\n\n");
			printf("\t1 - Usuario\n");
			printf("\t2 - Musica\n");
			printf("\t3 - Playlist\n");
			printf("\t4 - Finalizar programa\n");
			printf("\n");
			printf("|-----------------------------------|\n\n");

			printf("Qual opcao deseja acessar? ");
			scanf("%d", &opcao);

			if (opcao < 1 || opcao > 4) {
				printf("Opcao incorreta. Digite novamente!");
				scanf("%d", &opcao);
				system("pause>>NULL");
				system("cls");
			}

		} while (opcao < 1 || opcao > 4);
		system("cls");

		switch(opcao) {
			case 1:
				retorno = subGrupoUsuario(login);
				break;

			case 2:
				retorno = subGrupoMusica();
				break;

			case 3:
				retorno = subGrupoPlaylist(login);
				break;

			case 4:
				exit(0);
				system("pause>>NULL");
                system("cls");
				break;
		}
	} while (retorno != 0);
}

//MENU USUÁRIO
int subGrupoUsuario(char login[]) {

	int opcao = 0, retorno = 0;

	do {
		do {
			printf("\n");
			printf("|-------------USUARIO----------------|\n\n");
			printf("\t1 - Listar dados\n");
			printf("\t2 - Alterar dados\n");
			printf("\t3 - Listar usuarios\n");
			printf("\t4 - Consultar\n");
			printf("\t5 - Voltar ao menu anterior\n");
			printf("\t6 - Finalizar programa\n");
			printf("\n");
			printf("|-----------------------------------|\n\n");

			printf("Qual opcao deseja acessar? ");
			scanf("%d", &opcao);

			if (opcao < 1 || opcao > 6) {
				printf("Opcao incorreta. Digite novamente!");
				system("pause>>NULL");
				system("cls");
			}

		} while (opcao < 1 || opcao > 6);
		system("cls");

		switch(opcao) {
			case 1:
				retorno = listarDadosUsuario(login);
				break;

			case 2:
				retorno = alterarDadosUsuario(login);
				break;

			case 3:
				retorno = listarUsuarios();
				break;

			case 4:
				retorno = consultarUsuarios();
				break;

			case 5:
				system("cls");
				retorno = 1;
				return retorno;
				break;

			case 6:
				exit(0);
				system("pause>>NULL");
                system("cls");
				break;
		}
	} while(retorno != 0);

	system("cls");
	return retorno;
}

//MENU MÚSICA
int subGrupoMusica() {

	int opcao = 0, retorno = 0;

	do {
		do {
			printf("\n");
			printf("|--------------MUSICA-----------------|\n\n");
			printf("\t1 - Listar\n");
			printf("\t2 - Consultar\n");
			printf("\t3 - Voltar ao menu anterior\n");
			printf("\t4 - Finalizar programa\n");
			printf("\n");
			printf("|-------------------------------------|\n\n");

			printf("Qual opcao deseja acessar? ");
			scanf("%d", &opcao);

			if (opcao < 1 || opcao > 4) {
				printf("Opcao incorreta. Digite novamente!");
				system("pause>>NULL");
				system("cls");
			}

		} while (opcao < 1 || opcao > 4);
		system("cls");

		switch(opcao) {
			case 1:
				retorno = listarMusica();
				break;

			case 2:
				retorno = consultarMusica();
				break;

			case 3:
				system("cls");
				retorno = 1;
				return retorno;
				break;

			case 4:
				exit(0);
				system("pause>>NULL");
                system("cls");
				break;
		}
	} while(retorno != 0);

	system("cls");
	return retorno;
}

//MENU PLAYLIST
int subGrupoPlaylist(char login[]) {

  int opcao = 0, retorno = 0;

    do
    {
        do
        {
            printf("\n");
            printf("|-------------PLAYLIST----------------|\n\n");
            printf("\t1  - Listar minha(s) playlist(s)\n");
            printf("\t2  - Listar outra(s) playlist(s)\n");
            printf("\t3  - Consultar playlist\n");
            printf("\t4  - Cadastrar playlist\n");
            printf("\t5  - Inserir musica\n");
            printf("\t6  - Excluir musica numa playlist\n");
            printf("\t7  - Excluir playlist\n");
            printf("\t8  - Favoritar uma playlist\n");
            printf("\t9  - Voltar ao menu anterior\n");
            printf("\t10 - Finalizar programa\n");
            printf("\n");
            printf("|-------------------------------------|\n\n");

            printf("Qual opcao deseja acessar? ");
            scanf("%d", &opcao);

            if (opcao < 1 || opcao > 10)
            {
                printf("Opcao incorreta. Digite novamente!");
                system("pause>>NULL");
                system("cls");
            }

        }
        while (opcao < 1 || opcao > 10);
        system("cls");

        switch(opcao)
        {
        case 1:
            retorno = listarMinhaPlaylist(login);
            break;

        case 2:
            retorno = listarOutrasPlaylists(login);
            break;

        case 3:
            retorno = consultarPlaylist();
            break;

        case 4:
            retorno = cadastrarPlaylist(login);
            break;
        case 5:
            retorno = inserirMusicaPlaylist(login);
            break;

        case 6:
            retorno = excluirMusicaPlaylist(login);
            break;

        case 7:
            retorno = excluirPlaylist();
            break;

        case 8:
            retorno = favoritarPlaylist(login);
            break;

        case 9:
            system("cls");
            retorno = 1;
            return retorno;
            break;

        case 10:
            exit(0);
            system("pause>>NULL");
            system("cls");
            break;
        }
    }
    while(retorno != 0);

    system("cls");
    return retorno;
}

//OPÇÕES PARA SEREM UTILIZADAS DENTRO DO MENU USUÁRIO

//LISTAR DADOS DO USUÁRIO QUE ESTÁ LOGADO
int listarDadosUsuario(char login[])
{
    int retorno = 0;
    TUsuario usuario;
    FILE *arq = NULL;

    if (!(arq = fopen(ARQUIVO_USUARIOS, "rb")))
    {
        printf("Erro ao abrir arquivo!\n");
        retorno = 1;
        return retorno;
    }

    if(fread(&usuario, sizeof(TUsuario), 1, arq) != 1)
    {
        printf("\nErro ao ler arquivo!\n");
        retorno = 1;
        return retorno;
    }
    while (!feof(arq))
    {
        if (strcmp(usuario.login, login) == 0)
        {
            printf("\n");
       		printf("|-------------USUARIOS---------------|\n\n");
	        printf("\tUsuario: %d\n", usuario.codigo);
            printf("\tNome: %s\n", usuario.nome);
            printf("\tLogin: %s\n", usuario.login);
            printf("\n|----------------------------------|\n\n");

            system("pause>>NULL");
            system("cls");
            retorno = 1;
            return retorno;
            break;
        }
        if (fread(&usuario, sizeof(TUsuario), 1, arq) != 1 && !feof(arq))
        {
            printf("\nErro ao ler arquivo!\n");
            retorno = 1;
            return retorno;
        }
    }
    system("pause>>NULL");
    system("cls");
}


// Se for possivel cadastar Login: Retorna 0
// Caso nao, retorna 1
int possivelCadastrarLogin(char login[], FILE* arq){

	int retorno = 0, isArqNull = 0;
    TUsuario usuario;

	if(arq == NULL){
		isArqNull = 1;
		if (!(arq = fopen(ARQUIVO_USUARIOS, "rb")))
		{
			printf("Erro ao abrir arquivo!\n");
			retorno = 1;
		}
	}

	fseek(arq, 0, SEEK_SET);

    if(fread(&usuario, sizeof(TUsuario), 1, arq) != 1)
    {
        printf("\nErro ao ler arquivo!\n");
        retorno = 1;
    }

    while (!feof(arq))
    {
        if (strcmp(usuario.login, login) == 0)
        {
			// Existe
			retorno = 1;
			break;
        }

        if (fread(&usuario, sizeof(TUsuario), 1, arq) != 1 && !feof(arq))
        {
            printf("\nErro ao ler arquivo!\n");
            retorno = 1;
			break;
        }
    }

	if(isArqNull == 0){
		fseek(arq, 0, SEEK_SET);
	}else{
		fclose(arq);
	}

    return retorno;
}

//ALTERAR DADOS DO USUÁRIO
int alterarDadosUsuario(char login[])
{
    int opcao = 0, retorno = 0;

    do
    {
        do
        {
            printf("\n");
            printf("|-------------ALTERAR DADOS------------|\n\n");
            printf("\t1 - Nome\n");
            printf("\t2 - Login\n");
            printf("\t3 - Senha\n");
            printf("\t4 - Voltar ao menu anterior\n");
            printf("\t5 - Finalizar programa\n");
            printf("\n");
            printf("|-----------------------------------|\n\n");

            printf("Qual opcao deseja acessar? ");
            scanf("%d", &opcao);

            if (opcao < 1 || opcao > 5)
            {
                printf("Opcao incorreta. Digite novamente!");
                system("pause>>NULL");
                system("cls");
            }

        }
        while (opcao < 1 || opcao > 5);
        system("cls");

        switch(opcao)
        {
        case 1:
			retorno = alterarDadosUsuarioNome(login);
            break;
        case 2:
			retorno = alterarDadosUsuarioLogin(login);
            break;
        case 3:
			retorno = alterarDadosUsuarioSenha(login);
            break;
        case 4:
            system("cls");
            retorno = 1;
            return retorno;
            break;
        case 5:
            exit(0);
            system("pause>>NULL");
            system("cls");
            break;
        }
    }
    while (retorno != 0);

	return retorno;
}

int alterarDadosUsuarioLogin(char login[]){
    FILE *arq = NULL;
    TUsuario usuario;
    int retorno = 0, quantidade = 0, statusLogin = 0, posicao = 0;
	char opcao = ' ', resp = ' ', novoLogin[TAM_LOGIN];

	if(!(arq = fopen(ARQUIVO_USUARIOS, "r+b")))
	{
		printf("\nErro ao acessar o arquivo.\n");
		retorno = 1;
		return retorno;
	}

    while(!feof(arq))
    {
		quantidade += fread(&usuario, sizeof(TUsuario), 1, arq);
    }

    if(quantidade == 0)
    {
        printf("\nNao existem usuarios para alterar no arquivo.\n");
        retorno = 1;
        return retorno;
    }
    else
    {
        if(fseek(arq, 0, SEEK_SET) != 0)
        {
            printf("\nErro de arquivo.\n");
            retorno = 1;
            return retorno;
        }

        printf("\n|-------------ALTERAR USUARIO---------------|\n\n");

		if(fread(&usuario, sizeof(TUsuario), 1, arq) != 1)
        {
            printf("\nErro ao ler arquivo!\n");
            retorno = 1;
            return retorno;
        }

        while(!feof(arq))
        {
			if(strcmp(usuario.login,login) == 0)
            {
				printf("\n\tLogin atual: %s", usuario.login);
                printf("\n\n");

                do{
                    printf("Deseja alterar o login? (S ou N): ");
                    fflush(stdin);
					scanf("%c", &resp);
                    resp = toupper(resp);
                }while(resp != 'S' && resp != 'N');

				break;
			}else{
			
				++posicao;
				
				if(fread(&usuario, sizeof(TUsuario), 1, arq) != 1 && !feof(arq))
				{
					printf("\nErro ao ler arquivo!\n");
					retorno = 1;
					return retorno;
				}
			
			}


        }

		if(resp == 'S')
        {
			do{
				printf("\nDigite o novo login: ");
				fflush(stdin);
				fgets(novoLogin, TAM_NOME, stdin);
				novoLogin[strlen(novoLogin) - 1] = '\0';

				statusLogin = possivelCadastrarLogin(novoLogin, arq);

				if(statusLogin == 0){

					strcpy(usuario.login, novoLogin);

					if(fseek(arq, (long) posicao * sizeof(TUsuario), SEEK_SET) != 0)
					{
						printf("\nErro de arquivo.\n");
						retorno = 1;
						return retorno;
					}

					if(fwrite(&usuario, sizeof(TUsuario), 1, arq) != 1)
					{
						printf("\nErro ao escrever no arquivo!\n");
						retorno = 1;
						return retorno;
					}
				}
				else{
					printf("\nO login escolhido ja existe! Tente novamente.");
					system("pause>>NULL");
					system("cls");
				}

			}while(statusLogin == 1);

			printf("\nLogin alterado com sucesso!\n");
			system("pause>>NULL");
			system("cls");
			retorno = 1;
        }
		fflush(arq);
	}

	fclose(arq);

    return retorno;
}

int alterarDadosUsuarioSenha(char login[]){
	FILE *arq = NULL;
	TUsuario usuario;
	int retorno = 0, quantidade = 0;
	char opcao = ' ', resp = ' ', confirmacaoSenha[TAM_SENHA];

	if(!(arq = fopen(ARQUIVO_USUARIOS, "r+b")))
	{
		printf("\nErro ao acessar o arquivo.\n");
		retorno = 1;
		return retorno;
	}

	while(!feof(arq))
	{
		quantidade += fread(&usuario, sizeof(TUsuario), 1, arq);
	}

	if(quantidade == 0)
	{
		printf("\nNao existem usuarios para alterar no arquivo.\n");
		retorno = 1;
		return retorno;
	}
	else
	{
		if(fseek(arq, 0, SEEK_SET) != 0)
		{
			printf("\nErro de arquivo.\n");
			retorno = 1;
			return retorno;
		}

		if(fread(&usuario, sizeof(TUsuario), 1, arq) != 1)
		{
			printf("\nErro ao ler arquivo!\n");
			retorno = 1;
			return retorno;
		}

		while(!feof(arq))
		{

			if(strcmp(usuario.login,login) == 0)
			{
				do{
					printf("Deseja alterar a senha? (S ou N): ");
					fflush(stdin);
					scanf("%c", &resp);
					resp = toupper(resp);
				}while(resp != 'S' && resp != 'N');

				break;
			}

			if(fread(&usuario, sizeof(TUsuario), 1, arq) != 1 && !feof(arq))
			{
				printf("\nErro ao ler arquivo!\n");
				retorno = 1;
				return retorno;
			}
		}

		if(resp == 'S')
		{
			if(fseek(arq, -sizeof(TUsuario), SEEK_CUR) != 0)
			{
				printf("\nErro de arquivo.\n");
				retorno = 1;
				return retorno;
			}

			printf("\nDigite a nova senha: ");
			mascararSenha(usuario.senha);
			printf("\nConfirme a nova senha: ");
			mascararSenha(confirmacaoSenha);
			fflush(stdin);

			if(strcmp(usuario.senha, confirmacaoSenha) == 0)
			{
				printf("\n\nSENHA ALTERADA COM SUCESSO!\n");
				system("pause>>NULL");
				system("cls");
			}
			else
			{
				while(strcmp(usuario.senha, confirmacaoSenha) != 0)
				{
					printf("\n");
					printf("\nAs senhas sao diferentes! ");
					printf("\n");
					printf("\nFavor digite novamente a nova senha: ");
					mascararSenha(usuario.senha);
					printf("\nConfirme sua senha: ");
					mascararSenha(confirmacaoSenha);
					fflush(stdin);
				}

				printf("\n\nSENHA ALTERADA COM SUCESSO!\n");
				system("pause>>NULL");
				system("cls");
			}
			if(fwrite(&usuario, sizeof(TUsuario), 1, arq) != 1)
			{
				printf("\nErro ao escrever no arquivo!\n");
				retorno = 1;
				return retorno;
			}
		}
	}
	fflush(arq);
	fclose(arq);

	retorno = 1;
    return retorno;
}

int alterarDadosUsuarioNome(char login[]){
	
	FILE *arq = NULL;
    TUsuario usuario;
    int retorno = 0, quantidade = 0, statusLogin = 0;
	char opcao = ' ', resp = ' ', novoNome[TAM_LOGIN];
	
	if(!(arq = fopen(ARQUIVO_USUARIOS, "r+b")))
	{
		printf("\nErro ao acessar o arquivo.\n");
		retorno = 1;
		return retorno;
	}

    while(!feof(arq))
    {
		quantidade += fread(&usuario, sizeof(TUsuario), 1, arq);
    }

    if(quantidade == 0)
    {
        printf("\nNao existem usuarios para alterar no arquivo.\n");
        retorno = 1;
        return retorno;
    }
    else
    {
        if(fseek(arq, 0, SEEK_SET) != 0)
        {
            printf("\nErro de arquivo.\n");
            retorno = 1;
            return retorno;
        }

		if(fread(&usuario, sizeof(TUsuario), 1, arq) != 1)
        {
            printf("\nErro ao ler arquivo!\n");
            retorno = 1;
            return retorno;
        }

        while(!feof(arq))
        {

			if(strcmp(usuario.login,login) == 0)
            {
				printf("\n\tNome atual: %s", usuario.nome);
                printf("\n\n");

                do{
                    printf("Deseja alterar o nome? (S ou N): ");
                    fflush(stdin);
					scanf("%c", &resp);
                    resp = toupper(resp);
                }while(resp != 'S' && resp != 'N');

				break;
            }
            
			if(fread(&usuario, sizeof(TUsuario), 1, arq) != 1 && !feof(arq))
			{
				printf("\nErro ao ler arquivo!\n");
				retorno = 1;
				return retorno;
			}
        }
            
		if(resp == 'S')
        {
			printf("\nDigite o novo nome: ", usuario.nome);
			fflush(stdin);
			fgets(usuario.nome, TAM_NOME, stdin);
			usuario.nome[strlen(usuario.nome) - 1] = '\0';
			
			if(fseek(arq, (long) -sizeof(TUsuario), SEEK_CUR) != 0)
			{
				printf("\nErro de arquivo.\n");
				retorno = 1;
				return retorno;
			}

			if(fwrite(&usuario, sizeof(TUsuario), 1, arq) != 1)
			{
				printf("\nErro ao escrever no arquivo!\n");
				retorno = 1;
				return retorno;
			}

			printf("\nNome alterado com sucesso!\n");
			system("pause>>NULL");
			
		}
	}

	fflush(arq);
	fclose(arq);

	system("cls");
	retorno = 1;
	return retorno;
}

//LISTAR USUÁRIOS
int listarUsuarios()
{
    TUsuario usuario;
    FILE *arq = NULL;
    int usuariosLidos = 0, retorno = 0;

    if (!(arq = fopen(ARQUIVO_USUARIOS, "rb")))     // isso pode retornar NULL
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }

    while (!feof(arq))
    {
        usuariosLidos += fread(&usuario, sizeof(TUsuario), 1, arq);
    }

    printf("\nExistem %d usuario(s) cadastrado(s).\n", usuariosLidos);

    if (usuariosLidos == 0)
    {
        retorno = 1;
        return retorno;
    }
    else
    {
        fseek(arq, 0, SEEK_SET);
        if(fread(&usuario, sizeof(TUsuario), 1, arq) != 1 && !feof(arq))
        {
            printf("\nErro ao ler o arquivo!\n");
            retorno = 1;
            return retorno;
        }
        printf("\n|-----------USUARIOS------------|\n\n");
        while(!feof(arq))
        {
            printf("\t %s\n", usuario.nome);
            if(fread(&usuario, sizeof(TUsuario), 1, arq) != 1 && !feof(arq))
            {
                printf("\nErro ao ler o arquivo!\n");
                retorno = 1;
                return retorno;
            }
        }
        printf("\n|-----------------------------|\n\n");
    }

    retorno = 1;
    system("pause>>NULL");
    system("cls");
    return retorno;
}

//CONSULTAR USUÁRIOS
int consultarUsuarios()
{
    TUsuario usuario;
    FILE *arq = NULL;
    char nome[TAM_NOME];
    char resp = ' ', opcao = ' ';
    int retorno = 0;

	if (!(arq = fopen(ARQUIVO_USUARIOS, "rb")))
	{
		printf("Erro ao abrir arquivo!\n");
		retorno = 1;
		return retorno;
	}

    do
    {
        //NOME
        fseek(arq, 0, SEEK_SET);
        printf("\tInforme o nome: ");
        fflush(stdin);
        fgets(nome, TAM_NOME, stdin);
        nome[strlen(nome) - 1] = '\0';

        if(fread(&usuario, sizeof(TUsuario), 1, arq) != 1)
        {
            printf("\nErro ao ler arquivo!\n");
            retorno = 1;
            return retorno;
        }
        while (!feof(arq))
        {
            if (strstr(usuario.nome, nome))
            {
                printf("\n");
                printf("\tUsuario: %s\n", usuario.nome);
            }
            if (fread(&usuario, sizeof(TUsuario), 1, arq) != 1 && !feof(arq))
            {
                printf("\nErro ao ler arquivo!\n");
                retorno = 1;
                return retorno;
            }
        }
        system("pause>>NULL");
        system("cls");


        printf("\nDeseja consultar outro nome? (S ou N)\n");
        scanf("%c", &opcao);
        fflush(stdin);
        opcao = toupper(opcao);
        while (opcao != 'S' && opcao != 'N')
        {
            printf("Opcao invalida!\n");
            scanf("%c", &opcao);
            fflush(stdin);
            opcao = toupper(opcao);
        }


    }while (opcao != 'N');
}

//OPÇÕES PARA SEREM UTILIZADAS DENTRO DO MENU MÚSICA

//LISTAR MÚSICAS
int listarMusica() {

	TMusica musica[QTD_MUSICAS];
	FILE *arq = NULL;
	int registrosLidos = 0, contador = 0, retorno = 0;

	if (!(arq = fopen(ARQUIVO_MUSICAS, "rb"))) {   // isso pode retornar NULL
		printf("\nErro ao acessar o arquivo!\n");
		retorno = 1;
		return retorno;
	}
	registrosLidos = fread(musica, sizeof(TMusica), QTD_MUSICAS, arq);

	if (fclose(arq) == EOF) {
		printf("\nErro ao fechar o arquivo!\n");
		retorno = 1;
		return retorno;
	}

	printf("\nExistem %d musicas cadastradas. \n", registrosLidos);

	if (registrosLidos == 0) {
		retorno = 1;
		return retorno;
	} else {
	    printf("|--------------MUSICAS----------------|\n\n");
		for (contador = 0; contador < registrosLidos; ++contador) {
			printf("\n\tCodigo: %d", musica[contador].codigo);
			printf("\n\tArtista: %s", musica[contador].artista);
			printf("\n\tTitulo: %s", musica[contador].titulo);
			printf("\n\n");
		}
	}

	system("pause>>NULL");
	system("cls");
	retorno = 1;
	return retorno;
}

//CONSULTAR MÚSICAS
int consultarMusica() {
	TMusica musica;
	FILE *arq = NULL;

	char artista[TAM_NOME], titulo[TAM_NOME];
	int opcao, codigo, retorno = 0;

	if (!(arq = fopen(ARQUIVO_MUSICAS, "rb"))) {
		printf("Erro ao abrir arquivo!\n");
		retorno = 1;
		return retorno;
	}

	do {
		printf("\n");
		printf("|----------CONSULTAR MUSICA----------|\n\n");
		printf("\t1 - Codigo\n");
		printf("\t2 - Artista\n");
		printf("\t3 - Titulo\n");
		printf("\t4 - Voltar ao menu anterior\n");
		printf("\t5 - Finalizar programa\n");
		printf("\n");
		printf("|-----------------------------------|\n\n");

		printf("Qual opcao deseja acessar? ");
		scanf("%d", &opcao);

		if (opcao < 1 || opcao > 5) {
			printf("Opcao incorreta. Digite novamente!");
			system("pause>>NULL");
			system("cls");
		}
	} while (opcao < 1 || opcao > 5);
	system("cls");

	switch(opcao) {

		//CODIGO
		case 1:
			fseek(arq, 0, SEEK_SET);
			printf("\n|--------------CONSULTAR-------------|\n\n");
			printf("\tInforme o codigo: ");
			scanf("%d",&codigo);
			fflush(stdin);

			if(fread(&musica, sizeof(TMusica), 1, arq) != 1) {
				printf("\nErro ao ler arquivo!\n");
				retorno = 1;
				return retorno;
			}
			while (!feof(arq)) {
				if (musica.codigo == codigo) {
					printf("\n");
					printf("\tCodigo: %d\n", musica.codigo);
					printf("\tArtista: %s\n", musica.artista);
					printf("\tTitulo: %s\n", musica.titulo);
					printf("\n|-----------------------------------|\n\n");

					system("pause>>NULL");
					system("cls");
					break;
				}
				if (fread(&musica, sizeof(TMusica), 1, arq) != 1 && !feof(arq)) {
					printf("\nErro ao ler arquivo!\n");
					retorno = 1;
					return retorno;
				}
			}
			
			system("pause>>NULL");
			system("cls");
			retorno = 1;
			return retorno;
			break;

			//ARTISTA
		case 2:
			fseek(arq, 0, SEEK_SET);
			printf("\n|--------------CONSULTAR-------------|\n\n");
			printf("\tInforme o artista: ");
			fflush(stdin);
			fgets(artista, TAM_NOME, stdin);
			artista[strlen(artista) - 1] = '\0';

			if(fread(&musica, sizeof(TMusica), 1, arq) != 1) {
				printf("\nErro ao ler arquivo!\n");
				retorno = 1;
				return retorno;
			}

			while (!feof(arq)) {

				if (strstr(musica.artista,artista)) {
					printf("\n");
					printf("\tCodigo: %d\n", musica.codigo);
					printf("\tArtista: %s\n", musica.artista);
					printf("\tTitulo: %s\n", musica.titulo);
					printf("\n|-----------------------------------|\n\n");

					system("pause>>NULL");
					system("cls");
					break;

				} else if (fread(&musica, sizeof(TMusica), 1, arq) != 1 && !feof(arq)) {
					printf("\nErro ao ler arquivo!\n");
					retorno = 1;
					return retorno;
				}
			}
            retorno = 1;
			return retorno;
			system("pause>>NULL");
			system("cls");
			break;

			//TITULO
		case 3:
			fseek(arq, 0, SEEK_SET);
			printf("\n|--------------CONSULTAR-------------|\n\n");
			printf("\tInforme o titulo: ");
			fflush(stdin);
			fgets(titulo, TAM_NOME, stdin);
			titulo[strlen(titulo) - 1] = '\0';

			if(fread(&musica, sizeof(TMusica), 1, arq) != 1) {
				printf("\nErro ao ler arquivo!\n");
				retorno = 1;
				return retorno;
			}

			while (!feof(arq)) {

				if (strstr(musica.titulo,titulo)) {
					printf("\n");
					printf("\tCodigo: %d\n", musica.codigo);
					printf("\tArtista: %s\n", musica.artista);
					printf("\tTitulo: %s\n", musica.titulo);
					printf("\n|-----------------------------------|\n\n");

					system("pause>>NULL");
					system("cls");
					break;
				}
				if (fread(&musica, sizeof(TMusica), 1, arq) != 1 && !feof(arq)) {
					printf("\nErro ao ler arquivo!\n");
					retorno = 1;
					return retorno;
				}
			}
			retorno = 1;
			return retorno;

			system("pause>>NULL");
			system("cls");
			break;

		case 4:
			system("cls");
			retorno = 1;
			return retorno;
			break;

		case 5:
			exit(0);
			break;
	}

	system("cls");
}


//OPÇÕES PARA SEREM UTILIZADAS DENTRO DO MENU PLAYLIST

//LISTAR PLAYLIST DO USUÁRIO LOGADO
int listarMinhaPlaylist(char login[]){
    FILE *arq = NULL, *arq1 = NULL, *arq2 = NULL;
    TUsuario usuario;
    TPlaylist playlist;
    TMusica musica;
    int retorno = 0, contador, numero = 0, plylsts, qtd;

    if (!(arq = fopen(ARQUIVO_PLAYLISTS, "rb")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if (!(arq1 = fopen(ARQUIVO_USUARIOS, "rb")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if (!(arq2 = fopen(ARQUIVO_MUSICAS, "rb")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if(fread(&usuario, sizeof(TUsuario), 1, arq1) != 1) {
        printf("\nErro ao ler o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    while(!feof(arq1)) {
        if(strcmp(usuario.login, login) == 0) {
            break;
        }
        if(fread(&usuario, sizeof(TUsuario), 1, arq1) != 1 && !feof(arq1)) {
            printf("\nErro ao ler o arquivo!\n");
            retorno = 1;
            return retorno;
        }
    }
    if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1) {
        printf("\nErro ao ler o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    while(!feof(arq)) {
        if(usuario.codigo == playlist.codigoUsuario) {
            ++numero;
			printf("\n|--------MINHA PLAYLIST--------|\n\n");
            printf("\n\tCodigo: %d", playlist.codigo);
            printf("\n\tTitulo: %s", playlist.titulo);
            printf("\n\n");
            for(contador = 0; contador < playlist.qtdMusicas; ++contador) {
                if(fseek(arq2, 0, SEEK_SET) != 0) {
                    printf("\nErro de arquivo!\n");
                    retorno = 1;
                    return retorno;
                }
                if(fread(&musica, sizeof(TMusica), 1, arq2) != 1 && !feof(arq2)) {
                    printf("\nErro ao ler o arquivo!\n");
                    retorno = 1;
                    return retorno;
                }
                while(!feof(arq2)) {
                    if(playlist.musicas[contador] == musica.codigo) {
                        printf("\n\tArtista: %s", musica.artista);
                        printf("\n\tTitulo: %s", musica.titulo);
                        printf("\n\n");
						break;
                    }
                    if(fread(&musica, sizeof(TMusica), 1, arq2) != 1 && !feof(arq2)) {
                        printf("\nErro ao ler o arquivo!\n");
                        retorno = 1;
                        return retorno;
                    }
                }
            }
        } else {
			if(usuario.qtdPlaylistsFav != 0) {
				plylsts = usuario.qtdPlaylistsFav;
				for(qtd = 0; qtd < plylsts; ++qtd) {
					if(usuario.playlistsFav[qtd] == playlist.codigo) {
						++numero;
						if(fseek(arq1, 0, SEEK_SET) != 0) {
							printf("\nErro de arquivo!\n");
							retorno = 1;
							return retorno;
						}
						if(fread(&usuario, sizeof(TUsuario), 1, arq1) != 1) {
							printf("\nErro ao ler o arquivo!\n");
							retorno = 1;
							return retorno;
						}
						while(!feof(arq1)) {
							if(playlist.codigoUsuario == usuario.codigo) {
								break;
							}
							if((fread(&usuario, sizeof(TUsuario), 1, arq1) != 1) && !feof(arq1)) {
								printf("\nErro ao ler o arquivo!\n");
								retorno = 1;
								return retorno;
							}
						}
						printf("\n|--------PLAYLIST FAVORITA--------|\n\n");
						printf("\n\tCodigo: %d", playlist.codigo);
						printf("\n\tTitulo: %s", playlist.titulo);
						printf("\n\tCriada por: %s", usuario.nome);
						printf("\n\n");
						for(contador = 0; contador < playlist.qtdMusicas; ++contador) {
							if(fseek(arq2, 0, SEEK_SET) != 0) {
								printf("\nErro de arquivo!\n");
								retorno = 1;
								return retorno;
							}
							if((fread(&musica, sizeof(TMusica), 1, arq2) != 1) && !feof(arq2)) {
								printf("\nErro ao ler o arquivo!\n");
								retorno = 1;
								return retorno;
							}
							while(!feof(arq2)) {
								if(playlist.musicas[contador] == musica.codigo) {
									printf("\n\tArtista: %s", musica.artista);
									printf("\n\tTitulo: %s", musica.titulo);
									printf("\n\n");
									break;
								}
								if((fread(&musica, sizeof(TMusica), 1, arq2) != 1) && !feof(arq2)) {
									printf("\nErro ao ler o arquivo!\n");
									retorno = 1;
									return retorno;
								}
							}
						}
					}
				}
			}
		}
		if((fread(&playlist, sizeof(TPlaylist), 1, arq) != 1) && !feof(arq)) {
			printf("\nErro ao ler o arquivo!\n");
			retorno = 1;
            return retorno;
        }
    }
    if(numero == 0) {
        printf("\nNao existem playlists cadastradas.\n");
    }
    if(fclose(arq) != 0) {
        printf("\nErro ao fechar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if(fclose(arq1) != 0) {
        printf("\nErro ao fechar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if(fclose(arq2) != 0) {
        printf("\nErro ao fechar o arquivo!\n");
        retorno = 1;
        return retorno;
    }

    system("pause>>NULL");
    system("cls");
    retorno = 1;
    return retorno;
}

//LISTAR PLAYLISTS
int listarOutrasPlaylists(char login[]){
    FILE *arq = NULL, *arq1 = NULL, *arq2 = NULL;
    TUsuario usuario;
    TPlaylist playlist;
    TMusica musica;
    int retorno = 0, contador, numero = 0, existe = 0, qtd, plylsts;

    if (!(arq = fopen(ARQUIVO_PLAYLISTS, "rb")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
	if (!(arq1 = fopen(ARQUIVO_USUARIOS, "rb")))
	{
		printf("\nErro ao acessar o arquivo!\n");
		retorno = 1;
		return retorno;
	}
	if (!(arq2 = fopen(ARQUIVO_MUSICAS, "rb")))
	{
		printf("\nErro ao acessar o arquivo!\n");
		retorno = 1;
		return retorno;
	}

	if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1) {
		printf("\nErro ao ler o arquivo!\n");
		retorno = 1;
		return retorno;
	}
	while(!feof(arq)) {
		if((fread(&usuario, sizeof(TUsuario), 1, arq1) != 1) && !feof(arq1)) {
			printf("\nErro ao ler o arquivo!\n");
			retorno = 1;
			return retorno;
		}
		while(!feof(arq1)) {
			if(strcmp(usuario.login, login) == 0) {
				break;
			}
			if((fread(&usuario, sizeof(TUsuario), 1, arq1) != 1) && !feof(arq1)) {
				printf("\nErro ao ler o arquivo!\n");
				retorno = 1;
				return retorno;
			}
		}
		plylsts = usuario.qtdPlaylistsFav;
		if((usuario.codigo != playlist.codigoUsuario) && !feof(arq)) {
			for(qtd = 0; qtd < plylsts; ++qtd) {
				if(usuario.playlistsFav[qtd] == playlist.codigo) {
					existe = 1;
					break;
				}
			}
			if(existe == 0) {
				++numero;
				if(fseek(arq1, 0, SEEK_SET) != 0) {
					printf("\nErro de arquivo!\n");
					retorno = 1;
					return retorno;
				}
				if(fread(&usuario, sizeof(TUsuario), 1, arq1) != 1) {
					printf("\nErro ao ler o arquivo!\n");
					retorno = 1;
					return retorno;
				}
				while(!feof(arq1)) {
					if(playlist.codigoUsuario == usuario.codigo) {
						break;
					}
					if((fread(&usuario, sizeof(TUsuario), 1, arq1) != 1) && !feof(arq1)) {
						printf("\nErro ao ler o arquivo!\n");
						retorno = 1;
						return retorno;
					}
				}
				printf("\n|--------------PLAYLIST-------------|\n\n");
				printf("\n\tCodigo: %d", playlist.codigo);
				printf("\n\tTitulo: %s", playlist.titulo);
				printf("\n\tCriada por: %s", usuario.nome);
				printf("\n\n");
				for(contador = 0; contador < playlist.qtdMusicas; ++contador) {
					if(fseek(arq2, 0, SEEK_SET) != 0) {
						printf("\nErro de arquivo!\n");
						retorno = 1;
						return retorno;
					}
					if((fread(&musica, sizeof(TMusica), 1, arq2) != 1) && !feof(arq2)) {
						printf("\nErro ao ler o arquivo!\n");
						retorno = 1;
						return retorno;
					}
					while(!feof(arq2)) {
						if(playlist.musicas[contador] == musica.codigo) {
							printf("\n\tArtista: %s", musica.artista);
							printf("\n\tTitulo: %s", musica.titulo);
							printf("\n\n");
							break;
						}
						if((fread(&musica, sizeof(TMusica), 1, arq2) != 1) && !feof(arq2)) {
							printf("\nErro ao ler o arquivo!\n");
							retorno = 1;
							return retorno;
						}
					}
				}
			}
		}
		if((fread(&playlist, sizeof(TPlaylist), 1, arq) != 1) && !feof(arq)) {
		printf("\nErro ao ler o arquivo!\n");
		retorno = 1;
		return retorno;
		}
	}
	if(numero == 0) {
		printf("\nNao existem playlists cadastradas.\n");
	}
	if(fclose(arq) != 0) {
		printf("\nErro ao fechar o arquivo!\n");
		retorno = 1;
		return retorno;
	}
	if(fclose(arq1) != 0) {
		printf("\nErro ao fechar o arquivo!\n");
		retorno = 1;
		return retorno;
	}
	if(fclose(arq2) != 0) {
		printf("\nErro ao fechar o arquivo!\n");
		retorno = 1;
		return retorno;
	}

	system("pause>>NULL");
	system("cls");
	retorno = 1;
	return retorno;
}

//CONSULTAR PLAYLIST
int consultarPlaylist(){

    TUsuario usuario;
    TMusica musica;
    TPlaylist playlist;
    FILE *arq = NULL, *arq1 = NULL, *arq2 = NULL ;
    char titulo[TAM_NOME];
    char resp = ' ';
    int opcao, codigo, retorno = 0, contador;

    if (!(arq = fopen(ARQUIVO_PLAYLISTS, "rb")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if (!(arq1 = fopen(ARQUIVO_USUARIOS, "rb")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if (!(arq2 = fopen(ARQUIVO_MUSICAS, "rb")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }

    do
    {
        printf("\n");
        printf("|----------CONSULTAR PLAYLIST----------|\n\n");
        printf("\t1 - Codigo da playlist\n");
        printf("\t2 - Titulo da playlist\n");
        printf("\t3 - Codigo da Musica\n");
        printf("\t4 - Voltar ao menu anterior\n");
        printf("\t5 - Finalizar programa\n");
        printf("\n");
        printf("|-----------------------------------|\n\n");

        printf("Qual opcao deseja acessar? ");
        scanf("%d", &opcao);

        if (opcao < 1 || opcao > 5)
        {
            printf("Opcao incorreta. Digite novamente!");
            system("pause>>NULL");
            system("cls");
        }
    }
    while (opcao < 1 || opcao > 5);
    system("cls");

    switch(opcao)
    {

    //CODIGO DA PLAYLIST
    case 1:
        printf("\n|--------------CONSULTAR-------------|\n\n");
        printf("\tInforme o codigo da playlist: ");
        scanf("%d", &codigo);
        fflush(stdin);

       if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1)
        {
            printf("\nErro ao ler o arquivo!\n");
            retorno = 1;
            return retorno;
        }

        while (!feof(arq))
        {
            if(fread(&usuario, sizeof(TUsuario), 1, arq1) != 1 && !feof(arq1))
            {
                printf("\nErro ao ler o arquivo!\n");
                retorno = 1;
                return retorno;
            }
            while(!feof(arq1))
            {

                if(fread(&usuario, sizeof(TUsuario), 1, arq1) != 1 && !feof(arq1))
                {
                    printf("\nErro ao ler o arquivo!\n");
                    retorno = 1;
                    return retorno;
                }
            }
            if(codigo ==  playlist.codigo)
            {
                printf("\n|-----------------------------------|\n\n");
                printf("\t      PLAYLIST\n");
                printf("\n\n\t%d musicas cadastradas", playlist.qtdMusicas);
                printf("\n\n\tCodigo: %d", playlist.codigo);
                printf("\n\tTitulo: %s", playlist.titulo);
                printf("\n\tCriada por: %s", usuario.nome);
                printf("\n\n\t....................\n\n");
                printf("\t      MUSICAS\n");
                printf("\n");
                break;
            }
        }
        for(contador = 0; contador < playlist.qtdMusicas; ++contador)
        {
            if(fseek(arq2, 0, SEEK_SET) != 0)
            {
                printf("\nErro de arquivo!\n");
                retorno = 1;
                return retorno;
            }
            if(fread(&musica, sizeof(TMusica), 1, arq2) != 1 && !feof(arq2))
            {
                printf("\nErro ao ler o arquivo!\n");
                retorno = 1;
                return retorno;
            }
            while(!feof(arq2))
            {
                if(playlist.musicas[contador] == musica.codigo)
                {
                printf("\n\tArtista: %s", musica.artista);
                printf("\n\tTitulo: %s", musica.titulo);
                printf("\n\n");
                break;
                }
                if(fread(&musica, sizeof(TMusica), 1, arq2) != 1 && !feof(arq2))
                {
                    printf("\nErro ao ler o arquivo!\n");
                    retorno = 1;
                    return retorno;
                }
            }
        }

        system("pause>>NULL");
        system("cls");
        retorno = 1;
        return retorno;
        break;

    //TITULO
    case 2:
        printf("\n|--------------CONSULTAR-------------|\n\n");
        printf("\tInforme o titulo da playlist: ");
        fflush(stdin);
        fgets(titulo, TAM_NOME, stdin);
        titulo[strlen(titulo) - 1] = '\0';

        if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1)
        {
            printf("\nErro ao ler o arquivo!\n");
            retorno = 1;
            return retorno;
        }

        while (!feof(arq))
        {
            if(fread(&usuario, sizeof(TUsuario), 1, arq1) != 1 && !feof(arq1))
            {
                printf("\nErro ao ler o arquivo!\n");
                retorno = 1;
                return retorno;
            }
            while(!feof(arq1))
            {

                if(fread(&usuario, sizeof(TUsuario), 1, arq1) != 1 && !feof(arq1))
                {
                    printf("\nErro ao ler o arquivo!\n");
                    retorno = 1;
                    return retorno;
                }
            }
            if(strstr(titulo, playlist.titulo)== 0)
            {
                printf("\n|-----------------------------------|\n\n");
                printf("\t      PLAYLIST\n");
                printf("\n\n\t%d musicas cadastradas", playlist.qtdMusicas);
                printf("\n\n\tCodigo: %d", playlist.codigo);
                printf("\n\tTitulo: %s", playlist.titulo);
                printf("\n\tCriada por: %s", usuario.nome);
                printf("\n\n\t....................\n\n");
                printf("\t      MUSICAS\n");
                printf("\n");
                break;
            }
        }
        for(contador = 0; contador < playlist.qtdMusicas; ++contador)
        {
            if(fseek(arq2, 0, SEEK_SET) != 0)
            {
                printf("\nErro de arquivo!\n");
                retorno = 1;
                return retorno;
            }
            if(fread(&musica, sizeof(TMusica), 1, arq2) != 1 && !feof(arq2))
            {
                printf("\nErro ao ler o arquivo!\n");
                retorno = 1;
                return retorno;
            }
            while(!feof(arq2))
            {
                if(playlist.musicas[contador] == musica.codigo)
                {
                printf("\n\tArtista: %s", musica.artista);
                printf("\n\tTitulo: %s", musica.titulo);
                printf("\n\n");
                break;
                }
                if (fread(&usuario, sizeof(TUsuario), 1, arq) != 1 && !feof(arq))
                {
                    printf("\nErro ao ler arquivo!\n");
                    retorno = 1;
                    return retorno;
                }
            }
        }

        system("pause>>NULL");
        system("cls");
        retorno = 1;
        return retorno;
        break;

    //CODIGO DA MUSICA
    case 3:

        system("pause>>NULL");
        system("cls");
        break;

    case 4:
        system("cls");
        retorno = 1;
        return retorno;
        break;

    case 5:
        exit(0);
        system("pause>>NULL");
        system("cls");
        break;
    }
}

//CADASTRAR PLAYLIST
int cadastrarPlaylist(char login[]){
    FILE *arq, *arq1 = NULL, *arq2 = NULL;
    TMusica codigo;
    TPlaylist playlist;
    TUsuario usuario;
    int retorno = 0, proximoCodigo = 0, posicao = 0, qtdPlaylist, qtdMusica;
    char opcao = ' ', resp = ' ';

    system("cls");
    arq = fopen(ARQUIVO_PLAYLISTS, "rb");

    if (arq != NULL)
    {
        proximoCodigo = autoIncrementoCodigo(arq, 3);
        fclose(arq);
    }
    else
    {
        ++proximoCodigo;
    }

    if (!(arq = fopen(ARQUIVO_PLAYLISTS, "a+b")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if (!(arq1 = fopen(ARQUIVO_MUSICAS, "rb")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if (!(arq2 = fopen(ARQUIVO_USUARIOS, "rb")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }

    qtdPlaylist = fread(&playlist, sizeof(TPlaylist), 1, arq);
    while(!feof(arq)) {
        qtdPlaylist += fread(&playlist, sizeof(TPlaylist), 1, arq);
    }

    qtdMusica = fread(&codigo, sizeof(TMusica), 1, arq1);
    while(!feof(arq1)) {
        qtdMusica += fread(&codigo, sizeof(TMusica), 1, arq1);
    }
    if(fseek(arq1, 0, SEEK_SET) != 0) {
        printf("\nErro de arquivo!\n");
        retorno = 1;
        return retorno;
    }
    printf("\n|--------------PLAYLIST-------------|\n\n");
    if(qtdPlaylist <= TAM_PLYLIST)
    {
        playlist.codigo = proximoCodigo;
        printf("\nDigite o titulo da nova playlist: ");
        fflush(stdin);
        fgets(playlist.titulo, TAM_NOME, stdin);
        playlist.titulo[strlen(playlist.titulo) - 1] = '\0';

        if(fread(&usuario, sizeof(TUsuario), 1, arq2) != 1)
        {
            printf("\nErro ao ler o arquivo!\n");
            retorno = 1;
            return retorno;
        }
        while(!feof(arq2))
        {
            if(strcmp(usuario.login,login) == 0)
            {
                playlist.codigoUsuario = usuario.codigo;
                break;
            }
            if(fread(&usuario, sizeof(TUsuario), 1, arq2) != 1)
            {
                printf("\nErro ao ler o arquivo!\n");
                retorno = 1;
                return retorno;
            }
        }

        do
        {
            printf("\nDigite o codigo da nova musica a ser adicionada na playlist: ");
            scanf("%d", &playlist.musicas[posicao]);
            fflush(stdin);
            while(playlist.musicas[posicao] < 0 || playlist.musicas[posicao] > qtdMusica) {
                printf("\nCodigo de musica digitado nao existe no arquivo.\n");
                scanf("%d", &playlist.musicas[posicao]);
                fflush(stdin);
            }
            if(fread(&codigo, sizeof(TMusica), 1, arq1) != 1)
            {
                printf("\nErro ao ler o arquivo!\n");
                retorno = 1;
                return retorno;
            }
            while(!feof(arq1))
            {
                if(playlist.musicas[posicao] == codigo.codigo)
                {
                    printf("\n\tCodigo: %d", codigo.codigo);
                    printf("\n\tArtista: %s", codigo.artista);
                    printf("\n\tTitulo: %s", codigo.titulo);
                    printf("\n\n");
                    printf("Tem certeza que deseja adicionar essa musica? (S ou N)\n");
                    scanf("%c", &resp);
                    resp = toupper(resp);
                    fflush(stdin);
                    while(resp != 'N' && resp != 'S')
                    {
                        printf("\nOpcao invalida!\n");
                        scanf("%c", resp);
                        resp = toupper(resp);
                        fflush(stdin);
                    }
                    if(resp == 'S')
                    {
                        ++posicao;
                        printf("\nMusica %s de %s adicionada a playlist com sucesso!\n", codigo.titulo, codigo.artista);
                        break;
                    } else {
                        printf("\nComece novamente o cadastro da playlist.\n");
                        retorno = 1;
                        return retorno;
                    }
                }
                if(fread(&codigo, sizeof(TMusica), 1, arq1) != 1)
                {
                    printf("\nErro ao ler o arquivo!\n");
                    retorno = 1;
                    return retorno;
                }
            }
            printf("Deseja inserir outra musica na playlist %s? (S ou N)\n", playlist.titulo);
            scanf("%c", &opcao);
            opcao = toupper(opcao);
            fflush(stdin);
            while(opcao != 'S' && opcao != 'N')
            {
                printf("\nOpcao invalida!\n");
                scanf("%c", &opcao);
                opcao = toupper(opcao);
                fflush(stdin);
            }
            fseek(arq1, 0, SEEK_SET);
        }while(opcao != 'N' && posicao < QTD_MUSICAS);

        playlist.qtdMusicas = posicao + 1;

        if(fwrite(&playlist, sizeof(TPlaylist), 1, arq) != 1)
        {
            printf("\nErro ao escrever no arquivo!\n");
            retorno = 1;
            return retorno;
        }
        fflush(arq);
    } else {
        printf("\nVoce atingiu o limite maximo de playlists.\n");
    }

    if(fclose(arq) != 0) {
        printf("\nErro ao fechar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if(fclose(arq1) != 0) {
        printf("\nErro ao fechar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if(fclose(arq2) != 0) {
        printf("\nErro ao fechar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    system("pause>>NULL");
    system("cls");
    retorno = 1;
    return retorno;
}

//INSERIR MÚSICAS NA PLAYLIST
int inserirMusicaPlaylist(char login[]){
    FILE *arq = NULL, *arq1 = NULL, *arq2 = NULL;
    TMusica codigo;
    TPlaylist playlist;
    TUsuario usuario;
    int retorno = 0, posicao, qtdPlaylist, qtdMusica, inteiro, erro = 1;
    char opcao = ' ', resp = ' ';

    system("cls");
    if (!(arq = fopen(ARQUIVO_PLAYLISTS, "r+b")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if (!(arq1 = fopen(ARQUIVO_MUSICAS, "rb")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if (!(arq2 = fopen(ARQUIVO_USUARIOS, "rb")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }

    qtdPlaylist = fread(&playlist, sizeof(TPlaylist), 1, arq);
    while(!feof(arq)) {
        qtdPlaylist += fread(&playlist, sizeof(TPlaylist), 1, arq);
    }
    qtdMusica = fread(&codigo, sizeof(TMusica), 1, arq1);
    while(!feof(arq1)) {
        qtdMusica += fread(&codigo, sizeof(TMusica), 1, arq1);
    }


	if(fseek(arq1, 0, SEEK_SET) != 0) {
        printf("\nErro de arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if(fseek(arq, 0, SEEK_SET) != 0)
    {
        printf("\nErro de arquivo!\n");
        retorno = 1;
        return retorno;
    }

    if(fread(&usuario, sizeof(TUsuario), 1, arq2) != 1)
    {
        printf("\nErro ao ler o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    while(!feof(arq2))
    {
        if(strcmp(usuario.login, login) == 0)
        {
            break;
        }
        if(fread(&usuario, sizeof(TUsuario), 1, arq2) != 1)
        {
            printf("\nErro ao ler o arquivo!\n");
            retorno = 1;
            return retorno;
        }
    }
    printf("\nDigite o codigo da playlist para inserir novas musicas: ");
    scanf("%d", &inteiro);
    fflush(stdin);
    if(inteiro <= 0 || inteiro > qtdPlaylist)
    {
        printf("\nO codigo nao corresponde a nenhuma playlist cadastrada no arquivo. Digite novamente: ");
        scanf("%d", &inteiro);
        fflush(stdin);
    }
    if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1)
    {
        printf("\nErro ao ler o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    while(!feof(arq))
    {
        if((playlist.codigo == inteiro) && (playlist.codigoUsuario == usuario.codigo))
        {
            erro = 0;
			break;
		}

		if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1) {
			if (feof(arq)) {
				printf("\nA playlist esolhida nao pertence a voce. Tente novamente.\n");
				system("pause>>NULL");
				system("cls");
				retorno = 1;
				return retorno;
			}
			else {
				printf("\nErro ao ler arquivo!\n");
				retorno = 1;
				break;
			}
        }
    }
    do
    {
        posicao = playlist.qtdMusicas;
        printf("\nDigite o codigo da nova musica a ser adicionada na playlist %s: ", playlist.titulo);
        scanf("%d", &playlist.musicas[posicao]);
        fflush(stdin);
        while(playlist.musicas[posicao] < 0 || playlist.musicas[posicao] > qtdMusica)
        {
            printf("\nCodigo de musica digitado nao existe no arquivo.\n");
            scanf("%d", &playlist.musicas[posicao]);
            fflush(stdin);
        }
        if(fread(&codigo, sizeof(TMusica), 1, arq1) != 1)
        {
            printf("\nErro ao ler o arquivo!\n");
            retorno = 1;
            return retorno;
        }
        while(!feof(arq1))
        {
            if(playlist.musicas[posicao] == codigo.codigo)
            {
                printf("\n\tCodigo: %d", codigo.codigo);
                printf("\n\tArtista: %s", codigo.artista);
                printf("\n\tTitulo: %s", codigo.titulo);
                printf("\n\n");
                printf("Tem certeza que deseja adicionar essa musica? (S ou N)\n");
                scanf("%c", &resp);
                resp = toupper(resp);
                fflush(stdin);
                while(resp != 'N' && resp != 'S')
                {
                    printf("\nOpcao invalida!\n");
                    scanf("%c", resp);
                    resp = toupper(resp);
                    fflush(stdin);
                }
                if(resp == 'S')
                {
					++posicao;
					printf("\nMusica %s de %s adicionada a playlist com sucesso!\n", codigo.titulo, codigo.artista);
                    break;
                }
            }
            if(fread(&codigo, sizeof(TMusica), 1, arq1) != 1)
            {
                printf("\nErro ao ler o arquivo!\n");
                retorno = 1;
                return retorno;
            }
        }
        printf("Deseja inserir outra musica na playlist %s? (S ou N)\n", playlist.titulo);
        scanf("%c", &opcao);
        opcao = toupper(opcao);
        fflush(stdin);
        while(opcao != 'S' && opcao != 'N')
        {
            printf("\nOpcao invalida!\n");
            scanf("%c", &opcao);
            opcao = toupper(opcao);
            fflush(stdin);
        }

		if(fseek(arq1, 0, SEEK_SET) != 0) {
			printf("\nErro de arquivo!\n");
			retorno = 1;
			return retorno;
		}

	}while(opcao != 'N' && posicao < QTD_MUSICAS);

	playlist.qtdMusicas = posicao;

	if(fseek(arq, -1 * sizeof(TPlaylist), SEEK_CUR) != 0)
    {
        printf("\nErro de arquivo!\n");
        retorno = 1;
        return retorno;
    }

    if(fwrite(&playlist, sizeof(TPlaylist), 1, arq) != 1)
    {
        printf("\nErro ao escrever no arquivo!\n");
        retorno = 1;
        return retorno;
    }
    fflush(arq);

    if(fclose(arq) != 0)
    {
        printf("\nErro ao fechar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if(fclose(arq1) != 0) {
        printf("\nErro ao fechar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if(fclose(arq2) != 0) {
        printf("\nErro ao fechar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    system("pause>>NULL");
    system("cls");
    retorno = 1;
    return retorno;
}

//EXCLUIR MÚSICAS DA PLAYLIST
int excluirMusicaPlaylist(char login[]) {
	FILE *arq = NULL, *arq1 = NULL, *arq2 = NULL, *arq3 = NULL;
	TPlaylist playlist;
	TMusica musica;
	TUsuario usuario;
	int playlistEditada, quantidade, retorno = 0, codigo, contador, existe = 0;
	int musicasTemp[QTD_MUSICAS];
	char opcao = ' ', resp = ' ', continuar = ' ';

	do
	{
		quantidade = 0;
		if (!(arq = fopen(ARQUIVO_PLAYLISTS, "rb")))
		{
			printf("\nErro ao acessar o arquivo!\n");
			retorno = 1;
			return retorno;
		}

		if (!(arq1 = fopen(ARQUIVO_TEMP, "wb")))
		{
			printf("\nErro ao acessar o arquivo!\n");
			retorno = 1;
			return retorno;
		}

		if (!(arq2 = fopen(ARQUIVO_MUSICAS, "rb")))
		{
			printf("\nErro ao acessar o arquivo!\n");
			retorno = 1;
			return retorno;
		}

		if (!(arq3 = fopen(ARQUIVO_USUARIOS, "rb")))
		{
			printf("\nErro ao acessar o arquivo!\n");
			retorno = 1;
			return retorno;
		}

		while (!feof(arq))
		{
			quantidade += fread(&playlist, sizeof(TPlaylist), 1, arq);
		}

		if (quantidade == 0)
		{
			printf("\nNao existem playlists para excluir no arquivo.\n");
			system("pause>>NULL");
			retorno = 1;
			return retorno;
		}
		else
		{
			if(fread(&usuario, sizeof(TUsuario), 1, arq3) != 1) {
				printf("\nErro ao ler o arquivo!\n");
				retorno = 1;
				return retorno;
			}
			while(!feof(arq3)) {
				if(strcmp(usuario.login, login) == 0) {
					break;
				}
				if(fread(&usuario, sizeof(TUsuario), 1, arq3) != 1 && !feof(arq3)) {
					printf("\nErro ao ler o arquivo!\n");
					retorno = 1;
					return retorno;
				}
			}
			fseek(arq, 0, SEEK_SET);
			printf("Qual o codigo da playlist que deseja editar?\n");
			scanf("%d", &playlistEditada);
			fflush(stdin);
			while(playlistEditada <= 0 || playlistEditada > quantidade) {
				printf("\nA playlist digitada nao existe no arquivo. Digite novamente.\n");
				scanf("%d", &playlistEditada);
				fflush(stdin);
			}
			if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1)
			{
				printf("\nErro ao ler arquivo!\n");
				retorno = 1;
				return retorno;
			}

			while (!feof(arq))
			{
				if ((playlist.codigo == playlistEditada) && (playlist.codigoUsuario == usuario.codigo))
				{
					printf("\n\tCodigo: %d", playlist.codigo);
					printf("\n\tTitulo: %s", playlist.titulo);
					printf("\n\n");
					for(contador = 0; contador < playlist.qtdMusicas; ++contador) {
						if(fseek(arq2, 0, SEEK_SET) != 0) {
							printf("\nErro de arquivo!\n");
							retorno = 1;
							return retorno;
						}
						if(fread(&musica, sizeof(TMusica), 1, arq2) != 1 && !feof(arq2)) {
							printf("\nErro ao ler o arquivo!\n");
							retorno = 1;
							return retorno;
						}
						while(!feof(arq2)) {
							if(playlist.musicas[contador] == musica.codigo) {
								printf("\n\tCodigo da musica: %d", musica.codigo);
								printf("\n\tArtista: %s", musica.artista);
								printf("\n\tTitulo: %s", musica.titulo);
								printf("\n\n");
								break;
							}
							if(fread(&musica, sizeof(TMusica), 1, arq2) != 1 && !feof(arq2)) {
								printf("\nErro ao ler o arquivo!\n");
								retorno = 1;
								return retorno;
							}
						}
					}
					printf("Tem certeza que deseja editar essa playlist?\n");
					scanf("%c", &resp);
					fflush(stdin);
					resp = toupper(resp);
					break;
				} else {
					printf("\nA playlist nao pertence a voce e por isso nao pode ser editada. Tente novamente.\n");
					retorno = 1;
					return retorno;
				}
				if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1 && !feof(arq))
				{
					printf("\nErro ao ler arquivo!\n");
					retorno = 1;
					return retorno;
				}
			}

			if (resp == 'S')
			{

				do {
					printf("Qual o codigo da musica que deseja excluir?");
					scanf("%d", &codigo);
					for(contador = 0; contador < playlist.qtdMusicas; ++contador){
						if(playlist.musicas[contador] == codigo) {
							existe = 1;
							break;
						}
					}
					if(existe = 1) {
						for(contador = 0; contador < playlist.qtdMusicas; ++contador) {
							if(codigo != playlist.musicas[contador]) {
								musicasTemp[contador] = playlist.musicas[contador];
							}
						}
						if (fseek(arq, 0, SEEK_SET) != 0)
						{
							printf("\nErro de arquivo!\n");
							retorno = 1;
							return retorno;
						}
						if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1)
						{
							printf("\nErro ao ler arquivo!\n");
							retorno = 1;
							return retorno;
						}
						while (!feof(arq))
						{
							if (playlist.codigo != playlistEditada)
							{
								if(fwrite(&playlist, sizeof(TPlaylist), 1, arq1) != 1)
								{
									printf("\nErro ao escrever no arquivo!\n");
									retorno = 1;
									return retorno;
								}
							}
							if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1 && !feof(arq))
							{
								printf("\nErro ao ler arquivo!\n");
								retorno = 1;
								return retorno;
							}
						}
						fflush(arq1);
						if (fclose(arq) == EOF)
						{
							printf("\nErro ao fechar o arquivo.\n");
							retorno = 1;
							return retorno;
						}

						if (remove(ARQUIVO_MUSICAS) != 0)
						{
							printf("\nErro ao remover o arquivo.\n");
							retorno = 1;
							return retorno;
						}

						if (fclose(arq1) == EOF)
						{
							printf("\nErro ao fechar o arquivo.\n");
							retorno = 1;
							return retorno;
						}

						if (fclose(arq2) == EOF)
						{
							printf("\nErro ao fechar o arquivo.\n");
							retorno = 1;
							return retorno;
						}

						if (rename(ARQUIVO_TEMP, ARQUIVO_PLAYLISTS) != 0)
						{
							printf("\nErro ao renomear o arquivo.\n");
							retorno = 1;
							return retorno;
						}
					} else {
						printf("\nA musica digitada nao esta contida na playlist escolhida. Tente novamente.\n");
						retorno = 1;
						return retorno;
					}
				} while(continuar != 'N');
			}
			do
			{
				printf("\nDeseja continuar excluindo playlists? (S ou N): ");
				scanf("%c", &opcao);
				fflush(stdin);
				opcao = toupper(opcao);

				if (opcao != 'N' && opcao != 'S')
				{
					printf("\nOpcao Invalida! ");
					printf("\n");
				}

			}
			while (opcao != 'N' && opcao != 'S');
		}
	}
	while (opcao != 'N');

	system("pause>>NULL");
	system("cls");
	retorno = 1;
	return retorno;
}

//EXCLUIR PLAYLIST
int excluirPlaylist(){
    FILE *arq = NULL, *arq1 = NULL, *arq2 = NULL;
    TPlaylist playlist;
	TMusica musica;
    int playlistExcluida, quantidade, retorno = 0, codigo, contador;
    char opcao = ' ', resp = ' ';

    do
    {
        quantidade = 0;
        if (!(arq = fopen(ARQUIVO_PLAYLISTS, "rb")))
        {
            printf("\nErro ao acessar o arquivo!\n");
            retorno = 1;
            return retorno;
        }

        if (!(arq1 = fopen(ARQUIVO_TEMP, "wb")))
        {
            printf("\nErro ao acessar o arquivo!\n");
            retorno = 1;
            return retorno;
        }

		if (!(arq2 = fopen(ARQUIVO_MUSICAS, "rb")))
        {
            printf("\nErro ao acessar o arquivo!\n");
            retorno = 1;
            return retorno;
        }

        while (!feof(arq))
        {
            quantidade += fread(&playlist, sizeof(TPlaylist), 1, arq);
        }

        if (quantidade == 0)
        {
            printf("\nNao existem playlists para excluir no arquivo.\n");
            system("pause>>NULL");
            retorno = 1;
            return retorno;
        }
        else
        {
            fseek(arq, 0, SEEK_SET);
            printf("Qual o codigo da playlist que deseja excluir?\n");
            scanf("%d", &playlistExcluida);
            fflush(stdin);
            if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1)
            {
                printf("\nErro ao ler arquivo!\n");
                retorno = 1;
                return retorno;
            }

            while (!feof(arq))
            {
                if (playlist.codigo == playlistExcluida)
                {
                    printf("\n\tCodigo: %d", playlist.codigo);
                    printf("\n\tTitulo: %s", playlist.titulo);
					printf("\n\n");
					for(contador = 0; contador < playlist.qtdMusicas; ++contador) {
						if(fseek(arq2, 0, SEEK_SET) != 0) {
							printf("\nErro de arquivo!\n");
							retorno = 1;
							return retorno;
						}
						if(fread(&musica, sizeof(TMusica), 1, arq2) != 1 && !feof(arq2)) {
							printf("\nErro ao ler o arquivo!\n");
							retorno = 1;
							return retorno;
						}
						while(!feof(arq2)) {
							if(playlist.musicas[contador] == musica.codigo) {
								printf("\n\tArtista: %s", musica.artista);
								printf("\n\tTitulo: %s", musica.titulo);
								printf("\n\n");
								break;
							}
							if(fread(&musica, sizeof(TMusica), 1, arq2) != 1 && !feof(arq2)) {
								printf("\nErro ao ler o arquivo!\n");
								retorno = 1;
								return retorno;
							}
						}
					}
					printf("Tem certeza que deseja excluir essa playlist?\n");
                    scanf("%c", &resp);
                    fflush(stdin);
                    resp = toupper(resp);
                    break;
                }
                if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1 && !feof(arq))
                {
                    printf("\nErro ao ler arquivo!\n");
                    retorno = 1;
                    return retorno;
                }
            }

            if (resp == 'S')
            {
                if (fseek(arq, 0, SEEK_SET) != 0)
                {
                    printf("\nErro de arquivo!\n");
                    retorno = 1;
                    return retorno;
                }
                if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1)
                {
                    printf("\nErro ao ler arquivo!\n");
                    retorno = 1;
                    return retorno;
                }
                while (!feof(arq))
                {
                    if (playlist.codigo != playlistExcluida)
                    {
                        if(fwrite(&playlist, sizeof(TPlaylist), 1, arq1) != 1)
                        {
                            printf("\nErro ao escrever no arquivo!\n");
                            retorno = 1;
                            return retorno;
                        }
                    }
                    if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1 && !feof(arq))
                    {
                        printf("\nErro ao ler arquivo!\n");
                        retorno = 1;
                        return retorno;
                    }
                }
                fflush(arq1);
                if (fclose(arq) == EOF)
                {
                    printf("\nErro ao fechar o arquivo.\n");
                    retorno = 1;
                    return retorno;
                }

                if (remove(ARQUIVO_PLAYLISTS) != 0)
                {
                    printf("\nErro ao remover o arquivo.\n");
                    retorno = 1;
                    return retorno;
                }

                if (fclose(arq1) == EOF)
                {
                    printf("\nErro ao fechar o arquivo.\n");
                    retorno = 1;
                    return retorno;
                }

				if (fclose(arq2) == EOF)
                {
                    printf("\nErro ao fechar o arquivo.\n");
                    retorno = 1;
                    return retorno;
                }

                if (rename(ARQUIVO_TEMP, ARQUIVO_PLAYLISTS) != 0)
                {
                    printf("\nErro ao renomear o arquivo.\n");
                    retorno = 1;
                    return retorno;
                }
            }

            do
            {
                printf("\nDeseja continuar excluindo playlists? (S ou N): ");
                scanf("%c", &opcao);
                fflush(stdin);
                opcao = toupper(opcao);

                if (opcao != 'N' && opcao != 'S')
                {
                    printf("\nOpcao Invalida! ");
                    printf("\n");
                }

            }
            while (opcao != 'N' && opcao != 'S');
        }
    }
    while (opcao != 'N');
    system("pause>>NULL");
    system("cls");
    retorno = 1;
    return retorno;
}

//FAVORITAR PLAYLIST
int favoritarPlaylist(char login[]){
    TUsuario usuario;
    TPlaylist playlist;
    FILE *arq = NULL, *arq1 = NULL;
    int playlistFavoritada, retorno = 0, qtdPlaylist;
    char opcao = ' ';

    system("cls");
    if (!(arq = fopen(ARQUIVO_PLAYLISTS, "rb")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if (!(arq1 = fopen(ARQUIVO_USUARIOS, "r+b")))
    {
        printf("\nErro ao acessar o arquivo!\n");
        retorno = 1;
        return retorno;
    }

    qtdPlaylist = fread(&playlist, sizeof(TPlaylist), 1, arq);
    while(!feof(arq)) {
        qtdPlaylist += fread(&playlist, sizeof(TPlaylist), 1, arq);
    }

    if(fseek(arq, 0, SEEK_SET) != 0) {
        printf("\nErro de arquivo!\n");
        retorno = 1;
        return retorno;
    }

    if(fread(&usuario, sizeof(TUsuario), 1, arq1) != 1 && !feof(arq1))
    {
        printf("\nErro ao ler o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    while(!feof(arq1))
    {
        if(strcmp(usuario.login, login) == 0)
        {
            break;
        }
        if(fread(&usuario, sizeof(TUsuario), 1, arq1) != 1 && !feof(arq1))
        {
            printf("\nErro ao ler o arquivo!\n");
            retorno = 1;
            return retorno;
        }
    }

    do
    {
        printf("\nDigite o codigo da playlist a ser favoritada: ");
        scanf("%d", &playlistFavoritada);
        fflush(stdin);
        while(playlistFavoritada <= 0 || playlistFavoritada > qtdPlaylist) {
            printf("\nO codigo escolhido nao existe no arquivo. Digite outro codigo.");
            scanf("%d", &playlistFavoritada);
            fflush(stdin);
        }

        if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1 && !feof(arq))
        {
            printf("\nErro ao ler o arquivo!\n");
            retorno = 1;
            return retorno;
        }
        while(!feof(arq))
        {
            if(playlistFavoritada == playlist.codigo)
            {
                if(playlist.codigoUsuario == usuario.codigo){
                    printf("\nA playlist escolhida pertence a voce e nao pode ser favoritada. Tente outra vez.\n");
                    retorno = 1;
                    return retorno;
                }
                usuario.playlistsFav[usuario.qtdPlaylistsFav] = playlistFavoritada;
                ++usuario.qtdPlaylistsFav;
                if(fseek(arq1, -sizeof(TUsuario), SEEK_CUR) != 0) {
                    printf("\nErro de arquivo!\n");
                    retorno = 1;
                    return retorno;
                }
                if(fwrite(&usuario, sizeof(TUsuario), 1, arq1) != 1) {
                    printf("\nErro ao escrever no arquivo!\n");
                    retorno = 1;
                    return retorno;
                }
                break;
            }
            if(fread(&playlist, sizeof(TPlaylist), 1, arq) != 1 && !feof(arq))
            {
                printf("\nErro ao ler o arquivo!\n");
                retorno = 1;
                return retorno;
            }
        }
        do
        {
            printf("\nDeseja continuar favoritando playlists? (S ou N)");
            scanf("%c", &opcao);
            fflush(stdin);
            opcao = toupper(opcao);
            if (opcao != 'N' && opcao != 'S')
            {
                printf("\nOpcao Invalida! ");
                printf("\n");
            }
        }
        while (opcao != 'N' && opcao != 'S');
        system("cls");
        fflush(arq);
    }
    while (opcao != 'N');

    if (fclose(arq) == EOF)
    {
        printf("\nErro ao fechar o arquivo!\n");
        retorno = 1;
        return retorno;
    }
    if (fclose(arq1) == EOF)
    {
        printf("\nErro ao fechar o arquivo!\n");
        retorno = 1;
        return retorno;
    }

    system("pause>>NULL");
    system("cls");
    retorno = 1;
    return retorno;
}

//FUNÇÃO PARA UTILIZAR O AUTO INCREMENTO
int autoIncrementoCodigo(FILE* arq, int flag) {

	TUsuario ultimoUsuario;
	TMusica ultimaMusica;
	TPlaylist ultimaPlaylist;

	int ultimoCodigo = 0;

	if (arq != NULL) {
		if (flag == 1) {
			fseek(arq, (long) -sizeof(TUsuario), SEEK_END);
			fread(&ultimoUsuario, sizeof(TUsuario), 1, arq);
			ultimoCodigo = ultimoUsuario.codigo;

		} else if (flag == 2) {
			fseek(arq, (long) -sizeof(TMusica), SEEK_END);
			fread(&ultimaMusica, sizeof(TMusica), 1, arq);
			ultimoCodigo = ultimaMusica.codigo;
		} else {
			fseek(arq, (long) -sizeof(TPlaylist), SEEK_END);
			fread(&ultimaPlaylist, sizeof(TPlaylist), 1, arq);
			ultimoCodigo = ultimaPlaylist.codigo;
		}
	}

	return (ultimoCodigo + 1);
}
