function cntFib(n: integer): integer;
var fibTek, fibPred1, fibPred2, i: integer;
begin
 fibPred2:= 1;
 fibPred1:= 2;
 if n > 2
 then begin
       for i:= 3 to n do
        begin
         fibTek:= fibPred1 + fibPred2;
         fibPred2:= fibPred1;
         fibPred1:= fibTek;
        end;
       cntFib:=fibTek; 
      end 
 else cntFib:= n;
end;
