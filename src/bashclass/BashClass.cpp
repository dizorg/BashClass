#include <bashclass/BashClass.h>
#include <bashclass/BTypes.h>
#include <bashclass/BClass.h>
#include <iostream>

BashClass::BashClass() {
    m_global = std::make_shared<BGlobal>();
    m_scopeStack.push_back(m_global);
    initHandlers();
}

void BashClass::printStructure() {
    std::cout << m_global->getStructure().str() << std::endl;
}

/**
 * Link variable types in the provided scope
 */
void _linkVariablesTypes(std::shared_ptr<BScope> scope, std::shared_ptr<BScope> globalScope) {
    // Evaluate all variables in that scope
    for(auto variable : scope->findAllVariables()) {
        if(!BType::isBuiltInType(variable->getType())) {
            // Find class scope of that type
            auto cls = globalScope->findAllClasses(variable->getType().c_str());
            if(cls.empty()) {
                std::cerr << "Undefined type " << variable->getType() <<
                " for variable " << variable->getName() << std::endl;
            } else {
                variable->setTypeScope(cls.front());
            }
        }
    }
}

/**
 * Link functions types in the provided scope
 */
void _linkFunctionsTypes(std::shared_ptr<BScope> scope, std::shared_ptr<BScope> globalScope) {
    // Evaluate all functions in that scope
    for(auto function : scope->findAllFunctions()) {
        auto castFunction = std::dynamic_pointer_cast<BFunction>(function);
        if(!BType::isBuiltInType(castFunction->getType())) {
            // Find class scope of that type
            auto cls = globalScope->findAllClasses(castFunction->getType().c_str());
            if(cls.empty()) {
                std::cerr << "Undefined type " << castFunction->getType() <<
                " for function " << castFunction->getName() << std::endl;
            } else {
                castFunction->setTypeScope(cls.front());
            }
        }
    }
}

/**
 * Run a depth-first traversal and link all the variables
 * to their types
 */
void _linkTypes(std::shared_ptr<BScope> globalScope) {

    std::stack<std::shared_ptr<BScope>> scopeStack;
    scopeStack.push(globalScope);
    while(!scopeStack.empty()) {

        auto top = scopeStack.top();
        scopeStack.pop();

        _linkVariablesTypes(top,globalScope);
        _linkFunctionsTypes(top,globalScope);

        for(auto scope : top->getAllScopes()) {
            scopeStack.push(scope);
        }
    }
}

/**
 * Run a depth-first traversal on variable types and detect
 * all circular references
 */
void _detectCircularReference(std::shared_ptr<BScope> scope) {
    for(auto cls : scope->findAllClasses()) {
        std::stack<std::shared_ptr<BScope>> scopeStack;
        scopeStack.push(cls);
        std::set<std::shared_ptr<BScope>> visited;
        while(!scopeStack.empty()) {

            // Get top class scope
            auto top = scopeStack.top();
            scopeStack.pop();

            // Mark class as visited
            visited.insert(top);

            // Evaluate all variables in the class scope
            for(auto variable : top->findAllVariables()) {
                if(variable->getTypeScope()) {
                    if(visited.find(variable->getTypeScope()) == visited.end()) {
                        scopeStack.push(variable->getTypeScope());
                    } else {
                        auto castClass = std::dynamic_pointer_cast<BClass>(cls);
                        std::cerr << "Circular reference detected from class " << castClass->getName() << std::endl;
                    }
                }
            }
        }

    }
}

/**
 * Check if class was already defined
 */
void _checkDuplicateClass(ecc::LexicalToken &token, std::vector<std::shared_ptr<BScope>> &scopeStack) {
    auto getClass = scopeStack.back()->getParentScope()->findAllClasses(token.getValue().c_str());
    if(!getClass.empty()) {
        std::cerr << "Class '" << token.getValue() << "' at line " << token.getLine()
        << " was defined previously" << std::endl;
    }
}

/**
 * Check if function was already defined in the scope
 */
void _checkDuplicateFunction(ecc::LexicalToken &token, std::vector<std::shared_ptr<BScope>> &scopeStack) {
    auto getFunc = scopeStack.back()->getParentScope()->findAllFunctions(token.getValue().c_str());
    if(!getFunc.empty()) {
        std::cerr << "Function " << token.getValue() << " at line " << token.getLine()
        <<" was defined previously in the same scope" << std::endl;
    }
}

/**
 * Check if variable was already defined in the scope
 */
void _checkDuplicateVariable(ecc::LexicalToken &token, std::vector<std::shared_ptr<BScope>> &scopeStack) {
    auto getVar = scopeStack.back()->findAllVariables(token.getValue().c_str());
    if(!getVar.empty()) {
        std::cerr << "Variable '" << token.getValue() << "' at line " << token.getLine()
        << " was defined previously in the same scope" << std::endl;
    }
}

/**
 * Make sure that functions, members of classes, have at least one param
 */
void _requiredParam(std::shared_ptr<BScope> functionScope) {

    // Check if function is a member of a class
    auto castClass = std::dynamic_pointer_cast<BClass>(functionScope->getParentScope());
    if(castClass) {
        auto params = functionScope->findAllParameters();
        if(params.empty() || params[0]->getTypeScope() != castClass) {
            auto castFunction = std::dynamic_pointer_cast<BFunction>(functionScope);
            std::cerr << "Function " << castFunction->getName() << " in class " << castClass->getName()
            <<" must have the first argument of type " << castClass->getName() << std::endl;
        }
    }
}

void BashClass::initHandlers() {

    /**************************************
     *          PROGRAM
     **************************************/
    m_start = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            _linkTypes(m_global);
            _detectCircularReference(m_global);
        }
    };

    m_end = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){};

    /**************************************
     *          CLASSES
     **************************************/
    m_startClass = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newClass = m_scopeStack.back()->createClass(lexicalVector[index]);
            m_scopeStack.push_back(newClass);
        } else if(phase == BashClass::PHASE_EVAL) {
            auto newClass = m_scopeStack.back()->getScope(lexicalVector[index]);
            m_scopeStack.push_back(newClass);
        }
    };

    m_className = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Check if class already exists
            _checkDuplicateClass(*lexicalVector[index], m_scopeStack);

            // Set class name
            auto createdClass = std::dynamic_pointer_cast<BClass>(m_scopeStack.back());
            createdClass->setName(lexicalVector[index]->getValue());
        }
    };

    m_endClass = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL) {
            m_scopeStack.pop_back();
        }
    };

    /**************************************
     *          FUNCTIONS DECLARATION
     **************************************/
    m_startFunction = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newFunction = m_scopeStack.back()->createFunction(lexicalVector[index]);
            m_scopeStack.push_back(newFunction);
        } else if(phase == BashClass::PHASE_EVAL) {
            auto newFunction = m_scopeStack.back()->getScope(lexicalVector[index]);
            m_scopeStack.push_back(newFunction);
        }
    };

    m_functionType = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto createdFunction = std::dynamic_pointer_cast<BFunction>(m_scopeStack.back());
            createdFunction->setType(lexicalVector[index]->getValue());
        }
    };

    m_functionName = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Check if function already exists in the current scope
            _checkDuplicateFunction(*lexicalVector[index],m_scopeStack);

            auto createdFunction = std::dynamic_pointer_cast<BFunction>(m_scopeStack.back());
            createdFunction->setName(lexicalVector[index]->getValue());
        }
    };

    m_endFunction = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_EVAL) {
            _requiredParam(m_scopeStack.back());
            m_scopeStack.pop_back();
        }
    };

    /**************************************
     *          VARIABLE AND FUNCTION CALL
     **************************************/

    m_startCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_callableStack.clear();
        }
    };

    m_varCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            // If first variable in the chain, search for it
            if(m_callableStack.empty()) {
                auto variable = m_scopeStack.back()->findClosestVariable(lexicalVector[index]->getValue());
                if(variable) {
                    m_callableStack.push_back(variable);
                } else {
                    std::cerr << "Undefined variable " << lexicalVector[index]->getValue()
                    << " at line " << lexicalVector[index]->getLine() << " and column "
                    << lexicalVector[index]->getColumn() << std::endl;
                    m_callableStack.push_back(nullptr);
                }
            } else {
                if(!m_callableStack.back()) {
                    std::cerr << "Cannot access member " << lexicalVector[index]->getValue()
                    << " of undefined at line " << lexicalVector[index]->getLine() << " and column "
                    << lexicalVector[index]->getColumn() << std::endl;
                } else {
                    auto castPrevVariable = std::dynamic_pointer_cast<BVariable>(m_callableStack.back());
                    auto castPrevFunction = std::dynamic_pointer_cast<BFunction>(m_callableStack.back());
                    if(castPrevVariable) {
                        auto typeScope = castPrevVariable->getTypeScope();
                        if(typeScope) {
                            auto variables = typeScope->findAllVariables(lexicalVector[index]->getValue().c_str());
                            if(!variables.empty()) {
                                m_callableStack.push_back(variables.front());
                            } else {
                                auto castClass = std::dynamic_pointer_cast<BClass>(typeScope);
                                std::cerr << "Class " << castClass->getName() << " does not have "
                                        "member " << lexicalVector[index]->getValue() << " at line "
                                << lexicalVector[index]->getLine() << " and column "
                                << lexicalVector[index]->getColumn() << std::endl;
                                m_callableStack.push_back(nullptr);
                            }
                        } else {
                            std::cerr << "Member " << lexicalVector[index]->getValue() << " does not exist. "
                            << "Previous element does not have any known members." << std::endl;
                            m_callableStack.push_back(nullptr);
                        }
                    } else if(castPrevFunction) {
                        // TODO Grammar right now does not allow that, but to be done later
                        // FIXME DO THIS NOW
                    }
                }
            }
        }
    };

    m_functionCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){

    };

    m_endCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){

    };

    /**************************************
     *          VARIABLES DECLARATION
     **************************************/
    m_startVar = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto variable = m_scopeStack.back()->createVariable(lexicalVector[index]);
            m_focusVariable = variable;
        }
    };

    m_varType = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable->setType(lexicalVector[index]->getValue());
        }
    };

    m_varName = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Check if variable already exists in the current scope
            _checkDuplicateVariable(*lexicalVector[index], m_scopeStack);

            m_focusVariable->setName(lexicalVector[index]->getValue());
        }
    };

    m_endVar = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable = nullptr;
        }
    };

    /**************************************
     *          PARAMETERS
     **************************************/
    m_startParam = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto createdVar = m_scopeStack.back()->createVariable(lexicalVector[index]);
            createdVar->setIsParam(true);
            m_focusVariable = createdVar;
        }
    };

    m_paramType = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable->setType(lexicalVector[index]->getValue());
        }
    };

    m_paramName = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable->setName(lexicalVector[index]->getValue());
        }
    };

    m_endParam = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable = nullptr;
        }
    };

    /**************************************
     *          WHILE
     **************************************/

    m_startWhile = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newWhile = m_scopeStack.back()->createWhile(lexicalVector[index]);
            m_scopeStack.push_back(newWhile);
        } else if(phase == BashClass::PHASE_EVAL) {
            auto newWhile = m_scopeStack.back()->getScope(lexicalVector[index]);
            m_scopeStack.push_back(newWhile);
        }
    };

    m_endWhile = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL) {
            m_scopeStack.pop_back();
        }
    };

    /**************************************
     *          IF
     **************************************/

    m_startIf = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newIf = m_scopeStack.back()->createIf(lexicalVector[index]);
            m_scopeStack.push_back(newIf);
        } else if(phase == BashClass::PHASE_EVAL) {
            auto newIf = m_scopeStack.back()->getScope(lexicalVector[index]);
            m_scopeStack.push_back(newIf);
        }
    };

    m_endIf = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL) {
            m_scopeStack.pop_back();
        }
    };
}
