#include <bashclass/BGenerateCode.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

void BGenerateCode::writePreCode(){
    m_bashFile << "#!/bin/bash" << std::endl
              << "#" << std::endl
              << "# Bash version 4.4+ required!" << std::endl
              << "# This code is auto generated by bashc, do not make manual changes" << std::endl
              << "# -----------------------------------------------------------------" << std::endl;
}

void BGenerateCode::writePostCode() {
    m_bashFile << "# -----------------------------------------------------------------" << std::endl;
}

void BGenerateCode::write(std::stringstream &code) {
    m_bashFile << code.str();
}

void BGenerateCode::openFile(std::string name) {
    m_bashFile.open(name);
    chmod(name.c_str(), S_IRWXU);
}

void BGenerateCode::closeFile() {
    m_bashFile.close();
}