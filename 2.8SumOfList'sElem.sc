def cntSum(l : List[Int]) : Int = {
  def cntAcc(l : List[Int], res : Int) : Int ={
    if (l.isEmpty)
      res
    else cntAcc(l.tail, res + l.head)
  }
  cntAcc(l, 0)
}
