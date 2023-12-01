#include "PPM.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdint>
#include <cstring>

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName){

    // Check if file is a ppm file
    if (fileName.substr(fileName.find_last_of(".") + 1) != "ppm"){
        std::cerr << "Error: File is not a ppm file!" << std::endl;
        return;
    }

    // Open file
    std::ifstream ppmFile(fileName);

    // Check if file is open
    if (!ppmFile.is_open()){
        std::cerr << "Error: File could not be opened!" << std::endl;
        return;
    }

    // read first line
    std::string line;

    // Check if file is P3
    std::getline(ppmFile, line);
    if (line != "P3"){
        std::cerr << "Error: File is not P3!" << std::endl;
        return;
    }

    // get all numbers (in data) seprated by a white space or a new line and store it in the 
    // pixel data vector, furthermore save width, height and a maximum value
    getAllNumbers_InPPM(ppmFile, m_width, m_height, m_maxColorValue);

    // shrink the datastructure to minimum
    // m_PixelData.shrink_to_fit();

    // Close file
    ppmFile.close();

}

// Destructor deletes(delete or delete[]) any memory that has been allocated
// or otherwise calls any 'shutdown' or 'destroy' routines for this deletion
// to occur.
PPM::~PPM(){
    
    // nothing to be done here - std::vector will be freed automatically

}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName) const {

    // Check if file name includes .ppm
    if (outputFileName.substr(outputFileName.find_last_of(".") + 1) != "ppm"){
        outputFileName += ".ppm";
    }

    // open file
    std::ofstream ppmFile(outputFileName);

    // Check if file is open
    if (!ppmFile.is_open()){
        std::cerr << "Error: File could not be opened!" << std::endl;
        return;
    }

    // write header
    ppmFile << "P3" << std::endl;

    // write comment
    ppmFile << "# Created by: Adam's PPM Library" << std::endl;

    // write width and height
    ppmFile << m_width << " " << m_height << std::endl;

    // write max color value
    ppmFile << m_maxColorValue << std::endl;

    // write pixel data 
    writePixelData(ppmFile);

    // close file
    ppmFile.close();

}

// Darken halves (integer division by 2) each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken(){

    unsigned int numPixelData = m_PixelData.size();
    for (unsigned int i = 0; i < numPixelData; i++){
        m_PixelData[i] = static_cast<uint8_t>(round( static_cast<int>(m_PixelData[i]) / 2 ));
    } 

}

// Lighten doubles (integer multiply by 2) each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be greater than
// 255 in a ppm.
void PPM::lighten(){
    
    unsigned int numPixelData = m_PixelData.size();
    for(unsigned int i = 0; i < numPixelData; i++){

        int result = std::min(2*static_cast<int>(m_PixelData[i]), 255);
        m_PixelData[i] = static_cast<uint8_t>(result);

    }

}

// flips the image upside down
void PPM::flipUD(){

    // alloc helper buffer
    std::vector<uint8_t> temp (this->m_PixelData.size());
    std::vector<uint8_t> data = this->m_PixelData;

    int height = this->m_height;
    int width  = this->m_width;

    for (unsigned int row = 0; row < height; row++){
        for (unsigned int column = 0; column < width; column++){

            temp[ height*width*3 - (row + 1)*width*3 + column*3 ] = data[row*width*3 + column*3];
            temp[ height*width*3 - (row + 1)*width*3 + column*3 + 1] = data[row*width*3 + column*3 + 1];
            temp[ height*width*3 - (row + 1)*width*3 + column*3 + 2] = data[row*width*3 + column*3 + 2];

        }
    }

    this->m_PixelData = temp;   // swap the vectors 

}

// flips the image upside down
void PPM::flipLR(){

    // alloc helper buffer
    std::vector<uint8_t> temp (this->m_PixelData.size());
    std::vector<uint8_t> data = this->m_PixelData;

    int height = this->m_height;
    int width  = this->m_width;

    for (unsigned int row = 0; row < height; row++){
        for (unsigned int column = 0; column < width; column++){

            temp[ (row+1)*width*3 - (column+1)*3 ] = data[row*width*3 + column*3];
            temp[ (row+1)*width*3 - (column+1)*3 +1] = data[row*width*3 + column*3+1];
            temp[ (row+1)*width*3 - (column+1)*3 +2] = data[row*width*3 + column*3+2];

        }
    }

    this->m_PixelData = temp;   // swap the vectors 

}


///////////////////////////////////////// helper functions ///////////////////////////////////////

// write the data of the ppm file
void PPM::writePixelData(std::ofstream &outputFile) const{

    unsigned int numberOfPixels = m_PixelData.size() / 3;

    // write each rgb values on 1 line
    for (unsigned int i = 0; i < numberOfPixels; i++){

        outputFile << static_cast<unsigned int>(m_PixelData[i * 3]) << " ";
        outputFile << static_cast<unsigned int>(m_PixelData[i * 3 + 1]) << " ";
        outputFile << static_cast<unsigned int>(m_PixelData[i * 3 + 2]) << std::endl;
        
    }

}

// Pushes a number in a string to a vector of unsigned chars
void PPM::pushStringNumberToVector(std::vector<char> &curNumber, unsigned int &width, 
                            unsigned int &height, unsigned int &maxValue, unsigned int &counter){

    curNumber.push_back('\0'); // end string

    // still havent recorded height and width and max value
    if (counter < 3){

        unsigned int number = static_cast<unsigned int>( std::stoi(curNumber.data()) ); 

        if (counter == 0){
            width = number;
        } else if (counter == 1){
            height = number;
        }
        else{
            maxValue = number;
        }
        counter++;  
        return;       // dont save the number to the vector
    }


    uint8_t number = static_cast<uint8_t>( std::stoi(curNumber.data()) ); // convert string to integer
    m_PixelData.push_back(number);
}

// Get all numbers in a line
void PPM::getAllNumbers_InLine(std::string &line, unsigned int &width, unsigned int &height, 
                                                unsigned int &maxValue, unsigned int &counter){
    
    std::vector<char> curNumber;

    // ignore all blank spaces
    for (unsigned int i = 0; i < line.size(); i++){
        
        // should split the line on blank spaces
        if (line[i] == ' ' && curNumber.size() != 0){

            // convert string to integer and save the data
            pushStringNumberToVector(curNumber, width, height, maxValue, counter);

            // clear the vector
            curNumber.clear();
            continue;

        }

        if (line[i] == ' '){    // just a blank space
            continue;
        }

        // save the next character
        curNumber.push_back(line[i]);

        // if the number is the last number in the line - save it
        if (i == line.size() - 1){


            // convert string to integer and save the data
            pushStringNumberToVector(curNumber, width, height, maxValue, counter);

        }

    }


}

// Splits a string by a delimiter and return a vector of unsigned chars
void PPM::getAllNumbers_InPPM(std::ifstream &ppmFile, unsigned int &width, 
                                    unsigned int &height, unsigned int &maxValue){

    std::string line;
    unsigned int counter = 0; // counter to register the width and height

    // Read line by line
    while( std::getline( ppmFile, line ) ){

        if ( line.empty() || line[0] == '#' ) continue; // skip empty lines and comments

        // Add all numbers to vector
        getAllNumbers_InLine(line, width, height, maxValue, counter);

    }

}