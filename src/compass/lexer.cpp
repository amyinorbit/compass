//===--------------------------------------------------------------------------------------------===
// lexer.cpp - Compass::Lexer lexing methods.
// This source is part of the Compass Engine
//
// Created on 2019-01-30 by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#include <algorithm>
#include <cctype>
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <compass/lexer.hpp>

std::string tokenNames__[] = {
    "Keyword",
    "Integer",
    "Word",
    "QuotedString",
    "Dash",
    "Comma",
    "Period",
    "Amp",
    "Colon",
    "End",
};

namespace Compass {
    
    std::string Token::type() const {
        return tokenNames__[kind];
    }
    
    Lexer::Lexer(const std::string& source)
        : source_(source), ptr_(0) {}
    
    void Lexer::reset() {
        ptr_ = 0;
    }
    
    const Token& Lexer::currentToken() const {
        return currentToken_;
    }

    std::size_t Lexer::remaining() const {
        return source_.length() - ptr_;
    }

    codepoint_t Lexer::current() const {
        return utf8_getCodepoint(source_.data() + ptr_, remaining());
    }

    codepoint_t Lexer::nextChar() {
        if(ptr_ >= source_.size()) return current_ = '\0';
    
        current_ = current();
        auto size = utf8_codepointSize(current_);
        if(size > 0) {
            ptr_ += size;
        }
        return current_;
    }

    bool Lexer::isIdentifier(codepoint_t c) {
        return utf8_isIdentifier(c)
            || c == '-'
            || c == '\'';
    }

    void Lexer::updateTokenStart() {
        start_ = ptr_;
    }

    const Token& Lexer::lexKeyword() {
        while(isIdentifier(current())) {
            nextChar();
        }
    
        const auto length = ptr_ - start_;
        const auto str = source_.substr(start_, length);
    
        return makeToken(Token::Keyword, str);
    }

    const Token& Lexer::lexString() {
    
        while(current() != '"') {
            codepoint_t c = nextChar();
            if(c == '\0') {
                std::cerr << "quoted string not finished\n";
                abort();
            }
        }
        nextChar();
    
        const auto length = (ptr_ - start_)-2;
        return makeToken(Token::QuotedString, source_.substr(start_+1, length));
    }

    const Token& Lexer::lexWord() {
        while(isIdentifier(current())) {
            nextChar();
        }
    
        const auto length = ptr_ - start_;
        const auto str = source_.substr(start_, length);
        return makeToken(Token::Word, str);
    }

    const Token& Lexer::lexNumber() {
        while(current() >= '0' && current() < '9') {
            nextChar();
        }
        const auto length = (ptr_ - start_);
        return makeToken(Token::Integer, source_.substr(start_, length));
    }

    const Token& Lexer::makeToken(Token::Kind kind, const std::string& str) {
        currentToken_.kind = kind;
        currentToken_.text = str;
        return currentToken_;
    }
    
    void Lexer::eatLineComment() {
        while(current() && (current() != '\n' && current() != '\r')) {
            nextChar();
        }
    }
    
    void Lexer::eatParenComment() {
        while(current() != ')') nextChar();
        nextChar();
    }

    const Token& Lexer::nextToken() {
        if(ptr_ >= source_.size()) {
            return makeToken(Token::End);
        }
    
        while(current() != '\0') {
            updateTokenStart();
            codepoint_t c = nextChar();
        
            switch(c) {
                //whitespace
                case 0x0020:
                case 0x000d:
                case 0x0009:
                case 0x000b:
                case 0x000c:
                case 0x000a:
                case 0xfeff:
                    break;

                // // line feed
                //     return makeToken(Token::Newline);
                    
                case '.':
                    return makeToken(Token::Period);
            
                case ',':
                    return makeToken(Token::Comma);
                
                case '&':
                    return makeToken(Token::Amp);
                
                case '-':
                    return makeToken(Token::Dash);
            
                case '@':
                    return lexKeyword();
                
                case '"':
                    return lexString();
                    
                case ':':
                    return makeToken(Token::Colon);
                    
                case '#':
                case '!':
                    eatLineComment();
                    break;
                    
                case '(':
                    eatParenComment();
                    break;
                
                default:
                    if (utf8_isIdentifierHead(c)) {
                        return lexWord();
                    }
                    std::cerr << "Invalid character: "<< (char)c <<"\n";
                    abort();
            }
        }
        return makeToken(Token::End);
    }
}