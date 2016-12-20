object LinComp {

  var num1 : Array[Int] = Array.emptyIntArray
  var num2 : Array[Int] = Array.emptyIntArray
  var thrNum = 1
  var numLen = 1 // will be changed in main after toEqualSize
  var ab : Array[(Int, Int)] = Array.empty

  def getNum (str : String, whatToPush : Int) : Array[Int] = {

    def getNearestPowerOf2 (x : Int) : Int = {
      var tmp = 1
      var curPow = 0
      while (tmp < x) {
        tmp *= 2
        curPow += 1
      }
      curPow
    }

    val strLen = str.length
    val arrSize = scala.math.pow (2, getNearestPowerOf2(strLen)).toInt
    val res: Array[Int] = new Array[Int](arrSize)
    for (i <- 0 until strLen) {
      res(i) = str.charAt(i).asDigit
    }
    for (i <- strLen until arrSize) {
      res(i) = whatToPush
    }
    res
  }

  def toEqualSize(arr : Array[Int], toSize : Int, whatToPush : Int) : Array[Int] = {
    val res : Array[Int] = new Array[Int](toSize)
    for (i <- arr.indices) {
      res(i) = arr(i)
    }
    val arrLen = arr.length
    for (i <- arrLen until toSize) {
      res(i) = whatToPush
    }
    res
  }

  def prefixScanOperator(fst : (Int, Int), snd : (Int, Int)) : (Int, Int) = {
    (fst._1 * snd._1, snd._1 * fst._2 + snd._2)
  }

  def collectPhase() : Array[(Int, Int)] = {

    val res : Array[(Int, Int)] = ab
    val numPerThr = numLen / thrNum

    def doOnPart(leftBorder : Int, rightBorder : Int) : Unit = {
      //rightBorder - leftBorder + 1 is power of 2
      var cur = 2
      val need = rightBorder - leftBorder + 1
      while (cur <= need) {
        var i = leftBorder + cur - 1
        while (i <= rightBorder) {
          res(i) = prefixScanOperator(res(i - cur / 2), res(i))
          i += cur
        }
        cur *= 2
      }
    }

    def handlePart(leftBorder : Int, rightBorder : Int) : Unit = {
      if (rightBorder - leftBorder + 1 == numPerThr) {
        doOnPart(leftBorder, rightBorder)
      }
      else {
        println(leftBorder, rightBorder)
        val newThr = new Thread() {
          override def run() : Unit = {
            handlePart((rightBorder - leftBorder) / 2 + 1 + leftBorder, rightBorder)
          }
        }
        newThr.start()
        handlePart(leftBorder, (rightBorder - leftBorder - 1) / 2 + leftBorder)
        newThr.join()
        res(rightBorder) = prefixScanOperator(res((rightBorder - 1 - leftBorder) / 2 + leftBorder), res(rightBorder))
      }
    }

    handlePart(0, numLen - 1)
    res
  }

  def distributePhase() : Array[(Int, Int)] = {

    val res : Array[(Int, Int)] = ab
    val numPerThr = numLen / thrNum

    def doOnPart(leftBorder : Int, rightBorder : Int) : Unit = {
      var cur = rightBorder - leftBorder + 1 // it's power of 2
      while (cur >= 2) {
        var i = cur - 1 + leftBorder
        while (i <= rightBorder) {
          val tmp = res(i)
          res(i) = prefixScanOperator(res(i), res(i - cur / 2))
          res(i - cur / 2) = tmp
          i += cur
        }
        cur /= 2
      }
    }

    def handlePart(leftBorder : Int, rightBorder : Int) : Unit = {
      if (rightBorder - leftBorder + 1 == numPerThr) {
        doOnPart(leftBorder, rightBorder)
      }
      else {
        println(leftBorder, rightBorder)
        val newThr = new Thread() {
          override def run() : Unit = {
            handlePart((rightBorder - leftBorder) / 2 + 1 + leftBorder, rightBorder)
          }
        }
        val tmp = res(rightBorder)
        res(rightBorder) = prefixScanOperator(res(rightBorder), res((rightBorder - leftBorder - 1) / 2 + leftBorder))
        res((rightBorder - leftBorder - 1) / 2 + leftBorder) = tmp
        newThr.start()
        handlePart(leftBorder, (rightBorder - leftBorder - 1) / 2 + leftBorder)
        newThr.join()
      }
    }

    handlePart(0, numLen - 1)
    res
  }

  def seqCalc(ab : Array[(Int, Int)], index : Int) : Int = {
    var x = ab(0)._2
    for (i <- 1 to index) {
      x = ab(i)._1 * x + ab(i)._2
    }
    x
  }

  def main(args: Array[String]): Unit = {
    num1 = getNum(scala.io.StdIn.readLine(), 1)
    num2 = getNum(scala.io.StdIn.readLine(), 0)
    println("Enter num of threads")
    thrNum = scala.io.StdIn.readInt()
    println("Enter index")
    val index : Int = scala.io.StdIn.readInt()
    val lastIndex = Math.max(num1.length, num2.length) - 1
    if (num1.length > num2.length) {
      num2 = toEqualSize(num2, num1.length, 0)
    } else {
      if (num1.length != num2.length) {
        num1 = toEqualSize(num1, num2.length, 1)
      }
    }
    numLen = num1.length
    ab = num1 zip num2
    for (i <- ab.indices) {
      print(ab(i))
    }
    println()
    var lastPair = (1,0)
    println(seqCalc(ab, index))
    if (index == lastIndex) {
      lastPair = ab(lastIndex)
    }
    ab = collectPhase()
    println("Collect done")
    ab(ab.length - 1) = (1,0)
    ab = distributePhase()
    if (index == lastIndex) {
      print(prefixScanOperator(ab(index), lastPair)._2)
    }
    else {
      print(ab(index + 1)._2)
    }
  }
}
