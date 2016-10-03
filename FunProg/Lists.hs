module TabledFunctions where
import Data.List
-- Табличное представление функции типа Int -> Int --- это список пар 
-- (аргумент, значение) без повторений; гарантируется, что в этом списке нет 
-- нескольких пар с одинаковым первым элементом. 

-- Реализовать следующие функции:

-- fromFun f s возвращает табличное представление сужения f на s; предполагается,
-- что f определена в каждой точке s.
fromFun :: (Int -> Int) -> [Int] -> [(Int, Int)]
fromFun f [] = []
fromFun f (x:xs) = (x, f x) : fromFun f xs

-- dom f по табличному представлению функции возвращает её область определения 
-- в виде списка без повторений.
dom :: [(Int, Int)] -> [Int]
dom [] = []
dom (x:xs) = fst x : dom xs

-- eval f x вычисляет значение таблично представленной функции для заданного
-- значения аргумента (если оно определено).
eval :: [(Int, Int)] -> Int -> Int
eval [] ar = error "Not defined"
eval (x:xs) ar = if ar==(fst x) then snd x else eval xs ar

-- invert f по табличному представлению функции возвращает табличное представление
-- её обратного отображения (это отображение не обязано быть функцией)
invert :: [(Int, Int)] -> [(Int, Int)]
invert [] = []
invert (x:xs) = (snd x, fst x) : invert xs

-- f .*. g по табличным представлениям функций возвращает табличное представление
-- их суперпозиции
infixr 9 .*.
(.*.) :: [(Int, Int)] -> [(Int, Int)] -> [(Int, Int)]
(.*.) [] [] = []
(.*.) x [] = []
(.*.) [] y = []
(.*.) (x:xs) (y:ys) = if (snd y == fst x) then (fst y, snd x) : (.*.) xs ys else (.*.) xs ys


-- image f s по табличному представлению функции и множеству целых чисел, представленному
-- в виде списка без повторений, возвращает образ этого множества (в виде списка
-- без повторений). Образ должен быть определен всегда.
image :: [(Int, Int)] -> [Int] -> [Int]
image [] s = []
image _ [] = []
image f (x:xs) = if argIn f x then nub (eval f x : image f xs) else image f xs
	where argIn f x | f == [] = False
        			| (fst.head) f == x = True
        			| True = argIn (tail f) x 

-- preimage f s вычисляет прообраз; агрументы и дополнительные условия --- как в
-- предыдущей задаче.
reEval :: [(Int, Int)] -> Int -> Int
reEval [] ar = error "Not defined"
reEval (x:xs) ar = if ar==(snd x) then fst x else reEval xs ar

preimage :: [(Int, Int)] -> [Int] -> [Int]
preimage [] s = []
preimage f [] = []
preimage f (y:ys) = if resIn f y then reEval f y : preimage f ys else preimage f ys
	where resIn f y | f == [] = False
					| (snd.head) f == y = True
					| True = resIn (tail f) y

-- isInjective f возвращает True тогда и только тогда, когда таблично-представленная функция 
-- инъективна.
isInjective :: [(Int, Int)] -> Bool
isInjective (x:[]) = True
isInjective [] = True
isInjective (x:xs) = if notProblem (x:xs) then notProblem xs else False
	where notProblem (x:xs) | xs == [] = True
						    | (fst x == (fst.head) xs) && (snd x /= (snd.head) xs) = False
						    | (fst x /= (fst.head) xs) && (snd x == (snd.head) xs) = False
						    | True = notProblem  (x:(tail xs))


-- areMutuallyInverse f g возвращает True тогда и только тогда, когда таблично-представленные
-- функции f и g --- взаимно-обратны.
areMutuallyInverse :: [(Int, Int)] -> [(Int, Int)] -> Bool
areMutuallyInverse f g = if (f == invert g) && (g == invert f) then True else False
