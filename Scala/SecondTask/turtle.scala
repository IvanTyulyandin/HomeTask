package secondHomeTask

/**
  * Created by ivan on 20.12.16.
  */
object Turtle {

  var movArr : Array[(Double, Double, Double)] = Array.empty
  var thrNum = 1
  var numLen = 1

  val Pi = Math.PI

  def asin(x: Double): Double =
    180 * Math.asin(x) / Pi

  def cos(x: Double): Double =
    Math.cos(x * Pi / 180)

  def sin(x: Double): Double =
    Math.sin(x * Pi / 180)

  def createTriples() : Array[(Double, Double, Double)] = {

    println("Enter num of threads")
    thrNum = scala.io.StdIn.readInt()
    println("Enter num of commands")
    numLen = scala.io.StdIn.readInt()

    var hlp = 0
    if (numLen % thrNum != 0) {
      hlp = thrNum - numLen % thrNum
    }
    val res : Array[(Double, Double, Double)] = new Array[(Double, Double, Double)](numLen + hlp)

    for (i <- 0 until numLen) {
      val angle = scala.io.StdIn.readInt().toDouble
      val dist = scala.io.StdIn.readInt().toDouble
      res(i) = (dist, angle, angle)
    }
    for (i <- numLen until numLen + hlp) {
      res(i) = (0.0, 0.0, 0.0)
    }

    numLen += hlp
    res
  }

  def parScanOperator(p : (Double, Double, Double), q : (Double, Double, Double)) : (Double, Double, Double) = {
    val (a, realAlpha, alpha) = p
    val (b, realBeta,  beta)  = q

    val M = Math.sqrt(a * a + b * b + 2 * a * b * cos(realBeta + alpha - realAlpha))
    if (M == 0) {
      (M, realAlpha, (alpha + beta) % 360)
    }
    else {
      (M,
        (realAlpha + asin(b * sin(realBeta + alpha - realAlpha) / M)) % 360,
        (alpha + beta) % 360)
    }
  }

  def collectPhase() : Array[(Double, Double, Double)] = {

    val res : Array[(Double, Double, Double)] = movArr
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
    movArr = createTriples()
    movArr = collectPhase()
    println(movArr.last._1 * cos(movArr.last._2), movArr.last._1 * sin(movArr.last._2))
  }
}
