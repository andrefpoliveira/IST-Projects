#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Limite descrição, do responsável e do nome dos
participantes */
#define LIMITE 64

/* Limite de caracteres de uma linha */
#define DATA 344

#define PARTICIPANTES 3
#define NUMSALAS 10
#define MAXEVENTOS 100

#define troca(x,y) {Evento t = x; x=y; y=t;}

/* Estrutura do Evento */
typedef struct {
	int flag;
	char desc[LIMITE];
	int data;
	int inicio;
	int duracao;
	int sala;
	char responsavel[LIMITE];
	char participantes[PARTICIPANTES][LIMITE];
} Evento;

/* Estrutura de um dia */
typedef struct {
	int dia;
	int mes;
	int ano;
} Dia;

/* Matriz que contêm os eventos. 10 salas x 100 eventos */
Evento salas[NUMSALAS][MAXEVENTOS];

/* Array que contêm o numero de eventos em cada sala */
int eventos[NUMSALAS];

/* Lê o resto da linha após o comando */
Evento readLine(char comando) {
	Evento e;
	char s[DATA];
	int i,j=0;
	char *token;
	char c[2]=":";
	char d[2]="\n";
	fgets(s,DATA,stdin); /* Obtem o resto da linha */

	/* Remove o espaco inicial antes dos dados */
	if (s[0] == ' ') 
    	memmove(s, s+1, strlen(s));


	token = strtok(s,d); /* Remove o caracter \n do final */
	token = strtok(s,c); /* Remove o primeiro caracter ':' */

	if (comando=='a') {
		for (i=0;i<PARTICIPANTES;i++) {
			strcpy(e.participantes[i],"\0");
		}

		while (token!=NULL) {
			if (j==0) {
				strcpy(e.desc,token);
			}
			else if (j==1) {
				e.data=atoi(token);
			}
			else if (j==2) {
				e.inicio=atoi(token);
			}
			else if (j==3) {
				e.duracao=atoi(token);
			}
			else if (j==4) {
				e.sala=atoi(token);
			}
			else if (j==5) {
				strcpy(e.responsavel,token);
			}
			else {
				strcpy(e.participantes[j-6],token);
			}
			j++;
			token=strtok(NULL,c);
		}

		e.flag=1;

	} else if (comando=='i') {
		strcpy(e.desc,token);
		e.inicio=atoi(strtok(NULL,c));
	} else if (comando=='t') {
		strcpy(e.desc,token);
		e.duracao=atoi(strtok(NULL,c));
	} else if (comando=='m') {
		strcpy(e.desc,token);
		e.sala=atoi(strtok(NULL,c));
	} else if (comando=='A' || comando=='R') {
		strcpy(e.desc,token);
		strcpy(e.participantes[0],strtok(NULL,c));
	}

	return e;
}

/* Transforma as horas no formato hhmm para minutos */
int transformaHoras(int hora) {
	int h,m;
	h=hora/100;
	m=hora%100;
	return h*60+m;
}

/* Verifica a disponibilidade dos participantes do evento que se quer adicionar */
int pessoaIndisponivel(Evento e, char nome[LIMITE],char comando) {
	int i,j,a,b,c,d,g,h;
	int inicioTemp,fimTemp,inicioE, fimE; /* Guardam os valores do inicio e fim de horas dos eventos */
	Evento temp;

	if (strcmp("\0",nome)) {

		inicioE=transformaHoras(e.inicio);
		fimE=inicioE+transformaHoras(e.duracao);

		for (i=0;i<NUMSALAS;i++) {
			for (j=0;j<eventos[i];j++) {
				temp=salas[i][j];
				if (temp.data==e.data) {

					inicioTemp=transformaHoras(temp.inicio);
					fimTemp=inicioTemp+transformaHoras(temp.duracao);

					a = inicioTemp<=inicioE;
					b = fimTemp>inicioE;
					c = inicioTemp<fimE;
					d = fimTemp>=fimE;
					g = inicioE<=inicioTemp;
					h = fimTemp<=fimE;

					if (!(strcmp(temp.responsavel,nome) && strcmp(temp.participantes[0],nome) &&
						strcmp(temp.participantes[1],nome) && strcmp(temp.participantes[2],nome))
						&& ((a && b) || (c && d) || (g && h))) {
							if (comando=='A')
								printf("Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n",nome);
							else
								printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n",e.desc,nome);
							return 1;
					}
				}
			}
		}
	}
	return 0;
}

/* Verifica a disponibilidade da sala em que se quer adicionar o evento */
int salaOcupada(Evento e, int x) {
	int a,b,c,d,g,h;
	int i,impossivel=0;
	int inicioTemp,fimTemp,inicioE, fimE; /* Guardam os valores do inicio e fim de horas dos eventos */

	Evento temp;

	inicioE=transformaHoras(e.inicio);
	fimE=inicioE+transformaHoras(e.duracao);

	for (i=0;i<x;i++){
		temp=salas[e.sala-1][i];

		if (temp.flag && temp.data==e.data) {

			inicioTemp=transformaHoras(temp.inicio);
			fimTemp=inicioTemp+transformaHoras(temp.duracao);

			a = inicioTemp<=inicioE;
			b = fimTemp>inicioE;
			c = inicioTemp<fimE;
			d = fimTemp>=fimE;
			g = inicioE<=inicioTemp;
			h = fimTemp<=fimE;

			if ((a && b) || (c && d) || (g && h)) {
				impossivel=1;
			}
		}
	}
	return impossivel;
}

/* Procura um evento pela descrição e se existir, devolve:
   Flag a 0 ou 1 - Indica se foi encontrado ou nao
   Numero da sala
   Numero do evento */
int *encontraEvento(char desc[LIMITE]) {
	int i,j,encontrado=0;
	static int r[3]; /* Inicializa o pointer */
	Evento temp;
	r[0]=encontrado;

	for (i=0;i<NUMSALAS;i++) {
		for (j=0;j<eventos[i];j++) {
			temp=salas[i][j];
			if (!strcmp(temp.desc,desc)) {
				r[0]=++encontrado;
				r[1]=i;
				r[2]=j;
			}
		}
	}
	return r;
}

/* Adicionar o evento pretendido, se possível */
int comando_a(Evento e,char comando) {
	int x = eventos[e.sala-1]; /*Armazena o valor da sala */
	int i,j,k,z;

	if(salaOcupada(e,x)) {
		printf("Impossivel agendar evento %s. Sala%d ocupada.\n",e.desc,e.sala);
		return 0;
	} else {
		i = pessoaIndisponivel(e,e.responsavel,comando);
		j = pessoaIndisponivel(e,e.participantes[0],comando);
		k = pessoaIndisponivel(e,e.participantes[1],comando);
		z = pessoaIndisponivel(e,e.participantes[2],comando);

		i = i || j || k || z;

		if (!i) {
			salas[e.sala-1][x]=e;
			eventos[e.sala-1]+=1;
			return 1;
		}
	}
	return 0;
}

/* Transforma a data no formato ddmmaaaa na struct Dia */
Dia transformaData(int data) {
	Dia dia;
	int ano,mes,dias;
	ano = data%10000;
	dia.ano=ano;
	data = data/10000;
	mes = data%100;
	dia.mes=mes;
	dias = data/100;
	dia.dia=dias;
	return dia;
}

/* Usado para ordenar as salas (comando s e comando l)*/
int comparaDatas(Dia d1, Dia d2, int h1, int h2, int s1, int s2) {
	if ((d2.ano<d1.ano)||(d2.ano==d1.ano && d2.mes<d1.mes)||
	((d2.ano==d1.ano && d2.mes==d1.mes && d2.dia<d1.dia))) {
		/* Verifica as datas */
		return 1;
	} else if (d2.ano==d1.ano&&d2.mes==d1.mes&&d2.dia==d1.dia&&h2<h1) {
		/* Caso as datas seja igual, verifica as horas */
		return 1;
	} else if (d2.ano==d1.ano && d2.mes==d1.mes && d2.dia==d1.dia && h2==h1 && s2<s1) {
		/* Caso as datas e as horas sejam iguais, ordena por salas */
		return 1;
	}
	return 0;
}

/* Algoritmo que ordena as salas (comando s) */
void ordenaSalas(int x) {
	Dia d1, d2;
	int i,j,h1,h2;
	for (i=0;i<eventos[x];i++) {
		d1=transformaData(salas[x][i].data);
		h1=transformaHoras(salas[x][i].inicio);

		for (j=i+1;j<eventos[x];j++) {

			if (salas[x][j].flag) {
				d2=transformaData(salas[x][j].data);
				h2=transformaHoras(salas[x][j].inicio);

				if (comparaDatas(d1,d2,h1,h2,salas[x][i].sala,salas[x][j].sala) || !salas[x][i].flag) {
					troca(salas[x][i],salas[x][j]);
					/* Renova os valores consoante o novo menor elemento */
					d1=transformaData(salas[x][i].data);
					h1=transformaHoras(salas[x][i].inicio);
				}
			}
		}
	}
}

/* Imprime os eventos de uma determinada sala por ordem cornológica */
void comando_s(int x) {
	int i,j;

	ordenaSalas(x);

	for (i=0; i<eventos[x]; i++) {
		printf("%s %08d %04d %d Sala%d %s\n*",
			salas[x][i].desc,salas[x][i].data,salas[x][i].inicio,salas[x][i].duracao,
			salas[x][i].sala,salas[x][i].responsavel);
		for (j=0;j<3;j++) {
			if (strcmp(salas[x][i].participantes[j],"\0"))
				printf(" %s",salas[x][i].participantes[j]);
		}
		printf("\n");
	}
}

/* Remove um evento */
void comando_r(char desc[LIMITE]) {
	int *array;

	array=encontraEvento(desc); /* Recebe as coordenadas */

	if (array[0]) {
		salas[array[1]][array[2]].flag=0;
		ordenaSalas(array[1]);
		eventos[salas[array[1]][array[2]].sala-1]-=1;
	} else {
		printf("Evento %s inexistente.\n",desc);
	}
}

/* Funcao que recebe os comandos i, t e m
   Faz as alterações nos eventos */
void comando_i_t_m(char comando) {
	Evento e = readLine(comando),novo,antigo;
	int *coords;

	coords=encontraEvento(e.desc); /* Recebe as coordenadas */

	if (coords[0]) {
		antigo=novo=salas[coords[1]][coords[2]]; /* Cria cópias do evento */
		
		if (comando=='i') {
			novo.inicio=e.inicio;
		} else if (comando=='t') {
			novo.duracao=e.duracao;
		} else {
			novo.sala=e.sala;
		}

		/* Apaga o evento se encontrado
		   Tenta colocar o novo evento
		   Se falhar recoloca o antigo */
		comando_r(e.desc);
		if (!comando_a(novo,'t')) {
			comando_a(antigo,'t');
		}

	} else {
		printf("Evento %s inexistente.\n",e.desc);
	}
}

/* Funcao auxiliar que verifica o numero de participantes
   e se um determinado participante já se encontra no evento */
int adicionaParticipante(Evento e,Evento novo) {
	int i;

	for (i=0;i<3;i++) {
		if (!strcmp("\0",novo.participantes[i])) {
			return i;
		} else if (!strcmp(e.participantes[0],novo.participantes[i]))
			return -1;
	}
	return 4;
}

/* Adiciona um participante a um evento */
void comando_A() {
	Evento e = readLine('A'),novo,antigo;
	int *coords,i;

	coords=encontraEvento(e.desc); /* Recebe as coordenadas */

	if (coords[0]) {
		antigo=novo=salas[coords[1]][coords[2]]; /* Cria cópias dos eventos */

		i=adicionaParticipante(e,novo);

		if (i!=4 && i!=-1) {
			strcpy(novo.participantes[i],e.participantes[0]);
			comando_r(e.desc);
			if (!comando_a(novo,'A'))
				comando_a(antigo,'A');
			
		} else if (i==4) {
			printf("Impossivel adicionar participante. Evento %s ja tem 3 participantes.\n",e.desc);
		}

		
	} else {
		printf("Evento %s inexistente.\n",e.desc);
	}
}

/* Funcao auxiliar que verifica se é possivel retirar um
   participante de um evento */
int *removeParticipante(Evento e,Evento novo) {
	int i,participantes=0;
	static int a[2];
	a[1]=3;

	for (i=0;i<3;i++) {
		if (!strcmp(e.participantes[0],novo.participantes[i])) {
			a[1]=i;
		}
		if (strcmp(novo.participantes[i],"\0"))
			participantes+=1;
	}
	a[0]=participantes;
	return a;
}

/* Remove um participante de um evento */
void comando_R() {
	Evento e = readLine('R'),novo,antigo;
	int *coords,*participantes;

	coords=encontraEvento(e.desc); /* Recebe as coordenadas */

	if (coords[0]) {
		antigo=novo=salas[coords[1]][coords[2]];

		participantes=removeParticipante(e,novo);

		if (participantes[0]==1 && participantes[1]!=3)
			printf("Impossivel remover participante. Participante %s e o unico participante no evento %s.\n",e.participantes[0],e.desc);
		else if (participantes[1]!=3) {
			strcpy(novo.participantes[participantes[1]],"\0");
			comando_r(e.desc);
			if (!comando_a(novo,'A'))
				comando_a(antigo,'A');
		}
	} else {
		printf("Evento %s inexistente.\n",e.desc);
	}
}

/* Ordena todos os eventos de todas as salas e imprime-os */
void comando_l() {
	Dia d1,d2;
	Evento s[1000];
	int i,j,k=0,h1,h2;

	/* Coloca os eventos todos num so array */
	for (i=0;i<NUMSALAS;i++) {
		for (j=0;j<eventos[i];j++) {
			s[k++]=salas[i][j];
		}
	}

	/*Procede a ordenacao dos eventos*/
	for (i=0;i<k;i++) {
		d1=transformaData(s[i].data);
		h1=transformaHoras(s[i].inicio);

		for (j=i+1;j<k;j++) {

				d2=transformaData(s[j].data);
				h2=transformaHoras(s[j].inicio);

				if (comparaDatas(d1,d2,h1,h2,s[i].sala,s[j].sala)) {
					troca(s[i],s[j]);
					d1=transformaData(s[i].data);
					h1=transformaHoras(s[i].inicio);
				}
		}
	}

	/* Imprime os eventos */
	for (i=0;i<k;i++) {
		printf("%s %08d %04d %d Sala%d %s\n*",
		s[i].desc,s[i].data,s[i].inicio,s[i].duracao,
		s[i].sala,s[i].responsavel);
		for (j=0;j<3;j++) {
			if (strcmp(s[i].participantes[j],"\0"))
				printf(" %s",s[i].participantes[j]);
		}
		printf("\n");
	}
}

/* Coloca todos as flags dos eventos a 0 */
void inicializaSalas() {
	int i,j;
	for (i=0; i<NUMSALAS; i++) {
		for (j=0; j<MAXEVENTOS; j++) {
			salas[i][j].flag=0;
		}
	}
}

/* Coloca o array eventos a 0, ou seja,
   nao existem eventos */
void inicializaEventos() {
	int i;
	for (i=0;i<NUMSALAS;i++) {
		eventos[i]=0;
	}
}

/* Procura todos os eventos em que o Ze e o responsavel e imprime o numero */
void comando_Z() {
	int i,j;
	int counter = 0;
	for(i=0;i<NUMSALAS;i++) {
		for(j=0;j<MAXEVENTOS;j++) {
			if (!strcmp(salas[i][j].responsavel,"Ze") && salas[i][j].flag) {
				++counter;
			}
		}
	}
	printf("%d\n",counter);
}

/* Funcao principal
   Le os comandos e procede a execucao de cada um */
int main() {
	Evento e;
	char comando;
	int i,sala;
	char desc[LIMITE];

	inicializaSalas();
	inicializaEventos();

	scanf("%c",&comando);

	while (comando!='x') {
		switch (comando) {
			case 'a':
				e = readLine('a');
				comando_a(e,'a');
				break;
			case 'r':
				getchar();
				for (i=0;(desc[i]=getchar())!='\n';i++);
				desc[i]='\0';
				comando_r(desc);
				break;
			case 'l':
				comando_l();
				getchar();
				break;
			case 's':
				getchar();
				scanf("%d",&sala);
				getchar();
				comando_s(sala-1);
				break;
			case 'i':
				comando_i_t_m('i');
				break;
			case 't':
				comando_i_t_m('t');
				break;
			case 'm':
				comando_i_t_m('m');
				break;
			case 'A':
				comando_A();
				break;
			case 'R':
				comando_R();
				break;
			case 'Z':
				comando_Z();
				break;
		}

		scanf("%c",&comando);
	}
	return 0;
}
