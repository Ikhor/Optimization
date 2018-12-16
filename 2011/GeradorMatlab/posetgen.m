% Geracao de um poset por meio de intersecao de ordens totais
% function [A,P] = posetgen(n, d)
% onde:
%   n = numero de elementos do poset
%   d = dimensao do poset (num. de ELs intersectadas)
%
% Tiberius O. Bonates
% Janeiro-Abril, 2012
function [A,P,NR] = posetgen(n, d, s)

%rand('seed',sum(100*clock))
rand('seed', s)

% Lista de relacoes do poset
% Inicialmente, inclui todas as relacoes (inclusive as transitivas).
% Subsequentemente, passa a conter somente as relacoes de cobertura.
P = [];

% Matriz de adjacencia  relativa a todas as relacoes.
A = zeros(n,n);

% Calcular vetor auxiliar "ordem" para cada ordem total
permutations = zeros(d,n);
ordem = zeros(d,n);
for i=1:d
    permutations(i,:) = randperm(n);
    for j=1:n
        ordem(i,permutations(i,j)) = j;
    end
end

% Popular "P" e "A"
for i=1:n
    for j=1:n
        if (i ~= j)
            containsRelation = 1;
            for linorder = 1:d
                % search for the (i<j) pair in each linear order
                if (ordem(linorder,i) > ordem(linorder,j))
                    containsRelation = 0;
                    break
                end
            end
            if (containsRelation == 1)
                P = [P ; [i,j]];
                A(i,j) = 1;
            end
        end
    end%for
end%for

%disp(sprintf('Poset -- representacao nao-processada (%d relacoes):', size(PC,1)))
%disp('digraph PC {')
%for i=1:size(PC,1)
%    disp(sprintf('%d -> %d', PC(i,1), PC(i,2)));
%end
%disp('}')

% Obter representacao compacta (unica) do poset.
% Em outras palavras, obter grafo de cobertura do poset.
[P] = covergraph(A);

nome = sprintf('graph_n%d_d%d_s%d.txt', n, d, s);
fp = fopen(nome, 'w+');

%disp(sprintf('Poset -- representacao compacta (%d relacoes):', size(P,1)))
fprintf(fp, 'digraph PRC {\n');
for i=1:n
    fprintf(fp, sprintf('node%d [label=\"%d\", shape=circle]\n', i, i-1));
end
for i=1:size(P,1)
    fprintf(fp, sprintf('node%d -> node%d\n', P(i,1), P(i,2)));
end
fprintf(fp, '}\n');
fclose(fp);

% Computar lista de nao-relacoes (relacoes de incomparabilidade)
% Atribuir custos aleatorios e nao necessariamente simetricos.
NR = [];
for i=1:n
    for j=1:n
        if ((i<j) && (A(i,j) == 0) && (A(j,i) == 0))
            rel = [[i, j, round(rand*100)] ; [j, i, round(rand*100)]];
            NR = [NR ; rel];
        end
    end
end

% Imprime arquivo com especificacao o poset
% Utiliza formato de entrada da nossa implementacao em C++ dos
% algoritmos de Wu et al, Greedy Topological Sorting e MergePoset
% (Aqui, indices sao deslocados, de forma a comecar a partir de zero.)
nome = sprintf('poset_n%d_d%d_s%d.txt', n, d, s);
fp = fopen(nome, 'w+');
fprintf(fp, sprintf('%d\n%d\n%d\n', n, size(P,1), size(NR,1)));
for i=1:size(P,1)
    fprintf(fp, sprintf('%d %d\n', P(i,1)-1, P(i,2)-1));
end
for i=1:size(NR,1)
    fprintf(fp, sprintf('%d %d %d\n', NR(i,1)-1, NR(i,2)-1, NR(i,3)));
end
fclose(fp);

end%function
