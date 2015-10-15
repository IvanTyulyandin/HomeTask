function cntZero(m: mas): integer;
var i, res: integer;
begin
 res:= 0;
 for i:= 1 to n do
  if m[i] = 0
  then inc(res);
 CntZero:= res; 
end;
