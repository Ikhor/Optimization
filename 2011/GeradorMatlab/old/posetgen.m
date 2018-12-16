% Geracao de um poset por meio de intersecao de ordens totais
% function [A,P] = posetgen(n, d)
% onde:
%   n = numero de elementos do poset
%   d = dimensao do poset (num. de ELs intersectadas)
%
% Tiberius O. Bonates
% Janeiro-Abril, 2012
function [densidade] = posetgen(n, d)

rand('seed',sum(100*clock))
P = [];

A = zeros(n,n);

permutations = zeros(d,n);
ordem = zeros(d,n);
for i=1:d
    permutations(i,:) = randperm(n);
    for j=1:n
        ordem(i,permutations(i,j)) = j;
    end
end


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

%disp(sprintf('Poset -- representacao nao-processada (%d relacoes):', size(P,1)))
%disp('digraph PNP {')
%for i=1:size(P,1)
%    disp(sprintf('%d -> %d', P(i,1), P(i,2)));
%end
%disp('}')

densidade = (size(P,1)*2) / (n*(n-1));

%disp(sprintf('dens = %6.3f', densidade));

end%function
