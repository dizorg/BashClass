#ifndef BASHCLASS_BBASHHELPER_H
#define BASHCLASS_BBASHHELPER_H

#include <memory>
#include <bashclass/BClass.h>
#include <bashclass/BVariable.h>
#include <bashclass/BFunction.h>
#include <bashclass/BChain.h>
#include <bashclass/BFunctionCall.h>
#include <bashclass/BReturn.h>
#include <bashclass/BIf.h>
#include <bashclass/BElif.h>
#include <bashclass/BElse.h>
#include <bashclass/BWhile.h>
#include <bashclass/BFor.h>

class BBashHelper {
private:
    /**
     * Generate code to convert strings into char array in bash
     */
    static void _bashStrToCharArray(std::stringstream &ss);

    /**
     * Generate code to create an array in bash
     */
    static void _bashCreateArray(std::stringstream &ss);
public:

    /**
     * Generate code for bash required functions
     */
    static void writeBashFunctions();

    /**
     * Generate header bash code
     */
    static void header();

    /**
     * Generate footer bash code
     */
    static void footer();

    /**
     * Generate counters wichi store the next index of the corresponding item
     */
    static void declareCounters();

    /**
     * Generate a variable in bash
     * @param variable
     * @param defaultValue
     */
    static void createVar(std::shared_ptr<BVariable> variable);

    /**
     * Generate a function in bash
     * @param function
     */
    static void createFunction(std::shared_ptr<BFunction> function);

    /**
     * Close a function in bash
     * @param function
     */
    static void closeFunction(std::shared_ptr<BFunction> function);

    /**
     * Write pure bash
     * @param scope
     * @param lexicalToken
     */
    static void bash(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> lexicalToken);

    /**
     * Write expression
     * @param scope
     * @param expression
     */
    static void writeExpression(std::shared_ptr<BScope> scope, std::shared_ptr<IBExpression> expression);

    /**
     * Generate code for a return statement
     * @param rtn
     */
    static void writeReturn(std::shared_ptr<BReturn> rtn);

    /**
     * Generate code for an if statement
     * @param ifStatement
     */
    static void createIf(std::shared_ptr<BIf> ifStatement);

    /**
     * Generate code for an elif statement
     * @param elifStatement
     */
    static void createElif(std::shared_ptr<BElif> elifStatement);

    /**
     * Generate code for an else statement
     * @param elseStatement
     */
    static void createElse(std::shared_ptr<BElse> elseStatement);

    /**
     * Generate code for closing the if statement
     * @param ifStatement
     */
    static void closeIf(std::shared_ptr<BIf> ifStatement);

    /**
     * Generate code for closing the elif statement
     * @param elifStatement
     */
    static void closeElif(std::shared_ptr<BElif> elifStatement);

    /**
     * Generate code for closing the else statement
     * @param elseStatement
     */
    static void closeElse(std::shared_ptr<BElse> elseStatement);

    /**
     * Generate code for while statement
     * @param whileStatement
     */
    static void createWhile(std::shared_ptr<BWhile> whileStatement);

    /**
     * Generate code for closing while statement
     * @param whileStatement
     */
    static void closeWhile(std::shared_ptr<BWhile> whileStatement);

    /**
     * Generate code for for statement
     * @param forStatement
     */
    static void createFor(std::shared_ptr<BFor> forStatement);

    /**
     * Generate code for closing for statement
     * @param forStatement
     */
    static void closeFor(std::shared_ptr<BFor> forStatement);

    /**
     * Write break or continue
     * @param scope
     * @param lexicalToken
     */
    static void writeBreakOrContinue(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> lexicalToken);
};

#endif
