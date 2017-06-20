function fastPower(x: real; n: integer): real; {x - number, n - power}
 var count: real;
   begin
    if n = 0 
    then begin
          fastPower:= 1; 
          exit;
         end;
    count:= 1;
    while n > 0 do
     begin
      if n mod 2 = 0 
      then begin
            n:= n div 2;
            x:= x * x;
           end
      else begin
            dec(n);
            count:= count * x;
           end;
     end;
    fastPower:= count;
end;
