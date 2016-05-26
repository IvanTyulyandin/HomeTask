abstract class ParElem(){
  def len : Int
}
class Word(str : String) extends ParElem(){
  def len = str.length
  override def toString = "Word(" + str + ")"
}
class Space(wdh : Int) extends ParElem(){
  def len = wdh
  override def toString = "Space(" + wdh + ")"
}
abstract class Alignment
case class Right() extends Alignment
case class Left() extends Alignment
case class Middle() extends Alignment
case class Width() extends Alignment

type Line = List[ParElem]
type Paragraph = List[Line]

def toListParElem(list : List[String]) : Line ={
  def toListParElemAcc(list : List[String], acc : Line) : Line ={
    if (list.isEmpty) acc.reverse
    else toListParElemAcc(list.tail, new Word(list.head) :: acc)
  }
  toListParElemAcc(list, Nil)
}

def delGettedLine(list : Line, wtd : Line) : Line ={
  def del(list : Line, wtd : Line) : Line ={
    if (wtd.isEmpty) list
    else if (wtd.head.isInstanceOf[Space]) del(list, wtd.tail)
    else del(list.tail, wtd.tail)
  }

  del(list, wtd)
}

def convertToPar(list : Line, wdh : Int, tAlign : Alignment) : Paragraph ={

  def getMaxLenInLine(list : Line, maxLen : Int) : Int={
    if (list.isEmpty) maxLen
    else getMaxLenInLine(list.tail, math.max(maxLen, list.head.len))
  }

  def getLine(str : Line, spaceFlag : Int, haveFreeWdh : Int, acc : Line) : Line ={
    if (str.isEmpty) acc.reverse
    else {
      if (str.head.len + spaceFlag > haveFreeWdh) acc.reverse
      else getLine(str.tail, 1, haveFreeWdh - str.head.len - spaceFlag, str.head ::  new Space(spaceFlag) :: acc)
    }
  }

  def alignment(line : Line, tAlign : Alignment) : Line ={

    def cntLine(line : Line, res : Int) : Int ={
      if (line.isEmpty) res
      else cntLine(line.tail, res + line.head.len)
    }

    val lineLen = cntLine(line, 0)

    tAlign match {
      case Right() => new Space(wdh - lineLen) :: line.tail
      case Left() => (new Space(wdh - lineLen) :: line.reverse).reverse
      case Middle() => {
        val lineHlp = new Space(math.round((wdh - lineLen) / 2)) :: line.tail
        val avail = wdh - cntLine(lineHlp, 0)
        (new Space(avail) :: lineHlp.reverse).reverse
      }
    }
  }

  def convAcc(list : Line, acc : Paragraph) : Paragraph ={
    if (list.isEmpty) acc.reverse
    else {
      val hlp = alignment(getLine(list, 0, wdh, Nil), tAlign)
      val B = delGettedLine(list, hlp)
      convAcc(B, hlp :: acc)
    }
  }



  val maxLen = getMaxLenInLine(list, 0)
  if (wdh < maxLen) convertToPar(list, maxLen, tAlign)
  else convAcc(list, Nil)
}
val lst : Line = toListParElem(List("st1","st2","st3"))
val testAlign = new Middle()
convertToPar(lst, 6, testAlign)
