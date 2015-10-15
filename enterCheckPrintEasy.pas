procedure printEasy(x: integer);
var tek, i: integer;
begin
 tek:=0;
 while  tek <= x do
  begin
   for i:= 2 to round(sqrt(tek)) + 1 do
    begin
     if tek mod i = 0
     then break;
    end;
   if i =  round(sqrt(tek)) + 1
   then writeln(tek);
   inc(tek);
  end;
end;
