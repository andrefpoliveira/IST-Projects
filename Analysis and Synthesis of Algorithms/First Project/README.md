### <b>INSTITUTO SUPERIOR TÉCNICO</b>

# Análise e Síntese de Algoritmos

# 2019/2020

## 1º Projecto

### Data Limite de Entrega: 20 de Março de 2020

## <b>Descrição do Problema</b>

O Professor João Caracol desenvolveu um algoritmo para prever as notas do projecto dos alunos da disciplina ASA tendo por base a nota que obtiveram no primeiro teste. Assim, o Professor João Caracol associou a cada aluno uma dada estimativa de nota. Acontece que o Professor João Caracol não considerou o facto de que os alunos partilham frequentemente soluções entre si. Alertado para esse problema por um colega mais experiente, o Professor João Caracol quer agora recalcular as suas estimativas associando a cada aluno uma nova estimativa que tem em conta relações de amizade entre alunos. A previsão de nota para um aluno A corresponderá agora ao máximo das previsões originais associadas a alunos aos quais A tem acesso transitivamente via relações de amizade. Ou seja, dado que os alunos partilham tanto as suas soluções como as soluções dos amigos, um aluno A terá acesso às soluções dos amigos, dos amigos dos amigos, dos amigos dos amigos dos amigos e assim sucessivamente. Note que as relações de amizade não são necessariamente simétricas.

## <b>Input</b>

O ficheiro de entrada contém a informação sobre as estimativas de nota dos alunos e sobre a relações de amizade. O input é definido da seguinte forma:

- Uma linha com o número de alunos inscritos na disciplina, <i>N</i> (<i>N</i> ≥ 2), e o número de relações de amizade entre alunos, <i>M</i> (<i>M</i> ≥ 1), separados por uma vírgula;
- Uma lista de <i>N</i> linhas, em que cada linha <i>i</i> contém um inteiro n<i>i</i> correspondente à estimativa de nota original do aluno cujo número coincide com o índice da linha <i>i</i> (<i>i</i> ∈ [ 1 ... <i>N</i> ]);
- Uma lista de <i>M</i> linhas, em que cada linha contém dois inteiros <i>u</i> e <i>v</i> (separados por um espaço em branco) indicando que o aluno <i>u</i> tem como amigo o aluno <i>v</i> (o que não significa que <i>v</i> tenha <i>u</i> como amigo).

## <b>Output</b>

O programa deverá escrever no output uma lista de <i>N</i> linhas, em que cada linha contém a estimativa de nota recalculada do aluno cujo número coincide com o índice da linha.

## <b>Exemplo</b>

### <i>Input</i>
```
9,12
11
12
13
14
15
16
17
18
19
1 2
1 3
2 4
2 5
3 4
4 1
4 8
5 6
6 7
7 5
8 9
9 8
```

### <i>Output</i>
```
19
19
19
19
17
17
17
19
19
```

## <b>Implementação</b>

A implementação do projecto deverá ser feita preferencialmente usando as linguagens de programação C ou C++. Submissões em linguagem Java também são possíveis, mas devem ter mais cuidado com alguns aspectos de implementação e overheads da VM. O tempo necessário para implementar este projecto é inferior a 15 horas.

## <b>Submissão do Projecto</b>

A submissão do projecto deverá incluir um relatório resumido e um ficheiro com o código fonte da solução. Informação sobre as linguagens de programação possíveis está disponível no website do sistema Mooshak. A linguagem de programação é identificada pela extensão do ficheiro. Por exemplo, um projecto escrito em C deverá ter a extensão .c. Após a compilação, <b>o programa resultante deverá ler do standard input e escrever para o standard output</b>. Informação sobre as opções e restrições de compilação podem ser obtidas através do botão help do sistema Mooshak. O comando de compilação não deverá produzir output, caso contrário será considerado um erro de compilação.

<b>Relatório</b>: deverá ser submetido através do sistema Fénix no formato PDF com não mais de <b>2</b> páginas, fonte de 12pt, e 3cm de margem. O relatório deverá incluir uma descrição da solução, a análise teórica e a avaliação experimental dos resultados. O relatório deverá incluir qualquer referência que tenha sido utilizada na realização do projecto. Relatórios que não sejam entregues em formato PDF terão nota 0. Atempadamente será divulgado um template do relatório.

<b>Código fonte</b>: deve ser submetido através do sistema Mooshak e o relatório (em formato PDF) deverá ser submetido através do Fénix. O código fonte será avaliado automaticamente pelo sistema Mooshak (``http://acp.tecnico.ulisboa.pt/~mooshak/``). Os alunos são encorajados a submeter, tão cedo quanto possível, soluções preliminares para o sistema Mooshak e para o Fénix. Note que apenas a última submissão será considerada para efeitos de avaliação.

Todas as submissões anteriores serão ignoradas: tal inclui o código fonte e o relatório.

## <b>Avaliação</b>

O projecto deverá ser realizado em grupos de um ou dois alunos e será avaliado em duas fases. Na primeira fase, durante a submissão, cada implementação será executada num conjunto de testes, os quais representam 90% da nota final. Na segunda fase, o relatório será avaliado. A nota do relatório contribui com 10% da nota final.

### <b>Avaliação Automática</b>

A primeira fase do projecto é avaliada automaticamente com um conjunto de testes, os quais são executados num computador com o sistema operativo <b>GNU/Linux</b>. É essencial que o código fonte compile sem erros e respeite os standards de entrada e saída indicados anteriormente. Os projectos que não respeitem os formatos especificados serão penalizados e poderão ter nota 0, caso falhem todos os testes. Um conjunto reduzido de testes utilizados pelo sistema Mooshak serão públicos. A maior parte dos testes <b>não serão divulgados antes da submissão</b>. No entanto, todos os testes serão disponibilizados após o deadline para submissão do projecto. Além de verificar a correcção do output produzido, o ambiente de avaliação restringe a memória e o tempo de execução disponíveis. A maior parte dos testes executa o comando ``diff`` da forma seguinte:

``diff output result``

O ficheiro ``result`` contém o output gerado pelo executável a partir do ficheiro ``input``. O ficheiro ``output`` contém o output esperado. Um programa passa num teste e recebe o valor correspondente, quando o comando ``diff`` não reporta quaisquer diferenças (i.e., não produz qualquer output). O sistema reporta um valor entre 0 e 180. A nota obtida na classificação automática poderá sofrer eventuais cortes caso a análise do código demonstre recurso a soluções ajustadas a inputs concretos ou outputs aleatórios/constantes.

### <b>Detecção de Cópias</b>

A avaliação dos projectos inclui um procedimento para detecção de cópias. A submissão de um projecto implica um compromisso de que o trabalho foi realizado exclusivamente pelos alunos. A violação deste compromisso ou a tentativa de submeter código que não foi desenvolvido pelo grupo implica a reprovação na unidade curricular, para todos os alunos envolvidos (includindo os alunos que disponibilizaram o código). Qualquer tentativa de fraude, directa ou indirecta, será comunicada ao Conselho Pedagógico do IST, ao coordenador de curso, e será penalizada de acordo com as regras aprovadas pela Universidade e publicadas em “Diário da República”.


