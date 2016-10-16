/**
  * Created by ivan on 06.10.16.
  */

import java.io.File
import javax.imageio.ImageIO
import java.awt.image.BufferedImage


package object BlurFilter {

  case class SplitColor(){
    var red : Int = 0
    var green : Int = 0
    var blue : Int = 0
    def setRed(newRed : Int) = {red = newRed}
    def setGreen(newGreen : Int) = {green = newGreen}
    def setBlue(newBlue : Int) = {blue = newBlue}
  }

  def getImg() : BufferedImage ={
    println("Enter name of file to Blur")
    val nameOfFile = scala.io.StdIn.readLine()
    ImageIO.read(new File(nameOfFile))
  }

  val img = getImg()
  val w = img.getWidth
  val h = img.getHeight

  def getInfoAboutBlur() : (Int, Int, String) ={
    println("Choose number of threads")
    val numOfThreads = scala.io.StdIn.readInt()
    println("Choose radius of Blur")
    val rad = scala.io.StdIn.readInt()
    println("Choose h or v (horizontal or vertical)")
    val blurType = scala.io.StdIn.readLine()
    (numOfThreads, rad, blurType)
  }

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

  def BlurOnePixel(x : Int, y : Int, rad : Int, SplitArray : Array[Array[SplitColor]]) : Int ={
    var redSum = 0
    var greenSum = 0
    var blueSum = 0
    var numOfPixels = 0
    for (i <- (x - rad) until (x + rad))
      for (j <- (y - rad) until (y + rad)){
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

  def start(): Unit = {
    val info = getInfoAboutBlur()
    info match {
      case (numOfThreads, rad, "h") => Horiz_filter.BlurHorPar(numOfThreads, rad, out)
      case (numOfThreads, rad, "v") => Vert_filter.BlurVertPar(numOfThreads, rad, out)
    }
    ImageIO.write(out, "jpg", new File("test.jpg"))
  }

}


object Main {
  def main(args: Array[String]): Unit = {
    BlurFilter.start()
  }
}

