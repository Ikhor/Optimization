We present a computational evaluation of existing and novel approaches
for the so-called Minimum Cost Linear Extension problem (MCLE). Given a
partially ordered set $\mathbf P$ and a cost function that associates a
non-negative real value with every (ordered) pair of incomparable
elements of $\mathbf P$, the MCLE asks for some linear extension
$\mathbf L$ of $\mathbf P$ that minimizes the total cost of adjacent
elements in $\mathbf L$. We discuss an algorithm by Liu <span>*et
al.*</span> for the general case of the MCLE, showing that it is not an
approximation algorithm, as originally suggested. We also propose a fast
and effective topological sorting-based heuristic, as well as an exact
model based on constraint programming. Computational results suggest
that the topological sorting heuristic is superior to the algorithm of
Liu <span>*et al.*</span> for sparse instances, while the exact model
shows that Liu <span>*et al.*</span>’s algorithm produces near-optimal
solutions for small-sized instances.

Introduction {#Intro}
============

A partially ordered set (or simply, a *poset*) is a pair
$\mathbf{P}=(E,\leq)$, where $E$ is a finite set and $\leq$ is a binary
relation on $E$ that is transitive, reflexive and antisymmetric, *i.e.*,
$\leq$ is a binary relation satisfying:

1.  $x \leq y, \;\; y \leq z \; \Rightarrow \; x \leq z, \;\; \forall \: x,y,z \in E$;

2.  $x \leq x, \;\; \forall \: x \in E$;

3.  $x \leq y, \;\; y \leq x \; \Rightarrow \; x=y, \;\; \forall \: x,y \in E$.

In a partial order $\mathbf{P}=(E,\leq)$ there might exist pairs of
elements $x,y \in E$, with $x \neq y$, such that we have neither
$x \leq y$ nor $y \leq x$ in $\mathbf P$. Two elements $x,y$ of a poset
$\mathbf P$ are said to be *comparable* if $x \leq y$ or $y \leq x$ in
$\mathbf{P}$; otherwise, they are said to be *incomparable*, which is
denoted by $x \sim y$. We say that $y$ *covers* $x$ in $\mathbf P$ if
$x < y$ and if there is no element $z$ in $\mathbf P$ satisfying
$x < z < y$. We denote the fact that $y$ covers $x$ by
$x <\hspace{-5pt}\cdot \; y$. For any element $x$ in ${\mathbf P}$, we
define $x^\downarrow$ as the subset of $E$ given by
$$x^\downarrow = \left\{z \in E: z \leq x\right\}.$$

The *Hasse diagram* of a finite poset $\mathbf{P}$ is a directed graph
whose vertices are the elements of $\mathbf{P}$ and the arcs are the
coverage relations in $\mathbf{P}$. A *chain* in $\mathbf{P}$ is a
subset $C \subseteq E$ such that any two elements of $C$ are comparable
in $\mathbf P$. A linear order is an ordered set
$\mathbf{L}=(E, \leq_{\mathbf L})$ satisfying $x \leq_{\mathbf L} y$ or
$y \leq_{\mathbf L}x$ for every pair of elements $x, y \in E$. A linear
order $\mathbf{L}$ is a linear extension of a poset $\textbf{P}$ if
$x \leq_{\mathbf L} y\;$ whenever $x \leq y$ em $\mathbf P$.

Is is a well-known fact that every poset admits at least one linear
extension. Consider a linear extension $\mathbf{L}$ of poset
$\mathbf{P}$. We will write
$${\mathbf L}=\{ x_{1} <_{\mathbf L} x_{2} <_{\mathbf L} \ldots <_{\mathbf L} x_{n}\},$$

where $x_i \in E \;(i=1,\ldots,n)$ and $n=|E|$. Since it is possible for
a poset to admit more than one linear extension, it is natural to
consider an optimization problem that asks for the best linear extension
of a poset $\mathbf P$, according to some criterion.

The Minimum Cost Linear Extension Problem
-----------------------------------------

Let $c: E \times E \rightarrow \mathbb{R}_+$ be a cost function that
associates a non-negative real value with every pair
$x,y \in \mathbf{P}$, while satisfying the following conditions:

$$\left\{
\begin{array}{lll}
c(x,y)=0, & \mbox{if} & x \leq y;\\
0< c(x,y)< +\infty, & \mbox{if} & x\sim y;\\
c(x,y)=+\infty, & \mbox{if} & x>y.
\end{array}
\label{func} \right.$$

By slightly abusing notation, we will write the total cost of a linear
extension
${\mathbf L}=\{ x_{1} <_{\mathbf L} x_{2} <_{\mathbf L} \ldots <_{\mathbf L} x_{n}\}$
as

$$\label{custo} \displaystyle c({\mathbf L})=\sum_{i=i}^{n-1}c(x_{i}, x_{i+1})$$

Given a poset $\mathbf P$ and a cost function $c$ as described above,
the Minimum Cost Linear Extension problem (thereby referred to as MCLE)
asks for a linear extension $L$ of $\mathbf P$ which minimizes
$(\ref{custo})$. In other words, we are interested in finding
$\mathbf{L}^*$ satisfying
$$\mathbf{L}^* = \operatorname*{arg\,min}\{c(\mathbf{L}) : \mathbf{L} \mbox{ is a linear extension of } {\mathbf P} \}.$$

The MCLE was originally defined in @Liu and further studied in @Wu. In
@Liu the authors showed that the MCLE generalizes the so-called bump
number and jump number problems on posets. Specifically, if we define
$c(x,y)=1$, for $x \sim y$, the problem becomes equivalent to the jump
number problem. Since the jump number problem is known to be NP-hard
(see @Bouchitte), the MCLE is also NP-hard. In this work, however, we
concentrate on the general version of the problem, in which $\mathbf P$
is an arbitrary poset and $c$ is an arbitrary non-negative real-valued
cost function satisfying conditions ([func]).

While discussing the algorithms described in this work, we shall assume
that the computational representation of a poset $\mathbf P=(E,\leq)$ is
that of a directed graph, in which vertices correspond to elements of
$E$ and arcs are associated with a superset of the coverage relations in
$\mathbf P$. Formally, this representation is a directed graph
$G=(V,A)$, with $V=E$ and $A=\{(x,y) : x, y \in E, x < y\}$. We do not
insist that only arcs corresponding to coverage relations are included
in this representation, since this does not have an impact on the
worst-case time complexity of the algorithms discussed here. We shall
make use of the notation $\delta^+(v)$ to denote the outward
neighborhood of vertex $v$, <span>*i.e.*</span>, the set of vertices
directly reachable from $v$. Similarly, $\delta^-(v)$ will be utilized
to denote the inward neighborhood of vertex $v$: the set of all vertices
$w$ for which there exists an arc $(w,v) \in A$. We shall refer to
$|\delta^+(v)|$ as the *out-degree* of vertex $v$, while $|\delta^-(v)|$
shall be referred to as the *in-degree* of $v$.

Algorithm 2 of Liu *et al*. {#Algo2}
===========================

In what folows we describe the greedy algorithm proposed by Liu
<span>*et al.*</span> in @Liu, referred to as “Algorithm 2” in their
paper. Here we provide a somewhat different description than the one
given in the original paper. The reason for that is that we believe our
description focus more closely on the conceptual steps, rather than on
implementation choices.

For the sake of simplifying the presentation of algorithm
<span>Liu2</span>, we shall define the operation of
<span>*injecting*</span> an element into a <span>*partial linear
extension*</span> of a poset $\mathbf{P}$. We define a partial linear
extension of a poset $\mathbf{P}=(E,\leq)$ as a linear order
$\mathbf{L}=(X,\leq_{\mathbf L})$ involving a subset $X$ of $E$ and
satisfying $x \leq_{\mathbf L} y$, whenever $x \leq y$ in $\mathbf P$.
Let $\mathbf{L}$ be a partial linear order of $\mathbf{P}$ and
$x \in E \setminus \mathbf{L}$. The operation of injecting $x$ into
$\mathbf{L}$ corresponds to augmenting the linear order $\mathbf{L}$ by
adding $x$ to it, while satisfying the following conditions:

1.  The resulting linear order $\mathbf{L'}$ is consistent with
    ${\mathbf P}$ (<span>*i.e.*</span>, $x \leq_{\mathbf{L'}} y$
    whenever $x \leq y$ in $\mathbf{P}$);

2.  $c(\mathbf{L'})$ is minimum.

In other words, we want to add $x$ into $\mathbf{L}$ while preserving
the relative order of the elements in $\mathbf{L}$ and minimizing the
resulting total cost of $\mathbf{L'}$. Let
$(y_1, \ldots, y_{|{\mathbf L}|})$ be the elements of $\mathbf{L}$,
where $y_i <_{\mathbf{L}} y_{i+1}, \;i=1,\ldots,|\mathbf{L}|-1$. Then,
the choices for positioning $x$ when injecting it into $\mathbf{L}$ are
the following:

$$\begin{aligned}
&& \{x, y_1, \ldots, y_{|\mathbf{L}|}\}, \\
&& \{y_1, \ldots, y_{|\mathbf{L}|}, x\}, \mbox{ or} \\
&& \{y_1, \ldots, y_i, x, y_{i+1}, \ldots, y_{|\mathbf{L}|}\}, \; i=1,\ldots,|{\mathbf L|}-1.\end{aligned}$$

Obviously, some of these positions might be invalid, as they might not
be consistent with the order relations in $\mathbf{P}$. We present in
Figure [Liu2Pseudocode] the pseudocode for algorithm <span>Liu2</span>.

<span>rl</span>\
**<span>Input</span>: & Cover graph $G$ of a poset
${\mathbf P}=(E,\leq)$ of height $T$.\
**<span>Output</span>: & Linear extension $\mathbf L$ of $\mathbf P$.\
& ****

\
**1. & Let $C$ be a maximum-size chain of $\textbf{P}$\
**2. & Let ${\mathbf L} := C$, $R := E \setminus C$\
**3. & <span>**while**</span> $R \neq \emptyset$ <span>**do**</span>\
**4. & ********

Select $r \in R$, $R := R \setminus \{r\}$\
**5. & **

Inject $r$ into $\mathbf{L}$\
**6. & <span>**end**</span>\
**

[Liu2Pseudocode]

It is interesting to note that the <span>Liu2</span> algorithm was
presented in @Liu as a “greedy approximation algorithm.” We believe that
the authors meant by it that the algorithm is a greedy procedure that
produces solutions whose objective function values are typically (but
not provably) close to the optimal value, what is a departure from the
standard concept of an approximation algorithm. Indeed, as we argue
below, it is possible to devise examples for which the solution produced
by the algorithm is arbitrarily large (in terms of objective function
value), as compared to the actual minimum.

The <span>Liu2</span> algorithm does not have a fixed approximation
rate.

Consider the poset $\mathbf{P}$ consisting of a 4-element ground set
$E=\{a,b,c,d\}$ and the single relation $b<a$. Let the costs between the
incomparable elements in $\mathbf P$ be as follows:

$$\begin{aligned}
c(a,c)=M,\; c(a,d)=M,\; \\
c(b,c)=1,\; c(b,d)=1,\; \\
c(c,a)=2,\; c(c,b)=M,\; c(c,d)=M,\; \\
c(d,a)=M,\; c(d,b)=M,\; c(d,c)=M,\end{aligned}$$

where $M$ is a positive constant strictly greater than $1$.

<span>p<span>4.7cm</span>p<span>4.7cm</span>p<span>4cm</span></span> &
&\
(a) Solution produced by algorithm <span>Liu2</span>; cost = $1+M$. &
(b) Solution produced by algorithm <span>Liu2</span>; cost = $1+M$. &
(c) Optimal solution; cost = $3$.

[figLiu2]

The application of algorithm <span>Liu2</span> to $\mathbf{P}$ yields a
linear extension with total cost $1+M$. Figure [figLiu2](a) and
[figLiu2](b) show two such linear extensions. Yet another alternative
solution with the same cost would be
$a \rightarrow b \rightarrow d \rightarrow c$. Which solution is to be
reported by the algorithm depends only on implementation choices
concerning how to break ties. The optimal solution, however, has an
objective function value of 3, as shown in Figure [figLiu2](c).
Therefore, by appropriately changing the value of $M$, one can make the
approximation ratio between the objective function value of the solution
produced by algorithm <span>Liu2</span> and the optimal value
arbitrarily large. <span>\<1.5em -1.5em plus0em minus0.5em height0.75em
width0.5em depth0.25em</span>

Greedy Topological Sorting
==========================

A topological sort of a directed acyclic graph $G=(V,A)$ is a linear
ordering of the elements of $V$ such that $A$ contains no arc $(u,v)$
with $u$ appearing before $v$ in the ordering (*i.e.*, the ordering is
consistent with the arcs of $G$). Let us define $n=|V|$ and $m=|A|$. A
topological sort can also be seen as a mapping
$\sigma: V \rightarrow \{1,\dots,n\}$ such that
$(u,v) \in A \Rightarrow \sigma(u)<\sigma(v)$. Since the cover graph of
a poset $\mathbf{P}$ is a directed acyclic graph, there is a one-to-one
correspondence between the set of topological sorts of the cover graph
of $\mathbf{P}$ and the set of linear extensions of $\mathbf{P}$.

It is, therefore, natural to consider an algorithm for the MCLE which is
based on a topological sorting algorithm. One of the standard algorithms
for topologically sorting the vertices of a directed acyclic graph is
essentially a graph traversal procedure and can be implemented to run in
$\Theta(n+m)$ time. The algorithm can de described as the process of
augmenting a partial linear extension (*i.e.*, a topological sort
involving a proper subset of the elements of $\mathbf{P}$) by including
into it a vertex with out-degree equal to zero (ties can be broken
arbitrarily). The algorithm starts with the empty partial linear
extension (PLE). Every time a vertex $v$ is added to the PLE the
vertices in $\delta^-(v)$ have their out-degree values decreased by 1.
Note that, by construction, the vertices in $\delta^-(v)$ have not yet
been added to the PLE. Moreover, the operation of updating their
out-degrees amounts to removing the set of arcs
$\{(u,v) : u \in \delta^-(v)\}$ from $A$.

Since the MCLE accounts for possibly different costs associated with
pairs of incomparable adjacent elements in the ordering, the standard
algorithm for topological sorting must be modified in order to reflect
that cost structure. A straightforward variant of the algorithm can be
obtained by applying a greedy criterion to break ties whenever there are
two or more elements with zero out-degree. At the first iteration, the
algorithm arbitrarily selects a vertex with zero out-degree. From the
second iteration and on, let $L$ be the current PLE and $w$ be the
vertex which was most recently added to $L$. The algorithm selects a
vertex $v^*$ with the minimum cost $c(w,v^*)$ to be added to $L$:
$$\displaystyle v^* = \operatorname*{arg\,min}_{\substack{v \; \notin \; L \\ \; |\delta^+(v)|=0}} c(v,w).$$

We are now ready to present the pseudocode for the greedy topological
sorting algorithm (see Figure [GTSAlgo]).

<span>rl</span>\
**<span>Input</span>: & Cover graph $G$ of a poset ${\mathbf P}$, cost
function $c:V \times V \rightarrow \mathbb{R}$.\
**<span>Output</span>: & Linear extension $L$ of $\mathbf P$.\
& ****

\
**1. & $L := \emptyset, \;\;S := \{v \in V : |\delta^+(v)|=0\}$, iter
$:=1$\
**2. & <span>**while**</span> $(|S|>0)$ <span>**do**</span>\
**3. & ******

$(\mbox{iter} = 1)$ <span>**then**</span>\
**4. & **

Arbitrarily select $v^* \in S$

$\triangleright$ First vertex in the ordering\
**5. & **

\
**6. & **

$v^* := \operatorname*{arg\,min}_{v \; \in \; S} c(w,v)$

$\triangleright$ Vertex of minimum cost w.r.t last element\
**7. & **

$L := \left[L, v^*\right]$\
**8. & **

Update out-degrees of vertices in $\delta^-(v^*)$, Update $S$\
**9. & **

$w:= v^*$, iter := iter + 1\
**10. & <span>**end**</span>\
**

[GTSAlgo]

The topological sorting heuristic <span>GreedyTopSort</span> runs in
${\cal O}(n^2)$ time.

Note that the set $S$ must be updated at the end of each iteration,
since the updating of the out-degrees of vertices in $\delta^-(v^*)$
might decrease the out-degree of other vertices to zero. Furthermore,
the main loop is guaranteed to run exactly $n$ times. Step 8 requires a
total of ${\cal O}(m)$ time over all iterations, since it inspects each
arc exactly once. Step 6 requires ${\cal O}(n)$ time per iteration,
assuming that the value of $c(u,v)$ can be computed, or looked-up, in
constant time. All other internal steps in the loop demand constant
time. Therefore, the algorithm demands a total time of
${\cal O}(n^2+m)={\cal O}(n^2)$. <span>\<1.5em -1.5em plus0em minus0.5em
height0.75em width0.5em depth0.25em</span>

More sophisticated greedy schemes can definitely be devised. In
particular, criteria that incorporate look-ahead features might prove to
be a better compromise between running time and solution quality. We
chose to test this simpler version due to its relatively modest time
complexity and as a way of initially assessing the quality of a
topological sorting-based heuristic, as compared to the greedy algorithm
of @Liu.

An Exact Model Based on Constraint Programming
==============================================

The basic components of a constraint programming model are a set of
decision variables, their specific domains, and a set of constraints on
those variables. A search engine is responsible for systematically
assigning values to decision variables and performing a process of
<span>*propagation*</span>, by means of which the data from the model’s
constraints is used to further restrict the domains of unassigned
variables. These steps are repeated up to the point when the domain of
every variable has been restricted to a single value. At that moment, a
feasible solution has been produced. Below we decribe a constraint
programming model for the MCLE problem, along with the strategy applied
by the constraint solver we utilized, in order to obtain an optimal
solution – rather than enumerating all possible feasible assignments.

We modeled the MCLE problem by defining decision variables associated to
the vertices of the directed graph $G=(V,A)$ described in Section
[Intro]. Each such decision variable corresponds to the position of its
associated vertex in the linear extension. We shall refer to each such
variable as <span>pos[$i$]</span>, $i=1,\ldots,n$. The domain of
<span>pos[$i$]</span> is, therefore, $\{1,\ldots,n\}$. Thus, a natural
constraint on those variable is
$$\mbox{\tt distinct( pos[1], pos[2], ..., pos[n] )},$$ which ensures
that each vertex occupies a distinct position in the resulting linear
extension.

The arcs in graph $G$ induce constraints in the model, as well. For each
arc $(x,y) \in A$, a constraint of the “relational” type is added to
ensure that the position of vertex $x$ will be less than that of vertex
$y$: <span>pos[</span>$x$<span>] \< pos[</span>$y$<span>]</span>.

In order to model the objective function we need to account for the cost
associated with each pair of incomparable elements which happen to be
adjacent in the linear extension, <span>*i.e.*</span>, the cost
$c(x,y), \; x \sim y \mbox{ in } \mathbf P$, with
<span>pos[</span>$x$<span>] = pos[</span>$y$<span>]-1</span>. This is
accomplished with the use of four auxiliary decision variables for each
pair of incomparable elements $x \sim y$: <span>D</span>$_{x,y}$,
<span>e</span>$_{x,y}$, <span>D</span>$_{y,x}$ and
<span>e</span>$_{y,x}$. The following constraints make sure that
<span>e$_{x,y}$</span> is equal to $1$, if element $x$ is immediately
followed by element $y$ in the linear extension, and
<span>e$_{x,y}$</span> is equal to $0$, otherwise:

<span>D$_{x,y}$ = pos[</span>$y$<span>] - pos[</span>$x$<span>]</span>,\

, e$_{x,y} \in \{0,1\}$,

where <span>count($A$, value)</span> simply counts the number of times
when the decision variables in set $A$ are equal to <span>value</span>.
Therefore, the expression <span>count($\{$D$_{x,y}\}$, 1)</span> is
equal to 1 or 0, depending on whether <span>D</span>$_{x,y}$ is equal 1
or not, respectively. Similarly, we must add the corresponding
constraints for <span>D</span>$_{y,x}$ and <span>e</span>$_{y,x}$.

Now, defining the objective function is a matter of adding up the costs
for those incomparable pairs $x,y, \in \mathbf P$ for which
<span>e</span>$_{x,y}=1$ or <span>e</span>$_{y,x}=1$:
$${\tt ObjFcn := } \; \sum_{\substack{x,y \:\in\: E: \\ x \sim y }} \left[c(x,y)  \:  {\tt e}_{x,y} + c(y,x)  \:  {\tt e}_{y,x} \right]\;.$$

We implemented the model in C++ with the use of Gecode @Gecode, a
state-of-the-art constraint solver, which, in addition to the standard
constraint satisfaction search engine, is equipped with a
branch-and-bound module, allowing for an automatic search for optimal
solutions satisfying the underlying model.

An optimal solution to the problem is obtained with the definition of
the objective function <span>ObjFcn</span> and a special method, called
<span>constrain</span>. Whenever an incumbent solution
$\widehat{x}=(\widehat{\tt pos},\widehat{\tt D},\widehat{\tt e})$ is
found, the <span>constrain</span> method is invoked and a new constraint
is added to the model requiring that <span>ObjFcn</span>
$< v(\widehat{x})$, where $v(\widehat x)$ is the objective function
value of solution $\hat{x}$. Thus, as the search resumes, the
branch-and-bound search engine will restrict the decision variables
domains accordingly, ensuring that any subsequently found solution will
have an objective function value strictly less than that of the current
incumbent solution. At termination, the last incumbent solution
$x^*=({\tt pos}^*,{\tt D}^*,{\tt e}^*)$ will naturally be an optimal
solution to the problem, with the vector <span>pos</span>$^*$ specifying
the absolute position of each element in the resulting linear extension.

Computational Results
=====================

We evaluated the two heuristic algorithms <span>GreedyTopSort</span> and
<span>Liu2</span> on a number of randomly generated instances of the
MCLE problem. For a small subset of the instances, we were also able to
run the exact constraint-based model (instances with more than 20
vertices demanded an excessive running time and were, therefore,
excluded from the experiments). The results of the exact model are
particularly important, as they provide information on how close to
being optimal the heuristic solutions are (with respect to objective
function value).

Instances were created with a special-purpose generator implemented in
C++. The generator makes use of the standard procedure for generating
random graphs: given a positive integer $n$ (specifying the number of
vertices of the graph) and a real value $p \in (0,1]$, we create the arc
from vertex $x$ to vertex $y$ if a randomly generated value in $(0,1]$
is greater than $p$ (<span>*e.g.*</span>, see @Bianco). Since we are
interested in generating acyclic graphs, we only consider the creation
of arc $(x,y)$ if $x<y$. Each graph created in this manner is considered
as the representation of a poset, possibly including some arcs that
correspond to transitive (and, therefore, redundant) relations. Finally,
for each pair $x,y \in \mathbf P$ of incomparable elements, we randomly
select two integers in the range $\left[0,n^2\right]$ as the costs
$c(x,y)$ and $c(y,x)$.

Table [TableSmall] presents the solution values and running times of the
two heuristics on a set of “small” instances, consisting of posets with
a number $n$ of elements ranging from 10 to 80. The parameter $p$ takes
on four different values: 0.2, 0.4, 0.6 and 0.8. For each combination of
$n$ and $p$, we generated three different posets, by varying the random
seed used to initialize the random generator. Table [TableSmall] also
includes results for the exact model on instances with up to 20
vertices.

The heuristics are referred to in the table as <span>GTS</span> (short
for <span>GreedyTopSort</span>), <span>Liu2</span> and <span>CP</span>
(short for “constraint programming model”). Columns 3-5 correspond to
the average running times of each algorithm on the three instances with
the corresponding values of $n$ and $p$. Columns 6-7 inform the average
value of the solutions produced by <span>GTS</span> and
<span>Liu2</span>, as percentages of the optimum value found by
<span>CP</span> (as mentioned before, that information is only available
for $n \leq 20$, as instances with larger values of $n$ demanded a
prohibitive amount of time to solve to optimality). Column 8 provides a
direct comparison between the objective function values of the solutions
produced by <span>GTS</span> as percentages of the values of the
solutions produced by <span>Liu2</span>. As before, each value in that
column is the average over three instances with the corresponding values
of $n$ and $p$.

[hptb]

<span>cc|rrr|rr|r</span> & & &\
$n$ & $p$ & GTS & Liu2 & CPM & GTS & Liu2 &
<span>GTS</span>/<span>Liu2</span> (%)\
10 & 0.2 & \<0.01 & \<0.01 & \<0.01 & 191.7 & 100.0 & 191.7\
20 & 0.2 & \<0.01 & 0.03 & 0.02 & 138.4 & 100.0 & 138.4\
30 & 0.2 & 0.01 & 0.09 & — & — & — & 184.0\
40 & 0.2 & 0.01 & 0.21 & — & — & — & 157.7\
50 & 0.2 & 0.02 & 0.40 & — & — & — & 43.5\
60 & 0.2 & 0.03 & 0.69 & — & — & — & 137.8\
70 & 0.2 & 0.04 & 1.13 & — & — & — & 155.4\
80 & 0.2 & 0.05 & 1.69 & — & — & — & 158.2\
 & **158.4\
\
\
 & & &\
$n$ & $p$ & GTS & Liu2 & CPM & GTS & Liu2 &
<span>GTS</span>/<span>Liu2</span> (%)\
10 & 0.4 & \<0.01 & \<0.01 & 0.02 & 179.9 & 154.6 & 122.4\
20 & 0.4 & \<0.01 & 0.02 & 0.44 & 165.4 & 136.1 & 115.1\
30 & 0.4 & 0.01 & 0.07 & — & — & — & 169.3\
40 & 0.4 & 0.01 & 0.16 & — & — & — & 148.6\
50 & 0.4 & 0.02 & 0.32 & — & — & — & 126.4\
60 & 0.4 & 0.02 & 0.53 & — & — & — & 119.2\
70 & 0.4 & 0.03 & 0.89 & — & — & — & 119.2\
80 & 0.4 & 0.04 & 1.33 & — & — & — & 127.6\
 & **131.0\
\
\
 & & &\
$n$ & $p$ & GTS & Liu2 & CPM & GTS & Liu2 &
<span>GTS</span>/<span>Liu2</span> (%)\
10 & 0.4 & \<0.01 & \<0.01 & 0.05 & 164.6 & 199.0 & 84.6\
20 & 0.4 & \<0.01 & 0.02 & 186.22 & 198.6 & 209.2 & 105.4\
30 & 0.4 & 0.01 & 0.05 & — & — & — & 126.3\
40 & 0.4 & 0.01 & 0.13 & — & — & — & 92.3\
50 & 0.4 & 0.01 & 0.21 & — & — & — & 123.1\
60 & 0.4 & 0.02 & 0.37 & — & — & — & 110.8\
70 & 0.4 & 0.02 & 0.53 & — & — & — & 112.0\
80 & 0.4 & 0.03 & 0.83 & — & — & — & 107.2\
 & **107.7\
\
\
 & & &\
$n$ & $p$ & GTS & Liu2 & CPM & GTS & Liu2 &
<span>GTS</span>/<span>Liu2</span> (%)\
10 & 0.8 & \<0.01 & \<0.01 & 0.50 & 225.1 & 216.6 & 107.9\
20 & 0.8 & \<0.01 & 0.01 & 31,464.60 & 312.1 & 264.0 & 117.7\
30 & 0.8 & \<0.01 & 0.03 & — & — & — & 85.2\
40 & 0.8 & 0.01 & 0.06 & — & — & — & 84.1\
50 & 0.8 & 0.01 & 0.11 & — & — & — & 85.3\
60 & 0.8 & 0.01 & 0.18 & — & — & — & 69.7\
70 & 0.8 & 0.02 & 0.28 & — & — & — & 111.2\
80 & 0.8 & 0.02 & 0.41 & — & — & — & 83.1\
 & **93.0\
********

[TableSmall]

Table [TableLarge] presents similar data concerning solution values and
running times of the two heuristics on “large” instances – those with a
number of vertices varying from 100 to 1,000. Columns 3-4 are average
running times over three instances, while column 5 has the same meaning
as column 8 of Table [TableSmall].

[hbt]

<span>cc</span>

<span>cc|rr|r</span> & &\
& & Obj.Fcn.\
$n$ & $p$ & GTS & Liu2 & <span>GTS</span>/<span>Liu2</span> (%)\
100 & 0.2 & 0.07 & 3.19 & 154.3\
200 & 0.2 & 0.29 & 25.48 & 159.6\
300 & 0.2 & 0.62 & 83.61 & 139.4\
400 & 0.2 & 1.08 & 194.82 & 142.0\
500 & 0.2 & 1.68 & 380.38 & 149.9\
750 & 0.2 & 3.76 & 1,278.56 & 139.0\
1,000 & 0.2 & 6.66 & 3,031.05 & 137.6\
 & **146.0\
\
\
& &\
& & Obj.Fcn.\
$n$ & $p$ & GTS & Liu2 & <span>GTS</span>/<span>Liu2</span> (%)\
100 & 0.4 & 0.06 & 2.31 & 173.7\
200 & 0.4 & 0.22 & 18.37 & 125.4\
300 & 0.4 & 0.48 & 61.98 & 139.6\
400 & 0.4 & 0.84 & 146.20 & 135.8\
500 & 0.4 & 1.31 & 285.12 & 139.8\
750 & 0.4 & 2.92 & 960.87 & 132.0\
1,000 & 0.4 & 5.18 & 2,274.18 & 137.1\
 & **140.5\
****

&

<span>cc|rr|r</span> & &\
& & Obj.Fcn.\
$n$ & $p$ & GTS & Liu2 & <span>GTS</span>/<span>Liu2</span> (%)\
100 & 0.6 & 0.04 & 1.54 & 141.1\
200 & 0.6 & 0.16 & 12.15 & 105.6\
300 & 0.6 & 0.34 & 40.93 & 120.4\
400 & 0.6 & 0.60 & 97.10 & 112.4\
500 & 0.6 & 0.93 & 189.56 & 107.3\
750 & 0.6 & 2.05 & 639.78 & 117.3\
1,000 & 0.6 & 3.64 & 1,513.18 & 110.2\
 & **116.3\
\
\
& &\
& & Obj.Fcn.\
$n$ & $p$ & GTS & Liu2 & <span>GTS</span>/<span>Liu2</span> (%)\
100 & 0.8 & 0.03 & 0.76 & 91.2\
200 & 0.8 & 0.10 & 5.99 & 81.3\
300 & 0.8 & 0.21 & 20.20 & 82.3\
400 & 0.8 & 0.36 & 48.00 & 98.0\
500 & 0.8 & 0.54 & 93.86 & 91.1\
750 & 0.8 & 1.18 & 316.72 & 92.0\
1,000 & 0.8 & 2.10 & 751.06 & 90.4\
 & **89.5\
****

[TableLarge]

It can seen from Table [TableSmall] that <span>GTS</span> outperforms
<span>Liu2</span> substantially when it comes to running time. On the
other hand, <span>Liu2</span> performs better, with respect to objective
function value, than <span>GTS</span> in most cases. In fact, this is
true for instances with $p \leq 60$. For instances with $p=0.80$, the
tendency reverts: heuristic <span>GTS</span> produces solutions that
are, in average, slightly less costly than those produced by
<span>Liu2</span>.

In Table [TableLarge], the running time of <span>GTS</span> remains very
low. Indeed, it is sometimes several orders of magnitude lower than that
of <span>Liu2</span>. Here it is important to highlight the fact that
the algorithms’ implementations are homogenized, in the sense that the
two programs share as much code and data structures as possible.
Regarding the objective function of the heuristic solutions, we can see
a similar trend to the one present in the Table [TableSmall]: algorithm
<span>Liu2</span> obtains better objective function values for instances
with $p \leq 0.6$, at the expense of higher running times. For $p=0.8$,
<span>GTS</span> outperforms <span>Liu2</span>, producing solutions
whose objective function values are 89.5% of those of the solutions
produced by <span>Liu2</span>, in average. This behavior seems to be
related to the scenario described in Section [Algo2], where algorithm
<span>Liu2</span> can produce solutions that are far from being optimal.
Indeed, the lower the density of the graphs (resulting from high values
of $p$), the more likely it is that the scenario described in Section
[Algo2] will happen.

Concluding Remarks
==================

In this paper, we presented two novel solution approaches for the
so-called Minimum Cost Linear Extension problem (MLCE): a topological
sorting-based heuristic and a constraint programming model. We revisited
an existing greedy algorithm for the MCLE, due to Liu <span>*et
al.*</span> @Liu, and showed that their algorithm is not an
approximation one, as originally suggested in their paper.

We also carried out a number of computational experiments on randonly
generated posets and reported on the relative performance of the three
procedures, which shows a clear trend as we vary the density of the
posets. The constraint programming approach was unable to compute
provably optimal solutions for posets with more than 20 elements
($n>20$), but it has proven instrumental in determining empirically that
the algorithm of Liu <span>*et al.*</span> obtains near-optimal
solutions for small-sized instances. In addition to that, the Liu
<span>*et al.*</span> algorithm clearly outperforms the topological
sorting heuristic for posets with high density (values of the parameter
$p \leq 0.4$). For $p=0.6$, the scenario is no longer entirely clear,
but their algorithm was still slightly superior. For $p=0.8$ (lower
densities), the topological sorting heuristic provided better solutions
than Liu <span>*et al.*</span>’s algorithm. A remarkable feature of the
topological sorting heuristic is its consistently low running time,
which was, at times, several orders of magnitude smaller than that of
the algorithm of Liu <span>*et al*</span>.

<span>99</span>

L. Bianco, P. Dell’Olmo, S. Giordani, <span>**An optimal algorithm to
find the jump number of partially ordered sets**</span>, Computational
Optimization and Applications, 8(2):197-210, 1997.

V. Bouchitte, M. Habib, **NP-Completeness properties about linear
extensions**, Order 4:13-154, 1987.

Gecode Team, <span>**Gecode: Generic Constraint Development
Environment**</span>, Available from `http://www.gecode.org`, 2006.

L. Liu, B. Wu, E. Yao, <span>**Minimizing the sum cost in linear
extensions of a poset**</span>, J. Comb. Optim. 21:247-253, 2009.

B. Wu, E. Yao, L. Liu, <span>**A polynomially solvable case of optimal
linear extension problem of a poset**</span>, J. Comb. Optim.
20:422-428, 2010.
