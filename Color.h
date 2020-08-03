#ifndef _COLOR_H
#define _COLOR_H

class Color {
    //change special to reflectivity
    double red, green, blue, special;

    public:
        Color();

        Color(double, double, double, double);

        //methods
        double getColorRed() {
            return red;
        }

        double getColorGreen() {
            return green;
        }

        double getColorBlue() {
            return blue;
        }

        double getColorSpecial() {
            return special;
        }

        //methods
        const void* getColorRedAddr() {
            return &red;
        }

        const void* getColorGreenAddr() {
            return &green;
        }

        const void* getColorBlueAddr() {
            return &blue;
        }

        const void* getColorSpecialAddr() {
            return &special;
        }

        void setColorRed(double red) {
            this->red = red;
        }

        void setColorGreen(double green) {
            this->green = green;
        }

        void setColorBlue(double blue) {
            this->blue = blue;
        }

        void setColorSpecial(double special) {
            this->special = special;
        }

        double brightness() {
            return (red + green + blue)/3;
        }

        Color colorScalar(double scalar) {
            return Color(red*scalar, green*scalar, blue*scalar, special);
        }

        Color colorAdd(Color c) {
            return Color(red+c.getColorRed(), green+c.getColorGreen(), blue+c.getColorBlue(), special);
        }

        Color colorMultiply(Color c) {
            return Color(red*c.getColorRed(), green*c.getColorGreen(), blue*c.getColorBlue(), special);
        }

        Color colorAverage(Color c) {
            return Color((red+c.getColorRed())/2, (green+c.getColorGreen())/2, (blue+c.getColorBlue())/2, special);
        }

        Color clip() {
            double all_light = red + green + blue;
            double excess_light = all_light - 3;
            if(excess_light > 0) {
                red = red + excess_light*(red/all_light);
                green = green + excess_light*(green/all_light);
                blue = blue + excess_light*(blue/all_light);
            }

            if(red > 1) {
                red = 1;
            }

            if(green > 1) {
                green = 1;
            }

            if(blue > 1) {
                blue = 1;
            }

            if(red < 0) {
                red = 0;
            }

            if(green < 0) {
                green = 0;
            }

            if(blue < 0) {
                blue = 0;
            }

            return Color(red, green, blue, special);
        }
};

Color::Color() {
    red = 0.5;
    green = 0.5;
    blue = 0.5;
    special = 0.5;
}

Color::Color(double red, double green, double blue, double special) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->special = special;
}

#endif
