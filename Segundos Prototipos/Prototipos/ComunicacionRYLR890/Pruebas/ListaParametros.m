SP = [7 8 9 10 11 12];
BDW = [0 1 2 3];

contador = 1;
arreglo = zeros(1,4);

for i = 7:12
   for j = 0:9
      for k = 1:4
      for l = 4:7
          
          arreglo(contador,1) = i;
          arreglo(contador,2) = j;
          arreglo(contador,3) = k;
          arreglo(contador,4) = l;
          
        contador = contador + 1;
      end
      end
   end
end