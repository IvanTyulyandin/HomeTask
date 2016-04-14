
type Set = Int => Boolean

def contains(s : Set, elem : Int) : Boolean = s(elem)

def singletonSet(elem : Int) : Set = {
  (x : Int) => x == elem
}
def union(s : Set, t : Set) : Set = {
  (x: Int) => contains(s, x) || contains(t, x)
}

def intersect(s : Set, t : Set) : Set = {
  (x : Int) => contains(s,x) && contains(t,x)
}

def diff(s : Set, t : Set) : Set ={
  (x : Int) => contains(s,x) && !contains(t,x)
}

def filter(s : Set, p : Int => Boolean) : Set ={
  (x : Int) => contains(s,x) && p(x)
}

val bound = 1000

def forall(s: Set, p: Int => Boolean): Boolean = {
  def iter(a: Int) : Boolean = {
    if (a > bound) true
    else if (!p(a) && contains(s,a)) false
    else iter(a + 1)
  }
  iter(-bound)
}

def exists(s : Set, p : Int => Boolean) : Boolean ={
  def iter(a : Int) : Boolean ={
    if (a > bound) false
    else if (p(a) && contains(s,a)) true
    else iter(a + 1)
  }
  iter(-bound)
}

def map(s : Set, f : Int => Int) : Set ={
  def emptySet = {(x : Int) => false}
  def iter(a : Int) : Set ={
    if (a > bound) emptySet
    else {
      if (contains(s,a)){
        union(singletonSet(f(a)), iter(a + 1))
      }
      else iter(a + 1)
    }

  }
  iter(-bound)
}


