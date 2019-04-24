/*HEADER:
 *Course: CS106B
 *Year: 2018
 *Name: Chase Renick
 *Assignment: Two use recursion on two visual problems using a GUI
 */

#include "fractals.h"
using namespace std;

//Function Prototypes
void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order);


/*Function: Draws an equalateral sierpinski's triangle that will act as our base case in the recursive algorithm
 *Input: Takes in the window, an x coordinate, y coordinate, and how far a line should be drawn
 *Output: An equalateral triangle that takes into account the original x, y, size coordinates.
 */
void drawEqualiateral(GWindow& window, double x, double y, double size){
    double new_x = x+size;
    double halved_size = size/2.0;
    double halfed_x = x + halved_size;

    double new_y = y+sqrt(3.0)*halved_size;

    window.drawLine(x,y,new_x,y);
    window.drawLine(new_x,y,halfed_x,new_y);
    window.drawLine(halfed_x,new_y,x,y);
}

/*Function: Draws Sierpinskis triangles within base case
 *Input: GUI window, an x coordinate, a y coordinate, the size you want to increase x and y to make a line, and how many orders of triangles you want
 *Output:
 */
void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order) {
    double halved_size = size/2.0;
    double halfed_x = x + halved_size;

    if (order==0){

    }else if(order==1) {
        drawEqualiateral(window,x,y,size);
    }
    else if(x < 0 || y < 0 || size < 0 || order < 0 ){
        throw "You have passed a negative value for x,y,order, or size. Please correct this to a positive number";
    }else{
        drawSierpinskiTriangle(window, x,y,size*0.5, order-1);
        drawSierpinskiTriangle(window, halfed_x,y,size*0.5, order-1);
        drawSierpinskiTriangle(window, x+size*0.25,y+sqrt(3.0)*size/4.0,size*0.5, order-1);
    }
}


/*Function: Fills in a particular area on a map with a color based on initial x, y values
 *Input: GUI window, an x coordinate, a y coordinate, the size you want to increase x and y to make a line, and how many orders of triangles you want
 *Output: At the moment, it fills in too much based on the intial color passed.  It does not stop filling in when there are overlapping rectangles...
 */
int floodFill(GWindow& window, int x, int y, int color) {
    int color_of_your_click = window.getPixel(x,y); //green
    int check_color_left = window.getPixel(x-1,y); // green
    int check_color_right = window.getPixel(x+1,y);

    if (x<-1 || y < -1){
        throw "You have chosen a pixel outside the bounds of the graphical image capabilities. Please revise your selection.";
    } else if (x==0 || y==0){
        return 0;
    }else if(color_of_your_click == 16777215){
        return 0;
    }else {
        if (color_of_your_click!=color){
            if (color_of_your_click == 16777215){
                return 0;
            }
            else{
                window.setPixel(x,y,color);
                floodFill(window,x-1,y,color);
                floodFill(window,x+1,y,color);
                floodFill(window,x,y-1,color);
                floodFill(window,x,y+1,color);
                cout << "\n";}
        }else{
            return 0;
        }
    }
    return 0;
}

