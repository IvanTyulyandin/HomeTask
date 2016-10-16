/**
  * Created by ivan on 15.10.16.
  */

package BlurFilter

import org.scalatest.FlatSpec

class Unit_test extends FlatSpec{

  "getting RGB" should "return expected bytes" in {
    val testColor = 0x00010203
    assert(getRed(testColor) == 0x03)
    assert(getGreen(testColor) == 0x02)
    assert(getBlue(testColor) == 0x01)
  }

  "assembling color" should "return expected color from R + G + B" in {
    val r = 0xfa
    val g = 0xfb
    val b = 0xfc
    assert(getColor(r, g, b) == 0x00fcfbfa)
  }

  "blur 1 pixel" should "be blurred with checking bounds" in {
    val splitColor = Array.fill[SplitColor](2,2)(new SplitColor)

    // all pixels are red
    splitColor(0)(0).red = 0xff
    splitColor(0)(0).green = 0
    splitColor(0)(0).blue = 0
    splitColor(0)(1).red = 0xff
    splitColor(0)(1).green = 0
    splitColor(0)(1).blue = 0
    splitColor(1)(0).red = 0xff
    splitColor(1)(0).green = 0
    splitColor(1)(0).blue = 0
    splitColor(1)(1).red = 0xff
    splitColor(1)(1).green = 0
    splitColor(1)(1).blue = 0
    assert(BlurOnePixel(0, 0, 2, splitColor) == 0x000000ff)
  }

}
