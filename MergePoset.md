Introdução
==========

A partially ordered set (or simply, a *poset*) is a pair
$\mathbf{P}=(E,\preceq)$, where $E$ is a finite set and $\preceq$ is a
binary relation on $E$ that is transitive, reflexive and antisymmetric.
Two elements $x,y$ of a poset $\mathbf P$ are said to be *comparable* if
$x \preceq y$ or $y \preceq x$ in $\mathbf{P}$; otherwise, they are said
to be *incomparable*, which is denoted by $x \sim y$. We say that $y$
*covers* $x$ in $\mathbf P$ if $x \prec y$ and if there is no element
$z$ in $\mathbf P$ satisfying $x \prec z \prec y$. A *chain* in a poset
is simply a set of pairwise comparable elements.

Let $c: E \times E \rightarrow \mathbb{R}_+$ be a cost function that
associates a non-negative real value with every pair
$x,y \in \mathbf{P}$, while satisfying the following conditions:

$$\left\{
\begin{array}{lll}
c(x,y)=0, & \mbox{if} & x \preceq y;\\
0< c(x,y)< +\infty, & \mbox{if} & x \sim y;\\
c(x,y)=+\infty, & \mbox{if} & y \prec x.
\end{array}
\label{func} \right.$$

The total cost of a linear extension
${\mathbf L}=\left(x_{1}, x_{2}, \ldots, x_{n}\right)$ will be given by

$$\label{cost} \displaystyle c({\mathbf L})=\sum_{i=1}^{n-1}c(x_{i}, x_{i+1})$$

Given a poset $\mathbf P$ and a cost function $c$ as described in
([func]), the Minimum Cost Linear Extension (MCLE) problem asks for a
linear extension $\mathbf L$ of $\mathbf P$ which minimizes
$(\ref{cost})$.\

[Fazer aqui uma breve revisao dos resultados do artigo do JOCO.]

Algoritmo de Intercalação de Subposets
======================================

[Inserir aqui revisão do Algoritmo 1 de Liu <span>*et al*</span>.]\

[-\>,\>=stealth’,shorten \>=1pt,auto,node distance=2cm, thick,main
node/.style=<span>circle,fill=gray!15,draw,font=****</span>]

​(1) <span>$x_1$</span>; (2) [below of=1] <span>$x_2$</span>; (3) [below
of=2] <span>$x_3$</span>; (4) [below of=3] <span>$x_4$</span>;

​(5) [right of=1] <span>$x_5$</span>; (6) [below of=5]
<span>$x_6$</span>; (7) [below of=6] <span>$x_7$</span>;

​(1) edge node [right] (2) (2) edge node [right] (3) (3) edge node
[right] (4) (5) edge node [right] (6) (6) edge node [right] (7);

[Pkl]

Utilizaremos a notação ${\bf P}_{k,l}$ para designar o conjunto
parcialmente ordenado que consiste de duas cadeias disjuntas, cujas
cardinalidades são $k$ e $l$. A Figura [Pkl] ilustra um exemplo de poset
pertencente a esta classe.

[Liu *et al.*, 2011] Dados um poset ${\bf P}_{k,l}$ e uma função custo
conforme descrita em ([func]), o Algoritmo 1 calcula a extensão linear
de custo mínimo de ${\bf P}_{k,l}$ em tempo $\mathcal{O}(k \cdot l)$.

A seguir, descrevemos uma heurística para o problema ELCM sobre um poset
${\mathbf P}=(E,P)$ de estrutura arbitrária. A heurística consiste de
uma estratégia de divisão-e-conquista, na qual o Algoritmo 1 de Liu *et
al.* é aplicado de forma iterada a pares de extensões lineares parciais
do poset $\mathbf P$, até que seja obtida uma extensão linear contendo
todos os elementos de $E$. Devido a sua similaridade com o algoritmo de
ordenação por intercalação (*Mergesort*), iremos nos referir ao
algoritmo como `MergePoset`.

Para simplificar a apresentação do algoritmo, iremos assumir que o poset
em questão possui um único elemento máximo. Caso o poset possua mais de
um elemento maximal, é possível inserir um elemento artificial $x_0$ que
cubra todos os elementos maximais originais e satisfaça
$c(x_0,y) = 0, \; \forall y \in E$. É fácil ver que esta modificação não
altera o valor da solução ótima do problema ELCM e que uma solução para
o problema original pode ser obtida facilmente por meio de remoção do
elemento $x_0$ da extensão linear obtida. Doravante, iremos identificar
um poset com seu elemento máximo, denotado por $max(\mathbf P)$. Dado um
elemento $x$ de um poset ${\mathbf P} = (E,\preceq)$, iremos considerar
$x^\downarrow$ como o subposet de $\mathbf P$ dado por:
$$x^\downarrow = \left(\left\{z \in E: z \preceq x\right\}, P\right).$$

No pseudocódigo do algoritmo <span>MergePoset</span>, assumimos que a
representação computacional do poset $\mathbf P$ em questão coincide com
seu grafo de cobertura. Isto é, $\mathbf P$ é representado na forma de
uma árvore enraizada em $max(\mathbf P)$, onde vértices correspondem a
elementos do poset e arcos correspondem às relações de cobertura de
$\mathbf P$. É importante notar que relações transitivas não estão
presentes nesta representação. Formalmente, esta árvore será associada a
um digrafo $G=(V,A)$, com $V=E$ e
$A=\{(x,y) : x, y \in E, x<:y \mbox{ em } {\mathbf P}\}$. Utilizaremos a
notacao $\delta^+(v)$ para designar a vizinhança de saída do vértice
$v$, isto é, o conjunto de vértices diretamente alcançáveis a partir de
$v$. Similarmente, $\delta^-(v)$ será utilizado para designar a
vizinhança de entrada do vértice $v$: o conjunto de todos os vértices
que possuem um arco diretamente para $v$.\

O pseudocódigo assume a existência de uma função
<span>Intercalar</span>, que consiste da implementação do Algoritmo 1 de
Liu *et al.* A função <span>Intercalar</span> opera sobre dois subposets
$\mathbf A$ e $\mathbf B$ de $\mathbf P$ com a seguinte característica:
ambos são cadeias e seus elementos máximos, $max({\mathbf A})$ e
$max({\mathbf B})$, possuem o mesmo antecessor em $G$. A função retorna
uma nova cadeia que contém todos os elementos de $\mathbf A$ e
$\mathbf B$. A Figura [fig.Algo1Example] ilustra a entrada e a saída de
um possível execução da função.\

  ----- -----
   (a)   (b)
  ----- -----

[fig.Algo1Example]

A Figura [FigMP] apresenta o pseudocódigo do algoritmo
<span>MergePoset</span>.

<span>rl</span>\
**<span>Entrada</span>: & Grafo de cobertura $G$ de um poset
${\mathbf P}=(E,P)$ e $x=max({\mathbf P})$.\
**<span>Saída</span>: & Extensão linear de $\mathbf P$.\
& ****

\
**1. & Simplificar $G$, de forma que
$\delta^-(v)=1, \; \forall v \in E \setminus \{x\}$\
**2. & <span>MergePosetR</span>$(G)$\
****

[FigMP]

Note que o passo $\bf 1$ corresponde à tarefa de encontrar uma
$x$-arborescência de $G$, isto é, um subgrafo de $G$ que é uma árvore
enraizada em $x$. Para tanto, é suficiente aplicar um algoritmo qualquer
de árvore geradora sobre o grafo não-direcionado subjacente a $G$.
Assim, iremos considerar que o passo $\bf 1$ pode ser executado em tempo
${\cal O}(m \log n)$, onde $m$ é o número de arcos em $G$ (isto é,
relações de cobertura em $\bf P$) e $n$ é o número de elementos do
poset. Para obter este tipo de complexidade, é suficiente aplicar uma
variante do algoritmo de Jarník-Prim.\

Após a obtenção de tal estrutura de árvore, o procedimento recursivo
<span>MergePosetR</span>, cujo pseudocódigo é detalhado na Figura
[FigMPR], é aplicado à raiz da árvore. Na Figura [FigMPR], utilizamos a
notação $A(y)$ para denotar a subárvore de $A$ cuja raiz é $y$. Note
que, a depender da árvore obtida neste processo, o algoritmo
<span>MergePosetR</span> pode retornar diferentes soluções.

<span>rl</span>\
**<span>Entrada</span>: &\
**<span>Saída</span>: & Extensão linear de $\mathbf P$.\
& ****

\
**1. & <span>**Para cada**</span> $y \in \delta^+(x)$
<span>**faça**</span>\
**2. & ****

($A(y)$)

$\triangleright$ Obtém extensão linear de $A(y)$\
**3. & <span>**Enquanto**</span> $|\delta^+(x)| \; \geq \; 2$
<span>**faça**</span>\
**4. & ****

Sejam $w,z \in \delta^+(x)$\
**5. & **

Remover arcos $(x,w)$ e $(x,z)$ de $A$

$\triangleright$ Desconecta $A(w)$ e $A(z)$de $A(x)$\
**6. & **

$C$ $\leftarrow$ <span>Intercalar</span>($A(w)$, $A(z)$)

$\triangleright$ Obtém extensão linear de $A(w) \cup A(z)$\
**7. & **

Adicionar arco $(x,max(C))$ a $A$

$\triangleright$ Reconecta elementos de $A(w)$ e $A(z)$\
**8. & <span>**Retornar**</span> $A$\
**

[FigMPR]

[PropArvDeg] O Algoritmo <span>MergePosetR</span> executa em tempo
${\cal O}(n^2)$ caso a árvore de entrada seja uma árvore binária, na
qual cada nó interno possui pelo menos um filho que é uma folha.

Seja $A_n$ uma árvore binária com esta propriedade e contendo $n$ nós.
Então, podemos escrever $A_n = (A_1, r, A_{n-2})$, onde $r$ é a raiz de
$A_n$, $A_1$ é uma árvore que consiste de apenas um nó(-folha), e
$A_{n-2}$ é uma árvore contendo $n-2$ nós e com o mesmo tipo de
estrutura que $A_n$.

Note que a chamada do algoritmo <span>MergePosetR</span> sobre uma
árvore que consiste de apenas um elemento consome tempo trivial. Desta
forma, o tempo total de execução do algoritmo quando aplicado à árvore é
dado por

$$\begin{aligned}
T(n) = 1 + T(n-2) + n-2, \label{RecArvDeg}\end{aligned}$$

onde o termo $n-2$ corresponde à execução do algoritmo
<span>Intercalar</span> para uma entrada que consiste de uma cadeia de
um único elemento e uma cadeia com $n-2$ elementos. O esforço
computacional em cada nível da recursão, portanto, resume-se à execução
do algoritmo <span>Intercalar</span>. Assim, temos
$$T(n) = 1 + \sum_{i=1}^{\frac{(n-1)}{2}}(n - 2i) = {\cal O}(n^2).{\nobreak \ifvmode \relax \else
      \ifdim\lastskip<1.5em \hskip-\lastskip
      \hskip1.5em plus0em minus0.5em \fi \nobreak
      \vrule height0.75em width0.5em depth0.25em\fi}$$

[PropArvM] O Algoritmo <span>MergePosetR</span> executa em tempo
${\cal O}(n^2)$ caso a árvore de entrada seja uma árvore $m$-ária
completa.

Seja $n$ o número de nós de $A$. Como $A$ é uma árvore $m$-ária
completa, todo nó não-folha possui $m$ descendentes. Em particular, a
raiz $x$ de $A$ possui $m$ subárvores. Se assumirmos que a $i$-ésima
subárvore de $x$ possui $n_i$ nós, podemos escrever

$$\begin{aligned}
n = 1 + n_1 + n_2 + \dots + n_m. \label{Raizn}\end{aligned}$$

Assim, o esforço computacional relativo à aplicação do algoritmo
<span>MergePosetR</span> é dado pela recorrência

$$\begin{aligned}
T(n) &=& \sum_{k=1}^{m-1}\left( n_{k+1} \sum_{j=1}^k{n_j} \right) + \sum_{i=1}^m T(n_i) \nonumber\\
T(n) &=& \sum_{i=1}^{m-1} \sum_{j=i+1}^m n_{i} \: n_{j} \; + \; \sum_{i=1}^m T(n_i) \label{Recorrencia}\end{aligned}$$

Por se tratar de uma árvore $m$-ária completa, os valores de
$n_1,\ldots,n_m$ são aproximadamente iguais. Portanto, temos

$$\begin{aligned}
T(n) &=& \left(\frac{n-1}{m}\right)^2 + \frac{2(n-1)}{m}\cdot\frac{n-1}{m} + \dots + \frac{(m-1)(n-1)}{m}\cdot\frac{n-1}{m} \;+\; m \cdot T\left(\frac{n-1}{m}\right) \nonumber\\
&=& \frac{m\:(m-1)}{2} \cdot \left(\frac{n-1}{m}\right)^2 +\;m \cdot T\left(\frac{n-1}{m}\right) \nonumber\\
&=& \frac{m-1}{2m} \cdot (n-1)^2 +\; m \cdot T\left(\frac{n-1}{m}\right) \nonumber\\
T(n) &\leq& \frac{m-1}{2m} \cdot n^2 +\; m \cdot T\left(\frac{n}{m}\right) \label{TeoM}\end{aligned}$$

Aplicando o Teorema Mestre @CLRS sobre ([TeoM]), com $a=b=m$,
$c=\frac{m-1}{2m}$ e $k=2$, obtemos $T(n) = \Theta(n^2)$.<span>\<1.5em
-1.5em plus0em minus0.5em height0.75em width0.5em depth0.25em</span>

As Observações [PropArvDeg] e [PropArvM] sugerem uma complexidade
quadrática para o algoritmo <span>MergePosetR</span>, no caso geral.
Utilizando o método da substituição @CLRS, temos o seguinte resultado.

[TeoMPR] O Algoritmo <span>MergePosetR</span> executa em tempo
${\cal O}(n^2)$, no caso geral.

A prova pelo método da substituição é feita via indução sobre o
argumento da recorrência ([Recorrencia]), dada por
$$T(n) = \sum_{i=1}^{m-1} \sum_{j=i+1}^m n_{i} \: n_{j} \; + \; \sum_{i=1}^m T(n_i).$$

Primeiramente, notemos, a partir de ([Raizn]), que

$$\begin{aligned}
(n-1)^2 \;\;=\;\; \left( \sum_{i=1}^{m} n_i \right)^2 \;\;=\;\;
\sum_{i=1}^{m} n_i^2 \; + \; 2\sum_{i=1}^{m-1} \sum_{j=i+1}^{m} n_i  n_j \label{Exp1}\end{aligned}$$

Assumindo que $T(n_i)={\cal O}(n_i^2)$, para $i=1,\ldots,m$, queremos
mostrar que $T(n) \leq c \: n^2$. Substituindo, obtemos

$$\begin{aligned}
T(n) &\leq& \sum_{i=1}^{m-1} \sum_{j=i+1}^m n_{i} \: n_{j} \; + \; \sum_{i=1}^m c_i \: n_i^2 \nonumber \\
&\leq& \sum_{i=1}^{m-1} \sum_{j=i+1}^m n_{i} \: n_{j} \; + \; c^* \sum_{i=1}^m n_i^2 \label{PassoC} \\
&\leq& c^* \left( 2\;\sum_{i=1}^{m-1} \sum_{j=i+1}^m n_{i} \: n_{j} \; + \; \sum_{i=1}^m n_i^2 \right) \nonumber \\
&\leq& c^* (n-1)^2 \label{PassoQ} \\
&\leq& c^* n^2.\nonumber \end{aligned}$$

O passo ([PassoC]) corresponde à definiçao $c^*=\max_{i=1,\ldots,m}c_i$,
enquanto o passo ([PassoQ]) é consequência de ([Exp1]).

[Argumentar aqui acerca dos casos bases.]<span>\<1.5em -1.5em plus0em
minus0.5em height0.75em width0.5em depth0.25em</span>

Consequentemente, o algoritmo <span>MergePoset</span> também executa em
tempo quadrático, haja vista que o passo $\bf 1$ do mesmo pode ser
executado em tempo ${\cal O}(m \log n)$.
