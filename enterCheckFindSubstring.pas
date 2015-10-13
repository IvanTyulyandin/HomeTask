program KR;
var s, fw: string;
    needRes, tekRes, res: integer;
    
function memCmp(s1, s2: string): boolean;
begin
 memCmp:= s1 = s2;
end;

procedure reHash(var sh: string; ch: char);
begin
 tekRes:= tekRes - ord(sh[1]) + ord(ch);
 delete(sh,1,1);
 sh:= sh + ch;
end;

procedure algKR(s, fw: string);
var i: integer;
    sh: string;
    ch: char;    
begin
 for i:= 1 to length(fw) do
  begin
   needRes:= needRes + ord(fw[i]);
   tekRes:= tekRes + ord(s[i]);
   sh:= sh + s[i];
  end;
 if tekRes = needRes
 then if memCmp(fw, sh)
      then inc(res);
 for i:= 1 to length(s) - length(fw) do
  begin
   ch:= s[length(fw) + i];
   reHash(sh, ch);
   if tekRes = needRes
   then if memCmp(fw, sh)
        then inc(res);
  end;
end;

begin
 writeln('Enter string and substring');
 readln(s);
 readln(fw);
 algKR(s, fw);
 writeln(res);
end.
