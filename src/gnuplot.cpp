#include "gnuplot.hpp"
#include "color.hpp"
#include <cstdio>
#include <format>
#include <iostream>

namespace cubimg::Engine {

GpEngine::GpEngine() {
    initializeDefaultColors();
}

GpEngine::GpEngine(std::string_view filename, std::string_view format,
         int w, int h, int elev, int azim, int order)
    : output_filename(filename), output_format(format), 
      elevation(elev), azimuth(azim), cube_order(order) {
    
    output_fullname = std::format("{}.{}", output_filename, output_format);
    width = (w < 0) ? width : w;
    height = (h < 0) ? width : h;
    initializeDefaultColors();
}

void GpEngine::setOutputFormat(std::string_view format) {
    output_format = format;
    output_fullname = std::format("{}.{}", output_filename, output_format);
}

void GpEngine::setOutputFilename(std::string_view filename) {
    output_filename = filename;
    output_fullname = std::format("{}.{}", output_filename, output_format);
}

void GpEngine::setOutputDir(std::string_view directory) {
    output_dir = directory;
}

void GpEngine::setImageSize(int w, int h) {
    width = w;
    height = (h == -1) ? w : h;
}

void GpEngine::setKeepScriptFile(bool keep_) {
    keep_file = keep_;
}

void GpEngine::setView(int elev, int azim) {
    elevation = elev;
    azimuth = azim;
}

void GpEngine::setLineWidth(double lw) {
    linewidth = lw;
}

void GpEngine::setCubeOrder(int order) {
    cube_order = order;
    initializeDefaultColors();
}

std::string GpEngine::generateOutfilePreamble() const {
    // Convert file extension to lower
    std::string result;
    if (output_format == "svg") {
        result = std::format(
            "set term svg size {}, {} linewidth {}\n",
            width, height, linewidth
        );
    } else if (output_format == "png") {
        result = std::format(
            "set term png truecolor transparent size {}, {} linewidth {}\n",
            width, height, linewidth
        );
    } else if (output_format == "pdf") {
        result = std::format(
            "set term cairolatex pdf color transparent crop size 3in, 3in linewidth {}\n",
            linewidth
        );
    } else if (output_format == "tex" || output_format == "tikz") {
        result = std::format(
            "set term tikz latex color tightboundingbox size 3in, 3in linewidth {}\n",
            linewidth
        );
    } else {
        std::cerr << "Unknown file format: " << output_format << std::endl;
    }

    std::string fpath_;
    if (!output_dir.empty()) {
        fpath_ = output_dir + "/" + output_fullname;
    } else {
        fpath_ = output_fullname;
    }
    result += std::format("set output '{}'\n", fpath_);
    return result;
}

std::string GpEngine::code() {
    double block_width = 2.0;
    double vmin = -cube_order;
    double vmax = cube_order;
    double view_margin = 0.25;
    
    std::string gnuplot_code;
    
    // Set output file
    gnuplot_code += generateOutfilePreamble();
    
    // Set view angle
    gnuplot_code += std::format("set view {}, {}\n", elevation, azimuth);
    gnuplot_code += "unset key\n";
    gnuplot_code += "unset border\n";
    gnuplot_code += "unset xtics\n";
    gnuplot_code += "unset ytics\n";
    gnuplot_code += "unset ztics\n\n";
    
    // Set cube order and drawing unit
    gnuplot_code += std::format("cube_order = {}\n", cube_order);
    gnuplot_code += std::format("block_width = {}\n", block_width);
    gnuplot_code += std::format("vmin = {}; vmax = {}\n\n", vmin, vmax);
    
    // Draw the U face
    gnuplot_code += std::format("array colorU[{}] = [", cube_order * cube_order);
    for (size_t i = 0; i < colorU.size(); ++i) {
        gnuplot_code += std::format("\"{}\"", colorU[i]);
        if (i < colorU.size() - 1) {
            gnuplot_code += ", ";
        }
    }
    gnuplot_code += "]\n";

    gnuplot_code += std::format("do for [i=0:{}] {{\n", cube_order - 1);
    gnuplot_code += std::format("    do for [j=0:{}] {{\n", cube_order - 1);
    gnuplot_code += "        bx1 = vmin + i * block_width\n";
    gnuplot_code += "        bx2 = bx1 + block_width\n";
    gnuplot_code += "        by1 = vmin + j * block_width\n";
    gnuplot_code += "        by2 = by1 + block_width\n";
    gnuplot_code += "        bz = vmax\n\n";
    
    gnuplot_code += "        set object (i*" + std::to_string(cube_order) + "+j+1) polygon from bx1,by1,bz \\\n";
    gnuplot_code += "            to bx2,by1,bz \\\n";
    gnuplot_code += "            to bx2,by2,bz \\\n";
    gnuplot_code += "            to bx1,by2,bz \\\n";
    gnuplot_code += "            to bx1,by1,bz \\\n";
    gnuplot_code += "            fc rgb colorU[i*" + std::to_string(cube_order) + "+j+1] fs solid 1.0 border lc \"black\"\n";
    gnuplot_code += "    }\n";
    gnuplot_code += "}\n\n";
    
    // Draw the R face
    gnuplot_code += std::format("array colorR[{}] = [", cube_order * cube_order);
    for (size_t i = 0; i < colorR.size(); ++i) {
        gnuplot_code += std::format("\"{}\"", colorR[i]);
        if (i < colorR.size() - 1) {
            gnuplot_code += ", ";
        }
    }
    gnuplot_code += "]\n";

    gnuplot_code += std::format("do for [i=0:{}] {{\n", cube_order - 1);
    gnuplot_code += std::format("    do for [j=0:{}] {{\n", cube_order - 1);
    gnuplot_code += "        bx1 = vmax - i * block_width\n";
    gnuplot_code += "        bx2 = bx1 - block_width\n";
    gnuplot_code += "        by = vmax\n";
    gnuplot_code += "        bz1 = vmax - j * block_width\n";
    gnuplot_code += "        bz2 = bz1 - block_width\n\n";
    gnuplot_code += "        set object (i+j*" + std::to_string(cube_order) + "+101) polygon from bx1,by,bz1 \\\n";
    gnuplot_code += "            to bx2,by,bz1 \\\n";
    gnuplot_code += "            to bx2,by,bz2 \\\n";
    gnuplot_code += "            to bx1,by,bz2 \\\n";
    gnuplot_code += "            to bx1,by,bz1 \\\n";
    gnuplot_code += "            fc rgb colorR[i+j*" + std::to_string(cube_order) + "+1] fs solid 1.0 border lc \"black\"\n";
    gnuplot_code += "    }\n";
    gnuplot_code += "}\n\n";

    // Draw the F face
    gnuplot_code += std::format("array colorF[{}] = [", cube_order * cube_order);
    for (size_t i = 0; i < colorF.size(); ++i) {
        gnuplot_code += std::format("\"{}\"", colorF[i]);
        if (i < colorF.size() - 1) {
            gnuplot_code += ", ";
        }
    }
    gnuplot_code += "]\n";

    gnuplot_code += std::format("do for [i=0:{}] {{\n", cube_order - 1);
    gnuplot_code += std::format("    do for [j=0:{}] {{\n", cube_order - 1);
    gnuplot_code += "        bx = vmax\n";
    gnuplot_code += "        by1 = vmin + i * block_width\n";
    gnuplot_code += "        by2 = by1 + block_width\n";
    gnuplot_code += "        bz1 = vmax - j * block_width\n";
    gnuplot_code += "        bz2 = bz1 - block_width\n\n";
    gnuplot_code += "        set object (i+j*" + std::to_string(cube_order) + "+201) polygon from bx,by1,bz1 \\\n";
    gnuplot_code += "            to bx,by2,bz1 \\\n";
    gnuplot_code += "            to bx,by2,bz2 \\\n";
    gnuplot_code += "            to bx,by1,bz2 \\\n";
    gnuplot_code += "            to bx,by1,bz1 \\\n";
    gnuplot_code += "            fc rgb colorF[i+j*" + std::to_string(cube_order) + "+1] fs solid 1.0 border lc \"black\"\n";
    gnuplot_code += "    }\n";
    gnuplot_code += "}\n\n";

    // Set the plotting view box
    gnuplot_code += std::format("view_margin = {}\n", view_margin);
    gnuplot_code += std::format("set xrange [{}:{}]\n", 
                                vmin - view_margin, vmax + view_margin);
    gnuplot_code += std::format("set yrange [{}:{}]\n", 
                                vmin - view_margin, vmax + view_margin);
    gnuplot_code += std::format("set zrange [{}:{}]\n", 
                                vmin - view_margin, vmax + view_margin);
    
    // Splot none to output the drawn objects
    gnuplot_code += "splot 1/0\n";
    
    return gnuplot_code;
}

void GpEngine::draw() {
    std::string gnuplot_code = code();
    
    std::string temp_filename = std::format("{}_{}.gp", output_filename, output_format);
    FILE* temp_file = fopen(temp_filename.c_str(), "w");
    if (!temp_file) {
        std::cerr << "Error: Could not create temporary gnuplot script." << std::endl;
    }

    fwrite(gnuplot_code.c_str(), 1, gnuplot_code.size(), temp_file);
    fclose(temp_file);
    
    std::string command = "gnuplot " + temp_filename;
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Error: Gnuplot execution failed." << std::endl;
    } else {
        std::cout << "Saved: " << output_fullname << std::endl;
    }

    // Delete the temp Gnuplot file if don't keep
    if (!keep_file) {
        std::remove(temp_filename.c_str());
    } else {
        std::cout << "Gnuplot script saved: " << temp_filename << std::endl;
    }
}

void GpEngine::initializeDefaultColors() {
    using namespace Color;
    // Use the pre-defined color Hex codes
    cube_white = CubeColor::rgbToHex(CubeColor::white.rgba);
    cube_yellow = CubeColor::rgbToHex(CubeColor::yellow.rgba);
    cube_red = CubeColor::rgbToHex(CubeColor::red.rgba);
    cube_orange = CubeColor::rgbToHex(CubeColor::orange.rgba);
    cube_green = CubeColor::rgbToHex(CubeColor::green.rgba);
    cube_blue = CubeColor::rgbToHex(CubeColor::blue.rgba);
    cube_gray = CubeColor::rgbToHex(CubeColor::gray.rgba);
    cube_black = CubeColor::rgbToHex(CubeColor::black.rgba);
    
    int total_blocks = cube_order * cube_order;
    colorU.resize(total_blocks, cube_gray);
    colorR.resize(total_blocks, cube_gray);
    colorF.resize(total_blocks, cube_gray);
}

}