/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * URL: https://github.com/martinruenz/gnuplot-cpp
 * AUTHOR: Martin Rünz, 2015
 */

#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class GnuplotPipe {
public:
    inline GnuplotPipe(bool persist = true) {
        std::cout << "Opening gnuplot... ";
        pipe = popen(persist ? "gnuplot -persist" : "gnuplot", "w");
        if (!pipe)
            std::cout << "failed!" << std::endl;
        else
            std::cout << "succeded." << std::endl;
    }
    inline virtual ~GnuplotPipe(){
        if (pipe) pclose(pipe);
    }

    void sendLine(const std::string& text, bool useBuffer = false){
        if (!pipe) return;
        if (useBuffer)
            buffer.push_back(text + "\n");
        else
            fputs((text + "\n").c_str(), pipe);
    }
    void sendEndOfData(unsigned repeatBuffer = 1){
        if (!pipe) return;
        for (unsigned i = 0; i < repeatBuffer; i++) {
            for (auto& line : buffer) fputs(line.c_str(), pipe);
            fputs("e\n", pipe);
        }
        fflush(pipe);
        buffer.clear();
    }
    void sendNewDataBlock(){
        sendLine("\n", !buffer.empty());
    }

    void writeBufferToFile(const std::string& fileName){
        std::ofstream fileOut(fileName);
        for (auto& line : buffer) fileOut << line;
        fileOut.close();
    }

private:
    GnuplotPipe(GnuplotPipe const&) = delete;
    void operator=(GnuplotPipe const&) = delete;

    FILE* pipe;
    std::vector<std::string> buffer;
};
