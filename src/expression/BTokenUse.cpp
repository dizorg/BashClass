#include <bashclass/BTokenUse.h>
#include <bashclass/BException.h>
#include <bashclass/BTypeFactory.h>

void BTokenUse::setLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) {

    // Set lexical token
    m_lexicalToken = lexicalToken;

    // Configure expression type based no the lexical token data
    if(m_lexicalToken->getName() == IBType::DATA_TYPE_NAME_INT
       || m_lexicalToken->getName() == IBType::DATA_TYPE_NAME_BASH_SUB_INT) {
        m_type = BTypeFactory::createIntExpressionType();

    } else if(m_lexicalToken->getName() == IBType::DATA_TYPE_NAME_STRING
              || m_lexicalToken->getName() == IBType::DATA_TYPE_NAME_BASH_SUB) {

        // A string is a char[]
        m_type = BTypeFactory::createCharExpressionType();
        m_type->setDimension(1);

    } else if(m_lexicalToken->getName() == IBType::DATA_TYPE_NAME_CHAR) {
        m_type = BTypeFactory::createCharExpressionType();

    } else if(m_lexicalToken->getName() == IBType::DATA_TYPE_NAME_BOOLEAN) {
        m_type = BTypeFactory::createBooleanExpressionType();

    } else if(m_lexicalToken->getName() == IBType::NULL_VALUE) {
        m_type = BTypeFactory::createNullExpressionType();

    } else {
        throw BException("Token type value cannot be undefined");
    }
}

void BTokenUse::castType(std::shared_ptr<BElementType> type) {
    m_type->cast(type);
}
