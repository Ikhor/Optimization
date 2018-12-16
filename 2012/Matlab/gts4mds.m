function []=gts4mds( A )

[n, m] = size(A);

D=zeros(n);

for i=1:n
    for j=1:n
        dHamming = 0;
        for k=1:m
            if (A(i,k) ~= A(j,k))
                dHamming = dHamming + 1;
            end
        end
        dHamming = dHamming / m;
        D(i,j) = dHamming;
    end
end

% Perform MDS via Matlab...
[Y,Stress] = mdscale(D,3);

% Plotar dados mapeados para R^3
f=figure; hold on;
for i=1:size(A,1)
    plot3(A(i,1),A(i,2),A(i,3),'b*');
end

end%function
