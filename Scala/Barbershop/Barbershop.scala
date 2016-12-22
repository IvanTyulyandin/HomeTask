package Barbershop

import java.io.{File, PrintWriter}

/**
  * Created by ivan on 22.12.16.
  */

class Barbershop(visitors : Int, barbers : Int, newSizeOfSofa : Int) {

  class Logger(file_name: String) {
    private val file = new PrintWriter(new File(file_name))

    def close(): Unit = {
      file.close()
    }

    def write(msg: String): Unit = {
      file.write(msg + '\n')
    }
  }

  val r = scala.util.Random

  class Visitor() {

    val me = new Thread {
      override def run() = {
        Thread.sleep(r.nextInt(3000))
        takePlace()
        howMuchWasHere += 1
      }
    }

    val myID = me.getId
    me.start()

    def takePlace() : Boolean =  {
      lock.synchronized {
        if (curAtSofa < sizeOfSofa) {
          enq(myID)
          log.write(s"Visitor $myID at sofa")
          true
        }
        else {
          log.write(s"Visitor $myID go back")
          false
        }
      }
    }

  }

  class Barber() {

    var iWorkWith : Long = -2
    var isWaiting = true
    val me = new Thread {
      override def run() = {
        while(howMuchWasHere <= numOfVisitors) {
          while (!takeVisitor()) {
          }
          makeMyWork()
          if (howMuchWasHere == numOfVisitors) {
            log.close()
            System.exit(0)
          }
        }
      }
    }

    val myID = me.getId
    me.start()

    def takeVisitor() : Boolean = {
      lock.synchronized {
        if (curAtSofa > 0) {
          iWorkWith = deq()
          log.write(s"Barber $myID take $iWorkWith")
          true
        }
        else false
      }
    }

    def makeMyWork() : Unit = {
      Thread.sleep(500)
      log.write(s"Barber $myID done $iWorkWith")
    }
  }

  var lock : AnyRef = new Object()

  val log = new Logger("log.txt")

  val numOfVisitors = visitors
  val numObBarbers = barbers
  val sizeOfSofa = newSizeOfSofa
  var howMuchWasHere = 0

  var sofa = Array.fill[Long](sizeOfSofa)(-1)
  var head = 0
  var tail = 0
  var curAtSofa = 0
  var visitorList = Array.empty[Visitor]
  var barberList = Array.empty[Barber]

  def enq(x : Long) : Unit = {
    lock.synchronized {
      sofa(head) = x
      curAtSofa += 1
      head += 1
      if (head == sizeOfSofa)
        head = 0
    }
  }

  def deq() : Long = {
    lock.synchronized {
      val res = sofa(tail)
      curAtSofa -= 1
      tail += 1
      if (tail == sizeOfSofa)
        tail = 0
      res
    }
  }

  def main() = {
    visitorList = Array.fill[Visitor](visitors)(new Visitor())
    barberList = Array.fill[Barber](barbers)(new Barber())
  }

}

object StartBarberShop {

  def main(args: Array[String]): Unit = {
    println("Enter visitors number")
    val visitors = scala.io.StdIn.readInt()
    println("Enter barbers number")
    val barbers = scala.io.StdIn.readInt()
    println("Enter sofa size")
    val size = scala.io.StdIn.readInt()

    new Barbershop(visitors, barbers, size).main()

    new CheckLog(visitors, barbers, size).main()
  }
}


