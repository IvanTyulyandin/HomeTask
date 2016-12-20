object Brackets {

  var bracketsPair : Array[(Int, Int)] = Array.empty
  var thrNum = 1
  var numLen = 1


  def createBracketsPairs() : Array[(Int, Int)] = {
    val str = scala.io.StdIn.readLine()
    val res : Array[(Int, Int)] = new Array[(Int, Int)](str.length)

    def handlePart(leftBorder : Int, rightBorder : Int) : Unit = {
      for (i <- leftBorder to rightBorder) {
        if (str(i) == '(')
          res(i) = (1,0)
        else res(i) = (0,1)
      }
    }

    val bracPerThr = str.length / thrNum
    val forThread = bracPerThr until str.length by bracPerThr
    val tasks = forThread.map(x => {
      new Thread {
        override def run() : Unit = {
          handlePart(x, x + bracPerThr - 1)
        }
      }
    }
    )
    tasks.foreach(x => x.start())
    handlePart(0, bracPerThr - 1)
    tasks.foreach(x => x.join())

    res

  }

  def parScanOperator(fst : (Int, Int), snd : (Int, Int)) : (Int, Int) = {
    val hlp = Math.min(fst._1, snd._2)
    (fst._1 + snd._1 - hlp, fst._2 + snd._2 - hlp)
  }

  def collectPhase() : Array[(Int, Int)] = {

    val res : Array[(Int, Int)] = bracketsPair
    val numPerThr = numLen / thrNum

    def doOnPart(leftBorder : Int, rightBorder : Int) : Unit = {
      for (i <- leftBorder + 1 to rightBorder) {
        res(i) = parScanOperator(res(i - 1), res(i))
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
        res(rightBorder) = parScanOperator(res((rightBorder - 1 - leftBorder) / 2 + leftBorder), res(rightBorder))
      }
    }

    handlePart(0, numLen - 1)
    res
  }



  def main(args: Array[String]): Unit = {
    bracketsPair = createBracketsPairs()
    numLen = bracketsPair.length
    bracketsPair = collectPhase()
    if (bracketsPair(numLen - 1) == (0,0))
      println("Correct")
    else println("Not correct")
  }
}
