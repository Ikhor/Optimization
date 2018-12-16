function [sn] = alcanca(o, d)

global A

n = size(A,1);
sn = 0;

Fila = [o];
while (~isempty(Fila))
    x = Fila(1); Fila(1)=[];
    for j=(x+1):n
        if (A(x,j)==1)
            Fila = [Fila, j];
            if (j == d), sn = 1; Fila=[]; break, end
        end
    end
end

end
