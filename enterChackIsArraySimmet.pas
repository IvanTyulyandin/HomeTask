{mas = array[1.. n] of need type}
function isSimmet(m: mas): boolean;
var i: integer;
begin
 for i:= 1 to (n div 2) do
  if m[i] <> m[n - i + 1]
  then begin
        isSimmet:= false;
        exit;
       end;
 isSimmet:= true;
end;
