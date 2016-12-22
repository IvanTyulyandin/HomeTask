package KMeans

import org.scalatest.FlatSpec

/**
  * Created by ivan on 22.12.16.
  */
class UnitTest extends FlatSpec{

  type Point = (Double, Double)

  "findClosest" should "give correct answer" in {
    assert(KMeans.findClosest((0,0), Seq[Point]((1,0), (2,0), (3,0))) == (1,0))
  }

  "classify" should "give correct answer" in {
    val myPoints = Seq[Point]((1,0), (2,1), (3,2), (4,2))
    val myMeans = Seq[Point]((1,1), (3.5,2))
    assert(KMeans.classify(myPoints, myMeans) == Seq(((1,1), Seq((1,0), (2,1))), ((3.5,2), Seq((3,2), (4,2)))))
  }

  "findAverage" should "give correct answer" in {
    assert(KMeans.findAverage((0,0), Seq[Point]((1,1),(3,3))) == (2,2))
  }
}
