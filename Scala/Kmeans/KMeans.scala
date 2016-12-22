package KMeans
/**
  * Created by ivan on 21.12.16.
  */
import scala.collection.GenSeq, scala.annotation.tailrec, scala.util.Random, org.scalameter._

object KMeans {

  type Point = (Double, Double)

  def distance(a : Point, b : Point) : Double = {
    Math.sqrt(Math.pow(a._1 - b._1, 2) + Math.pow(a._2 - b._2, 2))
  }

  def generatePoints(num : Int, range : Int) : Seq[Point] = {
    val r = Random
     for {
        i <- 0 until num
      } yield (r.nextDouble() + r.nextInt(range), r.nextDouble() + r.nextInt(range))
  }

  def initializeMeans(k : Int, points : Seq[Point]) : Seq[Point] = {
    Random.shuffle(points).take(k)
  }

  def findClosest(p : Point, means : GenSeq[Point]) : Point = {
    means.minBy(distance(p, _))
  }

  def classify(points : GenSeq[Point], means : GenSeq[Point]) : GenSeq[(Point, GenSeq[Point])] = {
    val hlp = (-1.0, -1.0)
    val res = means.map(c => (c, points.map(
        p => if (findClosest(p, means) == c) p
             else hlp
        ).filter(_ != hlp)
      )
    )

    res
  }

  def findAverage(oldMean : Point, points : GenSeq[Point]) : Point = {
    if (points.isEmpty)
      oldMean
    else {
      val xy = points.unzip
      (xy._1.sum / points.length, xy._2.sum / points.length)
    }
  }

  def update(classified : GenSeq[(Point, GenSeq[Point])]) : GenSeq[Point] = {
    classified.map(pair => findAverage(pair._1, pair._2))
  }

  def converged(eta : Double)(oldMeans : GenSeq[Point], newMeans : GenSeq[Point]) : Boolean = {
    oldMeans.zip(newMeans).forall({
      case (old_mean, new_mean) => distance(old_mean, new_mean) < eta
    })
  }

  @tailrec
  final def kMeans(points : GenSeq[Point], means : GenSeq[Point], eta : Double) : GenSeq[Point] ={
    val newCenters = update(classify(points, means))

    if (converged(eta)(means, newCenters))
      newCenters
    else
      kMeans(points, newCenters, eta)
  }
}


object TimeMeasurement {

  type Point = (Double, Double)

  def main(args: Array[String]): Unit = {
    println("Enter num of points")
    val numOfPoints = scala.io.StdIn.readInt()
    println("Enter max range: from 0 to ...")
    val range = scala.io.StdIn.readInt()
    println("Enter num of clusters")
    val clusNum = scala.io.StdIn.readInt()
    println("Enter eta")
    val eta = scala.io.StdIn.readDouble()

    val points = KMeans.generatePoints(numOfPoints, range)
    val startClusters = KMeans.initializeMeans(clusNum, points)

    val my_config = config(
      Key.exec.minWarmupRuns -> 5,
      Key.exec.maxWarmupRuns -> 40
    ) withWarmer new Warmer.Default

    def SeqPoints(points: GenSeq[Point], means: GenSeq[Point]): Quantity[Double] = {
      val time = my_config measure {
        KMeans.kMeans(points, means, eta)
      }
      time
    }

    def ParSeqPoints(points: GenSeq[Point], means: GenSeq[Point]): Quantity[Double] = {
      val time = my_config measure {
        KMeans.kMeans(points.par, means.par, eta)
      }
      time
    }

    println("seq = ", SeqPoints(points, startClusters))
    println("parSeq = ", ParSeqPoints(points, startClusters))
  }
}
