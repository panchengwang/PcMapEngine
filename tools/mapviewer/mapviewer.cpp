#include <iostream>
#include <string>
#include "MapCanvas.h"

int main(int argc, char** argv) {

    MapCanvas canvas;
    canvas.setCanvasSize(1000, 1000);
    canvas.setEnvelope(-180, 180, -90, 90);
    canvas.setDotsPerMM(300.0 / 25.4);
    canvas.setFormat("png");

    if (!canvas.begin()) {
        std::cerr << "Failed to begin canvas." << std::endl;
        return EXIT_FAILURE;
    }

    // std::string wkt = "POINT(0 0)";
    // canvas.draw(wkt);
    // canvas.draw("POINT(10 10)");

    canvas.draw("LINESTRING(-180 -90, 180 -90, 180 90, -180 90, -180 -90)");

    canvas.draw("LINESTRING(0 0, 10 10, 180 20)");
    canvas.draw("POINT(0 0 )");
    canvas.draw("POINT(10 10)");
    canvas.draw("POINT(10 20)");
    canvas.draw("POINT(180 20)");
    canvas.draw("POINT(50 20)");
    canvas.end();

    char* data = NULL;
    size_t size = 0;
    data = canvas.imageData(size);
    if (data) {
        std::cerr << "Image data size: " << size << " bytes." << std::endl;
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i]; // Output the image data as needed.
        }
        // Here you would typically save the data to a file or process it further.
        free(data); // Assuming imageData returns dynamically allocated memory.
    }
    else {
        std::cerr << "Failed to retrieve image data." << std::endl;
    }


    return EXIT_SUCCESS;
}