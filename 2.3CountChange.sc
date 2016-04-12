def countChange(money : Int, coins : List[Int]) : Int ={
  var res : Int = 0
  def cnt(curSum : Int, curCoins : List[Int]) : Int ={
    if ((curSum >= money) || curCoins.isEmpty){
      if (curSum == money) {
        res += 1
      }
      res
    }
    else {
      cnt(curSum, curCoins.tail)
      cnt(curSum + curCoins.head, curCoins)
    }
  }
  cnt(0,coins)
}
