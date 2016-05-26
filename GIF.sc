package GIFReader

import javafx.animation.AnimationTimer

import scodec._
import bits._
import scodec.codecs._
import java.nio.file.{Files, Paths}
import java.io._

import scodec.bits.ByteOrdering.{BigEndian, LittleEndian}
import scala.collection.mutable.ArrayBuffer

import swing.{Panel, MainFrame, SimpleSwingApplication}
import java.awt.{Color, Graphics2D, Dimension}
import java.awt.image.BufferedImage
import java.awt.{Color, Graphics2D, Dimension}

case class GIFHeader(width: Int, height: Int)
case class LogicalScreenDescriptor(table: Int, resolution: Int, sort: Int, colorSize: Int, background: Int)
case class ImageDescriptor(left: Int, top: Int, width: Int, height: Int, local: Int, interlace: Int, res: Int,
                           sort: Int, colorSize: Int)
case class Graphics(delay: Int, tColor: Int)
case class PIC(images: List[Int], imageDescriptor: ImageDescriptor, graphics: Graphics)

class DataPanel(data: Array[Array[Color]], BG: String) extends Panel 
  override def paintComponent(g: Graphics2D) {
    val width = data.length
    val height = data.map(_.length).max
    val dx = g.getClipBounds.width.toFloat  / width
    val dy = g.getClipBounds.height.toFloat / height
    for {
      x <- 0 until data.length
      y <- 0 until data(x).length
      x1 = (x * dx).toInt
      y1 = (y * dy).toInt
      x2 = ((x + 1) * dx).toInt
      y2 = ((y + 1) * dy).toInt
    } {
      data(x)(y) match {
        case c: Color => g.setColor(c)
        case _ => g.setColor(Color.decode(BG))
      }
      g.fillRect(x1, y1, x2 - x1, y2 - y1)
    }
  }
}

object Timer {
  def apply(interval: Int, repeats: Boolean = true)(op: => Unit) {
    val timeOut = new javax.swing.AbstractAction() {
      def actionPerformed(e : java.awt.event.ActionEvent) = op
    }
    val t = new javax.swing.Timer(interval, timeOut)
    t.setRepeats(repeats)
    t.start()
  }
}

object render extends SimpleSwingApplication {
  val file = new Decoder("anim.gif")
  val width = file.header.width
  val height = file.header.height
  val scale = 100
  val data = Array.ofDim[Color](width, height)
  val pic = file.GIF
  
  def top = new MainFrame {
    var i = 0
    Timer(pic(i).graphics.delay) {
      if (i == pic.length) i = 0
      tick(i)
      i = i + 1
    }
    def tick(i: Int) = {
      val data = Array.ofDim[Color](width, height)
      for {
        x <- 0 until data.length
        y <- 0 until data(x).length
      }
        if (x < pic(i).imageDescriptor.left || y < pic(i).imageDescriptor.top) data(x)(y) = new Color(0, 0, 0, 0)
        else data(x + pic(i).imageDescriptor.left)(y + pic(i).imageDescriptor.top) = Color.decode(file.colorArray(i)(y)(x))
      contents = new DataPanel(data, Color.WHITE.toString)
      preferredSize = new Dimension(width * scale, height * scale)
    }
  }
}

class Decoder(path: String) {

  def toColorArray (l: List[Int], len: Int):Array[Array[String]] = {
    def loop(ls: List[Int]): Array[String] = {
      val st = new Array[String](len)
      for (i <- 0 until len) {
        st(i) = "#" + colorTable(ls(i))
        print(st(i) + " ")
      }
      st
    }
    def forall (Ls: List[Int], t: ArrayBuffer[Array[String]]): ArrayBuffer[Array[String]] = {
      print("\n")
      if (Ls.isEmpty) t
      else forall(Ls.drop(len), t += loop(Ls))
    }
    forall(l, ArrayBuffer[Array[String]]()).toArray
  }

  def lzwPrepare(bitVector: BitVector): BitVector = {
    if (!bitVector.isEmpty) {
      bitVector.take(8).reverse ++ lzwPrepare(bitVector.drop(8))
    }
    else bitVector
  }

  def lzw(dic0: Map[Int, List[Int]], minSize: Int, bVector: BitVector, res: List[Int]): List[List[Int]] = {
    def lzwEncode(dic: Map[Int, List[Int]], size: Int, bVector: BitVector, res: List[Int], flag: Boolean): List[List[Int]] = {
      var newSize = size
      if (bVector.isEmpty) Nil
      else {
        if (dic.size == math.pow(2, size)) newSize = size + 1
        val word = bVector.take(newSize).reverseBitOrder.toInt(false, LittleEndian)
        if (word == numColors + 1) return Nil //end
        if (word == numColors) lzw(dic, newSize, bVector.drop(newSize), res) //clear
        else dic.get(word) match {
          case Some(output) => {
            if (flag) output :: lzwEncode(dic, newSize, bVector.drop(newSize), output, false) //not allow adding the first element to the dict.
            else output :: lzwEncode(dic + (dic.size -> (res :+ output.head)), newSize, bVector.drop(newSize), output, false)
          }
          case None => {
            val text = res :+ res.head
            text :: lzwEncode(dic + (dic.size -> text), newSize, bVector.drop(newSize), text, false)
          }
        }
      }
    }
    lzwEncode(dic0, minSize, bVector, res, true)
  }

  def cutBlock() = {
    bitVector = bitVector.drop(8)
    val size = uint8.decode(bitVector).require.value
    bitVector = bitVector.drop(size*8 + 16)
  }

  def getTable(r: Int, c: Int, vector: BitVector): List[String] = {//c = numColors or size of table
    if (r == 0) Nil
    else vector.take(c).toHex :: getTable(r - 1, c, vector.drop(c))
  }

  val byteArray = Files.readAllBytes(Paths.get(path))
  var bitVector = BitVector(byteArray)
  
  val headerCodec = (uint16L :: uint16L).as[GIFHeader]
  val screenCodec = (uintL(1) :: uintL(3) :: uintL(1) :: uintL(3) :: uintL(8)).as[LogicalScreenDescriptor]
  val imageCodec = (uint16L :: uint16L :: uint16L :: uint16L :: uintL(1) :: uintL(1) ::
    uintL(1) :: uintL(2) :: uintL(3)).as[ImageDescriptor]
  val graphicCodec = (uint16L :: uint8L).as[Graphics]
  //Header
  val header = headerCodec.decode(bitVector.drop(48)).require.value //bitVector(48) - header Gif87 or 89a
  println(header)
  bitVector = bitVector.drop(80)
  //Logical Screen Descriptor
  val Screen = screenCodec.decode(bitVector).require.value
  println(Screen)
  bitVector = bitVector.drop(24)
  //screen(1+3+1+3+8) + ignored ratio(8)
  //Global Color Table
  val numColors = math.pow(2, Screen.colorSize + 1).toInt
  val tableSize = 3 * numColors * 8
  val colorTable = getTable(numColors, 24, bitVector) //global table
  bitVector = bitVector.drop(24 * numColors)
  val listColor = ((0 to numColors - 1) :+ (numColors) :+ (numColors + 1) zip (0 to numColors + 1).map(x => List(x))).toMap //reservation clear and end by List(smth)
  def readBlocks(images: List[Int], imageDescriptors: ImageDescriptor, graphics: Graphics): List[PIC] = {
    val byte = bitVector.take(8)
    if ((byte == hex"3b".bits) && (bitVector.drop(8).isEmpty)) Nil
    else {
      bitVector = bitVector.drop(8)
      if ((byte == hex"2c".bits) && (bitVector.take(16) == hex"0000".bits)) {
        val imageDecoder = imageCodec.decode(bitVector).require.value
        bitVector = bitVector.drop(72)
        val minLZW = uint8L.decode(bitVector).require.value + 1
        if (bitVector.isEmpty) Nil
        else {
          bitVector = bitVector.drop(8)
          val size = uint8L.decode(bitVector).require.value
          bitVector = bitVector.drop(8)
          val pic = lzw(listColor, minLZW, lzwPrepare(bitVector.take(size * 8)),
            List(bitVector.take(minLZW).drop(8).toInt(false, LittleEndian))).flatten
          bitVector = bitVector.drop(size * 8 + 8)
          PIC(pic, imageDecoder, graphics) :: readBlocks(Nil, imageDescriptors, graphics)
        }
      } else if (byte == hex"21".bits) {
        val byte0 = bitVector.take(8)
        if (byte0 == hex"f9".bits) {
          val graphic = graphicCodec.decode(bitVector).require.value
          cutBlock()
          readBlocks(images, imageDescriptors, graphic)
        } else if ((byte == hex"01") || (byte ==  hex"fe") || (byte == hex"ff")){//text, comment or application block
          cutBlock()
          readBlocks(images, imageDescriptors, graphics)
        } else readBlocks(images, imageDescriptors, graphics)
      } else {
        val slice = bitVector.toHex.indexOfSlice("21")
        bitVector = bitVector.drop(slice - slice % 8)
        readBlocks(images, imageDescriptors, graphics)
      }
    }
  }
  val GIF = readBlocks(Nil, null, Graphics(1000, 0))
  println(GIF)
  val colorArray = GIF.map(x => toColorArray(x.images, x.imageDescriptor.width))
}
