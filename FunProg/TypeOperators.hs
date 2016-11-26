{-# LANGUAGE TypeOperators #-}

data a * b = a :*: b
data a + b = L a | R b

data Fix f = F (f (Fix f))

data AList a b = A ( () + (a * b) )
type List a = Fix (AList a) -- F (AList a) (F (AList a) (Fix (AList a)))

type Maybe a = () + a

newtype ATree a b = T (a + (b * b)) 
type Tree a = Fix (ATree a)

main = print "compiled"


