def evalRoots(l : Array[Double]) : List[Double] ={
  def evalCub(l : Array[Double]) : List[Double] = {
    val i = l(0)
    val a = l(1) / i
    val b = l(2) / 1
    val c = l(3) / i
    val Q = (math.pow(a, 2) - 3 * b) / 9
    val R = (2 * math.pow(a, 3) - 9 * a * b + 27 * c) / 54
    val S = math.pow(Q, 3) - math.pow(R, 2)
    if (S > 0) {
      val fi =  math.acos(R / math.pow(Q, 3 / 2)) / 3
      val x1 = -2 * math.sqrt(Q) * math.cos(fi) - a / 3
      val x2 = -2 * math.sqrt(Q) * math.cos(fi + 2 / 3 * math.Pi) - a / 3
      val x3 = -2 * math.sqrt(Q) * math.cos(fi - 2 / 3 * math.Pi) - a / 3
      x1 :: x2 :: x3 :: Nil
    }
    else {
      val m = -math.signum(R) * math.pow(math.abs(R) + math.sqrt(-S), 1 / 3)
      var n : Double = 0
      if (m != 0) {
        n = Q / m
      }
      if (m == n) {
        ((m + n) - a / 3) :: (-m - a / 3) :: Nil
      } else {
        ((m + n) - a / 3) :: Nil
      }
    }
  }
  def evalQuadr(l : Array[Double]) : List[Double] ={
    // l(0) = 0
    val a = l(1)
    val b = l(2)
    val c = l(3)
    val D = math.pow(b,2) - 4 * a * c
    if (D < 0)
      Nil
    else {
      ((-b + math.sqrt(D)) / (2 * a)) :: ((-b - math.sqrt(D)) / (2 * a)) :: Nil
    }
  }
  if (l(0) == 0)
    evalQuadr(l)
  else
    evalCub(l)
}
