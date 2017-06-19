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

class BBashHelper {
public:

    /**
     * Generate header bash code
     */
    static void header();

    /**
     * Generate footer bash code
     */
    static void footer();

    /**
     * Generate an associative array
     * @param classScope
     */
    static void declareClass(std::shared_ptr<BClass> classScope);

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
};

#endif
