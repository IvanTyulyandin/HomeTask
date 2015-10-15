function isStrSimmet(s: string): boolean;
var i, sp: integer;
begin
 sp:= pos(' ', s);
 while sp <> 0 do
  begin
   delete(s, sp, 1);
   sp:=  pos(' ', s);
  end;
 for i:= 1 to length(s) do
  s[i]:= LowerCase(s[i]);
 for i:= 1 to (length(s) div 2) do
  if s[i] <> s[length(s) - i + 1]
  then begin
        isStrSimmet:= false;
        exit;
       end;
 isStrSimmet:= true; 
end;
