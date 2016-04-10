def factorial(num : Int) : Int ={
  def factAcc(num : Int, acc : Int) : Int ={
    if (num == 0) acc
    else factAcc(num - 1, acc * num)
  }
  factAcc(num, 1)
}

def PascalTriangle(n : Int, k : Int) : Double = {
  val nomin = factorial(n)
  val denom = factorial(k) * factorial(n - k)
  nomin / denom
}
