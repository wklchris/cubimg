#ifndef CUBIMG_GNUPLOT_H
#define CUBIMG_GNUPLOT_H

#include <string>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include "cube.hpp"
#include "algoset.hpp"

namespace cubimg::Engine {

class GpEngine {
public:
    GpEngine();
    
    GpEngine(std::string_view filename, std::string_view format,  
        int w = -1, int h = -1,
        int elev = 60, int azim = 120,
        int order = 3
    );
    
    void setEnginePath(std::filesystem::path fpath);
    void setOutputFormat(std::string_view format);
    void setOutputFilename(std::string_view filename);
    void setOutputDir(std::string_view directory);
    void setImageSize(int w, int h = -1);
    void setKeepScriptFile(bool keep_);
    void setView(int elev, int azim);
    void setReflectionDistance(double reflect_dist);
    void setReflection(std::string_view reflect_str);
    void setLineWidth(double lw);
    
    // Load the current cube's colors of a selected set of faces for drawing.
    template<size_t N>
    void setCube(const Cube::Cube<N>& cube) {
        setCubeOrder(static_cast<int>(N));
        auto dColors = cube.getFaceColorArray(Cube::Face::Down);
        auto lColors = cube.getFaceColorArray(Cube::Face::Left);
        auto bColors = cube.getFaceColorArray(Cube::Face::Back);
        auto uColors = cube.getFaceColorArray(Cube::Face::Up);
        auto rColors = cube.getFaceColorArray(Cube::Face::Right);
        auto fColors = cube.getFaceColorArray(Cube::Face::Front);
        
        colorD.clear();
        colorL.clear();
        colorB.clear();
        colorU.clear();
        colorR.clear();
        colorF.clear();
        
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                colorD.push_back(cube.getFaceColorHex(dColors[i][j]));
                colorL.push_back(cube.getFaceColorHex(lColors[i][j]));
                colorB.push_back(cube.getFaceColorHex(bColors[i][j]));
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

    // Draw a whole predefined algorithm set. Default: drawAlgoSet<3>.
    // It will first rotate the cube with the normal `setup` then the REVERSED `algo`.
    template<std::size_t N = 3, typename AlgosContainer>
    void drawAlgoSet(const AlgosContainer& algo_set) {
        static_assert(
            std::is_same_v<typename AlgosContainer::value_type, cubimg::ALGOSET::Algo>,
            "AlgosContainer must contain elements of type ALGOSET::Algo."
        );

        cubimg::Cube::Cube<N> cube;
        for (const auto& algo_item : algo_set) {
            cube.resetCubeState();
            cube.applyAlgo(algo_item.setup);
            cube.applyAlgoReverse(algo_item.algo);
            setOutputFilename(algo_item.name);
            setCube(cube);
            draw();
        }
    }

private:
    void initializeDefaultColors();
    void setCubeOrder(int order);
    // Generate the preamble for supported output file formats.
    std::string generateOutfilePreamble() const;

    std::string engine_path = "gnuplot";
    std::string output_format = "svg";
    std::string output_filename = "test";
    std::string output_dir = "";
    std::string output_fullname = "test.svg";
    bool keep_file = false;
    
    double reflection_distance = 2.25;
    bool show_reflection_D = false;
    bool show_reflection_L = false;
    bool show_reflection_B = false;

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

    std::vector<std::string> colorD;
    std::vector<std::string> colorL;
    std::vector<std::string> colorB;
    std::vector<std::string> colorU;
    std::vector<std::string> colorR;
    std::vector<std::string> colorF;
};

}

#endif // CUBIMG_GNUPLOT_H