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
    
    GpEngine(std::string_view filename, std::string_view format,  
        int w = -1, int h = -1,
        int elev = 60, int azim = 120,
        int order = 3
    );
    
    void setOutputFormat(std::string_view format);
    void setOutputFilename(std::string_view filename);
    void setOutputDir(std::string_view directory);
    void setImageSize(int w, int h = -1);
    void setKeepScriptFile(bool keep_);
    void setView(int elev, int azim);
    void setLineWidth(double lw);
    
    // Load the current cube's colors of a selected set of faces for drawing.
    template<size_t N>
    void setCube(const Cube::Cube<N>& cube) {
        setCubeOrder(static_cast<int>(N));
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

    // Draw a whole predefined algorithm set. Default: drawAlgoSet<3>
    template<std::size_t N = 3, typename AlgoSetContainer>
    void drawAlgoSet(const AlgoSetContainer& algo_set) {
        cubimg::Cube::Cube<N> cube;
        for (const auto& [name, setup] : algo_set) {
            cube.resetCubeState();
            cube.applyAlgo(setup);
            setOutputFilename(name);
            setCube(cube);
            draw();
        }
    }

private:
    void initializeDefaultColors();
    void setCubeOrder(int order);
    // Generate the preamble for supported output file formats.
    std::string generateOutfilePreamble() const;

    std::string output_format = "svg";
    std::string output_filename = "test";
    std::string output_dir = "";
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