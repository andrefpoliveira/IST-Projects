; *********************************************************************
; *
; * Teclas utilizadas no programa:
; * 0 - Utilizada para virar o volante para a esquerda
; * 1 - Utilizada para aumentar o score 3 pontos
; * 2 - Utilizada para virar o volante para a direita
; *
; *********************************************************************

; **********************************************************************
; * Constantes
; **********************************************************************
DISPLAYS       EQU 0A000H     ; endereço dos displays de 7 segmentos (periférico POUT-1)
TEC_LIN        EQU 0C000H     ; endereço das linhas do teclado (periférico POUT-2)
TEC_COL        EQU 0E000H     ; endereço das colunas do teclado (periférico PIN)
LINHA          EQU 1          ; linha a testar (1ª linha, 0001H)
ECRA           EQU 8000H      ; endereço do ecrã (pixelscreen)
BYTES_ECRA     EQU 128        ; tamanho do ecrã em bytes
BYTE_PADRAO    EQU 0CCH       ; byte para escrever no ecrã e desenhar um padrão 
PONTO		   EQU 0FH        ; primeiro byte a ser preenchido 
SCORE_MAX      EQU 99	      ; valor máximo do score

; **********************************************************************
; * Dados
; **********************************************************************
PLACE       1000H
pilha:      TABLE 100H        ; espaço reservado para a pilha 
                              ; (200H bytes, pois são 100H words)
SP_inicial:                   ; este é o endereço (1200H) com que o SP deve ser 
                              ; inicializado. O 1.º end. de retorno será 
                              ; armazenado em 11FEH (1200H-2)


; **********************************************************************
; * Código
; **********************************************************************
PLACE      0000H

inicio:	
	MOV  SP, SP_inicial      ; inicializa SP para a palavra a seguir à última da pilha	

    MOV  R2, TEC_LIN   ; endereço do periférico das linhas
    MOV  R3, TEC_COL   ; endereço do periférico das colunas
    MOV  R4, DISPLAYS  ; endereço do periférico dos displays

	MOV  R8,0		   ; inicia o score a 0

	MOV  R1, 0 		   ; valor a introduzir nos displays no inicio do programa
    MOVB [R4], R1      ; escreve linha e coluna a zero nos displays

    MOV  R10, PONTO     ; byte inicial para escrever no ecrã e desenhar um padrão                   

; corpo principal do programa

; **********************************************************************
; ESCREVE_ECRA - Escreve um byte no ecra
; Argumentos: R9 - Valor do bit em que se vai escrever
;             R10 - Valor do byte a escrever
; **********************************************************************
escreve_byte:
    ; Exterior da nave
     MOV  R9, 806CH            ; endereço do bit
	 MOV R10, PONTO			   ; endereço do byte
     MOVB [R9], R10            ; escreve no periférico de saída (PixelScreen)
	 MOV R9, 806DH
	 MOV R10, 0FFH
	 MOVB [R9], R10
	 MOV R9, 806EH
	 MOV R10, 0FFH
	 MOVB [R9], R10
	 MOV R9, 806FH
	 MOV R10, 0F0H
	 MOVB [R9], R10
	 MOV R9, 8070H
	 MOV R10, 010H
	 MOVB [R9], R10
	 MOV R9, 8073H
	 MOV R10, 08H
	 MOVB [R9], R10
	 MOV R9, 8074H
	 MOV R10, 020H
	 MOVB [R9], R10
	 MOV R9, 8077H
	 MOV R10, 04H
	 MOVB [R9], R10
	 MOV R9, 8078H
	 MOV R10, 040H
	 MOVB [R9], R10
	 MOV R9, 807BH
	 MOV R10, 02H
	 MOVB [R9], R10
	 MOV R9, 807CH
	 MOV R10, 080H
	 MOVB [R9], R10
	 MOV R9, 807FH
	 MOV R10, 01H
	 MOVB [R9], R10
	 
	; Volante
	 MOV R9, 8079H
	 MOV R10, 03H
	 MOVB [R9], R10
	 MOV R9, 807AH
	 MOV R10, 0C0H
	 MOVB [R9], R10

; **********************************************************************
; ESCREVE_ECRA - Escreve todos os bytes do ecra
; Argumentos: R3 - Valor do byte a escrever
; **********************************************************************

ciclo:
	; Dá reset ao volante sempre que se carregar na tecla de virar à esquerda ou à direita 
	 MOV R9, 8079H
	 MOV R10, 03H
	 MOVB [R9], R10
	 MOV R9, 807AH
	 MOV R10, 0C0H
	 MOVB [R9], R10

	; Apaga os pixeis acima e abaixo do volante quando não tem teclas premidas
	 MOV R9, 807DH
	 MOV R10, 00H
	 MOVB [R9], R10
	 MOV R9, 8076H
	 MOV R10, 00H
	 MOVB [R9], R10
	 MOV R9, 8075H
	 MOV R10, 00H
	 MOVB [R9], R10
	 MOV R9, 807EH
	 MOV R10, 00H
	 MOVB [R9], R10	 

	MOV  R1, LINHA     ; testar a linha 1 

espera_tecla:          ; neste ciclo espera-se até uma tecla ser premida
	ROL R1, 1		   ; o 1 alterna entre as 4 posições possiveis (0001, 0010, 0100, 1000)
    MOVB [R2], R1      ; escrever no periférico de saída (linhas)
    MOVB R0, [R3]      ; ler do periférico de entrada (colunas)
    CMP  R0, 0         ; há tecla premida?
    JZ   espera_tecla  ; se nenhuma tecla premida, repete
                       ; vai mostrar a linha e a coluna
	PUSH R1
					   
calculo_tecla:
	MOV R5, -1		   ; inicia a contagem das linhas em -1
	MOV R6, -1		   ; inicia a contagem das colunas em -1
	
linha:				   ; calcula o valor da linha da tecla pressionada
	SHR R1, 1 		   ; avança a posição do 1
	ADD R5, 1 		   ; aumenta o valor da linha
	CMP R1, 0		   ; já está a 0?
	JNZ linha		   ; se não está a 0, repete
	POP R1
	PUSH R0

coluna:				   ; calcula o valor da coluna da tecla pressionada
	SHR R0, 1		   ; avança a posição do 1
	ADD R6, 1		   ; aumenta o valor da coluna
	CMP R0, 0		   ; já está a 0?
	JNZ coluna	       ; se não está a 0, repete
	POP R0
	
verifica_teclas:       ; efetua as contas para obter o valor da tecla
	MOV R7, 4
	MUL R7, R5
	ADD R7, R6

	CMP R7, 1          ; se a tecla pressionada for 1, aumenta o score
	JEQ score

	CMP R7, 0		   ; se a tecla pressionada for 0, o volante vira para a esquerda
	JEQ esquerda

	CMP R7, 2          ; se a tecla pressionada for 2, o volante vira para a direita
	JEQ direita

escreve_score:         ; escreve o score nos displays
	POP R6
	PUSH R8
	PUSH R2
	PUSH R3
	PUSH R5
	MOV R2, R8         ; a variável R2 vai conter as dezenas
	MOV R3, R8         ; a variável R3 vai conter as unidades
	MOV R5, 0AH        ; armazena 10 na variável para fazer a divisão inteira e o módulo
	DIV R2, R5         ; obtem-se as dezenas do score      
	MOD R3, R5		   ; obtem-se as unidades do score

	MOV R5, R2
	SHL R5, 4
	OR R5, R3		   ; junta as dezenas e as unidades em R5
	
	MOVB [R4], R5      ; escreve no periférico de saída (displays)
	POP R5
	POP R3
	POP R2
	POP R8

ha_tecla:              ; neste ciclo espera-se até NENHUMA tecla estar premida
    MOVB [R2], R1      ; escrever no periférico de saída (linhas)
    MOVB R0, [R3]      ; ler do periférico de entrada (colunas)
    CMP  R0, 0         ; há tecla premida?
    JNZ  ha_tecla      ; se ainda houver uma tecla premida, espera até não haver
    JMP  ciclo         ; repete ciclo
	
score:                 ; aumenta o score caso seja inferior a 99
	PUSH R6
	MOV R6, SCORE_MAX  ; armazena o valor máximo para o score

	CMP R8, R6         ; o score já chegou ao valor máximo permitido?
	JEQ escreve_score  ; se o score for 99, para de aumentar

	ADD R8, 3

	JMP escreve_score

esquerda:              ; vira volante para a esquerda
	 MOV R9, 807DH
	 MOV R10, 02H
	 MOVB [R9], R10

	 MOV R9, 8079H
	 MOV R10, 01H
	 MOVB [R9], R10
	 MOV R9, 807AH
	 MOV R10, 080H
	 MOVB [R9], R10

	 MOV R9, 8076H
	 MOV R10, 040H
	 MOVB [R9], R10

	 JMP escreve_score

direita:               ; vira volante para a direita
	 MOV R9, 8075H
	 MOV R10, 02H
	 MOVB [R9], R10

	 MOV R9, 8079H
	 MOV R10, 01H
	 MOVB [R9], R10
	 MOV R9, 807AH
	 MOV R10, 080H
	 MOVB [R9], R10

	 MOV R9, 807EH
	 MOV R10, 040H
	 MOVB [R9], R10	 

	 JMP escreve_score
