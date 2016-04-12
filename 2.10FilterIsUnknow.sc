def p(i : Int) : Boolean ={
  if (i > 0){
    true
  }
  else {
    false
  }
}

def filterWithCond(p : Int => Boolean, l : List[Int]) : List[Int] ={
  def fltHlp(l : List[Int], ans : List[Int]) : List[Int] ={
    if (l.nonEmpty){
      if (p(l.head)){
        fltHlp(l.tail, l.head :: ans)
      }
      else {
        fltHlp(l.tail, ans)
      }
    }
    else ans
  }
  fltHlp(l,Nil).reverse
}
