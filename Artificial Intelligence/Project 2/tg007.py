# -*- coding: utf-8 -*-
"""
Grupo tg007
André Oliveira id #93686
Maria Ribeiro id #93735
"""

import numpy as np
import math

def calculate_information(pP, pN):
    return -pP * math.log(pP, 2) - pN * math.log(pN, 2)

def get_values_count(values):
    all_v = set(values)
    d = {}
    for v in all_v:
        d[v] = values.count(v)
    return d

def remove_lines_and_collumns(D, line_values, col_values, col, value):
    new_line_values = []
    new_col_values = [x for x in col_values if x != col]

    for i in range(len(D)):
        if D[i][col] != value: new_line_values.append(i)

    return new_line_values, new_col_values

def calculate_importance(D, Y, atributos, size):
    bestScore = 2
    bestCol = -1
    for i in atributos:

        column = D[:,i]
        pos_p = pos_n = neg_p = neg_n = 0

        for j in range(len(column)):
            index = 1 if Y[j] else 0 

            if column[j]:
                if index:
                    pos_p += 1
                else:
                    pos_n += 1
            else:
                if index:
                    neg_p += 1
                else:
                    neg_n += 1

        information = 0

        if neg_n != 0 and neg_p != 0:
            l_size = neg_n + neg_p
            information += (l_size / size) * calculate_information(neg_n/l_size, neg_p/l_size)
        if pos_n != 0 and pos_p != 0:
            l_size = pos_n + pos_p
            information += (l_size / size) * calculate_information(pos_n/l_size, pos_p/l_size)

        if information < bestScore:
            bestScore = information
            bestCol = i

    return bestCol

def create_new_exs(D, Y, colV):
    new_d_z = []
    new_y_z = []
    new_d_o = []
    new_y_o = []
    for j in range(len(Y)):
        if D[j][colV]:
            new_d_o.append(D[j])
            new_y_o.append(Y[j])
        else:
            new_d_z.append(D[j])
            new_y_z.append(Y[j])
    return np.array(new_d_z), np.array(new_y_z), np.array(new_d_o), np.array(new_y_o) 

def dtl(D, Y, atributos, D_pai, Y_pai, noise, base = -1):
    # Se exemplos vazio entao devolve VALOR-MAIORIA(exp_pais)
    if Y.size == 0:
        zero = np.count_nonzero(Y_pai == 0)
        one = np.count_nonzero(Y_pai == 1)
        return 0 if zero > one else 1

    # Senao se todos os exemplos tem a mesma classificação entao devolve classificacao
    zero_class = np.count_nonzero(Y == 0)
    one_class = np.count_nonzero(Y == 1)
    if zero_class == Y.size or one_class == Y.size:
        if len(atributos) != D[0].size:
            return 0 if zero_class > 0 else 1
        else:
            return [0,0,0] if zero_class > 0 else [0,1,1]

    # Senao se atributos esta vazio entao devolve VALOR-MAIORIA(exemplos)
    if len(atributos) == 0:
        zero = np.count_nonzero(Y == 0)
        one = np.count_nonzero(Y == 1)
        return 0 if zero > one else 1

    # A <- argmax (_a_ pertence a atributos) IMPORTANCIA (_a_, exemplos)
    colV = calculate_importance(D, Y, atributos, Y.size)
    
    if base != -1:
        colV = base

    # arvore <- nova arvore com _a_ raiz teste A
    tree = [colV, -1, -1]

    # exs <- {e: e pertence a exemplos AND e.A = vi}
    new_d_z, new_y_z, new_d_o, new_y_o = create_new_exs(D, Y, colV)
    # para cada valor vi de A
    for i in [0, 1]:
        # sub-arvore <- DTL(exs, atributos - A, exemplos)
        if i:
            sub_arvore = dtl(new_d_o, new_y_o, [x for x in atributos if x != colV], D, Y, noise)
        else:
            sub_arvore = dtl(new_d_z, new_y_z, [x for x in atributos if x != colV], D, Y, noise)

        # adiciona um ramo a arvore com etiqueta (A = vi) e sub-arvore
        tree[i+1] = sub_arvore

    # Devolve arvore
    return tree

def find_shorter(tree, current_size, tree_lst, D, Y, atributos, roots_tried):
    new_tree = tree
    if tree_lst[0] not in roots_tried:
        new_tree = dtl(D, Y, atributos, D, Y, False, tree_lst[0])

    roots_tried.append(tree_lst[0])

    if len(str(new_tree)) < current_size:
        tree = new_tree
    else:
        if type(tree_lst[1]) == list:
            tree = find_shorter(tree, current_size, tree_lst[1], D, Y, atributos, roots_tried)
        if type(tree_lst[2]) == list:
            tree = find_shorter(tree, current_size, tree_lst[2], D, Y, atributos, roots_tried)
    return tree

def createdecisiontree(D,Y, noise = False):
    tree = dtl(D, Y, [x for x in range(len(D[0]))], D, Y, noise)

    if not noise:
        current_size = len(str(tree))
        tree = find_shorter(tree, current_size, tree, D, Y, [x for x in range(len(D[0]))], [])

    return tree