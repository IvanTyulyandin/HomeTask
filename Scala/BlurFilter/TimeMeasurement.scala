/**
  * Created by ivan on 14.10.16.
  */

package BlurFilter

import java.awt.image.BufferedImage
import org.scalameter._

object TimeMeasurement extends Bench.LocalTime{

  val myConfig = config(
    Key.exec.minWarmupRuns -> 5,
    Key.exec.maxWarmupRuns -> 10
  ) withWarmer new Warmer.Default

  def get_time_horiz_filter(amountOfThreads : Int, rad : Int, dst : BufferedImage) : Quantity[Double] ={
    val time = myConfig measure{
      Horiz_filter.BlurHorPar(amountOfThreads, rad, dst)
    }
    time
  }

  def get_time_vert_filter(amountOfThreads : Int, rad : Int, dst : BufferedImage) : Quantity[Double] ={
    val time = myConfig measure{
      Vert_filter.BlurVertPar(amountOfThreads, rad, dst)
    }
   time
  }
}

object getTime {
  def main(args: Array[String]): Unit = {
    println("Enter data for time measure")
    val infoForTime = getInfoAboutBlur()
    infoForTime match {
      case (numOfThreads, rad, "h") => println(TimeMeasurement.get_time_horiz_filter(numOfThreads, rad, out))
      case (numOfThreads, rad, "v") => println(TimeMeasurement.get_time_vert_filter(numOfThreads, rad, out))
    }
  }
}
