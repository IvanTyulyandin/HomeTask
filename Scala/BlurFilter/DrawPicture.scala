/**
  * Created by ivan on 16.10.16.
  */

package BlurFilter

import swing.{Panel, MainFrame, SimpleSwingApplication}
import java.awt.{Color, Graphics2D, Dimension}
import java.awt.image.BufferedImage


class DrawPicture(img : BufferedImage) extends Panel {

  override def paintComponent(g: Graphics2D) {
    val width = img.getWidth()
    val height = img.getHeight()
    val dx = g.getClipBounds.width.toFloat / width
    val dy = g.getClipBounds.height.toFloat / height

    g.drawRenderedImage(img, java.awt.geom.AffineTransform.getScaleInstance(dx, dy))
  }
}

object before extends SimpleSwingApplication {

  def top = new MainFrame {
    contents = new DrawPicture(img) {
      preferredSize = new Dimension(w, h)
    }
  }
}

object after extends SimpleSwingApplication {

  val infoForDraw = getInfoAboutBlur()
  infoForDraw match {
    case (numOfThreads, rad, "h") => Horiz_filter.BlurHorPar(numOfThreads, rad, out)
    case (numOfThreads, rad, "v") => Vert_filter.BlurVertPar(numOfThreads, rad, out)
  }

  def top = new MainFrame {
    contents = new DrawPicture(out) {
      preferredSize = new Dimension(w, h)
    }
  }
}
