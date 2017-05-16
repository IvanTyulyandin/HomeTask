module Main where

import Data.List
import Data.Char

type Ans = [(Char,Char)]
type SortByLen = [String]
type Voc = [String]

revLengthOrd :: String -> String -> Ordering
revLengthOrd x y 
  | lx > ly = LT
  | ly > lx = GT
  | otherwise = EQ
  where lx = length x
        ly = length y

checkForInsert :: Ans -> (Char,Char) -> Bool
checkForInsert ans (from,to) = all (\(x,y) -> ((from == x && to == y) || (from /= x && to /= y))) ans

checkAllAreGood :: Ans -> Ans -> Bool
checkAllAreGood preAns forAdd = all (checkForInsert preAns) forAdd

preAnsCheck :: Ans -> Bool
preAnsCheck [] = True
preAnsCheck (x:xs) = (checkForInsert xs x) && preAnsCheck xs

addPart :: Ans -> Ans -> Ans
addPart preAns forAdd = nub (preAns ++ forAdd)

solver :: SortByLen -> Voc -> Ans -> Ans
solver [] voc preAns = [('@','@')]
solver tab [] preAns = if ((length preAns) == 26 * 2) then preAns else [('!','!')] -- 26 * 2 that means all letters were found
solver tab (v:voc) preAns = concat (map (\x -> solver tab voc x) possibleWays)
  where goodWordsLen = filter (\str -> (length str == length v)) tab
        goodWordsNub = if not $ null goodWordsLen
                       then map (\str -> nub (zip str v)) goodWordsLen 
                       else error "len is null"
        allNotContra = if not $ null goodWordsNub
                       then filter preAnsCheck goodWordsNub
                       else error "nub is null"
        notContraWithAns = if not $ null allNotContra
                           then filter (checkAllAreGood preAns) allNotContra
                           else error "allNotContra is null"
        possibleWays = if not $ null notContraWithAns
                       then map (\x -> addPart preAns x) notContraWithAns
                       else error $ "notContraWithAns is null" ++ concatMap (\x -> '\n' : show x) preAns ++ ('\n' : show v) ++ concatMap (\x -> '\n' : show x) goodWordsLen

--if ((length preAns) == 26 * 2) 
--  then preAns 
--  else 
main = do
  enc <- readFile "sample.txt.enc"
  let parsed = sortBy revLengthOrd $ nub $ map (\str -> filter isAlpha str) (words enc)
  vocab <- readFile "sample.txt.voc"
  let voc = words vocab
  let ans = solver parsed voc []
  if (ans == [])
  then putStrLn "Empty"
  else putStrLn "not Empty"
  mapM_ (\(x,y) -> putStrLn (show x ++ show y)) ans
  
