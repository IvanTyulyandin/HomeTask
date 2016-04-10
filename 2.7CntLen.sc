def cntLen(l : List[Int]) : Int ={
  def cntAcc(l : List[Int], res : Int) : Int ={
    if (l.isEmpty)
      res
    else cntAcc(l.tail, res + 1)
  }
  cntAcc(l, 0)
}
