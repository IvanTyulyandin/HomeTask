/**
  * Created by ivan on 19.11.16.
  */

object LongIntAdding {

  var num1 : Array[Int] = Array.emptyIntArray
  var num2 : Array[Int] = Array.emptyIntArray
  var carry : Array[Char] = Array.emptyCharArray
  var thrNum = 1
  var numLen = 1 // will be changed in main after toEqualSize

  def getNum (str : String) : Array[Int] = {

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
    val revStr = str.reverse
    for (i <- 0 until strLen) {
      res(i) = revStr.charAt(i).asDigit
    }
    for (i <- strLen until arrSize) {
      res(i) = 0
    }
    res
  }

  def toEqualSize(arr : Array[Int], toSize : Int) : Array[Int] = {
    val res : Array[Int] = new Array[Int](toSize)
    for (i <- arr.indices) {
      res(i) = arr(i)
    }
    val arrLen = arr.length
    for (i <- arrLen until toSize) {
      res(i) = 0
    }
    res
  }

  def parCreateCharArr() : Array[Char] = {

    val res : Array[Char] = new Array[Char](numLen)

    def handlePart(leftBorder: Int, rightBorder : Int) : Unit = {
      for (i <- leftBorder until rightBorder) {
        val sum = num1(i) + num2(i)
        if (sum > 9) res(i) = 'C'
        else if (sum == 9) res(i) = 'M'
        else res(i) = 'N'
        }
    }

    val digitsForThread = numLen / thrNum
    val forThread = digitsForThread until numLen by digitsForThread //suppose, that threads num is power of 2
    val tasks = forThread.map(x => {
      new Thread {
        override def run() : Unit = {
          handlePart(x, x + digitsForThread)
        }
      }
    }
    )
    tasks.foreach(x => x.start())
    handlePart(0, digitsForThread)
    tasks.foreach(x => x.join())

    res
  }

  def prefixScanOperator(fst : Char, snd : Char) : Char = {
    if (snd == 'M') fst
    else snd
  }

  def collectPhase() : Array[Char] = {

    val res : Array[Char] = carry
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

  def distributePhase() : Array[Char] = {

    val res : Array[Char] = carry
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

  def main(args: Array[String]): Unit = {
    num1 = getNum(scala.io.StdIn.readLine())
    num2 = getNum(scala.io.StdIn.readLine())
    println("Enter num of threads")
    thrNum = scala.io.StdIn.readInt()
    if (num1.length > num2.length) {
      num2 = toEqualSize(num2, num1.length)
    } else {
      if (num1.length != num2.length) {
        num1 = toEqualSize(num1, num2.length)
      }
    }
    numLen = num1.length
    carry = parCreateCharArr()
    carry = collectPhase()
    carry(numLen - 1) = 'M'
    carry = distributePhase()
    val res = new Array[Int](numLen + 1)
    res(numLen) = 0
    for (i <- 0 until numLen) {
      res(i) = num1(i) + num2(i)
      if (carry(i) == 'C') {
        res(i) += 1
      }
      res(i) %= 10
    }
    if (carry(numLen - 1) == 'C') {
      res(numLen) = 1
    }
    var state = 0
    for (i <- numLen to 0 by -1) {
      state match {
        case 0 => if (res(i) != 0) {
                    state = 1
                    print(res(i))
                  }
        case 1 => print(res(i))
      }
    }
  }
}
