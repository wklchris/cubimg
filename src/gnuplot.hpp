#ifndef CUBIMG_GNUPLOT_H
#define CUBIMG_GNUPLOT_H

#include <string>
#include <vector>
#include <cstdlib>
#include "cube.hpp"

namespace cubimg::Engine {

class GpEngine {
public:
    GpEngine();
    
    GpEngine(const std::string& filename, const std::string& format,  
        int w = -1, int h = -1,
        int elev = 60, int azim = 120,
        int order = 3
    );
    
    void setOutputFormat(const std::string& format);
    void setOutputFilename(const std::string& filename);
    void setImageSize(int w, int h = -1);
    void setKeepGpFile(bool keep_);
    void setView(int elev, int azim);
    void setLineWidth(double lw);
    void setCubeOrder(int order);
    
    // Load the current cube's colors of a selected set of faces for drawing.
    template<size_t N>
    void setCube(const Cube::Cube<N>& cube) {
        auto uColors = cube.getFaceColorArray(Cube::Face::Up);
        auto rColors = cube.getFaceColorArray(Cube::Face::Right);
        auto fColors = cube.getFaceColorArray(Cube::Face::Front);
        
        colorU.clear();
        colorR.clear();
        colorF.clear();
        
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                colorU.push_back(cube.getFaceColorHex(uColors[i][j]));
                colorR.push_back(cube.getFaceColorHex(rColors[i][j]));
                colorF.push_back(cube.getFaceColorHex(fColors[i][j]));
            }
        }
    }
    
    // Generate the Gnuplot code
    std::string code();
    
    // Draw the cube image by calling Gnuplot from command line
    void draw();

private:
    void initializeDefaultColors();
    // Generate the preamble for supported output file formats.
    std::string generateOutfilePreamble() const;

    std::string output_format = "svg";
    std::string output_filename = "test";
    std::string output_fullname = "test.svg";
    bool keep_file = false;

    int width = 400;
    int height = 400;
    int elevation = 60;
    int azimuth = 120;
    double linewidth = 3;
    int cube_order = 3;
    
    std::string cube_white;
    std::string cube_yellow; 
    std::string cube_red;
    std::string cube_orange;
    std::string cube_green;
    std::string cube_blue;
    // Misc colors
    std::string cube_gray;
    std::string cube_black;

    std::vector<std::string> colorU;
    std::vector<std::string> colorR;
    std::vector<std::string> colorF;
};

}

#endif // CUBIMG_GNUPLOT_H