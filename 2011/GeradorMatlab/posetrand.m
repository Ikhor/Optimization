% Geracao de um poset como grafo aleatorio
% function [A,P] = posetrand(n, p, s)
% onde:
%   n = numero de elementos do poset
%   p = probabilidade de criacao de arco
%   s = semente para gerador aleatorio
%
% Tiberius O. Bonates
% Abril, 2012
function [A,P,NR] = posetrand(n, p, s)

%rand('seed',sum(100*clock))
rand('seed', s)

% Lista de relacoes do poset
% Inicialmente, inclui todas as relacoes (inclusive as transitivas).
% Subsequentemente, passa a conter somente as relacoes de cobertura.
P = [];

% Matriz de adjacencia relativa a todas as relacoes.
global A
A = zeros(n,n);

% Popular "P" e "A"
for i=1:n
    for j=(i+1):n
        rn = rand;
        if (rn > p)
            A(i,j) = 1;
        end
    end%for
end%for

for i=(n-1):-1:1
    for j=(i+1):n
        if (A(i,j)==1)
            A(i,j) = 0;
            if (alcanca(i,j) == 0)
                A(i,j) = 1;
                P = [P ; [i,j]];
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
%[P] = covergraph(A);

nome = sprintf('rg_n%d_p%4.2f_s%d.txt', n, p, s);
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

% Imprime arquivo com especificacao do poset
% Utiliza formato de entrada da nossa implementacao em C++ dos
% algoritmos de Wu et al, Greedy Topological Sorting e MergePoset
% (Aqui, indices sao deslocados, de forma a comecar a partir de zero.)
nome = sprintf('rp_n%d_p%4.2f_s%d.txt', n, p, s);
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
