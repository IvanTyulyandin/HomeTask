----читать pragma  ... 
{-# LANGUAGE FlexibleInstances, UndecidableInstances, MultiParamTypeClasses, FunctionalDependencies #-}	

data Z = Z deriving Show
data S a = S a deriving Show

class Add a b c | a b -> c where
	add :: a -> b -> c

instance Add Z a a where
	add _ x = x

instance Add a b ab => Add (S a) b (S ab) where
	add (S a) b = S (add a b)

---------------------------------------------------------------------------------------

class Reify a where
	reify :: a -> Integer

instance Reify Z where
	reify Z = 0

instance Reify a => Reify (S a) where
	reify (S a) = 1 + reify a

---------------------------------------------------------------------------------------

class Sub a b c | a b -> c where
	sub :: a -> b -> c

instance Sub a Z a where
	sub a _ = a

instance Sub a b c => Sub (S a) (S b) c where
	sub (S a) (S b) = sub a b

---------------------------------------------------------------------------------------

class IsBigger a b where
	isBigger :: a -> b -> Bool

instance IsBigger (S a) Z where
	isBigger _ _ = True

instance IsBigger a b => IsBigger (S a) (S b) where
	isBigger _ _ = True

---------------------------------------------------------------------------------------


class IsSmaller a b where
	isSmaller :: a -> b -> Bool

instance IsSmaller Z (S a) where
	isSmaller _ _ = True

instance IsSmaller a b => IsSmaller (S a) (S b) where
	isSmaller _ _ = True

---------------------------------------------------------------------------------------

class Rem a b c | a b -> c where --all non-zero 
	remIsNotZero :: a -> b -> c

instance IsSmaller (S a) b => Rem (S a) b (S a) where
    remIsNotZero _ b = (S a)

instance (IsBigger a b, Rem (Sub a b x) b c) => Rem a b c where
	remIsNotZero a b = remIsNotZero (sub a b) c
--class Prime a where
--	isPrime :: x -> Bool

--instance Prime (S(S Z)) where
--	isPrime _ = True

--instance CoPrimeWithAllPrev (S a) a => Prime (S a) where
--	isPrime _ = True

main = print $ reify $ remIsNotZero (S (S (S Z))) (S (S Z))
