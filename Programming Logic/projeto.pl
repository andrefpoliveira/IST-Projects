/****************/
/*André Oliveira*/
/*IST193686     */
/*2019          */
/*Nota: 19,8   */
/****************/

:- consult(codigo_comum).

  /*************************************/
 /* 3.1.1. Predicado aplica_R1_triplo */
/*************************************/
escolhe_numero(X,C):-X==0,!,C=1.
escolhe_numero(X,C):-X==1,C=0.

/* Caso haja variaveis */

/* [_,0,0] ou [_,1,1] */
aplica_R1_triplo([X,Y,Z],[A,B,C]):-
            var(X),
            Y==Z,!,
            escolhe_numero(Y,A),
            B=Y,C=Z.

/* [0,_,0] ou [1,_,1] */
aplica_R1_triplo([X,Y,Z],[A,B,C]):-
            var(Y),
            X==Z,!,
            escolhe_numero(X,B),
            A=X,C=Z.

/* [0,0,_] ou [1,1,_] */
aplica_R1_triplo([X,Y,Z],[A,B,C]):-
            var(Z),
            X==Y,!,
            escolhe_numero(X,C),
            A=X,B=Y.

/* Sem variaveis mas pelo menos 1 diferente */
aplica_R1_triplo([X,Y,Z],[A,B,C]):-
            nonvar(X),nonvar(Y),nonvar(Z),!,
            findall(N,(member(N,[Y,Z]),N==X),L),
            length(L,Count),
            Count\=2,
            A=X,B=Y,C=Z.

/* Restantes casos com pelo menos 1 variavel */
aplica_R1_triplo([X,Y,Z],[X,Y,Z]):-
        findall(N,(member(N,[X,Y,Z]),var(N)),L),
        length(L,C),
        C>=1,!.

  /**************************************/
 /* 3.1.2 Predicado aplica_R1_fila_aux */
/**************************************/
aplica_R1_fila_aux([Y,Z],[Y,Z]):-!.

aplica_R1_fila_aux([X,Y,Z | R], [A | R2]):-
            aplica_R1_triplo([X,Y,Z],[A,B,C]),
            aplica_R1_fila_aux([B,C | R], R2).

  /**********************************/
 /* 3.1.3 Predicado aplica_R1_fila */
/**********************************/

aplica_R1_fila_acc(Fila,C,Nova_Fila):-
        C>0, C_1 is C-1,!,
        aplica_R1_fila_aux(Fila,Nova_Linha),
        aplica_R1_fila_acc(Nova_Linha,C_1,Nova_Fila).

aplica_R1_fila_acc(Fila,C,Fila):-
        C==0.

aplica_R1_fila(L1,L2):-
        findall(N,(member(N,L1),var(N)),L),
        length(L,C),
        C\=0,!,
        aplica_R1_fila_acc(L1,C,L2).

aplica_R1_fila(L,L1):-aplica_R1_fila_aux(L,L1).

  /**********************************/
 /* 3.1.4 Predicado aplica_R2_fila */
/**********************************/
substitui_var([],_,[]):-!.

substitui_var([X | R], Char, [Char | R2]):-
        var(X),!,
        substitui_var(R, Char, R2).

substitui_var([X | R], Char, [X | R2]):-
        nonvar(X),!,
        substitui_var(R,Char,R2).        

aplica_R2_fila(Fila, Nova_Fila):-
        length(Fila, Tamanho),

        Metade is div(Tamanho,2),

        findall(N,(member(N,Fila),N==1),Uns),
        findall(N,(member(N,Fila),var(N)),Espacos_Vazios),

        length(Uns,C),
        length(Espacos_Vazios,Vazios),

        C=<Metade,

        Faltam_Uns=Metade-C,
        Faltam_Uns=:=Vazios,!,
        substitui_var(Fila,1,Nova_Fila).

aplica_R2_fila(Fila, Nova_Fila):-
        length(Fila, Tamanho),

        Metade is div(Tamanho,2),

        findall(N,(member(N,Fila),N==0),Zeros),
        findall(N,(member(N,Fila),var(N)),Espacos_Vazios),

        length(Zeros,C),
        length(Espacos_Vazios,Vazios),

        C=<Metade,

        Faltam_Zeros=Metade-C,
        Faltam_Zeros=:=Vazios,!,
        substitui_var(Fila,0,Nova_Fila).

aplica_R2_fila(Fila,Fila):-
        length(Fila,Tamanho),
        Metade is div(Tamanho,2),
        
        findall(N,(member(N,Fila),N==1),Uns),
        findall(N,(member(N,Fila),N==0),Zeros),
        
        length(Uns,Size1),
        Size1=<Metade,
        length(Zeros,Size2),
        Size2=<Metade,!.

  /*************************************/
 /* 3.1.5 Predicado aplica_R1_R2_fila */
/*************************************/
aplica_R1_R2_fila(Fila,Nova_Fila):-
        aplica_R1_fila(Fila, L),
        aplica_R2_fila(L,Nova_Fila).

  /***************************************/
 /* 3.1.6 Predicado aplica_R1_R2_puzzle */
/***************************************/
aplica_R1_R2_puzzle_aux([],[]):-!.

aplica_R1_R2_puzzle_aux([Fila | Puz], [N_Fila | N_Puz]):-
        aplica_R1_R2_fila(Fila, N_Fila),
        aplica_R1_R2_puzzle_aux(Puz,N_Puz).

aplica_R1_R2_puzzle(Puz, N_Puz):-
        aplica_R1_R2_puzzle_aux(Puz,Puz1),
        mat_transposta(Puz1, Puz2),
        aplica_R1_R2_puzzle_aux(Puz2,Puz3),
        mat_transposta(Puz3,N_Puz).

  /******************************/
 /* 3.1.7 Predicado inicializa */
/******************************/
encontra_numero_variaveis([],C,C):-!.

encontra_numero_variaveis([Fila | Puz], Cout, Res):-
        findall(N, (member(N,Fila),var(N)), Lista),
        length(Lista, Tamanho),
        Cout_1 is Cout + Tamanho,
        encontra_numero_variaveis(Puz,Cout_1,Res).

inicializa_aux(Puz,_,Diff,Puz):-Diff==0,!.

inicializa_aux(Puz,Vars,_,N_Puz):-
        aplica_R1_R2_puzzle(Puz,P1),
        encontra_numero_variaveis(P1,0,N_variaveis),
        Diff_1 is Vars-N_variaveis,
        inicializa_aux(P1,N_variaveis,Diff_1,N_Puz).


inicializa(Puz, N_Puz):-
        encontra_numero_variaveis(Puz,0,N_variaveis),
        inicializa_aux(Puz,N_variaveis,1,N_Puz).

  /*****************************/
 /* 3.2 Predicado verifica_R3 */
/*****************************/
compara_linhas(_, []):-!.
compara_linhas(Linha, [L | Puz]):-
        Linha\==L,
        compara_linhas(Linha,Puz).

verifica_R3_aux([]):-!.

verifica_R3_aux([Linha | Puz]):-
        compara_linhas(Linha, Puz),
        verifica_R3_aux(Puz).

verifica_R3(Puz):-
        verifica_R3_aux(Puz),
        mat_transposta(Puz,P1),
        verifica_R3_aux(P1).

  /**********************************/
 /* 3.3 Predicado propaga_posicoes */
/**********************************/

obtem_linha([Linha | _],1,Linha):-!.

obtem_linha([_ | Puz], Count, Res):-
        Count>1,!,
        C is Count-1,
        obtem_linha(Puz, C, Res).



novas_coords(_,_,_,[],[],Coords,Coords):-!.

novas_coords(Sentido,Numero_linha,Count,[X|Fila],[Y|Fila_Alterada],Coords,Res):-
        Sentido=='linha',X\==Y,!,
        C is Count+1,
        novas_coords(Sentido,Numero_linha,C,Fila,Fila_Alterada,[(Numero_linha,C) | Coords],Res).

novas_coords(Sentido,Numero_linha,Count,[X|Fila],[Y|Fila_Alterada],Coords,Res):-
        Sentido=='coluna',X\==Y,!,
        C is Count+1,
        novas_coords(Sentido,Numero_linha,C,Fila,Fila_Alterada,[(C,Numero_linha) | Coords],Res).

novas_coords(Sentido,Numero_linha,Count,[X|Fila],[Y|Fila_Alterada],Coords,Res):-
        X==Y,!,
        C is Count+1,
        novas_coords(Sentido,Numero_linha,C,Fila,Fila_Alterada,Coords,Res).


propaga_posicoes([],Puz,Puz):-!.

propaga_posicoes([(L,C) | Posicoes],Puz,N_Puz):-!,
        /* Obtem a linha e transforma */
        obtem_linha(Puz, L, Linha),

        aplica_R1_R2_fila(Linha,Linha_alterada),

        /* Novas Coordenadas a acrescentar vindas da linha */
        novas_coords('linha',L,0,Linha,Linha_alterada,[],Coords1),

        /* Coloca a linha alterada no Puz */
        mat_muda_linha(Puz, L, Linha_alterada, P1),

        /* Obtem coluna e transforma */
        mat_elementos_coluna(P1, C, Coluna),
        aplica_R1_R2_fila(Coluna,Coluna_alterada),!,

        /* Novas Coordenadas a acrescentar vindas da Coluna */
        novas_coords('coluna',C,0,Coluna,Coluna_alterada,[],Coords2),

        /* Coloca a coluna alterada no Puz*/
        mat_muda_coluna(P1, C, Coluna_alterada, P2),

        append([Posicoes,Coords1,Coords2],Coords),

        propaga_posicoes(Coords,P2,N_Puz).


  /*************************/
 /* 3.4 Predicado resolve */
/*************************/

obtem_coords_prim_variavel_linha(_,_,[],_,[]):-!.

obtem_coords_prim_variavel_linha(_,_,_,Pos,Pos):-
        Pos\=[],!.

obtem_coords_prim_variavel_linha(Linha, Coluna,[X | Lista],Pos,Res):-
        Pos==[],var(X),!,
        C1 is Coluna+1,
        obtem_coords_prim_variavel_linha(Linha,C1,Lista,[(Linha,Coluna)],Res).

obtem_coords_prim_variavel_linha(Linha, Coluna,[X | Lista],Pos,Res):-
        Pos==[],nonvar(X),!,
        C1 is Coluna+1,
        obtem_coords_prim_variavel_linha(Linha,C1,Lista,[],Res).

obtem_coords_prim_variavel_puz(Line,[Linha | Puz],Pos,Res):-
        Pos==[],!,
        obtem_coords_prim_variavel_linha(Line,1,Linha,[],R1),
        L1 is Line+1,
        obtem_coords_prim_variavel_puz(L1,Puz,R1,Res).

obtem_coords_prim_variavel_puz(_,_,Pos,Pos):-!.

resolve_aux(Puz,Pos,Puz):-
    Pos=[],!.

resolve_aux(Puz,[Pos],N_Puz):-
    mat_muda_posicao(Puz,Pos,0,P1),
    propaga_posicoes([Pos],P1,P2),
    verifica_R3(P2),

    /*escreve_Puzzle(P2),writeln(' '),*/

    obtem_coords_prim_variavel_puz(1,P2,[],N_Pos),
    resolve_aux(P2,N_Pos,N_Puz).

resolve_aux(Puz,[Pos],N_Puz):-
    mat_muda_posicao(Puz,Pos,1,P1),
    propaga_posicoes([Pos],P1,P2),
    verifica_R3(P2),
    
    /*escreve_Puzzle(P2),writeln(' ')*/

    obtem_coords_prim_variavel_puz(1,P2,[],N_Pos),
    resolve_aux(P2,N_Pos,N_Puz).

resolve(Puz,Sol):-
    inicializa(Puz,P1),

    /*escreve_Puzzle(P1),*/

    obtem_coords_prim_variavel_puz(1,P1,[],Pos),
    resolve_aux(P1,Pos,Sol),
    verifica_R3(Sol),!.
