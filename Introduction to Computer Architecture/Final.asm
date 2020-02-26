; *********************************************************************
; *
; * Grupo 13 - Quinta-Feira 10H30
; * André Filipe Pinto de Oliveira - ist193686
; * Beatriz Cruz Alves - ist193691
; * Maria Francisco Ribeiro - ist193735
; *
; *********************************************************************

; *********************************************************************
; *
; * Teclas utilizadas no programa:
; * 0 - Utilizada para virar o volante para a esquerda
; * 1 - Utilizada para enviar um missil
; * 2 - Utilizada para enviar um missil
; * 3 - Utilizada para virar o volante para a direita
; * 4 - Utilizada para terminar o jogo e depois regressar ao start
; * 5 - Utilizada para iniciar o jogo
; * 6 - Utilizada para parar o jogo e retomar
; *
; *********************************************************************

; **********************************************************************
; * Constantes
; **********************************************************************
DISPLAYS       			EQU 0A000H     ; endereço dos displays de 7 segmentos (periférico POUT-1)
TEC_LIN       			EQU 0C000H     ; endereço das linhas do teclado (periférico POUT-2)
TEC_COL        			EQU 0E000H     ; endereço das colunas do teclado (periférico PIN)
LINHA          			EQU 8          ; linha a testar (1ª linha, 0001H)
ECRA           			EQU 8000H      ; endereço do ecrã (pixelscreen)
BYTES_ECRA     			EQU 128        ; tamanho do ecrã em bytes
BYTE_PADRAO    			EQU 0CCH       ; byte para escrever no ecrã e desenhar um padrão 
PONTO		   			EQU 0FH        ; primeiro byte a ser preenchido 
SCORE_MAX      			EQU 99	       ; valor máximo do score
PRIMEIROPIXEL			EQU 8000H
ULTIMOPIXEL   			EQU 8080H
TECLA_START 			EQU 5
TECLA_RESET				EQU 4
TECLA_PAUSA				EQU 6

PLACE 2000H
pilha:      TABLE 200H        
SP_inicial: 

mascaras_pintar:
	STRING 80H, 40H, 20H, 10H, 08H, 04H, 02H, 01H

nave:
	STRING 27, 0, 5, 32						; Linha inicial / Coluna Inicial / Altura / Tamanho
	STRING 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	STRING 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	STRING 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	STRING 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	STRING 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1

volante:
	STRING 29, 14, 3, 4
	STRING 0,0,0,0
	STRING 1,1,1,1
	STRING 0,0,0,0

volante_esquerda:
	STRING 29, 14, 3, 4
	STRING 0,0,0,1
	STRING 0,1,1,0
	STRING 1,0,0,0

volante_direita:
	STRING 29, 14, 3, 4
	STRING 1,0,0,0
	STRING 0,1,1,0
	STRING 0,0,0,1

pause:
	STRING 1,1,4,3
	STRING 1,0,1
	STRING 1,0,1
	STRING 1,0,1
	STRING 1,0,1

start:
	STRING 3, 1, 7, 29
	STRING 0,1,1,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1
	STRING 1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0
	STRING 1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0
	STRING 0,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,1,0,1,1,1,1,0,0,0,0,1,0,0
	STRING 0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0
	STRING 0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,0,0,1,0,0
	STRING 1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0

press:
	STRING 13, 1, 7, 29
	STRING 1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1
	STRING 1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0
	STRING 1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0
	STRING 1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0
	STRING 1,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1
	STRING 1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1
	STRING 1,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,0

tecla_5:
	STRING 22, 13, 7, 5
	STRING 1,1,1,1,1
	STRING 1,0,0,0,0
	STRING 1,0,0,0,0
	STRING 1,1,1,1,0
	STRING 0,0,0,0,1
	STRING 1,0,0,0,1
	STRING 0,1,1,1,0

game:
	STRING 3, 3, 7, 26
	STRING 1,1,1,1,0,0,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,1,1,1,1,1
	STRING 1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,1,1,0,0,1,0,0,0,0
	STRING 1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0
	STRING 1,0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,0,0,1,0,0,1,1,1,1,0
	STRING 1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0
	STRING 1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0
	STRING 1,1,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,1,1,1,1

over:
	STRING 14, 3, 7, 26
	STRING 0,1,1,1,0,0,0,1,0,0,0,1,0,0,1,1,1,1,1,0,0,1,1,1,1,0
	STRING 1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,1
	STRING 1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,1
	STRING 1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,1,1,1,0,0,0,1,1,1,1,0
	STRING 1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,0,0
	STRING 1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0
	STRING 0,1,1,1,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,0,1,0,0,0,1
	
smile:
	STRING 24, 13, 7, 6
	STRING 0,1,0,0,1,0
	STRING 0,1,0,0,1,0
	STRING 0,1,0,0,1,0
	STRING 0,0,0,0,0,0
	STRING 0,1,1,1,1,0
	STRING 1,0,0,0,0,1
	STRING 1,0,0,0,0,1

missil_ativo:
	STRING 0
missil:
	STRING 26, 15, 1, 1
	STRING 1

meteorito_1:
	STRING 0,0,1,1
	STRING 1

meteorito_2:
	STRING 3,14,2,2
	STRING 1,1
	STRING 1,1

meteorito_bom_3:
	STRING 6,9,3,3
	STRING 1,0,1
	STRING 0,1,0
	STRING 1,0,1

meteorito_mau_3:
	STRING 6,19,3,3
	STRING 0,1,0
	STRING 1,1,1
	STRING 0,1,0

meteorito_bom_4:
	STRING 10,6,4,4
	STRING 1,0,0,1
	STRING 0,1,1,0
	STRING 0,1,1,0
	STRING 1,0,0,1

meteorito_mau_4:
	STRING 10,23,4,4
	STRING 0,1,1,0
	STRING 1,1,1,1
	STRING 1,1,1,1
	STRING 0,1,1,0

meteorito_bom_5:
	STRING 15,2,5,5
	STRING 1,0,0,0,1
	STRING 0,1,0,1,0
	STRING 0,0,1,0,0
	STRING 0,1,0,1,0
	STRING 1,0,0,0,1

meteorito_mau_5:
	STRING 15,27,5,5
	STRING 0,1,1,1,0
	STRING 1,1,1,1,1
	STRING 1,1,1,1,1
	STRING 1,1,1,1,1
	STRING 0,1,1,1,0

meteorito_destruido:
	STRING 15,14,5,5
	STRING 0,1,0,1,0
	STRING 1,0,1,0,1
	STRING 0,1,0,1,0
	STRING 1,0,1,0,1
	STRING 0,1,0,1,0

adicionar_as_coordenadas:
	STRING 0,0

proximas_coordenadas_meteorito:
	STRING 0,0

proximas_coordenadas_missil:
	STRING 0,0

tecla_carregada:
	STRING 0

meteorito_obj_1:
	STRING 1, 0 				; Nº de linhas a descer / Nº de colunas para o lado
	WORD meteorito_1

missil_obj:
	STRING -1,0
	WORD missil


rot_teclas:						; Tabela com rotinas para quando se pressiona uma tecla
	WORD rot_tecla_0 
	WORD rot_tecla_1
	WORD rot_tecla_2
	WORD rot_tecla_3
	WORD rot_tecla_4  
	WORD rot_tecla_5
	WORD rot_tecla_6
	WORD rot_tecla_sem_funcao   

rot_int:
	WORD rot_int_0
	WORD rot_int_1

event_int:
	WORD 0
	WORD 0

; **********************************************************************
; * Código
; **********************************************************************
PLACE      0000H

inicilizacoes:
    MOV BTE, rot_int
	MOV SP, SP_inicial

    MOV R1, 0
    MOV R4, DISPLAYS   ; endereço do periférico dos displays
	MOV R6, 0		   ; Armazena a tecla carregada

    MOVB [R4], R1      ; Coloca os displays a 0

    EI0
    EI1
    EI

; corpo principal do programa
reset_pixelscreen:
	MOV R8, 0		   ; inicia o score a 0
	MOV R4, DISPLAYS
	MOVB [R4], R8
	MOV R2, meteorito_obj_1
	ADD R2,2
	MOV R3, meteorito_1
	MOV [R2], R3
	MOV R2, proximas_coordenadas_meteorito
	MOVB [R2], R8
	ADD R2,1
	MOVB [R2], R8
	MOV R2, proximas_coordenadas_missil
	MOVB [R2], R8
	ADD R2,1
	MOVB [R2], R8

	CALL limpa_ecra
	CALL start_screen

start_game:
	CALL teclado_inicio
	MOV R2, tecla_carregada
	MOVB R7, [R2]
	MOV R3, TECLA_START
	CMP R7, R3
	JNZ start_game

comeco:
	CALL limpa_ecra
	CALL escreve_inicio

main_ciclo:
	CALL teclado_inicio
	CALL funcao_tecla

	CALL ciclo_asteroides
	CALL ciclo_missil
	JMP main_ciclo

start_screen:
	PUSH R5
	MOV R5, start
	CALL tabelas
	MOV R5, press
	CALL tabelas
	MOV R5, tecla_5
	CALL tabelas
	POP R5
	RET

ciclo_asteroides:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R6
	PUSH R7
	MOV R0, event_int
	MOV R1, [R0]
	CMP R1, 0
	JZ  nao_ha_interrupcao_meteorito

	CALL teclado_inicio

	CALL existe_meteorito

	MOV R6, meteorito_obj_1
	ADD R6,2
	MOV R5, [R6]
	CALL apagar_tabelas 

	CALL mudar_coordenadas
	CALL mudar_sprite
	CALL asteroide_colide_ou_sai
	CALL escrever_meteorito

	MOV R1, 0
	MOV [R0], R1

nao_ha_interrupcao_meteorito:
	POP R7
	POP R6
	POP R3
	POP R2
	POP R1
	POP R0
	RET

asteroide_colide_ou_sai:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	PUSH R7
	PUSH R9
	PUSH R10
	PUSH R11

	MOV R0, proximas_coordenadas_meteorito
	MOVB R1, [R0] ; // Armazena a linha
	ADD R0,1
	MOVB R2, [R0] ; // Armazena a coluna
	ADD R0,1
	MOVB R3, [R0] ; // Armazena a "sprite"

	ADD R3, 2
	MOVB R4, [R3] ; // Armazena a altura da "sprite"
	ADD R3, 1
	MOVB R5, [R3] ; // Armazena o comprimento da "sprite"

	MOV R6, 23
	CMP R1, R6
	JEQ dead_or_score

	MOV R6, -1
	CMP R2, R6
	JEQ reset_asteroide

	ADD R2, R5
	MOV R6, 43
	CMP R6,R2
	JEQ reset_asteroide

	MOV R0, proximas_coordenadas_missil
	MOVB R7, [R0] ; // Armazena a linha do missil
	ADD R7,1
	MOVB R11, [R0] ; // Armazena a coluna do missil

	JMP fim_asteroide_colide_ou_sai

dead_or_score:
	MOV R3, meteorito_obj_1
	ADD R3,2
	MOV R7, [R3]
	MOV R2, meteorito_bom_5
	CMP R7,R2
	JEQ score

	MOV R2, meteorito_mau_5
	CMP R7,R2
	JEQ dead

	JMP fim_asteroide_colide_ou_sai

dead:
	CALL limpa_ecra
	CALL game_over
espera_dead:
	CALL teclado_inicio
	MOV R2, tecla_carregada
	MOVB R7, [R2]
	MOV R3, TECLA_RESET
	CMP R7, R3
	JNZ espera_dead

	JMP reset_pixelscreen

score:
	ADD R8,3
	CALL escreve_score
	JMP reset_asteroide

fim_asteroide_colide_ou_sai:
	POP R11
	POP R10
	POP R9
	POP R7
	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	RET

reset_asteroide:
	MOV R2, proximas_coordenadas_meteorito
	MOV R7,0
	MOVB [R2], R7
	ADD R2,1
	MOVB [R2],R7
	MOV R2, meteorito_obj_1
	ADD R2,2
	MOV R3, meteorito_1
	MOV [R2], R3
	CALL existe_meteorito
	JMP fim_asteroide_colide_ou_sai


ciclo_missil:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R6
	PUSH R7
	PUSH R8
	PUSH R9
	PUSH R10

	MOV R4, missil_ativo
	MOVB R7, [R4]
	CMP R7, 0
	JEQ fim_ciclo_missil

	MOV R0, event_int
	ADD R0,2
	MOV R1, [R0]
	CMP R1, 0
	JZ  fim_ciclo_missil

	CALL existe_missil

	MOV R5, missil
	CALL apagar_tabelas

	CALL update_coordenadas_missil

	MOV R6, proximas_coordenadas_missil
	MOVB R7, [R6]
	MOV R10, 15
	CMP R7, R10
	JEQ limite_missil

	CALL desenha_missil

	MOV R5, missil
	CALL tabelas
	MOV R1, 0
	MOV [R0], R1
	JMP fim_ciclo_missil

limite_missil:
	MOV R8, missil_ativo
	MOV R9, 0
	MOVB [R8], R9

	MOV R8, proximas_coordenadas_missil
	MOVB [R8], R9
	ADD R8,1
	MOVB [R8], R9


fim_ciclo_missil:
	POP R10
	POP R9
	POP R8
	POP R7
	POP R6
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	RET

update_coordenadas_missil:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4

	MOV R0, proximas_coordenadas_missil
	MOV R1, missil_obj
	MOVB R2, [R0]
	MOVB R3, [R1]
	ADD R2,R3
	MOVB [R0], R2
	ADD R0,1
	ADD R1,1
	MOVB R2, [R0]
	MOVB R3, [R1]
	ADD R2,R3
	MOVB [R0], R2

	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	RET

desenha_missil:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4

	MOV R0, missil
	MOV R1, proximas_coordenadas_missil
	MOVB R2, [R1]

	MOVB [R0], R2
	ADD R0,1
	ADD R1,1
	MOVB R2, [R1]
	MOVB [R0], R2

fim_desenha_missil:
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	RET

escreve_score:         ; escreve o score nos displays
	PUSH R8
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	MOV R2, R8         ; a variável R2 vai conter as dezenas
	MOV R3, R8         ; a variável R3 vai conter as unidades
	MOV R5, 0AH        ; armazena 10 na variável para fazer a divisão inteira e o módulo
	DIV R2, R5         ; obtem-se as dezenas do score      
	MOD R3, R5		   ; obtem-se as unidades do score

	MOV R5, R2
	SHL R5, 4
	OR R5, R3		   ; junta as dezenas e as unidades em R5

	MOV R4, DISPLAYS
	
	MOVB [R4], R5      ; escreve no periférico de saída (displays)
	POP R5
	POP R4
	POP R3
	POP R2
	POP R8
	RET

; *****************************************************************************
; *	ROTINAS DE INTERRUPÇÃO: Rotinas de interrupção ocorridas devido aos clocks
; *****************************************************************************

rot_int_0:
	PUSH R0
	PUSH R1
	MOV R0, event_int
	MOV R1, 1
	MOV [R0], R1
	POP R1
	POP R0
	RFE

rot_int_1:
	PUSH R0
	PUSH R1
	MOV R0, event_int
	ADD R0,2
	MOV R1, 1
	MOV [R0], R1
	POP R1
	POP R0
	RFE

; *****************************************************************************
; *	ROTINAS DAS TECLAS PRESSIONADAS: Rotinas provocadas pelas teclas
; *****************************************************************************

rot_tecla_0:								; Volante vira à esquerda e consequencias
	MOV R5, volante_esquerda
	CALL tabelas
	MOV R6, adicionar_as_coordenadas
	ADD R6,1
	MOV R7,2
	MOVB [R6], R7
	MOV R6, missil_obj
	ADD R6,1
	MOVB [R6], R7
	RET

rot_tecla_1:
	PUSH R0
	PUSH R1
	MOV R0, missil_ativo
	MOV R1, 1
	MOVB [R0],R1

	POP R1
	POP R0
	RET 	 							 ; Inicia um missil

rot_tecla_2:
	PUSH R0
	PUSH R1
	MOV R0, missil_ativo
	MOV R1, 1
	MOVB [R0],R1

	POP R1
	POP R0
	RET 							 ; Inicia um missil

rot_tecla_3:
	MOV R5, volante_direita
	CALL tabelas
	MOV R6, adicionar_as_coordenadas
	ADD R6,1
	MOV R7,-2
	MOVB [R6], R7
	MOV R6, missil_obj
	ADD R6,1
	MOVB [R6], R7
	RET 							 ; Volante vira à direita e consequencias

rot_tecla_4: 								; Termina o jogo
	JMP dead
rot_tecla_5: 								; Inicia o jogo, por isso, sem rotina
	RET
rot_tecla_6: 								; Pausa o jogo
	MOV R5, pause
	CALL tabelas
	CALL pausa
	RET 							 
rot_tecla_sem_funcao: 						; As restantes teclas não têm função e como tal, executam esta função
	MOV R5, volante
	CALL tabelas
	MOV R6, adicionar_as_coordenadas
	ADD R6,1
	MOV R7,0
	MOVB [R6], R7
	MOV R6, missil_obj
	ADD R6,1
	MOVB [R6], R7
	RET

; *****************************************************************************
; * PAUSA: Executa a função pausa do jogo
; *****************************************************************************

pausa:
	CALL teclado_inicio
	MOV R2, tecla_carregada
	MOVB R7, [R2]
	MOV R3, 7
	CMP R7,R3
	JNZ pausa

sair_da_pausa:
	CALL teclado_inicio
	MOV R2, tecla_carregada
	MOVB R7, [R2]
	MOV R3, TECLA_PAUSA
	CMP R7,R3
	JNZ sair_da_pausa

	MOV R5, pause
	CALL apagar_tabelas

	RET


; *****************************************************************************
; * EXISTE_MISSIL: Verifica a existência de um missil no ecrã
; *****************************************************************************
existe_missil:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	MOV R6, proximas_coordenadas_missil
	MOVB R0, [R6]
	ADD R6,1
	MOVB R1, [R6] 

	CMP R0, 0
	JEQ iniciar_missil

fim_existe_missil:
	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	RET

iniciar_missil:
	CMP R1, 0
	JNZ fim_existe_missil

	MOV R2, 15
	MOVB [R6], R2
	SUB R6,1
	MOV R2,26
	MOVB [R6], R2
	JMP fim_existe_missil

; *****************************************************************************
; * EXISTE_METEORITO: Verifica se já existe um meteorito no ecrã
; *****************************************************************************

existe_meteorito:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	PUSH R7
	MOV R6, proximas_coordenadas_meteorito
	MOV R7, meteorito_obj_1
	ADD R7, 2
	MOV R5, [R7]
	MOVB R4, [R6] 						; Armazena a linha do meteorito
	MOV R1, 1
	ADD R6, R1
	MOVB R2, [R6]						; Armazena a coluna do meteorito

	CMP R2, 0
	JZ inicia_meteorito

fim_existe_meteorito:
	POP R7
	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	RET

inicia_meteorito:
	CMP R4, 0
	JNZ fim_existe_meteorito

	MOV R3, 14
	MOVB [R6], R3

	MOVB [R5], R4
	ADD R5,1
	MOVB [R5], R3
	SUB R5,1

	CALL tabelas
	JMP fim_existe_meteorito

; *****************************************************************************
; * MUDA_COORDENADAS: Faz os calculos para a posição seguinte do meteorito
; *****************************************************************************

mudar_coordenadas:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	PUSH R7
	PUSH R8
	PUSH R9
	MOV R8, adicionar_as_coordenadas
	MOV R7, meteorito_obj_1
	MOV R6, proximas_coordenadas_meteorito
	MOVB R9, [R8]						; Armazena o número de linhas a aumentar devido a teclas
	MOVB R0, [R7] 						; Armazena o nº de linhas a aumentar
	MOVB R1, [R6]						; Armazena a linha anterior
	ADD R0,R1
	ADD R0, R9
	MOVB [R6], R0
	ADD R8,1
	ADD R7,1
	ADD R6,1
	MOVB R9, [R8]						; Armazena o número de colunas a aumentar devido a teclas
	MOVB R0, [R7]						; Armazena o nº de colunas a aumentar
	MOVB R1, [R6]						; Armazena a coluna anterior
	ADD R0,R9
	ADD R0,R1
	MOVB [R6], R0
	POP R9
	POP R8
	POP R7
	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	RET

; *****************************************************************************
; * MUDAR_SPRITE: Verifica se o meteorito já chegou à posição de 
; *						  mudar de "sprite"
; *****************************************************************************

mudar_sprite:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R5
	PUSH R6
	PUSH R7
	MOV R6, proximas_coordenadas_meteorito
	MOVB R1, [R6] 
								; Armazena a proxima linha a ser pintada
	MOV R0,3
	CMP R1, R0
	JEQ muda_para_meteo_2

	MOV R0,6
	CMP R1, R0
	JEQ muda_para_meteo_3

	MOV R0, 10
	CMP R1,R0
	JEQ muda_para_meteo_4

	MOV R0, 15
	CMP R1,R0
	JEQ muda_para_meteo_5

fim_mudar_sprite:
	POP R7
	POP R6
	POP R5
	POP R2
	POP R1
	POP R0
	RET

muda_para_meteo_2:
	MOV R7, meteorito_obj_1
	ADD R7, 2
	MOV R2, meteorito_2
	MOV [R7], R2
	JMP fim_mudar_sprite

muda_para_meteo_3:
	MOV R7, meteorito_obj_1
	ADD R7, 2
	MOV R2, meteorito_bom_3
	MOV [R7], R2
	JMP fim_mudar_sprite

muda_para_meteo_4:
	MOV R7, meteorito_obj_1
	ADD R7, 2
	MOV R2, meteorito_bom_4
	MOV [R7], R2
	JMP fim_mudar_sprite

muda_para_meteo_5:
	MOV R7, meteorito_obj_1
	ADD R7, 2
	MOV R2, meteorito_bom_5
	MOV [R7], R2
	JMP fim_mudar_sprite

; *****************************************************************************
; * ESCREVER_METEORITO: Escreve o meteorito seguinte
; *****************************************************************************

escrever_meteorito:
	PUSH R1
	PUSH R5
	PUSH R6
	PUSH R7
	MOV R6, proximas_coordenadas_meteorito
	MOV R7, meteorito_obj_1
	ADD R7, 2
	MOV R5, [R7] 							; R5 armazena a "sprite"
	MOVB R1, [R6]
	MOVB [R5], R1
	ADD R6,1
	ADD R5,1
	MOVB R1, [R6]
	MOVB [R5], R1
	SUB R5,1
	CALL tabelas
	POP R7
	POP R6
	POP R5
	POP R1
	RET

; *****************************************************************************
; * FUNCAO_TECLA: Chama a rotina corresponde às teclas
; *****************************************************************************

funcao_tecla:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	MOV R0, rot_teclas

	MOV R1, tecla_carregada
	MOVB R2, [R1]
	MOV R3, 2
	MUL R2, R3

	ADD R0, R2
	MOV R4, [R0]
	CALL R4

	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	RET


; *****************************************************************************
; * TECLADO_INICIO: Verifica se existe alguma tecla a ser pressionada
; *****************************************************************************

teclado_inicio:
    PUSH R0
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4
    PUSH R9
    MOV R0, 0
    MOV  R2, TEC_LIN         ; endereço do periférico das linhas
    MOV  R3, TEC_COL         ; endereço do periférico das colunas
    MOV  R1, LINHA           ; testar a linha 4 

teclado:
    MOVB [R2], R1            ; escrever no periférico de saída (linhas)
    MOVB R0, [R3]            ; ler do periférico de entrada (colunas)

    CMP R0, 0                ; há tecla premida?
    JNZ ha_tecla

    JMP nova_linha

ha_tecla:
    MOV R2, R0               ; houve uma tecla premida

sai_teclado:
    CALL calcula_tecla
    POP R9
    POP R4
    POP R3
    POP R2
    POP R1
    POP R0
    RET

nova_linha:
	SHR R1, 1
	CMP R1,0
	JZ sem_tecla

	JMP teclado

sem_tecla:
	MOV R1, 2
	MOV R2, 8
	JMP sai_teclado

; ******************************************************************************
; * CALCULA_TECLA: Lê a linha e a coluna e calcula a letra/numero da tecla
; * ARGUMENTOS: [R1] - coluna_carregada
; * 			[R4] - linha_carregada
; ******************************************************************************

calcula_tecla:
	PUSH R5
	PUSH R6
	PUSH R7
	PUSH R8
	MOV R5, -1		   ; inicia a contagem das linhas em -1
	MOV R6, -1		   ; inicia a contagem das colunas em -1
	
linha:				   ; calcula o valor da linha da tecla pressionada
	SHR R1, 1 		   ; avança a posição do 1
	ADD R5, 1 		   ; aumenta o valor da linha
	CMP R1, 0		   ; já está a 0?
	JNZ linha		   ; se não está a 0, repete

coluna:				   ; calcula o valor da coluna da tecla pressionada
	SHR R2, 1		   ; avança a posição do 1
	ADD R6, 1		   ; aumenta o valor da coluna
	CMP R2, 0		   ; já está a 0?
	JNZ coluna	       ; se não está a 0, repete
	
verifica_teclas:       ; efetua as contas para obter o valor da tecla
	MOV R7, 4
	MUL R7, R5
	ADD R7, R6

	CMP R7, 6
	JGT tecla_sem_valor

guarda_tecla:
	MOV R8, tecla_carregada
	MOVB [R8], R7
	POP R8
	POP R7
	POP R6
	POP R5
	RET

tecla_sem_valor:
	MOV R7, 7
	JMP guarda_tecla

; ******************************************************************************
; * ESCREVE_PIXEL: Calcula o byte a escrever
; * ARGUMENTOS: R2 - LINHA
; * 			R6 - COLUNA
; *				R3 - COPIA COLUNA
; * 			R1 - Nº DE LINHAS
; *				R10 - Nº DE COLUNAS
; *				R7 - COPIA Nº DE COLUNAS
; ******************************************************************************
escreve_pixel:
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	PUSH R7
	PUSH R8
	PUSH R10
	PUSH R11

	MOV R8, PRIMEIROPIXEL    ;8000H
	MOV R1, 4
	MUL R2, R1      		 ; R2 - Linha
	MOV R3, 8
	MOV R4, R6				 ; 
	DIV R4, R3				 ; R4 - Coluna
	ADD R2, R4      		 ; Linha + coluna
	ADD R2, R8      		 ; Linha + coluna + 8000H

	MOVB R5, [R2] 			 ; Armazena valor do bit
	MOD R6, R3				 ; Descobre a coluna dentro do byte

	MOV R7, mascaras_pintar
	ADD R7, R6               ; Adiciona às máscaras a coluna para obter a mascara correta

	MOVB R8, [R7]            ; Coloca a máscara no R8

	CMP R9, 0                ; Se o bit for 0, não queremos pintar
	JZ loop

	OR R5, R8				 ; Coloca bit a 1

fim:
	MOVB [R2], R5            ; Imprime no PixelScreen
	POP R11
	POP R10
	POP R8
	POP R7
	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	RET

loop:						; Passa a máscara a 0 no ponto que nao queremos
	NOT R8
	AND R5, R8
	JMP fim

; ******************************************************************************
; * APAGA_PIXEL: Apaga todos os pixeis
; ******************************************************************************

apaga_pixel:
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	PUSH R7
	PUSH R8
	PUSH R10
	PUSH R11

	MOV R8, PRIMEIROPIXEL    ;8000H
	MOV R1, 4
	MUL R2, R1      		 ; R2 - Linha
	MOV R3, 8
	MOV R4, R6				 ; 
	DIV R4, R3				 ; R4 - Coluna
	ADD R2, R4      		 ; Linha + coluna
	ADD R2, R8      		 ; Linha + coluna + 8000H

	MOVB R5, [R2] 			 ; Armazena valor do bit
	MOD R6, R3				 ; Descobre a coluna dentro do byte

	MOV R7, mascaras_pintar
	ADD R7, R6               ; Adiciona às máscaras a coluna para obter a mascara correta

	MOVB R8, [R7]            ; Coloca a máscara no R8

	NOT R8
	AND R5, R8

fim_apaga_pixel:
	MOVB [R2], R5            ; Imprime no PixelScreen
	POP R11
	POP R10
	POP R8
	POP R7
	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	RET

; ******************************************************************************
; * TABELAS: Lê a tabela dada e guarda os dados da mesma
; * Argumentos: R5 - Tabela
; * Output: R2 - Linha
; *			R6 - Coluna
; *			R3 - Cópia da Coluna
; *			R1 - Numero de linhas
; *			R10 - Numero de colunas
; *			R7 - Cópia do numero de colunas
; *			R9 - Valor do bit (0 ou 1)
; ******************************************************************************
tabelas:
	PUSH R1
	PUSH R2
	PUSH R3
		PUSH R5
	PUSH R6
	PUSH R7
	PUSH R8
	PUSH R9
	PUSH R10

	MOVB R2, [R5]    ; armazena linha
	ADD R5, 1
	MOVB R6, [R5]    ; armazena coluna
	MOVB R3, [R5]    ; cópia da coluna
	ADD R5, 1
	MOVB R1, [R5]    ; armazena numero de linhas
	ADD R5, 1
	MOVB R10, [R5]   ; armazena numero de colunas
	MOVB R7, [R5]    ; cópia do numero de colunas

ciclo_tabelas:
	ADD R5, 1        
	MOVB R9, [R5]    	; Move o valor do bit para R9 (0 ou 1)

	CALL escreve_pixel  ; Vai escrever no PixelScreen

	ADD R6, 1           ; Aumenta a coluna
	SUB R10, 1			; Diminui o número de colunas que faltam

	JZ ciclo_muda_linha ; Se o número de colunas for 0, passa para a linha de baixo

	JMP ciclo_tabelas 			; Se não for 0, repete o ciclo

ciclo_muda_linha:
	
	MOV R6, R3       ; devolve o valor inicial das colunas
	MOV R10, R7      ; devolve o valor inicial do numero de colunas
	ADD R2, 1 		 ; Aumenta a linha
	SUB R1, 1		 ; Diminui o número de linhas
	JZ terminar      ; Se chegamos à última linha, termina

	JMP ciclo_tabelas        ; Senão, imprime essa linha

terminar:
	POP R10
	POP R9
	POP R8
	POP R7
	POP R6
		POP R5
	POP R3
	POP R2
	POP R1
	RET

; ******************************************************************************
; * APAGAR_TABELAS: Contrária à TABELAS; apaga a "sprite"
; ******************************************************************************
apagar_tabelas:
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R6
	PUSH R7
	PUSH R8
	PUSH R9
	PUSH R10

	MOVB R2, [R5]    ; armazena linha
	ADD R5, 1
	MOVB R6, [R5]    ; armazena coluna
	MOVB R3, [R5]    ; cópia da coluna
	ADD R5, 1
	MOVB R1, [R5]    ; armazena numero de linhas
	ADD R5, 1
	MOVB R10, [R5]   ; armazena numero de colunas
	MOVB R7, [R5]    ; cópia do numero de colunas

ciclo_apagar_tabelas:
	ADD R5, 1        
	MOVB R9, [R5]    	; Move o valor do bit para R9 (0 ou 1)

	MOV R9, 0
	CALL escreve_pixel  ; Vai escrever no PixelScreen

segue:
	ADD R6, 1           ; Aumenta a coluna
	SUB R10, 1			; Diminui o número de colunas que faltam

	JZ ciclo_muda_linha_apagar ; Se o número de colunas for 0, passa para a linha de baixo

	JMP ciclo_apagar_tabelas 			; Se não for 0, repete o ciclo

ciclo_muda_linha_apagar:
	
	MOV R6, R3       ; devolve o valor inicial das colunas
	MOV R10, R7      ; devolve o valor inicial do numero de colunas
	ADD R2, 1 		 ; Aumenta a linha
	SUB R1, 1		 ; Diminui o número de linhas
	JZ terminar_apagar      ; Se chegamos à última linha, termina

	JMP ciclo_apagar_tabelas        ; Senão, imprime essa linha

terminar_apagar:
	POP R10
	POP R9
	POP R8
	POP R7
	POP R6
	POP R3
	POP R2
	POP R1
	RET

; *****************************************************************************
; * LIMPA_ECRA: Limpa o ecrã todo
; *****************************************************************************
limpa_ecra:
	PUSH 	R3				 ;guarda o valor de R0			
	PUSH 	R4				 ;guarda o valor de R1
	PUSH 	R5				 ;guarda o valor de R2
	
	MOV		R3,PRIMEIROPIXEL ;inicializa com o valor do primeiro pixel
	MOV		R4,ULTIMOPIXEL	 ;incializa com o valor do ultimo pixel
	MOV		R5,00H		 ;mascara para limpar o pixel, isto e, por a zero

limpa:	
	CMP		R3,R4			 ;ve se o endereco atualizado é o ultimo
	JZ		ecra_limpo		 ;o ecra esta limpo	
	MOV 	[R3],R5			 ;coloca o endereco de memoria a zero, limpa o pixel
	ADD		R3,2			 ;muda de endereco
	JMP		limpa		 	 ;volta para o ciclo

ecra_limpo:
	MOV		R3,R5			 ;coloca o ultimo endereco a zero
	POP		R5				 ;recupera o valor de R2
	POP 	R4   					;recupera o valor de R1
	POP 	R3						;recupera o valor de R0
	RET						 ;sai da rotina

; *****************************************************************************
; * ESCREVE_INICIO: Desenha a nave e o volante inicial
; *****************************************************************************

escreve_inicio:
	MOV R5, nave
	CALL tabelas

	MOV R5, volante
	CALL tabelas

	RET

; *****************************************************************************
; * GAME_OVER: Quando o jogador perde, mostra a mensagem final
; *****************************************************************************

game_over:
	MOV R5, game
	CALL tabelas

	MOV R5, over
	CALL tabelas
	RET