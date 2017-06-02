#include <bashclass/BGenerateCode.h>
#include <iostream>

void BGenerateCode::writePreCode(){
    std::cout << "#!/bin/bash" << std::endl
              << "#" << std::endl
              << "# Bash version 4+ required!" << std::endl
              << "# This code is auto generated by bashc, do not make manual changes" << std::endl
              << "# -----------------------------------------------------------------" << std::endl;
}

void BGenerateCode::writePostCode() {
    std::cout << "# -----------------------------------------------------------------" << std::endl;
}

void BGenerateCode::write(std::stringstream &code) {
    std::cout << code.str();
}