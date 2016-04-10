def reverse(l : List[Int]) : List[Int] ={
  def revHlp(l : List[Int], res : List[Int]) : List[Int] ={
    if (l.isEmpty)
      res
    else revHlp(l.tail, l.head :: res)
  }
  revHlp(l, Nil)
}
