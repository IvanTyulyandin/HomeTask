/**
  * Created by ivan on 06.10.16.
  */
import java.io.File
import javax.imageio.ImageIO
import java.awt.image.BufferedImage

class SplitColor(){
  var red : Int = 0
  var green : Int = 0
  var blue : Int = 0
}


object Blur_filter {

  def getImg(nameOfFile : String) : BufferedImage ={
    ImageIO.read(new File(nameOfFile))
  }

  val img = getImg("me.jpg")
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
    val res = Array.ofDim[SplitColor](w, h)
    for (x <- 0 until w)
      for (y <- 0 until h){
        val pixel = src.getRGB(x, y)
        println(x, y)
        res(x)(y).red = getRed(pixel)
        res(x)(y).green = getGreen(pixel)
        res(x)(y).blue = getBlue(pixel)
      }
    res
  }

  var SplitArray = getSplitArray(img)

  def BlurOnePixel(x : Int, y : Int, rad : Int) : Int ={
    var redSum = 0
    var greenSum = 0
    var blueSum = 0
    var numOfPixels = 0
    for (i <- (x - rad) until (x + rad))
      for (j <- (y - rad) until (y + rad)){
        if ((i >= 0) && (i < h) && (j >= 0) && (j < w)) {
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
      for (x <- 0 until w)
        out.setRGB(x,y, BlurOnePixel(x,y, rad))
    out
  }

  def main(args: Array[String]): Unit = {
    ImageIO.write(BlurHorizontal(0, h, 2), "jpg", new File("test.jpg"))
  }
}

