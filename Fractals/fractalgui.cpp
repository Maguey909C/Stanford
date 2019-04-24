/*
 * CS 106B/X Fractal Recursion Problems
 * This instructor-provided file defines the members of a class representing the
 * overall graphical user interface (GUI) for this part of the program.
 * See fractalgui.h for the class and member declarations.
 *
 * You don't need to modify this file.
 * Your recursive functions should work with an unmodified version of this file.
 *
 * @author Marty Stepp
 * @version 2018/10/11
 * - 18au 106B initial version;
 *   added H-tree (but disabled);
 *   new Qt-based GUI;
 *   crosshairs on drag rectangle;
 *   2px-thick Sierpinski lines; etc.
 * @version 2018/01/25
 * - removed Mandelbrot set for CS 106B version
 * @version 2017/10/14
 * - modified Complex parse logic to use default on failure
 *   (addresses unusual parsing failures on some systems)
 * @version 2017/10/12
 * - initial 17au version;
 *   modified with additional fractals (Mandelbrot Set) based on cgregg 17wi/sp version
 * - added spinner to show while graphics are drawing
 * @version 2016/10/16
 * - modified for 16au with additional fractals (tree)
 * @version 2015/04/24
 * - modified for 15sp version, including exception-handling
 * @version 2014/10/13
 * - modified for 14au version, including new version of floodFill
 */

#include "fractalgui.h"
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "filelib.h"
#include "grid.h"
#include "gcolor.h"
#include "random.h"
#include "strlib.h"
#include "timer.h"
#include "fractals.h"

/*static*/ const int FractalGUI::WINDOW_WIDTH = 800;
/*static*/ const int FractalGUI::WINDOW_HEIGHT = 650;
/*static*/ const int FractalGUI::FLOOD_FILL_NUM_SHAPES = 100;
/*static*/ const int FractalGUI::FLOOD_FILL_MIN_RECT_SIZE = 20;
/*static*/ const int FractalGUI::FLOOD_FILL_MAX_RECT_SIZE = 100;
/*static*/ const int FractalGUI::DRAG_RECT_COLOR = 0xaa8822;
/*static*/ const int FractalGUI::MIN_ORDER = -10;
/*static*/ const int FractalGUI::MAX_ORDER = 50;

/*static*/ FractalGUI* FractalGUI::_instance = nullptr;

/*static*/ FractalGUI* FractalGUI::instance() {
    if (!_instance) {
        _instance = new FractalGUI();
        _instance->initialize();
    }
    return _instance;
}

FractalGUI::FractalGUI()
        : window(nullptr),
          sierpinskiButton(nullptr),
          htreeButton(nullptr),
          treeButton(nullptr),
          mandelbrotButton(nullptr),
          floodFillButton(nullptr),
          floodFillRectsButton(nullptr),
          diffButton(nullptr),
          clearButton(nullptr),
          colorChooser(nullptr),
          xField(nullptr),
          yField(nullptr),
          sizeField(nullptr),
          orderField(nullptr),
          animationBox(nullptr),
          loadingLabel(nullptr),
          statusBar(nullptr),
          dragRect(nullptr),
          dragLineH(nullptr),
          dragLineV(nullptr),
          fillMode(false),
          fillInProgress(false),
          lastComplexRange("") {
    // empty
}

FractalGUI::~FractalGUI() {
    // empty
}

void FractalGUI::clear() {
    window->clearCanvas();
    if (dragRect) {
        delete dragRect;
        dragRect = nullptr;
    }
    if (dragLineH) {
        delete dragLineH;
        dragLineH = nullptr;
    }
    if (dragLineV) {
        delete dragLineV;
        dragLineV = nullptr;
    }
    window->repaint();
}

void FractalGUI::doFloodFill(double x, double y) {
    colorChooser->setEnabled(false);
    std::string colorStr = colorChooser->getSelectedItem();
    int color = getColorMap()[colorStr];

    std::ostringstream out;
    out << "Flood fill: x=" << std::dec << x << " y=" << y
        << " color " << std::hex << std::setw(6) << std::setfill('0') << color
        << std::dec << " ...";
#ifdef LOADING_LABEL_ENABLED
    loadingLabel->setVisible(true);
#endif // LOADING_LABEL_ENABLED
    std::string statusText = out.str();
    setStatusText(statusText);
    window->setColor(0x0);   // black; drawTree should change it
    fillInProgress = true;

    Timer timmy;
    timmy.start();

    int pixelsColored = floodFill(*window, x, y, color);
    if (!window->isAutoRepaint()) {
        window->repaint();
    }

    fillInProgress = false;
    long elapsed = timmy.stop();

    statusText = integerToString(pixelsColored) + " pixels colored "
            + " (" + longToString(elapsed) + "ms)";
    setStatusText(statusText);
    loadingLabel->setVisible(false);

    colorChooser->setEnabled(true);
}

#ifdef MANDELBROT_SET_ENABLED
void FractalGUI::doMandelbrot() {
    if (!hasAllCoords()) {
        setStatusText("Error: You must set an x, y, size, and order first.", /* isError */ true);
        return;
    }
    fillMode = false;

    int x = xField->getValueAsInteger();
    int y = yField->getValueAsInteger();
    int size = sizeField->getValueAsInteger();          // TODO: size => incX/Y?
    int iterations = orderField->getValueAsInteger();   // "order" == iterations here
    std::string colorStr = colorChooser->getSelectedItem();
    int color = getColorMap()[colorStr];

    // try to read and parse complex number range
    Complex min(-2, -1);
    Complex max(1, 1);
    if (!readComplexRange(min, max)) {
        return;
    }

    std::string statusText = "Mandelbrot Set: x=" + integerToString(x)
            + " y=" + integerToString(y)
            + " size=" + integerToString(size)
            + " iterations=" + integerToString(iterations)
            + " color=" + colorStr;
    setStatusText(statusText + " ...");
#ifdef LOADING_LABEL_ENABLED
    loadingLabel->setVisible(true);
#endif // LOADING_LABEL_ENABLED
    window->setColor(0x0);   // black

    Timer timmy;
    timmy.start();

    drawMandelbrotSet(*window, x, y, size,
                      min, max,
                      iterations, color);
    if (!window->isAutoRepaint()) {
        window->repaint();
    }

    long elapsed = timmy.stop();
    statusText += " (" + longToString(elapsed) + "ms)";
    setStatusText(statusText);
    loadingLabel->setVisible(false);
}
#endif // MANDELBROT_SET_ENABLED

void FractalGUI::doSierpinski() {
    if (!hasAllCoords()) {
        setStatusText("Error: You must set an x, y, size, and order first.", /* isError */ true);
        return;
    }
    fillMode = false;
    int x = xField->getValueAsInteger();
    int y = yField->getValueAsInteger();
    int size = sizeField->getValueAsInteger();
    int order = orderField->getValueAsInteger();
    std::string statusText = "Sierpinski triangle: x=" + integerToString(x)
            + " y=" + integerToString(y)
            + " size=" + integerToString(size)
            + " order=" + integerToString(order);
    setStatusText(statusText + " ...");
#ifdef LOADING_LABEL_ENABLED
    loadingLabel->setVisible(true);
#endif // LOADING_LABEL_ENABLED
    window->setColor(0x0);   // black
    window->setLineWidth(2);

    Timer timmy;
    timmy.start();

    drawSierpinskiTriangle(*window, x, y, size, order);
    if (!window->isAutoRepaint()) {
        window->repaint();
    }

    long elapsed = timmy.stop();
    statusText += " (" + longToString(elapsed) + "ms)";
    setStatusText(statusText);
    loadingLabel->setVisible(false);
}

#ifdef TREE_ENABLED
void FractalGUI::doTree() {
    if (!hasAllCoords()) {
        setStatusText("Error: You must set an x, y, size, and order first.", /* isError */ true);
        return;
    }
    fillMode = false;
    int x = xField->getValueAsInteger();
    int y = yField->getValueAsInteger();
    int size = sizeField->getValueAsInteger();
    int order = orderField->getValueAsInteger();

    std::string statusText = "Recursive tree: x=" + integerToString(x)
            + " y=" + integerToString(y)
            + " size=" + integerToString(size)
            + " order=" + integerToString(order);
    setStatusText(statusText + " ...");
#ifdef LOADING_LABEL_ENABLED
    loadingLabel->setVisible(true);
#endif // LOADING_LABEL_ENABLED
    window->setColor(0x0);   // black; drawTree should change it

    Timer timmy;
    timmy.start();

    drawTree(*window, x, y, size, order);
    if (!window->isAutoRepaint()) {
        window->repaint();
    }

    long elapsed = timmy.stop();
    statusText += " (" + longToString(elapsed) + "ms)";
    setStatusText(statusText);
    loadingLabel->setVisible(false);
}
#endif // TREE_ENABLED

#ifdef H_TREE_ENABLED
void FractalGUI::doHTree() {
    if (!hasAllCoords()) {
        setStatusText("Error: You must set an x, y, size, and order first.", /* isError */ true);
        return;
    }
    fillMode = false;
    int x = xField->getValueAsInteger();
    int y = yField->getValueAsInteger();
    int size = sizeField->getValueAsInteger();
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int order = orderField->getValueAsInteger();

    std::string statusText = "H-tree: centerX=" + integerToString(centerX)
            + " centerY=" + integerToString(centerY)
            + " size=" + integerToString(size)
            + " order=" + integerToString(order);
    setStatusText(statusText + " ...");
#ifdef LOADING_LABEL_ENABLED
    loadingLabel->setVisible(true);
#endif // LOADING_LABEL_ENABLED
    window->setColor(0x0);   // black; drawTree should change it

    Timer timmy;
    timmy.start();

    drawHTree(*window, centerX, centerY, size, order);
    if (!window->isAutoRepaint()) {
        window->repaint();
    }

    long elapsed = timmy.stop();
    statusText += " (" + longToString(elapsed) + "ms)";
    setStatusText(statusText);
    loadingLabel->setVisible(false);
}
#endif // H_TREE_ENABLED

void FractalGUI::floodFill_addShapes() {
    // use buffered image to store individual pixels
    double canvasWidth = window->getCanvasWidth();
    double canvasHeight = window->getCanvasHeight();

    // draw several random shapes
    if (FLOOD_FILL_RANDOM_SEED > 0) {
        setRandomSeed(FLOOD_FILL_RANDOM_SEED);
    }

    Vector<std::string> colorVector = getColorMap().keys();
    for (int i = 0; i < FLOOD_FILL_NUM_SHAPES; i++) {
        double w = randomInteger(FLOOD_FILL_MIN_RECT_SIZE, FLOOD_FILL_MAX_RECT_SIZE);
        double h = randomInteger(FLOOD_FILL_MIN_RECT_SIZE, FLOOD_FILL_MAX_RECT_SIZE);
        double x = randomInteger(-w, canvasWidth);
        double y = randomInteger(-h, canvasHeight);
        int color = getColorMap()[randomElement(colorVector)];
        window->setColor(color);
        window->setFillColor(color);
        window->fillRect(x, y, w, h);
    }
    if (!window->isAutoRepaint()) {
        window->repaint();
    }
}

const Map<std::string, int>& FractalGUI::getColorMap() {
    static Map<std::string, int> colorMap;
    if (colorMap.isEmpty()) {
        colorMap["Red"]    = 0x8c1515;   // Stanford red
        colorMap["Yellow"] = 0xeeee00;   // yellow
        colorMap["Blue"]   = 0x0000cc;   // blue
        colorMap["Green"]  = 0x00cc00;   // green
        colorMap["Purple"] = 0xcc00cc;   // purple
        colorMap["Orange"] = 0xff8800;   // orange
    }
    return colorMap;
}

bool FractalGUI::hasAllCoords() {
    return xField->valueIsInt() && yField->valueIsInt()
            && sizeField->valueIsInt() && orderField->valueIsInt();
}


/*
 * Helper to create one button in the GUI.
 */
static GButton* createButton(const std::string& cmd, const std::string& icon) {
    GButton* button = new GButton();
    if (!cmd.empty()) {
        button->setText(cmd);
    }
    button->setTextPosition(GInteractor::TEXT_UNDER_ICON);
    button->setActionCommand(cmd);
    button->setTooltip(cmd);
    if (!icon.empty()) {
        button->setIcon(icon);
    }
    return button;
}

void FractalGUI::initialize() {
    GObject::setAntiAliasing(false);
    fillMode = false;
    fillInProgress = false;
    dragRect = nullptr;
    dragLineH = nullptr;
    dragLineV = nullptr;
    lastComplexRange = "(-2-1i) .. (1+1i)";

    // create window
    window = new GWindow(WINDOW_WIDTH, WINDOW_HEIGHT, /* visible */ false);
    window->setResizable(true);
    window->setTitle("CS 106B/X Fractals");
    window->setExitOnClose(true);

    window->setMouseListener([this](GEvent mouseEvent) {
        int mx = (int) mouseEvent.getX();
        int my = (int) mouseEvent.getY();
        if (mx < 0 || my < 0
                || mx >= window->getCanvasWidth()
                || my >= window->getCanvasHeight()) {
            return;
        }
        if (mouseEvent.getType() == MOUSE_MOVED) {
            if (!fillInProgress) {
                int px = window->getPixel(mx, my);
                std::string statusText = "(x=" + integerToString(mx) + ", y=" + integerToString(my)
                        + "), color=" + GColor::convertRGBToColor(px);
                setStatusText(statusText);
            }
        } else if (mouseEvent.getType() == MOUSE_PRESSED) {
            if (!mouseEvent.isLeftClick()) {
                if (dragRect) {
                    window->remove(dragRect);
                    window->remove(dragLineH);
                    window->remove(dragLineV);
                    delete dragRect;
                    delete dragLineH;
                    delete dragLineV;
                    dragRect = nullptr;
                    dragLineH = nullptr;
                    dragLineV = nullptr;
                    window->repaint();
                    return;
                }
            }

            xField->setValue(mx);
            yField->setValue(my);

            if (fillMode) {
                doFloodFill(mx, my);
            }
        } else if (mouseEvent.getEventType() == MOUSE_DRAGGED) {
            if (!fillMode) {
                int x = xField->getValueAsInteger();
                int y = yField->getValueAsInteger();
                if (mx > x && my > y) {
                    int size = std::min(mx - x, my - y);
                    sizeField->setValue(size);
                    if (dragRect) {
                        dragRect->setBounds(x, y, size, size);
                        dragLineH->setStartPoint(x, y + size / 2);
                        dragLineH->setEndPoint(x + size, y + size / 2);
                        dragLineV->setStartPoint(x + size / 2, y);
                        dragLineV->setEndPoint(x + size / 2, y + size);
                    } else {
                        dragRect = new GRect(x, y, size, size);
                        dragRect->setColor(DRAG_RECT_COLOR);
                        // dragRect->setLineWidth(2);
                        dragLineH = new GLine(x, y + size / 2,
                                              x + size, y + size / 2);
                        dragLineH->setColor(DRAG_RECT_COLOR);
                        dragLineH->setLineStyle(GLine::LINE_DASH);
                        dragLineV = new GLine(x + size / 2, y,
                                              x + size / 2, y + size);
                        dragLineV->setColor(DRAG_RECT_COLOR);
                        dragLineV->setLineStyle(GLine::LINE_DASH);
                        window->add(dragRect);
                        window->add(dragLineH);
                        window->add(dragLineV);
                    }
                }
            }
        } else if (mouseEvent.getEventType() == MOUSE_RELEASED) {
            if (!fillMode && dragRect) {
                window->remove(dragRect);
                window->remove(dragLineH);
                window->remove(dragLineV);
                delete dragRect;
                delete dragLineH;
                delete dragLineV;
                dragRect = nullptr;
                dragLineH = nullptr;
                dragLineV = nullptr;
            }
        }
    });

    // create controls
#ifdef SIERPINSKI_ENABLED
    sierpinskiButton = createButton("S.Triangle", "triangle-icon.png");
    sierpinskiButton->setActionListener([this]() {
        this->doSierpinski();
    });
#endif // SIERPINSKI_ENABLED

#ifdef H_TREE_ENABLED
    htreeButton = createButton("H Tree", "h-icon.png");
    htreeButton->setActionListener([this]() {
        this->doHTree();
    });
#endif // H_TREE_ENABLED

#ifdef TREE_ENABLED
    treeButton = createButton("Tree", "tree-icon.png");
    treeButton->setActionListener([this]() {
        this->doTree();
    });
#endif // TREE_ENABLED

#ifdef MANDELBROT_SET_ENABLED
    mandelbrotButton = createButton("M.Set", "mandelbrot-icon.png");
    mandelbrotButton->setActionListener([this]() {
        this->doMandelbrot();
    });
#endif // MANDELBROT_SET_ENABLED

#ifdef FLOOD_FILL_ENABLED
    floodFillButton = createButton("Flood Fill", "paintbrush-icon.png");
    floodFillButton->setActionListener([this]() {
        this->fillMode = true;
    });
    floodFillRectsButton = createButton("Rects", "");
    floodFillRectsButton->setActionListener([this]() {
        this->setStatusText("Flood fill: adding rectangles");
        this->floodFill_addShapes();
        this->fillMode = true;
    });
#endif // FLOOD_FILL_ENABLED

    diffButton = createButton("Diff", "diff-icon.png");
    diffButton->setActionListener([this](GEvent event) {
        if (event.isShiftKeyDown()) {
            // save current window to an image (probably for teacher only)
            std::string filename = GFileChooser::showSaveDialog();
            if (!filename.empty()) {
                if (!fileExists(filename)
                        || GOptionPane::showConfirmDialog("File " + getTail(filename) + " exists. Overwrite?")) {
                    window->saveCanvasPixels(filename);
                    setStatusText("Graphical output saved to " + getTail(filename) + ".");
                }
            }
        } else {
            // pop up DiffImage tool
            std::string filename = GFileChooser::showOpenDialog(
                        /* parent */ this->window->getWidget(),
                        "Choose image to compare to",
                        "output/");
            if (!filename.empty()) {
                this->window->compareToImage(filename);
            }
        }
    });

    clearButton = createButton("Clear", "recycle-icon.png");
    clearButton->setActionListener([this]() {
        this->clear();
        this->fillMode = false;
        this->setStatusText("Graphics cleared.");
    });

    colorChooser = new GChooser();
    colorChooser->setTooltip("Fill Color");
    colorChooser->addItems(getColorMap().keys());
    statusBar = new GLabel(" ");
    animationBox = new GCheckBox("Animate?", /* checked */ true);
    animationBox->setActionListener([this]() {
        this->window->setAutoRepaint(this->animationBox->isChecked());
    });

    // equalize width of west widgets
    Vector<GInteractor*> westInteractors {
        sierpinskiButton,
        htreeButton,
        treeButton,
        mandelbrotButton,
        floodFillButton,
        floodFillRectsButton,
        diffButton,
        clearButton,
        colorChooser};

    double maxWidth = -1;
    for (GInteractor* interactor : westInteractors) {
        if (!interactor) { continue; }
        maxWidth = std::max(maxWidth, interactor->getWidth());
    }
    for (GInteractor* interactor : westInteractors) {
        if (!interactor) { continue; }
        interactor->setWidth(maxWidth);
    }

    int screenWidth  = std::max(500, static_cast<int>(GWindow::getScreenWidth()));
    int screenHeight = std::max(500, static_cast<int>(GWindow::getScreenHeight()));
    xField = new GTextField(/* value */ 0, /* min */ 0, /* max */ screenWidth);
    xField->setMaxLength(4);
    xField->setPlaceholder("x");
    yField = new GTextField(/* value */ 0, /* min */ 0, /* max */ screenHeight);
    yField->setPlaceholder("y");
    sizeField = new GTextField(/* value */ 0, /* min */ 0, /* max */ std::max(screenWidth, screenHeight));
    sizeField->setPlaceholder("size");
    loadingLabel = new GLabel();
    loadingLabel->setIcon("progress.gif");
    loadingLabel->setEnabled(false);
    loadingLabel->setVisible(false);
    orderField = new GTextField(/* value */ 0, /* min */ MIN_ORDER, /* max */ MAX_ORDER);
    orderField->setMaxLength(2);
    orderField->setValue(1);
    orderField->setPlaceholder("order");

    // perform layout
    window->addToRegion(new GLabel("x"), "North");
    window->addToRegion(xField, "North");
    window->addToRegion(new GLabel("y"), "North");
    window->addToRegion(yField, "North");
    window->addToRegion(new GLabel("size"), "North");
    window->addToRegion(sizeField, "North");
    window->addToRegion(new GLabel("order"), "North");
    window->addToRegion(orderField, "North");
    window->addToRegion(animationBox, "North");
    window->addToRegion(loadingLabel, "North");
    loadingLabel->setVisible(false);

#ifdef SIERPINSKI_ENABLED
    window->addToRegion(sierpinskiButton, "West");
#endif // SIERPINSKI_ENABLED

#ifdef H_TREE_ENABLED
    window->addToRegion(htreeButton, "West");
#endif // H_TREE_ENABLED

#ifdef TREE_ENABLED
    window->addToRegion(treeButton, "West");
#endif // TREE_ENABLED

#ifdef MANDELBROT_SET_ENABLED
    window->addToRegion(mandelbrotButton, "West");
#endif // MANDELBROT_SET_ENABLED

#ifdef FLOOD_FILL_ENABLED
    window->addToRegion(floodFillButton, "West");
    window->addToRegion(floodFillRectsButton, "West");
    window->addToRegion(colorChooser, "West");
#endif // FLOOD_FILL_ENABLED

    window->addToRegion(diffButton, "West");
    window->addToRegion(clearButton, "West");

    window->addToRegion(statusBar, "South");

    loadingLabel->setVisible(false);

    window->pack();
    window->setWidth(WINDOW_WIDTH);
    window->center();
    window->setVisible(true);
}

#ifdef MANDELBROT_SET_ENABLED
bool FractalGUI::readComplexRange(Complex& min, Complex& max) {
    std::string complexRange = GOptionPane::showInputDialog(
                "Complex number range, in (min .. max) format?",
                /* title */ "",
                /* initial value */ lastComplexRange);
    if (complexRange.empty()) {
        return false;   // canceled
    }

    // add a bit of spacing to the complex number string;
    // this seems to help with istream parsing inconsistencies on Mac OS
    complexRange = stringReplace(complexRange, "i", " i");
    complexRange = stringReplace(complexRange, "+", " +");
    complexRange = stringReplace(complexRange, "-", " -");

    std::string throwaway;   // for ".."
    std::istringstream rangeInput(complexRange);
    if (!(rangeInput >> min >> throwaway >> max) || min.real() >= max.real() || min.imag() >= max.imag()) {
        std::string status = "Error: Invalid complex number range; using default of "
                + min.toString() + " .. " + max.toString();
        setStatusText(status, /* isError */ true);
        std::cout << status << std::endl;
        return false;
    } else {
        lastComplexRange = complexRange;
    }

    return true;
}
#endif // MANDELBROT_SET_ENABLED

void FractalGUI::setStatusText(const std::string& text, bool isError) {
    statusBar->setColor(isError ? 0xbb0000 : 0x0);
    statusBar->setText(text);
}

/*
 * Main function to run the program.
 */
int main() {
    FractalGUI::instance();
    return 0;
}
