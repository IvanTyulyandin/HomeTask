import org.scalatest.FlatSpec

class Unit_test extends FlatSpec{

  "collect phase" should "work correctly" in {
    val test : Array[Char] = new Array[Char](4)
    test(0) = 'N'
    test(1) = 'M'
    test(2) = 'C'
    test(3) = 'M'
    LongIntAdding.carry = test
    LongIntAdding.thrNum = 2
    LongIntAdding.numLen = 4

    val res : Array[Char] = new Array[Char](4)
    res(0) = 'N'
    res(1) = 'N'
    res(2) = 'C'
    res(3) = 'C'

    assert(LongIntAdding.collectPhase() sameElements res)
  }

  "distribute phase" should "work correctly" in {
    val test : Array[Char] = new Array[Char](4)
    test(0) = 'N'
    test(1) = 'M'
    test(2) = 'C'
    test(3) = 'M'
    LongIntAdding.carry = test
    LongIntAdding.thrNum = 2
    LongIntAdding.numLen = 4

    val res : Array[Char] = new Array[Char](4)
    res(0) = 'M'
    res(1) = 'N'
    res(2) = 'M'
    res(3) = 'C'

    assert(LongIntAdding.distributePhase() sameElements res)
  }

  "operator for Long Arifm" should "give correct return" in {
    assert(LongIntAdding.prefixScanOperator('C','M') == 'C')
    assert(LongIntAdding.prefixScanOperator('M','N') == 'N')
    assert(LongIntAdding.prefixScanOperator('C','N') == 'N')
  }

  "operator for LinComp" should "give correct return" in {
    assert(LinComp.prefixScanOperator((0, 0), (1, 1)) == (0, 1))
    assert(LinComp.prefixScanOperator((5, 5), (0, 0)) == (0, 0))
    assert(LinComp.prefixScanOperator((3, 3), (3, 3)) == (9, 12))
  }

  "operator for Brackets" should "give correct return" in {
    assert(Brackets.parScanOperator((1,0),(0,1)) == (0,0))
    assert(Brackets.parScanOperator((0,1),(1,0)) == (1,1))
  }
  
}
