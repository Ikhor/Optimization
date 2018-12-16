function [P]=covergraph(A)

n = size(A,1);

P = [];

for i=1:n
    depthv = zeros(1,n);
    Q = [i];
    while (~isempty(Q))
        v = Q(1);
        Q(1) = [];
        for k=1:n
            if (A(v,k) == 1)
                Q = [Q, k];
                depthv(k) = max(depthv(k), depthv(v) + 1);
            end
        end
    end%while

    % Include in P only the cover relations
    for j=1:n
        if (A(i,j)==1)
            if (depthv(j) == (depthv(i)+1))
                P = [P ; [i,j]];
            else
                A(i,j) = 0;
            end
        end
    end
end

end%function
