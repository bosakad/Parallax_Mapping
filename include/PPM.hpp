/** @file PPM.hpp
 *  @brief Class for working with PPM images
 *  
 *  Class for working with P3 PPM images specifically.
 *
 *  @author Adam Bosak
 *  @bug No known bugs.
 */
#ifndef PPM_HPP
#define PPM_HPP

#include <cstdint>
#include <string>
#include <vector>

class PPM{
public:
    // Constructor loads a filename with the .ppm extension
    PPM(std::string fileName);
    // Destructor clears any memory that has been allocated
    ~PPM();
    // Saves a PPM Image to a new file.
    void savePPM(std::string outputFileName) const;
    // Darken halves (integer division by 2) each of the red, green
    // and blue color components of all of the pixels
    // in the PPM. Note that no values may be less than
    // 0 in a ppm.
    void darken();
    // Lighten doubles (integer multiply by 2) each of the red, green
    // and blue color components of all of the pixels
    // in the PPM. Note that no values may be greater than
    // 255 in a ppm.
    void lighten();
    
    // flips the image upside down - since textures are read from left bottom (ppm from left top)
    void flipUD();

    // flips the image left to right
    void flipLR();

    // Returns the raw pixel data in an array.
    // You may research what 'inline' does.
    // You may also want to research what 'const' does on a function.
    // In brief, 'const' gaureentees that we are not modifying 
    // any member variables in a class, and this is useful if we are
    // returning private member variables.
    inline std::vector<uint8_t> pixelData() const { return m_PixelData; }
    // Returns image width
    inline int getWidth() const { return m_width; }
    // Returns image height
    inline int getHeight() const { return m_height; }
// NOTE:    You may add any helper functions you like in the
//          private section.

private:

void writePixelData(std::ofstream &outputFile) const;

void getAllNumbers_InPPM(std::ifstream &ppmFile, unsigned int &width, 
                            unsigned int &height, unsigned int &maxValue);

void pushStringNumberToVector(std::vector<char> &curNumber, 
        unsigned int &width, unsigned int &height, unsigned int &maxValue, unsigned int &counter);

void getAllNumbers_InLine(std::string &line, unsigned int &width, 
                            unsigned int &height, unsigned int &maxValue, unsigned int &counter);

private:    
    // Store the raw pixel data here
    // Data is R,G,B format
    // Note: Yes, you are allowed to replace 'uint8_t* m_PixelDatal' with a std::vector<uint8_t> m_PixelData.
    //       In fact, using a std::vector will likely make your life easier.    
    std::vector<uint8_t> m_PixelData;
    // Store width and height of image.
    unsigned int m_width{0};
    unsigned int m_height{0};
    unsigned int m_maxColorValue{0};
};


#endif
