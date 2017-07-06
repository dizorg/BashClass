#include <easycc/EasyCCPro.h>
#include <iostream>
#include <bashclass/BashClass.h>
#include <bashclass/BReport.h>

int main(int argc, char *argv[]) {

    // Create easycc production mode
    std::shared_ptr<ecc::EasyCC> easyccpro = std::make_shared<ecc::EasyCC>();

    // Create a bashclass
    BashClass bashClass;

    // Set easycc in bashclass
    bashClass.setEasyCC(easyccpro);

    // Initialize semantic action handlers
    bashClass.initHandlers();

    // Configure easycc on syntax error
    easyccpro->setOnSyntaxError([&](){
        easyccpro->setSilentSemanticEvents(true);
    });

    // Define the compiler phases
    std::vector<int> phases = {
            BashClass::PHASE_CREATE,
            BashClass::PHASE_EVAL,
            BashClass::PHASE_GENERATE
    };

    // Start compiling
    for(int phase : phases) {

        // Don't generate code if semantic errors were reported
        if(phase == BashClass::PHASE_GENERATE) {
            if(BReport::getInstance().hasError()) {
                return BashClass::ERR_CODE_SEMANTIC;
            }
        }

        // Set the phase number
        easyccpro->setParsingPhase(phase);

        // Show error message on create phase only
        easyccpro->setSilentSyntaxErrorMessages(phase != BashClass::PHASE_CREATE);

        // Compile all files passed as arguments
//        for(std::string fileName : easyccdev->getInputFilesNames()) {
//            code = easyccdev->compile(fileName);
//
//            // Store compiling if a file has syntax errors
//            if(code != ecc::EasyCC::OK_CODE) {
//                return code;
//            }
//        }
    }
    return 0;
}