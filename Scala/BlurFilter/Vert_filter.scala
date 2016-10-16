/**
  * Created by ivan on 14.10.16.
  */

package BlurFilter

import java.awt.image.BufferedImage

object Vert_filter {

  def BlurVertical(from : Int, to : Int, rad : Int) : BufferedImage ={
    for (x <- from until to)
      for (y <- 0 until h) {
        if ((x >= 0) && (x < w) && (y >= 0) && (y < h))
          out.setRGB(x, y, BlurOnePixel(x, y, rad, SplitArray))
      }
    out
  }

  def BlurVertPar(amountOfThreads : Int, rad : Int, dst : BufferedImage) ={
    val colPerTask : Int = w / amountOfThreads
    val forThread = Range(0, w) by colPerTask
    val tasks = forThread.map(x => {
      new Thread {
        override def run() : Unit = {
          BlurVertical(x, x + colPerTask, rad)
        }
      }
    }
    )
    tasks.foreach(x => x.start())
    tasks.foreach(x => x.join())

  }

}
