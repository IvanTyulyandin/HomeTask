/**
  * Created by ivan on 06.10.16.
  */
import java.io.File
import javax.imageio.ImageIO
import java.awt.image.BufferedImage

case class SplitColor(){
  var red : Int = 0
  var green : Int = 0
  var blue : Int = 0
  def setRed(newRed : Int) = {red = newRed}
  def setGreen(newGreen : Int) = {green = newGreen}
  def setBlue(newBlue : Int) = {blue = newBlue}
}



object Blur_filter {

  def getImg() : BufferedImage ={
    println("Enter name of file to Blur")
    val nameOfFile = scala.io.StdIn.readLine()
    ImageIO.read(new File(nameOfFile))
  }

  val img = getImg()
  val w = img.getWidth
  val h = img.getHeight

  def getRed(pixel : Int) : Int ={
    pixel >>> 0 & 0xFF
  }

  def getGreen(pixel : Int) : Int ={
    pixel >>> 8 & 0xFF
  }

  def getBlue(pixel : Int) : Int ={
    pixel >>> 16 & 0xFF
  }

  def getColor(r : Int, g : Int, b : Int) : Int ={
    (b << 16) | (g << 8) | (r << 0)
  }

  def getSplitArray(src : BufferedImage) : Array[Array[SplitColor]] ={
    val res = Array.fill[SplitColor](w,h)(new SplitColor)
    for (x <- 0 until w)
      for (y <- 0 until h){
        val pixel = src.getRGB(x, y)
        res(x)(y).setRed(getRed(pixel))
        res(x)(y).setGreen(getGreen(pixel))
        res(x)(y).setBlue(getBlue(pixel))
      }
    res
  }

  var SplitArray = getSplitArray(img)

  def BlurOnePixel(x : Int, y : Int, rad : Int) : Int ={
    var redSum = 0
    var greenSum = 0
    var blueSum = 0
    var numOfPixels = 1
    for (i <- (x - rad) until (x + rad + 1))
      for (j <- (y - rad) until (y + rad + 1)){
        if ((i >= 0) && (i < w) && (j >= 0) && (j < h)) {
            redSum = redSum + SplitArray(i)(j).red
            greenSum += SplitArray(i)(j).green
            blueSum += SplitArray(i)(j).blue
            numOfPixels += 1
          }
      }
    redSum = redSum / numOfPixels
    greenSum = greenSum / numOfPixels
    blueSum = blueSum / numOfPixels

    getColor(redSum, greenSum, blueSum)
  }

  var out : BufferedImage = img

  def BlurHorizontal(from : Int, to : Int, rad : Int) : BufferedImage ={
    for (y <- from until to)
      for (x <- 0 until w) {
        if ((x >= 0) && (x < w) && (y >= 0) && (y < h))
          out.setRGB(x, y, BlurOnePixel(x, y, rad))
      }
    out
  }

  def BlurHorPar(amountOfThreads : Int, rad : Int) : Unit ={
    val rowPerTask = h / amountOfThreads
    val forThread = Range(0, h - rowPerTask) by rowPerTask
    val tasks = forThread.map(x => {
        new Thread {
          override def run() : Unit = {
            println(x, x + rowPerTask)
            BlurHorizontal(x, x + rowPerTask, rad)
          }
        }
      }
    )
    tasks.foreach(x => x.start())
    tasks.foreach(x => x.join())
  }

  def BlurVertical(from : Int, to : Int, rad : Int) : BufferedImage ={
    for (x <- from until to)
      for (y <- 0 until h) {
        if ((x >= 0) && (x < w) && (y >= 0) && (y < h))
          out.setRGB(x, y, BlurOnePixel(x, y, rad))
      }
    out
  }

  def BlurVertPar(amountOfThreads : Int, rad : Int) : Unit ={
    val colPerTask : Int = w / amountOfThreads
    val forThread = Range(0, w) by colPerTask
    val tasks = forThread.map(x => {
        new Thread {
          override def run() : Unit = {
            println(x, x + colPerTask)
            BlurVertical(x, x + colPerTask, rad)
          }
        }
    }
    )
    tasks.foreach(x => x.start())
    tasks.foreach(x => x.join())
  }

  def main(args: Array[String]): Unit = {
    BlurHorPar(2, 2)
    ImageIO.write(out, "jpg", new File("test.jpg"))
  }
}

