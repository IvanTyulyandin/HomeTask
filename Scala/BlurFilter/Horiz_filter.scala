/**
  * Created by ivan on 14.10.16.
  */

package BlurFilter

import java.awt.image.BufferedImage

object Horiz_filter {

  def BlurHorizontal(from : Int, to : Int, rad : Int) : BufferedImage ={
    for (y <- from until to)
      for (x <- 0 until w) {
        if ((x >= 0) && (x < w) && (y >= 0) && (y < h))
          out.setRGB(x, y, BlurOnePixel(x, y, rad, SplitArray))
      }
    out
  }

  def BlurHorPar(amountOfThreads : Int, rad : Int, dst : BufferedImage) ={
    val rowPerTask = h / amountOfThreads
    val forThread = Range(0, h) by rowPerTask
    val tasks = forThread.map(x => {
      new Thread {
        override def run() : Unit = {
          BlurHorizontal(x, x + rowPerTask, rad)
        }
      }
    }
    )
    tasks.foreach(x => x.start())
    tasks.foreach(x => x.join())

  }
}
