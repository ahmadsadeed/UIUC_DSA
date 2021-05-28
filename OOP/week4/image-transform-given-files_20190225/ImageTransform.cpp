#include <iostream>
#include <cmath>
#include <cstdlib>

#include "uiuc/PNG.h"
#include "uiuc/HSLAPixel.h"
#include "ImageTransform.h"

/* ******************
(Begin multi-line comment...)

Write your name and email address in the comment space here:

Name:
Email:

(...end multi-line comment.)
******************** */

using uiuc::PNG;
using uiuc::HSLAPixel;

/**
 * Returns an image that has been transformed to grayscale.
 *
 * The saturation of every pixel is set to 0, removing any color.
 *
 * @return The grayscale image.
 */
PNG grayscale(PNG image) {
  /// This function is already written for you so you can see how to
  /// interact with our PNG class.
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);

      // `pixel` is a reference to the memory stored inside of the PNG `image`,
      // which means you're changing the image directly. No need to `set`
      // the pixel since you're directly changing the memory of the image.
      pixel.s = 0;
    }
  }

  return image;
}



/**
 * Returns an image with a spotlight centered at (`centerX`, `centerY`).
 *
 * A spotlight adjusts the luminance of a pixel based on the distance the pixel
 * is away from the center by decreasing the luminance by 0.5% per 1 pixel euclidean
 * distance away from the center.
 *
 * For example, a pixel 3 pixels above and 4 pixels to the right of the center
 * is a total of `sqrt((3 * 3) + (4 * 4)) = sqrt(25) = 5` pixels away and
 * its luminance is decreased by 2.5% (0.975x its original value).  At a
 * distance over 160 pixels away, the luminance will always decreased by 80%.
 * 
 * The modified PNG is then returned.
 *
 * @param image A PNG object which holds the image data to be modified.
 * @param centerX The center x coordinate of the crosshair which is to be drawn.
 * @param centerY The center y coordinate of the crosshair which is to be drawn.
 *
 * @return The image with a spotlight.
 */
PNG createSpotlight(PNG image, int centerX, int centerY) {
    double distance = 0.0;
    for (unsigned x = 0; x < image.width(); x++) {
        for (unsigned y = 0; y < image.height(); y++) {
            HSLAPixel & pixel = image.getPixel(x, y);
            distance = sqrt((x-centerX) * (x-centerX) + (y-centerY) * (y-centerY));
            /* For example, a pixel 3 pixels above and 4 pixels to the right of the center
             * is a total of sqrt(3*3 + 4*4) = sqrt(25) = 5 pixels away and its luminance
             * is decreased by 2.5% (0.975 its original value). At a distance over 160
             * pixels away, the luminance will always be decreased by 80% (0.2x its original value).
             * */
            if (distance >= 160) {
                pixel.l -= 0.8 * pixel.l;
            } else {
                pixel.l -= 0.005 * distance * pixel.l;
            }
        }
    }
    return image;

}
 

/**
 * Returns a image transformed to Illini colors.
 *
 * The hue of every pixel is set to the a hue value of either orange or
 * blue, based on if the pixel's hue value is closer to orange than blue.
 *
 * @param image A PNG object which holds the image data to be modified.
 *
 * @return The illinify'd image.
**/
PNG illinify(PNG image) {
    for (unsigned x = 0; x < image.width(); x++) {
        for (unsigned y = 0; y < image.height(); y++) {
            HSLAPixel & pixel = image.getPixel(x, y);
            /* To illinify an image is to transform the hue of every pixel to Illini Orange (11) or
             * Illini Blue (216). The hue of every pixel should be set to one or the other of
             * these two hue values, based on whether the pixel's original hue value is closer
             * to Illini Orange or Illini Blue
             * ( > 11 && < ((216-11)/2) ) || ( < 11 && > 216+((360-216)/2 )
             */
            double hue = pixel.h;
            if ((hue < 11) || (hue > 11 && hue < (11+((216-11)/2))) || (hue > 216+((360-216)/2))) {
                pixel.h = 11;
            } else {
                pixel.h = 216;
            }
        }
    }
    return image;
}
 

/**
* Returns an immge that has been watermarked by another image.
*
* The luminance of every pixel of the second image is checked, if that
* pixel's luminance is 1 (100%), then the pixel at the same location on
* the first image has its luminance increased by 0.2.
*
* @param firstImage  The first of the two PNGs, which is the base image.
* @param secondImage The second of the two PNGs, which acts as the stencil.
*
* @return The watermarked image.
*/
PNG watermark(PNG firstImage, PNG secondImage) {
    unsigned secWidth = secondImage.width();
    unsigned secHeight = secondImage.height();
    for (unsigned x = 0; x < firstImage.width(); x++) {
        for (unsigned y = 0; y < firstImage.height(); y++) {
            HSLAPixel & pixel = firstImage.getPixel(x, y);

            if (x < secWidth && y < secHeight) {
                HSLAPixel & secPixel = secondImage.getPixel(x, y);
                if (secPixel.l == 1.0) {
                    pixel.l += 0.2;
                    // reset to 1 if more
                    if (pixel.l > 1.0){
                        pixel.l = 1.0;
                    }
                }
            }
        }
    }
    return firstImage;
}
