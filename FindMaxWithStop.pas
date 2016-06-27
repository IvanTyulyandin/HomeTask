program task;
var teknum, maxNum, stopNum : integer;
{tekNum - текущее считываемое число}
{maxNum - максимальное число}
{stopNum - число остановки}
begin
  writeln('Введите символ остановки');
  readln(stopNum);
  maxNum := -2147483648; {максимальное отрицательное число в ABC паскале}
  tekNum := stopNum - 1; {делаем так, чтоб точно зайти в цикл хотя бы один раз}
  while (tekNum <> StopNum) do
    begin
      writeln('Введите очередной символ');
      readln(tekNum);
      if (tekNum > maxNum)
      then maxNum := tekNum;
    end;
  writeln('Максимальное число = ', maxNum);
end.
