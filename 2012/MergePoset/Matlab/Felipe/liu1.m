function [] = liu1(C1, C2, custos)

C1 = C1(:);
C2 = C2(:);
l = size(C1,1);
k = size(C2,1);

A = zeros(l+1,k+1);
B = zeros(l+1,k+1);

for j=1:k
    A(1,1+j)=Inf;
    if (j > 1)
        B(1,1+j) = B(1,j) + custos(C2(j-1),C2(j));
    end
end

for i=1:l
    B(1+i,1)=Inf;
    if (i > 1)
        A(1+i,1) = A(i,1) + custos(C1(i-1),C1(i));
    end
end


for i=1:l
    for j=1:k
        A(1+i,1+j) = min(A(i,1+j), B(i,1+j) + custos(C2(j),C1(i)));
        B(1+i,1+j) = min(A(1+i,j) + custos(C1(i),C2(j)), B(1+i,j));
    end
end

A
B

min(A(1+l,1+k), B(1+l,1+k))


end
