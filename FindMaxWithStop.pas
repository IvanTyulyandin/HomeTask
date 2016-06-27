program task;
var teknum, maxNum, stopNum : integer;
  f : text;
{tekNum - текущее считываемое число}
{maxNum - максимальное число}
{stopNum - число остановки}
begin
  assign(f, 'input.txt'); {если нужно, то имя файла ввода можно поменять}
  reset(f);
  readln(f, stopNum);
  maxNum := -2147483648; {максимальное отрицательное число в ABC паскале -2^31}
  tekNum := stopNum - 1; {делаем так, чтоб точно зайти в цикл хотя бы один раз}
  while (tekNum <> StopNum) do
    begin
      readln(f, tekNum);
      if (tekNum > maxNum)
      then maxNum := tekNum;
    end;
  close(f);
  writeln('Максимальное число = ', maxNum);
end.
