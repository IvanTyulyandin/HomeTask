def filter(l : List[Int]) : List[Int] ={
  def fltHlp(l : List[Int], ans : List[Int]) : List[Int] ={
    if (l.nonEmpty){
      if (l.head > 0){
        fltHlp(l.tail, l.head :: ans)
      }
      else {
        fltHlp(l.tail, ans)
      }
    }
    else ans
  }
  fltHlp(l, Nil).reverse
}
