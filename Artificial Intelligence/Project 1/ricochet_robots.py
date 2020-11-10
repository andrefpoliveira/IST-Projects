# ricochet_robots.py: Template para implementação do 1º projeto de Inteligência Artificial 2020/2021.
# Devem alterar as classes e funções neste ficheiro de acordo com as instruções do enunciado.
# Além das funções e classes já definidas, podem acrescentar outras que considerem pertinentes.

# Grupo 07:
# 93686 André Oliveira
# 93735 Maria Ribeiro

from search import Problem, Node, iterative_deepening_search
import sys


class RRState:
    state_id = 0

    def __init__(self, board):
        self.board = board
        self.id = RRState.state_id
        RRState.state_id += 1

    def __lt__(self, other):
        """ Este método é utilizado em caso de empate na gestão da lista
        de abertos nas procuras informadas. """
        return self.id < other.id


class Board:
    """ Representacao interna de um tabuleiro de Ricochet Robots. """
    def __init__(self, size, robots, objective, walls):
        self.size = size
        self.robots = robots
        self.objective = objective
        self.walls = walls

    def robot_position(self, robot: str):
        """ Devolve a posição atual do robô passado como argumento. """
        return tuple(map(lambda x: x + 1, self.robots[robot]))
    
    def is_goal_reached(self):
        coords = (self.objective[1], self.objective[2])

        return self.robots[self.objective[0]] == coords

    def is_free_space(self, row, col):
        if not (row >= 0 and col >= 0 and row < self.size and col < self.size):
            return False

        return (row, col) not in self.robots.values()

    def move_robot(self, action):
        robot, move = action
        row, col = self.robots[robot]

        self.robots[robot] = (-1,-1)

        d = {'u': [-1, 0],
             'd': [1, 0],
             'r': [0, 1],
             'l': [0, -1]}

        row += d[move][0]
        col += d[move][1]

        wall_dir = {'u': 'd',
                    'd': 'u',
                    'l': 'r',
                    'r': 'l'}

        while (row, col) not in self.walls[wall_dir[move]] and self.is_free_space(row, col):
            row += d[move][0]
            col += d[move][1]

        self.robots[robot] = (row - d[move][0], col - d[move][1])

def parse_instance(filename: str) -> Board:
    """ Lê o ficheiro cujo caminho é passado como argumento e retorna
    uma instância da classe Board. """
    robots = {}
    objective = []
    walls = {'r': [], 'l': [], 'u': [], 'd': []}

    f = open(filename, "r")

    board_side = int(f.readline())

    # Robôs
    for _ in range(4):
        robot, row, col = f.readline().split()
        row, col = int(row) - 1, int(col) - 1 
        robots[robot] = (row, col)

    # Objetivo
    robot, row, col = f.readline().split()
    row, col = int(row) -1, int(col) - 1
    objective = [robot, row, col]

    # Barreiras
    barriers = int(f.readline())
    for _ in range(barriers):
        row, col, side = f.readline().split()
        row, col = int(row)-1, int(col)-1

        walls[side].append((row, col))

        d = {'u': [-1, 0],
             'd': [1, 0],
             'r': [0, 1],
             'l': [0, -1]}
        
        row += d[side][0]
        col += d[side][1]

        if row >= 0 and row < board_side and col >= 0 and col < board_side:
            if side == 'r':
                walls['l'].append((row,col))
            elif side == 'l':
                walls['r'].append((row,col))
            elif side == 'u':
                walls['d'].append((row,col))
            else:
                walls['u'].append((row,col))

    f.close()    

    return Board(board_side, robots, objective, walls)


class RicochetRobots(Problem):
    def __init__(self, board: Board):
        """ O construtor especifica o estado inicial. """
        self.initial = RRState(board)

    def actions(self, state: RRState):
        """ Retorna uma lista de ações que podem ser executadas a
        partir do estado passado como argumento. """
        actions_list = []

        for robot in ['R', 'G', 'Y', 'B']:
            x, y = state.board.robots[robot]

            if state.board.is_free_space(x+1, y) and (x, y) not in state.board.walls['d']:
                actions_list.append((robot, 'd'))

            if state.board.is_free_space(x-1, y) and (x, y) not in state.board.walls['u']:
                actions_list.append((robot, 'u'))

            if state.board.is_free_space(x, y+1) and (x, y) not in state.board.walls['r']:
                actions_list.append((robot, 'r'))

            if state.board.is_free_space(x, y-1) and (x, y) not in state.board.walls['l']:
                actions_list.append((robot, 'l'))

        return actions_list

    def result(self, state: RRState, action):
        """ Retorna o estado resultante de executar a 'action' sobre
        'state' passado como argumento. A ação retornada deve ser uma
        das presentes na lista obtida pela execução de
        self.actions(state). """
        board = Board(state.board.size, state.board.robots.copy(), state.board.objective, state.board.walls)
        new_state = RRState(board)
        new_state.board.move_robot(action)
        return new_state

    def goal_test(self, state: RRState):
        """ Retorna True se e só se o estado passado como argumento é
        um estado objetivo. Deve verificar se o alvo e o robô da
        mesma cor ocupam a mesma célula no tabuleiro. """
        return state.board.is_goal_reached()

    def h(self, node: Node):
        """ Função heuristica utilizada para a procura A*. """
        # TODO
        robot, oX, oY = node.state.board.objective
        x, y = node.state.board.robots[robot]

        if oX == x or oY == y: return 1
        return 2


if __name__ == "__main__":
    # TODO:
    # Ler o ficheiro de input de sys.argv[1],
    # Usar uma técnica de procura para resolver a instância,
    # Retirar a solução a partir do nó resultante,
    # Imprimir para o standard output no formato indicado.
    board = parse_instance(sys.argv[1])
    #board = parse_instance("testing.txt")

    # Criar uma instância de RicochetRobots:
    problem = RicochetRobots(board)

    # Obter o nó solução usando a procura Iterative Deepening Search:
    solution_node = iterative_deepening_search(problem)

    print(len(solution_node.solution()))
    for i in solution_node.solution():
        print(i[0]+ " " + i[1])
